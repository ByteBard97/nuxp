/**
 * DocumentUtils - Document Information Utilities
 *
 * Provides helper functions for retrieving document information
 * from Adobe Illustrator. All functions must be called from the
 * main thread (via MainThreadDispatch if called from HTTP handlers).
 *
 * Usage:
 *   // From main thread or via MainThreadDispatch::Run
 *   json docInfo = DocumentUtils::GetDocumentInfo();
 *   json artboards = DocumentUtils::GetArtboards();
 */

#ifndef NUXP_DOCUMENT_UTILS_HPP
#define NUXP_DOCUMENT_UTILS_HPP

#include <nlohmann/json.hpp>
#include <string>

namespace DocumentUtils {

using json = nlohmann::json;

/**
 * Check if a document is currently open in Illustrator.
 *
 * @return true if at least one document is open
 */
bool HasDocument();

/**
 * Get current document information as JSON.
 *
 * Returns:
 * {
 *   "name": "document.ai",
 *   "path": "/path/to/document.ai",
 *   "width": 612.0,
 *   "height": 792.0,
 *   "units": "points"
 * }
 *
 * If no document is open, returns:
 * { "error": "no_document" }
 *
 * @return JSON object with document info
 */
json GetDocumentInfo();

/**
 * Get all artboards information as JSON array.
 *
 * Returns:
 * {
 *   "artboards": [
 *     {
 *       "index": 0,
 *       "name": "Artboard 1",
 *       "bounds": {
 *         "left": 0.0,
 *         "top": 792.0,
 *         "right": 612.0,
 *         "bottom": 0.0
 *       },
 *       "active": true
 *     },
 *     ...
 *   ],
 *   "count": 1,
 *   "activeIndex": 0
 * }
 *
 * If no document is open, returns:
 * { "error": "no_document" }
 *
 * @return JSON object with artboards array
 */
json GetArtboards();

/**
 * Get document ruler units as string.
 *
 * Possible return values:
 * - "inches"
 * - "points"
 * - "centimeters"
 * - "millimeters"
 * - "picas"
 * - "pixels"
 * - "unknown"
 *
 * @return String representation of ruler units
 */
std::string GetRulerUnits();

/**
 * Get available fonts as JSON array.
 *
 * Returns:
 * {
 *   "fonts": [
 *     { "name": "Arial" },
 *     { "name": "Helvetica" },
 *     ...
 *   ],
 *   "count": 500,
 *   "truncated": false
 * }
 *
 * Note: Font list is limited to 500 entries to avoid very long responses.
 * The "truncated" field indicates if there are more fonts available.
 *
 * @return JSON object with fonts array
 */
json GetFonts();

} // namespace DocumentUtils

#endif // NUXP_DOCUMENT_UTILS_HPP
