/**
 * NUXP Text Endpoints
 *
 * Hand-written endpoints for Adobe Text Engine (ATE) text frame operations.
 * These endpoints are hand-written because the ATE headers (ATESuites.h)
 * pull in ~375KB of type definitions that conflict with AITypes.h, making
 * them incompatible with the auto-generated code pipeline.
 *
 * Strategy:
 *   - All ATE operations are delegated to ATEBridge (src/bridges/ATEBridge.h),
 *     which is compiled as an isolated translation unit that includes the real
 *     Adobe ATE headers (AITextFrame.h, ATESuites.h).
 *   - ATEBridge acquires suites using the real SDK constant names and version
 *     numbers, eliminating the fragile hand-rolled vtable approach.
 *   - No ATE types leak through the ATEBridge.h interface.
 *
 * Endpoints:
 *   POST /api/text/create            - Create a new point text frame
 *   GET  /api/text/{id}/content      - Get text content from a text frame
 *   POST /api/text/{id}/content      - Set text content on a text frame
 *
 * Handler declarations are generated in CustomRouteHandlers.h (namespace NUXP).
 * Implementations live here in TextEndpoints.cpp.
 *
 * See also: NUXPHandlers.cpp for the hand-written handler pattern.
 */

#ifndef NUXP_TEXT_ENDPOINTS_HPP
#define NUXP_TEXT_ENDPOINTS_HPP

#include <string>

namespace TextEndpoints {

/**
 * Register text endpoint routes with the HTTP server.
 *
 * DEPRECATED: These routes are now registered via generated
 * CustomRouteRegistration.cpp from routes.json. This function is
 * retained temporarily for backward compatibility but should be
 * removed once the generated registration is fully active.
 */
void RegisterRoutes();

} // namespace TextEndpoints

#endif // NUXP_TEXT_ENDPOINTS_HPP
