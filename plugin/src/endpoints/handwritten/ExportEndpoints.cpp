/**
 * NUXP Export Endpoint Implementations
 *
 * Hand-written handlers for SVG export:
 *   - ExportSvgViaAction     — uses AIActionManagerSuite::PlayActionEvent
 *   - ExportSvgViaFileFormat — uses AIFileFormatSuite::GoExport
 *
 * Both export the current document as SVG to a file path, then optionally
 * read the file content back and return it as a JSON string.
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

// SDK action headers for export parameters
#include "actions/AIDocumentAction.h"
#include "actions/AISVGAction.h"
#include "AISVGTypes.h"

using json = nlohmann::json;

namespace {

// ---------------------------------------------------------------------------
// Helper: Generate a temp file path for SVG output
// ---------------------------------------------------------------------------
std::string MakeTempSvgPath() {
    // Use TMPDIR if available, else /tmp
    const char *tmpDir = std::getenv("TMPDIR");
    std::string dir = tmpDir ? tmpDir : "/tmp";
    // Remove trailing slash if present
    if (!dir.empty() && dir.back() == '/') {
        dir.pop_back();
    }
    return dir + "/nuxp-export-" + std::to_string(std::time(nullptr)) + ".svg";
}

// ---------------------------------------------------------------------------
// Helper: Read entire file contents into a string
// Polls briefly for non-zero file size to handle async flush from export.
// ---------------------------------------------------------------------------
std::string ReadFileContents(const std::string &path) {
    // Poll up to 3 seconds for the file to appear and have content
    for (int attempt = 0; attempt < 30; ++attempt) {
        std::ifstream file(path, std::ios::ate);
        if (file.is_open() && file.tellg() > 0) {
            file.seekg(0);
            std::ostringstream ss;
            ss << file.rdbuf();
            std::string content = ss.str();
            if (!content.empty()) return content;
        }
        // Wait 100ms before retrying
        struct timespec ts = {0, 100000000};
        nanosleep(&ts, nullptr);
    }
    return "";
}

// ---------------------------------------------------------------------------
// Helper: Build a VPB with SVG export parameters
// ---------------------------------------------------------------------------
ASErr BuildSvgExportVPB(AIActionManagerSuite *actionMgr,
                         const std::string &outputPath,
                         bool selectionOnly,
                         AIActionParamValueRef &vpb) {
    ASErr err = actionMgr->AINewActionParamValue(&vpb);
    if (err != kNoErr) return err;

    // File path
    err = actionMgr->AIActionSetString(vpb, kAIExportDocumentNameKey,
                                        outputPath.c_str());
    if (err != kNoErr) return err;

    // Format identifier
    err = actionMgr->AIActionSetString(vpb, kAIExportDocumentFormatKey,
                                        kAISVGFileFormat);
    if (err != kNoErr) return err;

    // File extension
    err = actionMgr->AIActionSetString(vpb, kAIExportDocumentExtensionKey,
                                        kAISVGFileFormatExtension);
    if (err != kNoErr) return err;

    // SVG options — clean, web-ready defaults
    actionMgr->AIActionSetInteger(vpb, kAISVGEncodingKey,
                                   kAISVGUTF8Encoding);
    actionMgr->AIActionSetInteger(vpb, kAISVGCompressionKey,
                                   kAISVGCompressionNone);
    actionMgr->AIActionSetInteger(vpb, kAISVGStyleTypeKey,
                                   kAISVGPresentationAttrsStyle);
    actionMgr->AIActionSetInteger(vpb, kAISVGPrecisionKey,
                                   kAISVGPrecision_3);
    actionMgr->AIActionSetInteger(vpb, kAISVGFontFormatKey,
                                   kAISVGFontOutline);
    actionMgr->AIActionSetInteger(vpb, kAISVGEmbedRasterLocationKey,
                                   kAISVGImageEmbed);
    actionMgr->AIActionSetInteger(vpb, kAISVGDTDKey, kAISVGDTD11);
    actionMgr->AIActionSetInteger(vpb, kAISVGIdTypeKey, kAISVGIdRegular);
    actionMgr->AIActionSetBoolean(vpb, kAISVGResponsiveKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGMinifyKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGIncludePGFKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGIncludeXAPKey, false);
    actionMgr->AIActionSetBoolean(vpb, kAISVGIncludeAdobeNameSpaceKey, false);

    // Clip SVG output to artboard bounds
    actionMgr->AIActionSetBoolean(vpb, kAISVGExportClipToArtboardKey, true);

    // Selection-only export (kCopyingSVG = 'Copy')
    actionMgr->AIActionSetBoolean(vpb, kCopyingSVG, selectionOnly);

    return kNoErr;
}

} // anonymous namespace

namespace NUXP {

// ===========================================================================
// Approach A: Export via AIActionManagerSuite::PlayActionEvent
// ===========================================================================
std::string HandleExportSvgViaAction(const std::string &body) {
    // Parse request
    std::string outputPath;
    bool useTempFile = true;
    bool selectionOnly = false;
    try {
        json params = json::parse(body);
        if (params.contains("outputPath") && params["outputPath"].is_string()) {
            outputPath = params["outputPath"].get<std::string>();
            if (!outputPath.empty()) {
                useTempFile = false;
            }
        }
        if (params.contains("selectionOnly") && params["selectionOnly"].is_boolean()) {
            selectionOnly = params["selectionOnly"].get<bool>();
        }
    } catch (const json::parse_error &e) {
        return json{{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", std::string("Invalid JSON: ") + e.what()}}
            .dump();
    }

    if (useTempFile) {
        outputPath = MakeTempSvgPath();
    }

    const std::string exportPath = outputPath;
    const bool readBack = useTempFile;
    const bool copyMode = selectionOnly;

    json result = MainThreadDispatch::Run([exportPath, readBack, copyMode]() -> json {
        auto *actionMgr = SuitePointers::AIActionManager();
        if (!actionMgr) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "AIActionManagerSuite not available"}};
        }

        // Build the value parameter block
        AIActionParamValueRef vpb = nullptr;
        ASErr err = BuildSvgExportVPB(actionMgr, exportPath, copyMode, vpb);
        if (err != kNoErr || !vpb) {
            if (vpb) actionMgr->AIDeleteActionParamValue(vpb);
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "Failed to build SVG export parameters, error: " +
                                    std::to_string(err)}};
        }

        // Play the export action with dialog suppressed
        err = actionMgr->PlayActionEvent(kAIExportDocumentAction, kDialogOff,
                                          vpb);
        actionMgr->AIDeleteActionParamValue(vpb);

        if (err != kNoErr) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", exportPath},
                    {"message", "PlayActionEvent failed, error: " +
                                    std::to_string(err)}};
        }

        // Read back SVG content if using temp file
        std::string svgContent;
        if (readBack) {
            svgContent = ReadFileContents(exportPath);
            if (svgContent.empty()) {
                return {
                    {"success", false},
                    {"svgContent", ""},
                    {"filePath", exportPath},
                    {"message",
                     "Export action completed but SVG file is empty or unreadable"}};
            }
        }

        return {{"success", true},
                {"svgContent", svgContent},
                {"filePath", exportPath},
                {"message", "SVG exported via action manager"}};
    });

    return result.dump();
}

// ===========================================================================
// Approach B: Export via AIFileFormatSuite::GoExport
// ===========================================================================
std::string HandleExportSvgViaFileFormat(const std::string &body) {
    // Parse request
    std::string outputPath;
    bool useTempFile = true;
    bool selectionOnly = false;
    try {
        json params = json::parse(body);
        if (params.contains("outputPath") && params["outputPath"].is_string()) {
            outputPath = params["outputPath"].get<std::string>();
            if (!outputPath.empty()) {
                useTempFile = false;
            }
        }
        if (params.contains("selectionOnly") && params["selectionOnly"].is_boolean()) {
            selectionOnly = params["selectionOnly"].get<bool>();
        }
    } catch (const json::parse_error &e) {
        return json{{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", std::string("Invalid JSON: ") + e.what()}}
            .dump();
    }

    if (useTempFile) {
        outputPath = MakeTempSvgPath();
    }

    const std::string exportPath = outputPath;
    const bool readBack = useTempFile;
    const bool copyMode = selectionOnly;

    json result = MainThreadDispatch::Run([exportPath, readBack, copyMode]() -> json {
        auto *actionMgr = SuitePointers::AIActionManager();
        auto *fileFormat = SuitePointers::AIFileFormat();
        if (!fileFormat) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "AIFileFormatSuite not available"}};
        }
        if (!actionMgr) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message",
                     "AIActionManagerSuite not available (needed for VPB)"}};
        }

        // Build the same VPB — GoExport uses the same parameter format
        AIActionParamValueRef vpb = nullptr;
        ASErr err = BuildSvgExportVPB(actionMgr, exportPath, copyMode, vpb);
        if (err != kNoErr || !vpb) {
            if (vpb) actionMgr->AIDeleteActionParamValue(vpb);
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "Failed to build SVG export parameters, error: " +
                                    std::to_string(err)}};
        }

        // Call GoExport directly on the file format suite
        err = fileFormat->GoExport(vpb);
        actionMgr->AIDeleteActionParamValue(vpb);

        if (err != kNoErr) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", exportPath},
                    {"message", "GoExport failed, error: " +
                                    std::to_string(err)}};
        }

        // Read back SVG content if using temp file
        std::string svgContent;
        if (readBack) {
            svgContent = ReadFileContents(exportPath);
            if (svgContent.empty()) {
                return {
                    {"success", false},
                    {"svgContent", ""},
                    {"filePath", exportPath},
                    {"message",
                     "GoExport completed but SVG file is empty or unreadable"}};
            }
        }

        return {{"success", true},
                {"svgContent", svgContent},
                {"filePath", exportPath},
                {"message", "SVG exported via file format suite"}};
    });

    return result.dump();
}

// ===========================================================================
// Approach C: Export only the current selection as SVG
//
// Strategy: hide all layers, duplicate selected art onto a fresh temp layer,
// export the whole document (which now contains only the duplicates),
// read back the SVG, then undo all document mutations.
// ===========================================================================
std::string HandleExportSelectionAsSvg(const std::string &body) {
    // Body is currently ignored (no required params), but parse to validate JSON
    try {
        json::parse(body);
    } catch (const json::parse_error &e) {
        return json{{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", std::string("Invalid JSON: ") + e.what()}}
            .dump();
    }

    const std::string exportPath = MakeTempSvgPath();

    json result = MainThreadDispatch::Run([exportPath]() -> json {
        auto *matchingArt = SuitePointers::AIMatchingArt();
        auto *layerSuite  = SuitePointers::AILayer();
        auto *artSuite    = SuitePointers::AIArt();
        auto *actionMgr   = SuitePointers::AIActionManager();
        auto *mdMemory    = SuitePointers::AIMdMemory();

        if (!matchingArt || !layerSuite || !artSuite ||
            !actionMgr || !mdMemory) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "One or more required SDK suites not available"}};
        }

        // --- 1. Get the current selection ---
        AIArtHandle **matches = nullptr;
        ai::int32 numMatches = 0;
        ASErr err = matchingArt->GetSelectedArt(&matches, &numMatches);
        if (err != kNoErr || !matches || numMatches == 0) {
            if (matches) {
                mdMemory->MdMemoryDisposeHandle(
                    reinterpret_cast<AIMdMemoryHandle>(matches));
            }
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "Nothing is selected"}};
        }

        // --- 2. Record original layer visibility, then hide all layers ---
        ai::int32 layerCount = 0;
        layerSuite->CountLayers(&layerCount);

        // Record which layers were visible so we can restore them
        std::vector<std::pair<AILayerHandle, AIBoolean>> layerVisibility;
        for (ai::int32 i = 0; i < layerCount; ++i) {
            AILayerHandle layer = nullptr;
            if (layerSuite->GetNthLayer(i, &layer) == kNoErr && layer) {
                AIBoolean visible = false;
                layerSuite->GetLayerVisible(layer, &visible);
                layerVisibility.push_back({layer, visible});
                layerSuite->SetLayerVisible(layer, false);
            }
        }

        // Helper: restore layer visibility
        auto restoreLayers = [&]() {
            for (auto &[layer, wasVisible] : layerVisibility) {
                layerSuite->SetLayerVisible(layer, wasVisible);
            }
        };

        // Helper: delete temp layer if it exists
        auto deleteTempLayer = [&](AILayerHandle tl) {
            if (tl) layerSuite->DeleteLayer(tl);
        };

        // --- 3. Create a new temporary layer at the top ---
        AILayerHandle tempLayer = nullptr;
        err = layerSuite->InsertLayer(nullptr, kPlaceAboveAll, &tempLayer);
        if (err != kNoErr || !tempLayer) {
            restoreLayers();
            mdMemory->MdMemoryDisposeHandle(
                reinterpret_cast<AIMdMemoryHandle>(matches));
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "Failed to create temporary export layer, error: " +
                                    std::to_string(err)}};
        }

        layerSuite->SetLayerTitle(tempLayer,
                                   ai::UnicodeString("__nuxp_export_temp__"));
        layerSuite->SetLayerVisible(tempLayer, true);

        // --- 4. Get the layer group art handle for duplication target ---
        AIArtHandle tempLayerGroup = nullptr;
        artSuite->GetFirstArtOfLayer(tempLayer, &tempLayerGroup);
        if (!tempLayerGroup) {
            deleteTempLayer(tempLayer);
            restoreLayers();
            mdMemory->MdMemoryDisposeHandle(
                reinterpret_cast<AIMdMemoryHandle>(matches));
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "Failed to get temp layer group art"}};
        }

        // --- 6. Duplicate each selected art onto the temp layer ---
        for (ai::int32 i = 0; i < numMatches; ++i) {
            AIArtHandle newArt = nullptr;
            artSuite->DuplicateArt((*matches)[i], kPlaceInsideOnTop,
                                    tempLayerGroup, &newArt);
            // Silently skip items that fail to duplicate
        }

        // --- 6b. Get art bounds for post-processing the SVG viewBox ---
        AIRealRect artBounds = {0, 0, 0, 0};
        artSuite->GetArtBounds(tempLayerGroup, &artBounds);

        // --- 7. Export the document as SVG (only temp layer is visible) ---
        AIActionParamValueRef vpb = nullptr;
        err = BuildSvgExportVPB(actionMgr, exportPath, false, vpb);
        if (err != kNoErr || !vpb) {
            if (vpb) actionMgr->AIDeleteActionParamValue(vpb);
            deleteTempLayer(tempLayer);
            restoreLayers();
            mdMemory->MdMemoryDisposeHandle(
                reinterpret_cast<AIMdMemoryHandle>(matches));
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", ""},
                    {"message", "Failed to build SVG export parameters, error: " +
                                    std::to_string(err)}};
        }

        err = actionMgr->PlayActionEvent(kAIExportDocumentAction, kDialogOff,
                                           vpb);
        actionMgr->AIDeleteActionParamValue(vpb);

        if (err != kNoErr) {
            deleteTempLayer(tempLayer);
            restoreLayers();
            mdMemory->MdMemoryDisposeHandle(
                reinterpret_cast<AIMdMemoryHandle>(matches));
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", exportPath},
                    {"message", "PlayActionEvent failed, error: " +
                                    std::to_string(err)}};
        }

        // --- 8. Read the exported SVG file ---
        std::string svgContent = ReadFileContents(exportPath);

        // --- 9. Clean up: delete temp layer, restore layer visibility ---
        deleteTempLayer(tempLayer);
        restoreLayers();

        // --- 10. Free the selection array ---
        mdMemory->MdMemoryDisposeHandle(
            reinterpret_cast<AIMdMemoryHandle>(matches));

        if (svgContent.empty()) {
            return {{"success", false},
                    {"svgContent", ""},
                    {"filePath", exportPath},
                    {"message",
                     "Export completed but SVG file is empty or unreadable"}};
        }

        // Art bounds in Illustrator coordinates (top > bottom, Y increases up)
        json bounds = {
            {"left", artBounds.left},
            {"top", artBounds.top},
            {"right", artBounds.right},
            {"bottom", artBounds.bottom},
            {"width", artBounds.right - artBounds.left},
            {"height", artBounds.top - artBounds.bottom}
        };

        return {{"success", true},
                {"svgContent", svgContent},
                {"filePath", exportPath},
                {"artBounds", bounds},
                {"message", "Selection exported as SVG (" +
                                std::to_string(numMatches) + " item(s))"}};
    });

    return result.dump();
}

} // namespace NUXP
