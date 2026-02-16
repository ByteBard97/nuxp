/**
 * NUXP Text Endpoint Implementations
 *
 * Hand-written endpoints for ATE text frame operations.
 *
 * ARCHITECTURE NOTE:
 * All ATE (Adobe Text Engine) operations are delegated to ATEBridge, which
 * is compiled as a separate translation unit that includes the real Adobe ATE
 * headers (AITextFrame.h, ATESuites.h). This avoids the type conflicts that
 * would occur if those headers were included alongside IllustratorSDK.h.
 *
 * See ATEBridge.h for the clean API and ATEBridge.cpp for the implementation.
 */

#include "TextEndpoints.hpp"
#include "ATEBridge.h"
#include "HandleManager.hpp"
#include "HttpServer.hpp"
#include "MainThreadDispatch.hpp"
#include "SuitePointers.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

// ============================================================================
// Handler Implementations
// ============================================================================
// These handlers implement the declarations in generated/CustomRouteHandlers.h
// which places them in the NUXP namespace. Route registration is handled by
// generated/CustomRouteRegistration.cpp.

namespace NUXP {

// ---------------------------------------------------------------------------
// POST /api/text/create — Create a new point text frame
// ---------------------------------------------------------------------------
std::string HandleCreateTextFrame(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    // x and y are required for meaningful text placement
    if (!params.contains("x") || !params.contains("y")) {
        return json{{"success", false},
                    {"error", "Missing required fields: x, y"}}.dump();
    }

    json result = MainThreadDispatch::Run([&params]() -> json {
        // Parse anchor point
        AIRealPoint anchor;
        anchor.h = params["x"].get<AIReal>();
        anchor.v = params["y"].get<AIReal>();

        // Parse orientation: 0 = horizontal (default), 1 = vertical
        ai::int16 orientation = static_cast<ai::int16>(
            params.value("orientation", 0));

        // Create the point text frame at the top of the paint order
        AIArtHandle newTextFrame = nullptr;
        ASErr err = ATEBridge::NewPointText(
            kPlaceAboveAll,     // paint order
            nullptr,            // prep object (ignored with kPlaceAboveAll)
            orientation,        // text orientation
            anchor,             // anchor point
            &newTextFrame       // output handle
        );

        if (err != kNoErr || newTextFrame == nullptr) {
            return {{"success", false},
                    {"error", "NewPointText failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        // Register the handle for cross-thread access
        int32_t artId = HandleManager::art.Register(newTextFrame);

        // Set initial text content if provided
        if (params.contains("contents") && params["contents"].is_string()) {
            std::string contents = params["contents"].get<std::string>();
            if (!contents.empty()) {
                std::string setError;
                if (!ATEBridge::SetTextContent(newTextFrame, contents, setError)) {
                    // Frame was created but content could not be set
                    return {{"success", true},
                            {"artId", artId},
                            {"warning", "Text frame created but content "
                                        "could not be set: " + setError}};
                }
            }
        }

        return {{"success", true},
                {"artId", artId}};
    });

    return result.dump();
}

// ---------------------------------------------------------------------------
// GET /api/text/{id}/content — Get text content from a text frame
// ---------------------------------------------------------------------------
std::string HandleGetTextContent(const std::string& id) {
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

        // Verify this is a text frame art object
        if (!SuitePointers::AIArt()) {
            return {{"success", false},
                    {"error", "AIArt suite not available"}};
        }
        short artType = kUnknownArt;
        SuitePointers::AIArt()->GetArtType(art, &artType);
        if (artType != kTextFrameArt) {
            return {{"success", false},
                    {"error", "Art object is not a text frame"},
                    {"artType", static_cast<int>(artType)}};
        }

        // Extract text content via ATEBridge
        std::string contents;
        std::string getError;
        if (!ATEBridge::GetTextContent(art, contents, getError)) {
            return {{"success", false},
                    {"error", getError},
                    {"artId", artId}};
        }

        return {{"success", true},
                {"artId", artId},
                {"contents", contents}};
    });

    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/text/{id}/content — Set text content on a text frame
// ---------------------------------------------------------------------------
std::string HandleSetTextContent(const std::string& id, const std::string& body) {
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

    if (!params.contains("contents") || !params["contents"].is_string()) {
        return json{{"success", false},
                    {"error", "Missing required field: contents (string)"}}.dump();
    }

    json result = MainThreadDispatch::Run([artId, &params]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }

        // Verify this is a text frame art object
        if (!SuitePointers::AIArt()) {
            return {{"success", false},
                    {"error", "AIArt suite not available"}};
        }
        short artType = kUnknownArt;
        SuitePointers::AIArt()->GetArtType(art, &artType);
        if (artType != kTextFrameArt) {
            return {{"success", false},
                    {"error", "Art object is not a text frame"},
                    {"artType", static_cast<int>(artType)}};
        }

        // Set text content via ATEBridge
        std::string contents = params["contents"].get<std::string>();
        std::string setError;
        if (!ATEBridge::SetTextContent(art, contents, setError)) {
            return {{"success", false},
                    {"error", setError},
                    {"artId", artId}};
        }

        return {{"success", true},
                {"artId", artId}};
    });

    return result.dump();
}

} // namespace NUXP

// ============================================================================
// Legacy Route Registration
// ============================================================================
// DEPRECATED: These routes are now registered via generated
// CustomRouteRegistration.cpp from routes.json. This function is retained
// temporarily for backward compatibility but should be removed once the
// generated registration is fully active.

namespace TextEndpoints {

void RegisterRoutes() {
    // NOTE: These routes are now also registered by CustomRouteRegistration.cpp.
    // This function should be removed once the generated registration is
    // confirmed working. Do NOT call both RegisterRoutes() and
    // RegisterCustomRoutes() for the same routes — it will cause duplicate
    // route registration.

    // POST /api/text/create - Create a new point text frame
    HttpServer::Post("/api/text/create", [](const std::string& body) {
        return NUXP::HandleCreateTextFrame(body);
    });

    // GET /api/text/{id}/content - Get text content from a text frame
    HttpServer::GetWithPattern(
        R"(/api/text/([^/]+)/content)",
        [](const std::string& /* body */,
           const std::vector<std::string>& params) {
            if (params.empty()) {
                return std::string(
                    R"({"success":false,"error":"missing text frame ID"})");
            }
            return NUXP::HandleGetTextContent(params[0]);
        });

    // POST /api/text/{id}/content - Set text content on a text frame
    HttpServer::PostWithPattern(
        R"(/api/text/([^/]+)/content)",
        [](const std::string& body,
           const std::vector<std::string>& params) {
            if (params.empty()) {
                return std::string(
                    R"({"success":false,"error":"missing text frame ID"})");
            }
            return NUXP::HandleSetTextContent(params[0], body);
        });
}

} // namespace TextEndpoints
