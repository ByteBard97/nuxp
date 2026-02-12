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
  static HandleRegistry<_AIMask> masks;                  // AIMaskRef = _AIMask*
  static HandleRegistry<_t_AIToolOpaque> tools;          // AIToolHandle = _t_AIToolOpaque*
  static HandleRegistry<_t_AITimerOpaque> timers;        // AITimerHandle = _t_AITimerOpaque*
  static HandleRegistry<_t_AINotifierOpaque> notifiers;  // AINotifierHandle = _t_AINotifierOpaque*

  // Bump all registries at once (document switch, undo/redo)
  static void InvalidateAll();
};
