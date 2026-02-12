/**
 * NUXP Custom Handler Implementations
 *
 * Hand-written handlers for SDK features that can't be auto-generated:
 *   - Selection access (AIMatchingArtSuite) — triple pointer patterns
 *   - Fill/Stroke colors (AIPathStyleSuite) — tagged union AIColor
 *   - Path geometry (AIPathSuite) — segment array I/O
 *
 * These implement the function signatures declared in generated/CustomRouteHandlers.h.
 * Route wiring is in generated/CustomRouteRegistration.cpp.
 */

#include "generated/CustomRouteHandlers.h"
#include "IllustratorSDK.h"
#include "SuitePointers.hpp"
#include "HandleManager.hpp"
#include "MainThreadDispatch.hpp"
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

namespace {

// ---------------------------------------------------------------------------
// Helper: Serialize AIColor tagged union to JSON
// ---------------------------------------------------------------------------
json SerializeColor(const AIColor& color) {
    switch (color.kind) {
    case kNoneColor:
        return {{"kind", "none"}};

    case kGrayColor:
        return {{"kind", "gray"},
                {"gray", color.c.g.gray}};

    case kThreeColor:
        return {{"kind", "rgb"},
                {"red", color.c.rgb.red},
                {"green", color.c.rgb.green},
                {"blue", color.c.rgb.blue}};

    case kFourColor:
        return {{"kind", "cmyk"},
                {"cyan", color.c.f.cyan},
                {"magenta", color.c.f.magenta},
                {"yellow", color.c.f.yellow},
                {"black", color.c.f.black}};

    case kCustomColor:
        return {{"kind", "custom"},
                {"tint", color.c.c.tint},
                {"color", HandleManager::customColors.Register(color.c.c.color)}};

    case kPattern:
        return {{"kind", "pattern"},
                {"pattern", HandleManager::patterns.Register(color.c.p.pattern)},
                {"shiftDist", color.c.p.shiftDist},
                {"shiftAngle", color.c.p.shiftAngle},
                {"scale", {{"h", color.c.p.scale.h}, {"v", color.c.p.scale.v}}},
                {"rotate", color.c.p.rotate},
                {"reflect", static_cast<bool>(color.c.p.reflect)},
                {"reflectAngle", color.c.p.reflectAngle},
                {"shearAngle", color.c.p.shearAngle},
                {"shearAxis", color.c.p.shearAxis}};

    case kGradient:
        return {{"kind", "gradient"},
                {"gradient", HandleManager::gradients.Register(color.c.b.gradient)},
                {"origin", {{"h", color.c.b.gradientOrigin.h},
                            {"v", color.c.b.gradientOrigin.v}}},
                {"angle", color.c.b.gradientAngle},
                {"length", color.c.b.gradientLength},
                {"hiliteAngle", color.c.b.hiliteAngle},
                {"hiliteLength", color.c.b.hiliteLength}};

    default:
        return {{"kind", "unknown"}};
    }
}

// ---------------------------------------------------------------------------
// Helper: Deserialize JSON to AIColor tagged union
// ---------------------------------------------------------------------------
AIColor DeserializeColor(const json& j) {
    AIColor color;
    color.Init();

    std::string kind = j.value("kind", "none");

    if (kind == "none") {
        color.kind = kNoneColor;
    } else if (kind == "gray") {
        color.kind = kGrayColor;
        color.c.g.gray = j.value("gray", 0.0);
    } else if (kind == "rgb") {
        color.kind = kThreeColor;
        color.c.rgb.red = j.value("red", 0.0);
        color.c.rgb.green = j.value("green", 0.0);
        color.c.rgb.blue = j.value("blue", 0.0);
    } else if (kind == "cmyk") {
        color.kind = kFourColor;
        color.c.f.cyan = j.value("cyan", 0.0);
        color.c.f.magenta = j.value("magenta", 0.0);
        color.c.f.yellow = j.value("yellow", 0.0);
        color.c.f.black = j.value("black", 0.0);
    } else if (kind == "custom") {
        color.kind = kCustomColor;
        color.c.c.tint = j.value("tint", 0.0);
        int colorId = j.value("color", -1);
        color.c.c.color = (colorId >= 0)
            ? HandleManager::customColors.Get(colorId) : nullptr;
    } else if (kind == "pattern") {
        color.kind = kPattern;
        int patId = j.value("pattern", -1);
        color.c.p.pattern = (patId >= 0)
            ? HandleManager::patterns.Get(patId) : nullptr;
        color.c.p.shiftDist = j.value("shiftDist", 0.0);
        color.c.p.shiftAngle = j.value("shiftAngle", 0.0);
        if (j.contains("scale")) {
            color.c.p.scale.h = j["scale"].value("h", 1.0);
            color.c.p.scale.v = j["scale"].value("v", 1.0);
        }
        color.c.p.rotate = j.value("rotate", 0.0);
        color.c.p.reflect = j.value("reflect", false);
        color.c.p.reflectAngle = j.value("reflectAngle", 0.0);
        color.c.p.shearAngle = j.value("shearAngle", 0.0);
        color.c.p.shearAxis = j.value("shearAxis", 0.0);
    } else if (kind == "gradient") {
        color.kind = kGradient;
        int gradId = j.value("gradient", -1);
        color.c.b.gradient = (gradId >= 0)
            ? HandleManager::gradients.Get(gradId) : nullptr;
        if (j.contains("origin")) {
            color.c.b.gradientOrigin.h = j["origin"].value("h", 0.0);
            color.c.b.gradientOrigin.v = j["origin"].value("v", 0.0);
        }
        color.c.b.gradientAngle = j.value("angle", 0.0);
        color.c.b.gradientLength = j.value("length", 0.0);
        color.c.b.hiliteAngle = j.value("hiliteAngle", 0.0);
        color.c.b.hiliteLength = j.value("hiliteLength", 0.0);
    }

    return color;
}

// ---------------------------------------------------------------------------
// Helper: Serialize AIPathSegment to JSON
// ---------------------------------------------------------------------------
json SerializeSegment(const AIPathSegment& seg) {
    return {{"p",   {{"h", seg.p.h},   {"v", seg.p.v}}},
            {"in",  {{"h", seg.in.h},  {"v", seg.in.v}}},
            {"out", {{"h", seg.out.h}, {"v", seg.out.v}}},
            {"corner", static_cast<bool>(seg.corner)}};
}

// ---------------------------------------------------------------------------
// Helper: Deserialize JSON to AIPathSegment
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

} // anonymous namespace

