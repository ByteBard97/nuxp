/**
 * NUXP Export Endpoint Implementations
 *
 * Hand-written handlers for SVG export:
 *   - ExportSvgViaAction       — uses AIActionManagerSuite::PlayActionEvent
 *   - ExportSvgViaFileFormat   — uses AIFileFormatSuite::GoExport
 *   - ExportSelectionAsSvg     — isolates selection onto temp layer, exports, restores
 *
 * These implement the function signatures declared in generated/CustomRouteHandlers.h.
 * Route wiring is in generated/CustomRouteRegistration.cpp.
 */

#include "CustomRouteHandlers.h"
#include "IllustratorSDK.h"
#include "SuitePointers.hpp"
#include "MainThreadDispatch.hpp"
#include "AIActionManager.h"
#include "AIFileFormat.h"
#include "AIFilePath.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <atomic>
#include <unistd.h>

#include "actions/AIDocumentAction.h"
#include "actions/AISVGAction.h"
#include "AISVGTypes.h"

using json = nlohmann::json;

namespace {

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

json ErrorResponse(const std::string &message, const std::string &filePath = "") {
    return {{"success", false}, {"svgContent", ""}, {"filePath", filePath}, {"message", message}};
}

std::string MakeTempSvgPath() {
    static std::atomic<int> counter{0};
    const char *tmpDir = std::getenv("TMPDIR");
    std::string dir = tmpDir ? tmpDir : "/var/folders";
    if (!dir.empty() && dir.back() == '/') dir.pop_back();
    return dir + "/nuxp-export-" + std::to_string(getpid()) +
           "-" + std::to_string(counter++) + ".svg";
}

// TODO: Polling is a workaround — the export SDK calls *should* be synchronous
// on the main thread. If this proves unreliable, investigate whether the file
// write is deferred by macOS or the SDK itself.
std::string ReadFileContents(const std::string &path) {
    for (int attempt = 0; attempt < 30; ++attempt) {
        std::ifstream file(path, std::ios::ate);
        if (file.is_open() && file.tellg() > 0) {
            file.seekg(0);
            std::ostringstream ss;
            ss << file.rdbuf();
            std::string content = ss.str();
            if (!content.empty()) return content;
        }
        struct timespec ts = {0, 100000000}; // 100ms
        nanosleep(&ts, nullptr);
    }
    return "";
}

ASErr BuildSvgExportVPB(AIActionManagerSuite *actionMgr,
                         const std::string &outputPath,
                         bool selectionOnly,
                         AIActionParamValueRef &vpb) {
    ASErr err = actionMgr->AINewActionParamValue(&vpb);
    if (err != kNoErr) return err;

    err = actionMgr->AIActionSetString(vpb, kAIExportDocumentNameKey, outputPath.c_str());
    if (err != kNoErr) return err;
    err = actionMgr->AIActionSetString(vpb, kAIExportDocumentFormatKey, kAISVGFileFormat);
    if (err != kNoErr) return err;
    err = actionMgr->AIActionSetString(vpb, kAIExportDocumentExtensionKey, kAISVGFileFormatExtension);
    if (err != kNoErr) return err;

    actionMgr->AIActionSetInteger(vpb, kAISVGEncodingKey, kAISVGUTF8Encoding);
    actionMgr->AIActionSetInteger(vpb, kAISVGCompressionKey, kAISVGCompressionNone);
    actionMgr->AIActionSetInteger(vpb, kAISVGStyleTypeKey, kAISVGPresentationAttrsStyle);
    actionMgr->AIActionSetInteger(vpb, kAISVGPrecisionKey, kAISVGPrecision_3);
    actionMgr->AIActionSetInteger(vpb, kAISVGFontFormatKey, kAISVGFontOutline);
    actionMgr->AIActionSetInteger(vpb, kAISVGEmbedRasterLocationKey, kAISVGImageEmbed);
    actionMgr->AIActionSetInteger(vpb, kAISVGDTDKey, kAISVGDTD11);
    actionMgr->AIActionSetInteger(vpb, kAISVGIdTypeKey, kAISVGIdRegular);
    actionMgr->AIActionSetBoolean(vpb, kAISVGResponsiveKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGMinifyKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGIncludePGFKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGIncludeXAPKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGIncludeAdobeNameSpaceKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGExportClipToArtboardKey, true);
    actionMgr->AIActionSetBoolean(vpb, kCopyingSVG, selectionOnly);

    return kNoErr;
}

// ---------------------------------------------------------------------------
// Shared export core: builds VPB, calls exportFn, reads back SVG if temp file
// ---------------------------------------------------------------------------

struct ExportParams {
    std::string outputPath;
    bool useTempFile;
    bool selectionOnly;
};

ExportParams ParseExportRequest(const std::string &body) {
    ExportParams p;
    p.useTempFile = true;
    p.selectionOnly = false;
    json params = json::parse(body);
    if (params.contains("outputPath") && params["outputPath"].is_string()) {
        p.outputPath = params["outputPath"].get<std::string>();
        if (!p.outputPath.empty()) p.useTempFile = false;
    }
    if (params.contains("selectionOnly") && params["selectionOnly"].is_boolean()) {
        p.selectionOnly = params["selectionOnly"].get<bool>();
    }
    if (p.useTempFile) p.outputPath = MakeTempSvgPath();
    return p;
}

using ExportFn = std::function<ASErr(AIActionParamValueRef vpb)>;

std::string RunExport(const std::string &body, ExportFn exportFn, const std::string &successMsg) {
    ExportParams p;
    try {
        p = ParseExportRequest(body);
    } catch (const json::parse_error &e) {
        return ErrorResponse(std::string("Invalid JSON: ") + e.what()).dump();
    }

    const std::string exportPath = p.outputPath;
    const bool readBack = p.useTempFile;
    const bool copyMode = p.selectionOnly;

    json result = MainThreadDispatch::Run([exportPath, readBack, copyMode, &exportFn, &successMsg]() -> json {
        auto *actionMgr = SuitePointers::AIActionManager();
        if (!actionMgr)
            return ErrorResponse("AIActionManagerSuite not available");

        AIActionParamValueRef vpb = nullptr;
        ASErr err = BuildSvgExportVPB(actionMgr, exportPath, copyMode, vpb);
        if (err != kNoErr || !vpb) {
            if (vpb) actionMgr->AIDeleteActionParamValue(vpb);
            return ErrorResponse("Failed to build SVG export parameters, error: " + std::to_string(err));
        }

        err = exportFn(vpb);
        actionMgr->AIDeleteActionParamValue(vpb);

        if (err != kNoErr)
            return ErrorResponse("Export failed, error: " + std::to_string(err), exportPath);

        std::string svgContent;
        if (readBack) {
            svgContent = ReadFileContents(exportPath);
            if (svgContent.empty())
                return ErrorResponse("Export completed but SVG file is empty or unreadable", exportPath);
        }

        return {{"success", true}, {"svgContent", svgContent},
                {"filePath", exportPath}, {"message", successMsg}};
    });

    return result.dump();
}

} // anonymous namespace

