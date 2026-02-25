/**
 * NUXP Shape & Dialog Endpoint Implementations
 *
 * Hand-written handlers for shape creation and file save dialog:
 *   - CreateRectangle  — 4-segment closed rectangle path
 *   - CreateEllipse    — 4-segment Bezier-approximated ellipse path
 *   - CreatePath       — arbitrary segment array path
 *   - CreateLine       — 2-segment open line path
 *   - ShowFileSaveDialog — native platform save dialog via AIUserSuite
 *
 * These implement the function signatures declared in generated/CustomRouteHandlers.h.
 * Route wiring is in generated/CustomRouteRegistration.cpp.
 */

#include "CustomRouteHandlers.h"
#include "IllustratorSDK.h"
#include "SuitePointers.hpp"
#include "HandleManager.hpp"
#include "MainThreadDispatch.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <cmath>

using json = nlohmann::json;

namespace {

// ---------------------------------------------------------------------------
// Helper: Deserialize JSON to AIPathSegment
// (Same logic as NUXPHandlers.cpp anonymous namespace helper)
// ---------------------------------------------------------------------------
AIPathSegment DeserializeSegment(const json& j) {
    AIPathSegment seg;
    seg.p.h   = j["p"].value("h", 0.0);
    seg.p.v   = j["p"].value("v", 0.0);
    seg.in.h  = j["in"].value("h", 0.0);
    seg.in.v  = j["in"].value("v", 0.0);
    seg.out.h = j["out"].value("h", 0.0);
    seg.out.v = j["out"].value("v", 0.0);
    seg.corner = j.value("corner", false);
    return seg;
}

// ---------------------------------------------------------------------------
// Helper: Get art name as a string identifier
// Returns the art name if set, otherwise returns the handle ID as a string.
// ---------------------------------------------------------------------------
std::string GetArtUuid(AIArtHandle art, int handleId) {
    if (SuitePointers::AIArt()) {
        ai::UnicodeString nameUni;
        ASErr err = SuitePointers::AIArt()->GetArtName(art, nameUni, nullptr);
        if (err == kNoErr && !nameUni.empty()) {
            return nameUni.as_UTF8();
        }
    }
    return std::to_string(handleId);
}

// ---------------------------------------------------------------------------
// Helper: Create new path art at top of paint order
// Returns error json on failure, or null json on success with newArt set.
// ---------------------------------------------------------------------------
json CreateNewPathArt(AIArtHandle& newArt) {
    if (!SuitePointers::AIArt() || !SuitePointers::AIPath()) {
        return {{"success", false},
                {"error", "Required suites not available (AIArt, AIPath)"}};
    }

    ASErr err = SuitePointers::AIArt()->NewArt(
        kPathArt, kPlaceAboveAll, nullptr, &newArt);
    if (err != kNoErr || newArt == nullptr) {
        return {{"success", false},
                {"error", "Failed to create path art object"},
                {"errorCode", static_cast<int>(err)}};
    }

    return nullptr; // success — no error
}

// ---------------------------------------------------------------------------
// Helper: Set segments on a path art, optionally close it, register handle
// ---------------------------------------------------------------------------
json FinalizePathArt(AIArtHandle newArt, AIPathSegment* segments,
                     ai::int16 count, bool closed) {
    ASErr err = SuitePointers::AIPath()->SetPathSegments(
        newArt, 0, count, segments);
    if (err != kNoErr) {
        SuitePointers::AIArt()->DisposeArt(newArt);
        return {{"success", false},
                {"error", "Failed to set path segments"},
                {"errorCode", static_cast<int>(err)}};
    }

    err = SuitePointers::AIPath()->SetPathClosed(newArt, closed);
    if (err != kNoErr) {
        // Non-fatal for close operation — art was still created
    }

    int handleId = HandleManager::art.Register(newArt);
    std::string uuid = GetArtUuid(newArt, handleId);

    return {{"success", true},
            {"handle", handleId},
            {"uuid", uuid}};
}

} // anonymous namespace

// ===========================================================================
// NUXP Handler Implementations
// ===========================================================================

