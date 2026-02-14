/**
 * NUXP XMP Endpoints
 *
 * HTTP endpoints for reading and writing XMP metadata on Illustrator documents.
 *
 * Endpoints:
 *   GET  /api/xmp              - Get full document XMP metadata (XML packet)
 *   POST /api/xmp              - Set full document XMP metadata (XML packet)
 *   GET  /api/xmp/status       - Check XMP availability and capabilities
 *   POST /api/xmp/property     - Get a specific XMP property (body: namespace, name)
 *   POST /api/xmp/property/set - Set a specific XMP property (body: namespace, name, value)
 *   POST /api/xmp/namespace    - Register a custom XMP namespace (body: uri, prefix)
 *
 * Handler declarations are generated in CustomRouteHandlers.h (namespace NUXP).
 * Implementations live here in XMPEndpoints.cpp.
 *
 * Document-level XMP (get/set full packet) is always available.
 * Property-level XMP (get/set individual properties) requires the XMP Toolkit SDK.
 */

#ifndef NUXP_XMP_ENDPOINTS_HPP
#define NUXP_XMP_ENDPOINTS_HPP

namespace XMPEndpoints {

/**
 * Register all XMP endpoints with the HTTP server.
 *
 * DEPRECATED: These routes are now registered via generated
 * CustomRouteRegistration.cpp from routes.json. This function is
 * retained temporarily for backward compatibility but should be
 * removed once the generated registration is fully active.
 */
void RegisterRoutes();

} // namespace XMPEndpoints

#endif // NUXP_XMP_ENDPOINTS_HPP
