/**
 * NUXP XMP Endpoints Implementation
 *
 * HTTP endpoints for reading and writing XMP metadata on Illustrator documents.
 * All SDK calls are dispatched to the main thread via MainThreadDispatch::Run().
 *
 * Handler declarations are generated in CustomRouteHandlers.h (namespace NUXP).
 * Route registration is handled by generated/CustomRouteRegistration.cpp.
 *
 * Document-level XMP (full packet) is always available through the Illustrator
 * SDK's AIDocumentSuite. Property-level XMP (individual properties) requires
 * the optional XMP Toolkit SDK.
 */

#include "XMPEndpoints.hpp"
#include "HttpServer.hpp"
#include "MainThreadDispatch.hpp"
#include "utils/XMPUtils.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ============================================================================
// Handler Implementations (NUXP namespace)
// ============================================================================
// These handlers implement the declarations in generated/CustomRouteHandlers.h.
// Function names match the generated names (Handle + PascalCase(route.name)).

namespace NUXP {

// ---------------------------------------------------------------------------
// GET /api/xmp/status - Check XMP availability and capabilities
// ---------------------------------------------------------------------------
std::string HandleGetXmpStatus() {
    json result = MainThreadDispatch::Run([]() -> json {
        return {
            {"success", true},
            {"available", XMPUtils::IsAvailable()},
            {"propertyAccess", XMPUtils::HasPropertyAccess()},
            {"capabilities", {
                {"getDocumentXMP", XMPUtils::IsAvailable()},
                {"setDocumentXMP", XMPUtils::IsAvailable()},
                {"getProperty", XMPUtils::HasPropertyAccess()},
                {"setProperty", XMPUtils::HasPropertyAccess()},
                {"registerNamespace", XMPUtils::HasPropertyAccess()}
            }}
        };
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// GET /api/xmp - Get full document XMP metadata
// ---------------------------------------------------------------------------
std::string HandleGetDocumentXmp() {
    json result = MainThreadDispatch::Run([]() -> json {
        if (!XMPUtils::IsAvailable()) {
            return {
                {"success", false},
                {"error", "XMP is not available (AIDocument suite not acquired)"}
            };
        }

        std::string xmp = XMPUtils::GetDocumentXMP();
        if (xmp.empty()) {
            return {
                {"success", true},
                {"xmp", nullptr},
                {"message", "No XMP metadata found on current document"}
            };
        }

        return {
            {"success", true},
            {"xmp", xmp}
        };
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/xmp - Set full document XMP metadata
// ---------------------------------------------------------------------------
std::string HandleSetDocumentXmp(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{
            {"success", false},
            {"error", std::string("Invalid JSON: ") + e.what()}
        }.dump();
    }

    if (!params.contains("xmp")) {
        return json{
            {"success", false},
            {"error", "Missing required field: 'xmp' (string with XML packet, or null to clear)"}
        }.dump();
    }

    // Allow null to clear metadata
    std::string xmpString;
    if (!params["xmp"].is_null()) {
        if (!params["xmp"].is_string()) {
            return json{
                {"success", false},
                {"error", "Field 'xmp' must be a string (XML packet) or null"}
            }.dump();
        }
        xmpString = params["xmp"].get<std::string>();
    }

    json result = MainThreadDispatch::Run([&xmpString]() -> json {
        if (!XMPUtils::IsAvailable()) {
            return {
                {"success", false},
                {"error", "XMP is not available (AIDocument suite not acquired)"}
            };
        }

        bool ok = XMPUtils::SetDocumentXMP(xmpString);
        if (!ok) {
            return {
                {"success", false},
                {"error", "Failed to set document XMP metadata"}
            };
        }

        return {
            {"success", true},
            {"message", xmpString.empty() ? "XMP metadata cleared" : "XMP metadata updated"}
        };
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/xmp/property - Get a specific XMP property
//   Uses POST because GET with body is non-standard.
// ---------------------------------------------------------------------------
std::string HandleGetXmpProperty(const std::string& body) {
    json params;
    if (!body.empty()) {
        try {
            params = json::parse(body);
        } catch (const json::parse_error& e) {
            return json{
                {"success", false},
                {"error", std::string("Invalid JSON: ") + e.what()}
            }.dump();
        }
    }

    if (!params.contains("namespace") || !params.contains("name")) {
        return json{
            {"success", false},
            {"error", "Missing required fields: 'namespace' (URI) and 'name' (property name)"}
        }.dump();
    }

    std::string namespaceURI = params["namespace"].get<std::string>();
    std::string propertyName = params["name"].get<std::string>();

    json result = MainThreadDispatch::Run([&namespaceURI, &propertyName]() -> json {
        if (!XMPUtils::HasPropertyAccess()) {
            return {
                {"success", false},
                {"error", "Property-level XMP access requires the XMP Toolkit SDK. "
                          "Use GET /api/xmp to retrieve the full XMP packet instead."}
            };
        }

        std::string value = XMPUtils::GetProperty(namespaceURI, propertyName);

        return {
            {"success", true},
            {"namespace", namespaceURI},
            {"name", propertyName},
            {"value", value.empty() ? json(nullptr) : json(value)},
            {"found", !value.empty()}
        };
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/xmp/property/set - Set a specific XMP property
// ---------------------------------------------------------------------------
std::string HandleSetXmpProperty(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{
            {"success", false},
            {"error", std::string("Invalid JSON: ") + e.what()}
        }.dump();
    }

    if (!params.contains("namespace") || !params.contains("name") || !params.contains("value")) {
        return json{
            {"success", false},
            {"error", "Missing required fields: 'namespace' (URI), 'name' (property name), 'value' (string)"}
        }.dump();
    }

    std::string namespaceURI = params["namespace"].get<std::string>();
    std::string propertyName = params["name"].get<std::string>();
    std::string value = params["value"].get<std::string>();

    json result = MainThreadDispatch::Run([&namespaceURI, &propertyName, &value]() -> json {
        if (!XMPUtils::HasPropertyAccess()) {
            return {
                {"success", false},
                {"error", "Property-level XMP access requires the XMP Toolkit SDK. "
                          "Use POST /api/xmp to set the full XMP packet instead."}
            };
        }

        bool ok = XMPUtils::SetProperty(namespaceURI, propertyName, value);
        if (!ok) {
            return {
                {"success", false},
                {"error", "Failed to set XMP property"},
                {"namespace", namespaceURI},
                {"name", propertyName}
            };
        }

        return {
            {"success", true},
            {"namespace", namespaceURI},
            {"name", propertyName},
            {"value", value}
        };
    });
    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/xmp/namespace - Register a custom XMP namespace
// ---------------------------------------------------------------------------
std::string HandleRegisterXmpNamespace(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{
            {"success", false},
            {"error", std::string("Invalid JSON: ") + e.what()}
        }.dump();
    }

    if (!params.contains("uri") || !params.contains("prefix")) {
        return json{
            {"success", false},
            {"error", "Missing required fields: 'uri' (namespace URI) and 'prefix' (suggested prefix)"}
        }.dump();
    }

    std::string uri = params["uri"].get<std::string>();
    std::string prefix = params["prefix"].get<std::string>();

    json result = MainThreadDispatch::Run([&uri, &prefix]() -> json {
        if (!XMPUtils::HasPropertyAccess()) {
            return {
                {"success", false},
                {"error", "Namespace registration requires the XMP Toolkit SDK"}
            };
        }

        std::string actualPrefix = XMPUtils::RegisterNamespace(uri, prefix);
        if (actualPrefix.empty()) {
            return {
                {"success", false},
                {"error", "Failed to register namespace"},
                {"uri", uri}
            };
        }

        return {
            {"success", true},
            {"uri", uri},
            {"requestedPrefix", prefix},
            {"actualPrefix", actualPrefix}
        };
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

namespace XMPEndpoints {

void RegisterRoutes() {
    // NOTE: These routes are now also registered by CustomRouteRegistration.cpp.
    // This function should be removed once the generated registration is
    // confirmed working. Do NOT call both RegisterRoutes() and
    // RegisterCustomRoutes() for the same routes — it will cause duplicate
    // route registration.

    HttpServer::Get("/api/xmp", [](const std::string&) {
        return NUXP::HandleGetDocumentXmp();
    });
    HttpServer::Post("/api/xmp", [](const std::string& body) {
        return NUXP::HandleSetDocumentXmp(body);
    });
    HttpServer::Get("/api/xmp/status", [](const std::string&) {
        return NUXP::HandleGetXmpStatus();
    });
    HttpServer::Post("/api/xmp/property", [](const std::string& body) {
        return NUXP::HandleGetXmpProperty(body);
    });
    HttpServer::Post("/api/xmp/property/set", [](const std::string& body) {
        return NUXP::HandleSetXmpProperty(body);
    });
    HttpServer::Post("/api/xmp/namespace", [](const std::string& body) {
        return NUXP::HandleRegisterXmpNamespace(body);
    });
}

} // namespace XMPEndpoints