namespace NUXP {

// ---------------------------------------------------------------------------
// POST /api/art/rectangle — Create a rectangle path
// ---------------------------------------------------------------------------
std::string HandleCreateRectangle(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    AIReal x      = params.value("x", 0.0);
    AIReal y      = params.value("y", 0.0);
    AIReal width  = params.value("width", 100.0);
    AIReal height = params.value("height", 100.0);

    if (width <= 0 || height <= 0) {
        return json{{"success", false},
                    {"error", "Width and height must be positive"}}.dump();
    }

    json result = MainThreadDispatch::Run([x, y, width, height]() -> json {
        AIArtHandle newArt = nullptr;
        json err = CreateNewPathArt(newArt);
        if (!err.is_null()) return err;

        // Define rectangle: 4 corner segments (clockwise from bottom-left)
        // Illustrator Y-axis increases upward
        AIPathSegment segments[4];

        // Bottom-left
        segments[0].p.h = x;
        segments[0].p.v = y;
        segments[0].in = segments[0].out = segments[0].p;
        segments[0].corner = true;

        // Bottom-right
        segments[1].p.h = x + width;
        segments[1].p.v = y;
        segments[1].in = segments[1].out = segments[1].p;
        segments[1].corner = true;

        // Top-right
        segments[2].p.h = x + width;
        segments[2].p.v = y + height;
        segments[2].in = segments[2].out = segments[2].p;
        segments[2].corner = true;

        // Top-left
        segments[3].p.h = x;
        segments[3].p.v = y + height;
        segments[3].in = segments[3].out = segments[3].p;
        segments[3].corner = true;

        return FinalizePathArt(newArt, segments, 4, true);
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/art/ellipse — Create an ellipse path
// ---------------------------------------------------------------------------
std::string HandleCreateEllipse(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    AIReal cx = params.value("cx", 0.0);
    AIReal cy = params.value("cy", 0.0);
    AIReal rx = params.value("rx", 50.0);
    AIReal ry = params.value("ry", 50.0);

    if (rx <= 0 || ry <= 0) {
        return json{{"success", false},
                    {"error", "Radii must be positive"}}.dump();
    }

    json result = MainThreadDispatch::Run([cx, cy, rx, ry]() -> json {
        AIArtHandle newArt = nullptr;
        json err = CreateNewPathArt(newArt);
        if (!err.is_null()) return err;

        // Bezier approximation of ellipse using 4 segments
        // Kappa constant for circular arc approximation:
        //   k = 4 * (sqrt(2) - 1) / 3 ~= 0.5522847498
        constexpr AIReal kappa = 0.5522847498;

        AIReal kx = kappa * rx;  // control point offset in X
        AIReal ky = kappa * ry;  // control point offset in Y

        AIPathSegment segments[4];

        // Right point (3 o'clock)
        segments[0].p.h   = cx + rx;
        segments[0].p.v   = cy;
        segments[0].in.h  = cx + rx;
        segments[0].in.v  = cy - ky;
        segments[0].out.h = cx + rx;
        segments[0].out.v = cy + ky;
        segments[0].corner = false;

        // Top point (12 o'clock)
        segments[1].p.h   = cx;
        segments[1].p.v   = cy + ry;
        segments[1].in.h  = cx + kx;
        segments[1].in.v  = cy + ry;
        segments[1].out.h = cx - kx;
        segments[1].out.v = cy + ry;
        segments[1].corner = false;

        // Left point (9 o'clock)
        segments[2].p.h   = cx - rx;
        segments[2].p.v   = cy;
        segments[2].in.h  = cx - rx;
        segments[2].in.v  = cy + ky;
        segments[2].out.h = cx - rx;
        segments[2].out.v = cy - ky;
        segments[2].corner = false;

        // Bottom point (6 o'clock)
        segments[3].p.h   = cx;
        segments[3].p.v   = cy - ry;
        segments[3].in.h  = cx - kx;
        segments[3].in.v  = cy - ry;
        segments[3].out.h = cx + kx;
        segments[3].out.v = cy - ry;
        segments[3].corner = false;

        return FinalizePathArt(newArt, segments, 4, true);
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/art/path — Create a path from arbitrary segments
// ---------------------------------------------------------------------------
std::string HandleCreatePath(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    if (!params.contains("segments") || !params["segments"].is_array()) {
        return json{{"success", false},
                    {"error", "Missing required field: segments (array)"}}.dump();
    }

    if (params["segments"].empty()) {
        return json{{"success", false},
                    {"error", "Segments array must not be empty"}}.dump();
    }

    bool closed = params.value("closed", false);

    json result = MainThreadDispatch::Run([&params, closed]() -> json {
        AIArtHandle newArt = nullptr;
        json err = CreateNewPathArt(newArt);
        if (!err.is_null()) return err;

        const auto& segArray = params["segments"];
        std::vector<AIPathSegment> segs;
        segs.reserve(segArray.size());

        for (const auto& s : segArray) {
            segs.push_back(DeserializeSegment(s));
        }

        return FinalizePathArt(newArt, segs.data(),
                               static_cast<ai::int16>(segs.size()), closed);
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/art/line — Create a line (open 2-segment path)
// ---------------------------------------------------------------------------
std::string HandleCreateLine(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    AIReal x1 = params.value("x1", 0.0);
    AIReal y1 = params.value("y1", 0.0);
    AIReal x2 = params.value("x2", 100.0);
    AIReal y2 = params.value("y2", 100.0);

    json result = MainThreadDispatch::Run([x1, y1, x2, y2]() -> json {
        AIArtHandle newArt = nullptr;
        json err = CreateNewPathArt(newArt);
        if (!err.is_null()) return err;

        AIPathSegment segments[2];

        // Start point
        segments[0].p.h = x1;
        segments[0].p.v = y1;
        segments[0].in = segments[0].out = segments[0].p;
        segments[0].corner = true;

        // End point
        segments[1].p.h = x2;
        segments[1].p.v = y2;
        segments[1].in = segments[1].out = segments[1].p;
        segments[1].corner = true;

        // Lines are open paths (not closed)
        return FinalizePathArt(newArt, segments, 2, false);
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/dialog/save-file — Show native file save dialog
// ---------------------------------------------------------------------------
std::string HandleShowFileSaveDialog(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    json result = MainThreadDispatch::Run([&params]() -> json {
        if (!SuitePointers::AIUser()) {
            return {{"success", false},
                    {"error", "AIUser suite not available"}};
        }

        // Parse optional parameters
        std::string defaultName = params.value("defaultName", "");
        std::string title = params.value("title", "Save File");

        // Build the dialog title
        ai::UnicodeString dialogTitle(title.c_str());

        // Set up default file path with the default name
        ai::FilePath defaultPath;
        if (!defaultName.empty()) {
            ai::UnicodeString nameStr(defaultName.c_str());
            defaultPath.Set(nameStr);
        }

        // Build file filter string from fileTypes array if provided
        // AIUserSuite::PutFileDialog expects a filter string.
        // The format varies by platform; on macOS it's typically unused or
        // a simple extension filter.
        ai::UnicodeString filterStr;
        if (params.contains("fileTypes") && params["fileTypes"].is_array()) {
            std::string filter;
            for (const auto& ft : params["fileTypes"]) {
                if (ft.is_string()) {
                    if (!filter.empty()) filter += ";";
                    filter += "*." + ft.get<std::string>();
                }
            }
            filterStr = ai::UnicodeString(filter.c_str());
        }

        // Call AIUserSuite::PutFileDialog
        // Signature: PutFileDialog(const ai::UnicodeString& title,
        //                          const AIFileDialogFilters* filters,
        //                          const ai::UnicodeString& defaultName,
        //                          ai::FilePath& ioFilePath)
        // Returns kNoErr if user selected a file, kCanceledErr if cancelled
        ai::UnicodeString defaultNameStr(defaultName.c_str());
        ASErr err = SuitePointers::AIUser()->PutFileDialog(
            dialogTitle,
            nullptr,
            defaultNameStr,
            defaultPath);

        if (err == kCanceledErr) {
            return {{"success", true},
                    {"path", ""},
                    {"cancelled", true}};
        }

        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "PutFileDialog failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        std::string resultPath = defaultPath.GetFullPath().as_UTF8();

        return {{"success", true},
                {"path", resultPath},
                {"cancelled", false}};
    });
    return result.dump();
}

} // namespace NUXP
