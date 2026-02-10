/**
 * NUXP Layer Utilities Implementation
 *
 * Provides helper functions for working with Adobe Illustrator layers.
 */

#include "LayerUtils.hpp"
#include "../SuitePointers.hpp"
#include "IllustratorSDK.h"

namespace LayerUtils {

// -------------------------------------------------------------------------
// GetLayers
// -------------------------------------------------------------------------

json GetLayers() {
  json layers = json::array();

  if (!SuitePointers::AILayer()) {
    return layers;
  }

  // Get layer count
  ai::int32 layerCount = 0;
  ASErr err = SuitePointers::AILayer()->CountLayers(&layerCount);
  if (err != kNoErr) {
    return layers;
  }

  // Iterate through layers
  for (ai::int32 i = 0; i < layerCount; ++i) {
    AILayerHandle layer = nullptr;
    err = SuitePointers::AILayer()->GetNthLayer(i, &layer);
    if (err != kNoErr || layer == nullptr) {
      continue;
    }

    // Get layer title
    ai::UnicodeString layerTitle;
    err = SuitePointers::AILayer()->GetLayerTitle(layer, layerTitle);
    std::string name = "Untitled";
    if (err == kNoErr) {
      name = layerTitle.as_UTF8();
    }

    // Get layer visibility
    AIBoolean visible = true;
    SuitePointers::AILayer()->GetLayerVisible(layer, &visible);

    // Get layer lock state (editable = not locked)
    AIBoolean editable = true;
    SuitePointers::AILayer()->GetLayerEditable(layer, &editable);

    layers.push_back({{"name", name},
                      {"visible", visible != 0},
                      {"locked", editable == 0},
                      {"index", i}});
  }

  return layers;
}

// -------------------------------------------------------------------------
// GetLayerByName
// -------------------------------------------------------------------------

AILayerHandle GetLayerByName(const std::string &name) {
  if (!SuitePointers::AILayer()) {
    return nullptr;
  }

  ai::UnicodeString nameUni(name);
  AILayerHandle layer = nullptr;

  ASErr err = SuitePointers::AILayer()->GetLayerByTitle(&layer, nameUni);
  if (err == kNoErr && layer != nullptr) {
    return layer;
  }

  return nullptr;
}

// -------------------------------------------------------------------------
// GetOrCreateLayer
// -------------------------------------------------------------------------

AILayerHandle GetOrCreateLayer(const std::string &name) {
  if (!SuitePointers::AILayer()) {
    return nullptr;
  }

  ai::UnicodeString nameUni(name);
  AILayerHandle layer = nullptr;

  // Check if layer already exists
  ASErr err = SuitePointers::AILayer()->GetLayerByTitle(&layer, nameUni);
  if (err == kNoErr && layer != nullptr) {
    return layer;
  }

  // Create new layer at the top of the layer stack
  err = SuitePointers::AILayer()->InsertLayer(nullptr, kPlaceAboveAll, &layer);
  if (err != kNoErr || layer == nullptr) {
    return nullptr;
  }

  // Set the layer title
  err = SuitePointers::AILayer()->SetLayerTitle(layer, nameUni);
  if (err != kNoErr) {
    // Layer was created but naming failed - still return the layer
    // The layer will have a default name
  }

  return layer;
}

// -------------------------------------------------------------------------
// GetCurrentLayer
// -------------------------------------------------------------------------

AILayerHandle GetCurrentLayer() {
  if (!SuitePointers::AILayer()) {
    return nullptr;
  }

  AILayerHandle layer = nullptr;
  ASErr err = SuitePointers::AILayer()->GetCurrentLayer(&layer);
  if (err == kNoErr) {
    return layer;
  }

  return nullptr;
}

// -------------------------------------------------------------------------
// SetCurrentLayer
// -------------------------------------------------------------------------

void SetCurrentLayer(AILayerHandle layer) {
  if (!SuitePointers::AILayer() || layer == nullptr) {
    return;
  }

  SuitePointers::AILayer()->SetCurrentLayer(layer);
}

// -------------------------------------------------------------------------
// GetLayerCount
// -------------------------------------------------------------------------

int GetLayerCount() {
  if (!SuitePointers::AILayer()) {
    return 0;
  }

  ai::int32 count = 0;
  ASErr err = SuitePointers::AILayer()->CountLayers(&count);
  if (err == kNoErr) {
    return static_cast<int>(count);
  }

  return 0;
}

} // namespace LayerUtils
