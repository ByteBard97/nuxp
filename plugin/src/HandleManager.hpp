#pragma once

#include "HandleRegistry.hpp"

// Forward declarations of SDK types to avoid including big headers here if
// possible Ideally we include "IllustratorSDK.h" or similar if we have a common
// header. For now, assuming these typedefs exist or will be pulled in via PCH.
// In a real plugin, you'd allow including this file after the SDK headers.
#include "IllustratorSDK.h"

class HandleManager {
public:
  // --- Per-type registries ---
  static HandleRegistry<AIArtHandle> art;
  static HandleRegistry<AILayerHandle> layers;
  static HandleRegistry<AIDocumentHandle> documents;

  // Add more as needed:
  // static HandleRegistry<AIPatternHandle>   patterns;
  // static HandleRegistry<AISymbolHandle>    symbols;
  // static HandleRegistry<AIColor>           colors; // If AIColor is used by
  // pointer

  // Bump all registries at once (document switch, undo/redo)
  static void InvalidateAll();
};
