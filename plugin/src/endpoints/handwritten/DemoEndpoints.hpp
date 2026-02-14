/**
 * NUXP Demo Endpoints
 *
 * Hand-written endpoints that demonstrate real Adobe Illustrator SDK
 * integration. These endpoints are designed to be simple, reliable,
 * and serve as examples for building custom endpoints.
 *
 * Endpoints:
 *   GET  /demo/document-info   - Returns current document name, size
 *   GET  /demo/layers          - Lists all layers
 *   GET  /demo/selection       - Returns selected art info
 *   POST /demo/create-rectangle - Creates a rectangle in Illustrator
 */

#ifndef NUXP_DEMO_ENDPOINTS_HPP
#define NUXP_DEMO_ENDPOINTS_HPP

#include <nlohmann/json.hpp>

namespace DemoEndpoints {

using json = nlohmann::json;

/**
 * Get information about the current document.
 * Returns document name, width, height, and artboard count.
 *
 * GET /demo/document-info
 *
 * Response:
 * {
 *   "success": true,
 *   "document": {
 *     "name": "Untitled-1",
 *     "width": 612.0,
 *     "height": 792.0,
 *     "artboardCount": 1
 *   }
 * }
 */
json GetDocumentInfo();

/**
 * Get a list of all layers in the document.
 *
 * GET /demo/layers
 *
 * Response:
 * {
 *   "success": true,
 *   "layers": [
 *     { "name": "Layer 1", "visible": true, "locked": false }
 *   ]
 * }
 */
json GetLayers();

/**
 * Get information about the current selection.
 *
 * GET /demo/selection
 *
 * Response:
 * {
 *   "success": true,
 *   "selection": {
 *     "count": 2,
 *     "items": [
 *       { "type": "path", "bounds": {...} }
 *     ]
 *   }
 * }
 */
json GetSelection();

/**
 * Create a rectangle in the document.
 *
 * POST /demo/create-rectangle
 *
 * Request:
 * {
 *   "x": 100,
 *   "y": 100,
 *   "width": 200,
 *   "height": 100
 * }
 *
 * Response:
 * {
 *   "success": true,
 *   "artId": 123
 * }
 */
json CreateRectangle(const json &params);

/**
 * Register demo endpoints with the HTTP server.
 * Called from HttpServer::ConfigureRoutes().
 */
void RegisterRoutes();

} // namespace DemoEndpoints

#endif // NUXP_DEMO_ENDPOINTS_HPP