namespace NUXP {

// ===========================================================================
// Approach A: Export via AIActionManagerSuite::PlayActionEvent
// ===========================================================================
std::string HandleExportSvgViaAction(const std::string &body) {
    return RunExport(body,
        [](AIActionParamValueRef vpb) -> ASErr {
            return SuitePointers::AIActionManager()->PlayActionEvent(
                kAIExportDocumentAction, kDialogOff, vpb);
        },
        "SVG exported via action manager");
}

// ===========================================================================
// Approach B: Export via AIFileFormatSuite::GoExport
// ===========================================================================
std::string HandleExportSvgViaFileFormat(const std::string &body) {
    auto *fileFormat = SuitePointers::AIFileFormat();
    if (!fileFormat)
        return ErrorResponse("AIFileFormatSuite not available").dump();

    return RunExport(body,
        [fileFormat](AIActionParamValueRef vpb) -> ASErr {
            return fileFormat->GoExport(vpb);
        },
        "SVG exported via file format suite");
}

// ===========================================================================
// Approach C: Export only the current selection as SVG
//
// Strategy: hide all layers, duplicate selected art onto a fresh temp layer,
// export the document (only the temp layer is visible), read back the SVG,
// then restore layer state and delete the temp layer.
// ===========================================================================
std::string HandleExportSelectionAsSvg() {
    const std::string exportPath = MakeTempSvgPath();

    json result = MainThreadDispatch::Run([exportPath]() -> json {
        auto *matchingArt = SuitePointers::AIMatchingArt();
        auto *layerSuite  = SuitePointers::AILayer();
        auto *artSuite    = SuitePointers::AIArt();
        auto *actionMgr   = SuitePointers::AIActionManager();
        auto *mdMemory    = SuitePointers::AIMdMemory();

        if (!matchingArt || !layerSuite || !artSuite || !actionMgr || !mdMemory)
            return ErrorResponse("One or more required SDK suites not available");

        // 1. Get the current selection
        AIArtHandle **matches = nullptr;
        ai::int32 numMatches = 0;
        ASErr err = matchingArt->GetSelectedArt(&matches, &numMatches);
        if (err != kNoErr || !matches || numMatches == 0) {
            if (matches)
                mdMemory->MdMemoryDisposeHandle(reinterpret_cast<AIMdMemoryHandle>(matches));
            return ErrorResponse("Nothing is selected");
        }

        // RAII-ish cleanup helpers
        auto freeMatches = [&]() {
            mdMemory->MdMemoryDisposeHandle(reinterpret_cast<AIMdMemoryHandle>(matches));
        };

        // 2. Record and hide all layer visibility
        ai::int32 layerCount = 0;
        layerSuite->CountLayers(&layerCount);
        std::vector<std::pair<AILayerHandle, AIBoolean>> layerVis;
        for (ai::int32 i = 0; i < layerCount; ++i) {
            AILayerHandle layer = nullptr;
            if (layerSuite->GetNthLayer(i, &layer) == kNoErr && layer) {
                AIBoolean visible = false;
                layerSuite->GetLayerVisible(layer, &visible);
                layerVis.push_back({layer, visible});
                layerSuite->SetLayerVisible(layer, false);
            }
        }
        auto restoreLayers = [&]() {
            for (auto &[layer, wasVisible] : layerVis)
                layerSuite->SetLayerVisible(layer, wasVisible);
        };

        // 3. Create temp layer
        AILayerHandle tempLayer = nullptr;
        err = layerSuite->InsertLayer(nullptr, kPlaceAboveAll, &tempLayer);
        if (err != kNoErr || !tempLayer) {
            restoreLayers();
            freeMatches();
            return ErrorResponse("Failed to create temp export layer, error: " + std::to_string(err));
        }
        layerSuite->SetLayerTitle(tempLayer, ai::UnicodeString("__nuxp_export_temp__"));
        layerSuite->SetLayerVisible(tempLayer, true);

        auto cleanup = [&]() {
            if (tempLayer) layerSuite->DeleteLayer(tempLayer);
            restoreLayers();
            freeMatches();
        };

        // 4. Get layer group for duplication target
        AIArtHandle tempLayerGroup = nullptr;
        artSuite->GetFirstArtOfLayer(tempLayer, &tempLayerGroup);
        if (!tempLayerGroup) {
            cleanup();
            return ErrorResponse("Failed to get temp layer group art");
        }

        // 5. Duplicate selection onto temp layer
        for (ai::int32 i = 0; i < numMatches; ++i) {
            AIArtHandle newArt = nullptr;
            artSuite->DuplicateArt((*matches)[i], kPlaceInsideOnTop, tempLayerGroup, &newArt);
        }

        // 6. Get art bounds for response
        AIRealRect artBounds = {0, 0, 0, 0};
        artSuite->GetArtBounds(tempLayerGroup, &artBounds);

        // 7. Export
        AIActionParamValueRef vpb = nullptr;
        err = BuildSvgExportVPB(actionMgr, exportPath, false, vpb);
        if (err != kNoErr || !vpb) {
            if (vpb) actionMgr->AIDeleteActionParamValue(vpb);
            cleanup();
            return ErrorResponse("Failed to build SVG export parameters, error: " + std::to_string(err));
        }
        err = actionMgr->PlayActionEvent(kAIExportDocumentAction, kDialogOff, vpb);
        actionMgr->AIDeleteActionParamValue(vpb);
        if (err != kNoErr) {
            cleanup();
            return ErrorResponse("PlayActionEvent failed, error: " + std::to_string(err), exportPath);
        }

        // 8. Read back + cleanup
        std::string svgContent = ReadFileContents(exportPath);
        cleanup();

        if (svgContent.empty())
            return ErrorResponse("Export completed but SVG file is empty or unreadable", exportPath);

        return {{"success", true},
                {"svgContent", svgContent},
                {"filePath", exportPath},
                {"artBounds", {{"left", artBounds.left}, {"top", artBounds.top},
                               {"right", artBounds.right}, {"bottom", artBounds.bottom},
                               {"width", artBounds.right - artBounds.left},
                               {"height", artBounds.top - artBounds.bottom}}},
                {"message", "Selection exported as SVG (" +
                            std::to_string(numMatches) + " item(s))"}};
    });

    return result.dump();
}

} // namespace NUXP
