/**
 * NUXP Selection Utilities
 *
 * Helper functions for working with Adobe Illustrator art selection.
 * Provides convenient wrappers around AIMatchingArtSuite and AIArtSuite
 * for common selection operations.
 *
 * Usage:
 *   #include "utils/SelectionUtils.hpp"
 *
 *   json selection = SelectionUtils::GetSelection();
 *   int count = SelectionUtils::GetSelectionCount();
 *   SelectionUtils::ClearSelection();
 *   int deleted = SelectionUtils::DeleteSelection();
 *   int selected = SelectionUtils::SelectByLayerName("My Layer");
 *   SelectionUtils::SelectArt(artHandle);
 */

#ifndef NUXP_SELECTION_UTILS_HPP
#define NUXP_SELECTION_UTILS_HPP

#include <nlohmann/json.hpp>
#include <string>

// Forward declare Adobe types
typedef struct _t_AIArtOpaque *AIArtHandle;

namespace SelectionUtils {

using json = nlohmann::json;

/**
 * Get info about currently selected art as JSON.
 *
 * Returns an array of objects with information about each selected item:
 * [
 *   {
 *     "type": "path",       // Art type: path, group, text, etc.
 *     "name": "Shape 1",    // Art item name (may be empty)
 *     "bounds": {           // Bounding box in document coordinates
 *       "left": 100.0,
 *       "top": 200.0,
 *       "right": 300.0,
 *       "bottom": 50.0,
 *       "width": 200.0,
 *       "height": 150.0
 *     }
 *   },
 *   ...
 * ]
 *
 * Returns empty array if no document is open or nothing is selected.
 */
json GetSelection();

/**
 * Get count of selected items.
 *
 * @return Number of selected art items, or 0 if none/error
 */
int GetSelectionCount();

/**
 * Clear all selection (deselect all art).
 * Safe to call even if nothing is selected.
 */
void ClearSelection();

/**
 * Delete all selected items.
 *
 * Items on locked layers are skipped. Returns the count of
 * items that were actually deleted.
 *
 * @return Number of items deleted
 */
int DeleteSelection();

/**
 * Select all art on a layer by the layer's name.
 *
 * Clears current selection first, then selects all art items
 * that belong to the specified layer.
 *
 * @param layerName The name of the layer whose art should be selected
 * @return Number of items selected, or 0 if layer not found/empty
 */
int SelectByLayerName(const std::string& layerName);

/**
 * Deselect all art and select only the specified art handle.
 *
 * @param art The art handle to select (must be valid)
 */
void SelectArt(AIArtHandle art);

} // namespace SelectionUtils

#endif // NUXP_SELECTION_UTILS_HPP
