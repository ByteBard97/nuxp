/**
 * NUXP Layer Utilities
 *
 * Helper functions for working with Adobe Illustrator layers.
 * Provides convenient wrappers around the AILayerSuite for common operations.
 *
 * Usage:
 *   json layers = LayerUtils::GetLayers();
 *   AILayerHandle layer = LayerUtils::GetOrCreateLayer("My Layer");
 *   LayerUtils::SetCurrentLayer(layer);
 */

#ifndef NUXP_LAYER_UTILS_HPP
#define NUXP_LAYER_UTILS_HPP

#include <nlohmann/json.hpp>
#include <string>

// Forward declare Adobe types
typedef struct _t_AILayerOpaque *AILayerHandle;

namespace LayerUtils {

using json = nlohmann::json;

/**
 * Get all layers as a JSON array.
 *
 * Returns:
 * [
 *   { "name": "Layer 1", "visible": true, "locked": false, "index": 0 },
 *   { "name": "Layer 2", "visible": true, "locked": true, "index": 1 }
 * ]
 *
 * Returns empty array if AILayerSuite is not available or no document is open.
 */
json GetLayers();

/**
 * Get a layer by its name/title.
 *
 * @param name The layer name to search for
 * @return The layer handle, or nullptr if not found
 */
AILayerHandle GetLayerByName(const std::string &name);

/**
 * Get an existing layer by name, or create it if it doesn't exist.
 * New layers are created at the top of the layer stack.
 *
 * @param name The layer name
 * @return The layer handle (existing or newly created), or nullptr on error
 */
AILayerHandle GetOrCreateLayer(const std::string &name);

/**
 * Get the currently active/current layer.
 *
 * @return The current layer handle, or nullptr if none/error
 */
AILayerHandle GetCurrentLayer();

/**
 * Set a layer as the current/active layer.
 * Art created after this call will be placed on this layer.
 *
 * @param layer The layer to make current
 */
void SetCurrentLayer(AILayerHandle layer);

/**
 * Get the number of layers in the current document.
 *
 * @return Layer count, or 0 if no document/error
 */
int GetLayerCount();

} // namespace LayerUtils

#endif // NUXP_LAYER_UTILS_HPP
