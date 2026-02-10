#pragma once

#include "HandleRegistry.hpp"

// Include SDK headers to get the struct forward declarations
// Handles are typedef'd as pointers to opaque structs, e.g.:
//   typedef struct ArtObject *AIArtHandle;
// The registry stores pointers, so we need HandleRegistry<ArtObject>
// which gives us Register(ArtObject*) = Register(AIArtHandle)
#include "IllustratorSDK.h"

class HandleManager {
public:
  // --- Per-type registries ---
  // Use the underlying struct type, not the Handle typedef
  // This ensures Register(T*) accepts the handle type directly
  static HandleRegistry<ArtObject> art;              // AIArtHandle = ArtObject*
  static HandleRegistry<_t_AILayerOpaque> layers;    // AILayerHandle = _t_AILayerOpaque*
  static HandleRegistry<_t_AIDocument> documents;    // AIDocumentHandle = _t_AIDocument*

  // Add more as needed:
  // static HandleRegistry<...>   patterns;
  // static HandleRegistry<...>   symbols;

  // Bump all registries at once (document switch, undo/redo)
  static void InvalidateAll();
};