// ===========================================================================
// NUXP Handler Implementations
// ===========================================================================

namespace NUXP {

// ---------------------------------------------------------------------------
// GET /api/selection — Get all currently selected art as handle IDs
// ---------------------------------------------------------------------------
std::string HandleGetSelection() {
    json result = MainThreadDispatch::Run([]() -> json {
        if (!SuitePointers::AIMatchingArt()) {
            return {{"success", false},
                    {"error", "AIMatchingArt suite not available"}};
        }

        AIArtHandle** matches = nullptr;
        ai::int32 numMatches = 0;

        ASErr err = SuitePointers::AIMatchingArt()->GetSelectedArt(
            &matches, &numMatches);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetSelectedArt failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        json handles = json::array();
        if (matches != nullptr && numMatches > 0) {
            for (ai::int32 i = 0; i < numMatches; ++i) {
                AIArtHandle art = (*matches)[i];
                if (art != nullptr) {
                    handles.push_back(HandleManager::art.Register(art));
                }
            }
            // Free SDK-allocated memory block
            if (SuitePointers::AIMdMemory()) {
                SuitePointers::AIMdMemory()->MdMemoryDisposeHandle(
                    reinterpret_cast<AIMdMemoryHandle>(matches));
            }
        }

        return {{"success", true},
                {"handles", handles},
                {"count", static_cast<int>(numMatches)}};
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/selection/match — Get art matching type/attribute filters
// ---------------------------------------------------------------------------
std::string HandleGetMatchingArt(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    if (!params.contains("type")) {
        return json{{"success", false},
                    {"error", "Missing required field: type"}}.dump();
    }

    json result = MainThreadDispatch::Run([&params]() -> json {
        if (!SuitePointers::AIMatchingArt()) {
            return {{"success", false},
                    {"error", "AIMatchingArt suite not available"}};
        }

        AIMatchingArtSpec spec;
        spec.type      = params["type"].get<ai::int16>();
        spec.whichAttr = params.value("whichAttr", 0);
        spec.attr      = params.value("attr", 0);

        AIArtHandle** matches = nullptr;
        ai::int32 numMatches = 0;

        ASErr err = SuitePointers::AIMatchingArt()->GetMatchingArt(
            &spec, 1, &matches, &numMatches);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetMatchingArt failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        json handles = json::array();
        if (matches != nullptr && numMatches > 0) {
            for (ai::int32 i = 0; i < numMatches; ++i) {
                AIArtHandle art = (*matches)[i];
                if (art != nullptr) {
                    handles.push_back(HandleManager::art.Register(art));
                }
            }
            if (SuitePointers::AIMdMemory()) {
                SuitePointers::AIMdMemory()->MdMemoryDisposeHandle(
                    reinterpret_cast<AIMdMemoryHandle>(matches));
            }
        }

        return {{"success", true},
                {"handles", handles},
                {"count", static_cast<int>(numMatches)}};
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// GET /api/art/{id}/style — Get fill/stroke style of an art object
// ---------------------------------------------------------------------------
std::string HandleGetPathStyle(const std::string& id) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

    json result = MainThreadDispatch::Run([artId]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }
        if (!SuitePointers::AIPathStyle()) {
            return {{"success", false},
                    {"error", "AIPathStyle suite not available"}};
        }

        AIPathStyle style;
        style.Init();
        AIBoolean hasAdvFill = false;

        ASErr err = SuitePointers::AIPathStyle()->GetPathStyle(
            art, &style, &hasAdvFill);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetPathStyle failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        // Serialize dash array
        json dashArray = json::array();
        for (ai::int16 i = 0; i < style.stroke.dash.length; ++i) {
            dashArray.push_back(
                static_cast<double>(style.stroke.dash.array[i]));
        }

        return {{"success", true},
                {"fillPaint", static_cast<bool>(style.fillPaint)},
                {"strokePaint", static_cast<bool>(style.strokePaint)},
                {"fill", {
                    {"color", SerializeColor(style.fill.color)},
                    {"overprint", static_cast<bool>(style.fill.overprint)}}},
                {"stroke", {
                    {"color", SerializeColor(style.stroke.color)},
                    {"overprint", static_cast<bool>(style.stroke.overprint)},
                    {"width", style.stroke.width},
                    {"cap", static_cast<int>(style.stroke.cap)},
                    {"join", static_cast<int>(style.stroke.join)},
                    {"miterLimit", style.stroke.miterLimit},
                    {"dash", {
                        {"length", style.stroke.dash.length},
                        {"offset", static_cast<double>(style.stroke.dash.offset)},
                        {"array", dashArray}}}}},
                {"evenodd", static_cast<bool>(style.evenodd)},
                {"resolution", style.resolution}};
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/art/{id}/style — Set fill/stroke style (partial update)
// ---------------------------------------------------------------------------
std::string HandleSetPathStyle(const std::string& id, const std::string& body) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    json result = MainThreadDispatch::Run([artId, &params]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }
        if (!SuitePointers::AIPathStyle()) {
            return {{"success", false},
                    {"error", "AIPathStyle suite not available"}};
        }

        // Get current style so we only overlay provided fields
        AIPathStyle style;
        style.Init();
        AIBoolean hasAdvFill = false;

        ASErr err = SuitePointers::AIPathStyle()->GetPathStyle(
            art, &style, &hasAdvFill);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetPathStyle failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        // Overlay top-level booleans
        if (params.contains("fillPaint"))
            style.fillPaint = params["fillPaint"].get<bool>();
        if (params.contains("strokePaint"))
            style.strokePaint = params["strokePaint"].get<bool>();
        if (params.contains("evenodd"))
            style.evenodd = params["evenodd"].get<bool>();

        // Overlay fill properties
        if (params.contains("fill")) {
            const auto& fill = params["fill"];
            if (fill.contains("color"))
                style.fill.color = DeserializeColor(fill["color"]);
            if (fill.contains("overprint"))
                style.fill.overprint = fill["overprint"].get<bool>();
        }

        // Overlay stroke properties
        if (params.contains("stroke")) {
            const auto& stroke = params["stroke"];
            if (stroke.contains("color"))
                style.stroke.color = DeserializeColor(stroke["color"]);
            if (stroke.contains("overprint"))
                style.stroke.overprint = stroke["overprint"].get<bool>();
            if (stroke.contains("width"))
                style.stroke.width = stroke["width"].get<double>();
            if (stroke.contains("cap"))
                style.stroke.cap =
                    static_cast<AILineCap>(stroke["cap"].get<int>());
            if (stroke.contains("join"))
                style.stroke.join =
                    static_cast<AILineJoin>(stroke["join"].get<int>());
            if (stroke.contains("miterLimit"))
                style.stroke.miterLimit = stroke["miterLimit"].get<double>();
            if (stroke.contains("dash")) {
                const auto& dash = stroke["dash"];
                if (dash.contains("length"))
                    style.stroke.dash.length = dash["length"].get<ai::int16>();
                if (dash.contains("offset"))
                    style.stroke.dash.offset =
                        static_cast<AIFloat>(dash["offset"].get<double>());
                if (dash.contains("array") && dash["array"].is_array()) {
                    const auto& arr = dash["array"];
                    for (size_t i = 0;
                         i < arr.size() && i < kMaxDashComponents; ++i) {
                        style.stroke.dash.array[i] =
                            static_cast<AIFloat>(arr[i].get<double>());
                    }
                }
            }
        }

        err = SuitePointers::AIPathStyle()->SetPathStyle(art, &style);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "SetPathStyle failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        return {{"success", true}};
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// GET /api/art/{id}/segments — Get path segment geometry
// ---------------------------------------------------------------------------
std::string HandleGetPathSegments(const std::string& id) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

    json result = MainThreadDispatch::Run([artId]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }
        if (!SuitePointers::AIPath()) {
            return {{"success", false},
                    {"error", "AIPath suite not available"}};
        }

        ai::int16 count = 0;
        ASErr err = SuitePointers::AIPath()->GetPathSegmentCount(art, &count);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetPathSegmentCount failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        AIBoolean closed = false;
        err = SuitePointers::AIPath()->GetPathClosed(art, &closed);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetPathClosed failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        std::vector<AIPathSegment> segs(count);
        if (count > 0) {
            err = SuitePointers::AIPath()->GetPathSegments(
                art, 0, count, segs.data());
            if (err != kNoErr) {
                return {{"success", false},
                        {"error", "GetPathSegments failed"},
                        {"errorCode", static_cast<int>(err)}};
            }
        }

        json segments = json::array();
        for (ai::int16 i = 0; i < count; ++i) {
            segments.push_back(SerializeSegment(segs[i]));
        }

        return {{"success", true},
                {"count", static_cast<int>(count)},
                {"closed", static_cast<bool>(closed)},
                {"segments", segments}};
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/art/{id}/segments — Set path segments (partial or full)
// ---------------------------------------------------------------------------
std::string HandleSetPathSegments(const std::string& id, const std::string& body) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

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

    json result = MainThreadDispatch::Run([artId, &params]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }
        if (!SuitePointers::AIPath()) {
            return {{"success", false},
                    {"error", "AIPath suite not available"}};
        }

        ai::int16 startIndex = params.value("startIndex", 0);
        const auto& segArray = params["segments"];

        std::vector<AIPathSegment> segs;
        segs.reserve(segArray.size());
        for (const auto& s : segArray) {
            segs.push_back(DeserializeSegment(s));
        }

        ASErr err = SuitePointers::AIPath()->SetPathSegments(
            art, startIndex, static_cast<ai::int16>(segs.size()), segs.data());
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "SetPathSegments failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        if (params.contains("closed")) {
            err = SuitePointers::AIPath()->SetPathClosed(
                art, params["closed"].get<bool>());
            if (err != kNoErr) {
                return {{"success", false},
                        {"error", "SetPathClosed failed"},
                        {"errorCode", static_cast<int>(err)}};
            }
        }

        return {{"success", true}};
    });
    return result.dump();
}

} // namespace NUXP
