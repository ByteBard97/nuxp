#pragma once

#include "HandleRegistry.hpp"
#include "ManagedHandleRegistry.hpp"

// Include SDK headers to get the struct forward declarations
// Handles are typedef'd as pointers to opaque structs, e.g.:
//   typedef struct ArtObject *AIArtHandle;
// The registry stores pointers, so we need HandleRegistry<ArtObject>
// which gives us Register(ArtObject*) = Register(AIArtHandle)
#include "IllustratorSDK.h"

class HandleManager {
public:
  // --- Per-type registries (non-owning, SDK-managed handles) ---
  // Use the underlying struct type, not the Handle typedef
  // This ensures Register(T*) accepts the handle type directly
  static HandleRegistry<ArtObject> art;              // AIArtHandle = ArtObject*
  static HandleRegistry<_t_AILayerOpaque> layers;    // AILayerHandle = _t_AILayerOpaque*
  static HandleRegistry<_t_AIDocument> documents;    // AIDocumentHandle = _t_AIDocument*
  static HandleRegistry<_t_AIMenuItemOpaque> menuItems;  // AIMenuItemHandle = _t_AIMenuItemOpaque*
  static HandleRegistry<_AIMask> masks;                  // AIMaskRef = _AIMask*
  static HandleRegistry<_t_AIToolOpaque> tools;          // AIToolHandle = _t_AIToolOpaque*
  static HandleRegistry<_t_AITimerOpaque> timers;        // AITimerHandle = _t_AITimerOpaque*
  static HandleRegistry<_t_AINotifierOpaque> notifiers;  // AINotifierHandle = _t_AINotifierOpaque*

  // --- Managed registries (owning, plugin-controlled RAII objects) ---
  static ManagedHandleRegistry<ai::ArtboardProperties> artboardProperties;
  static ManagedHandleRegistry<ai::ArtboardList> artboardLists;

  // --- Dictionary handle registries ---
  static HandleRegistry<_AIDictionary> dictionaries;            // AIDictionaryRef = _AIDictionary*
  static HandleRegistry<_AIEntry> entries;                      // AIEntryRef = _AIEntry*
  static HandleRegistry<_t_AIDictKey> dictKeys;                 // AIDictKey = const _t_AIDictKey*
  static HandleRegistry<_AIDictionaryIterator> dictIterators;   // AIDictionaryIterator = _AIDictionaryIterator*

  // --- Additional handle registries ---
  static HandleRegistry<_AIArray> arrays;              // AIArrayRef = _AIArray*
  static HandleRegistry<_t_AIArtStyle> artStyles;      // AIArtStyleHandle = _t_AIArtStyle*
  // AIPatternHandle, AIGradientHandle, AICustomColorHandle are typedef'd as void*
  // so we use HandleRegistry<void> for them
  static HandleRegistry<void> patterns;                // AIPatternHandle = void*
  static HandleRegistry<void> gradients;               // AIGradientHandle = void*
  static HandleRegistry<void> customColors;            // AICustomColorHandle = void*

  // Bump all registries at once (document switch, undo/redo)
  static void InvalidateAll();
};
