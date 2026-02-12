#include "HandleManager.hpp"

// Define the static members using underlying struct types
// (see HandleManager.hpp for explanation)

// Core handle registries (non-owning)
HandleRegistry<ArtObject> HandleManager::art;
HandleRegistry<_t_AILayerOpaque> HandleManager::layers;
HandleRegistry<_t_AIDocument> HandleManager::documents;
HandleRegistry<_t_AIMenuItemOpaque> HandleManager::menuItems;
HandleRegistry<_AIMask> HandleManager::masks;
HandleRegistry<_t_AIToolOpaque> HandleManager::tools;
HandleRegistry<_t_AITimerOpaque> HandleManager::timers;
HandleRegistry<_t_AINotifierOpaque> HandleManager::notifiers;

// Managed registries (owning, RAII objects)
ManagedHandleRegistry<ai::ArtboardProperties> HandleManager::artboardProperties;
ManagedHandleRegistry<ai::ArtboardList> HandleManager::artboardLists;

// Dictionary handle registries
HandleRegistry<_AIDictionary> HandleManager::dictionaries;
HandleRegistry<_AIEntry> HandleManager::entries;
HandleRegistry<_t_AIDictKey> HandleManager::dictKeys;
HandleRegistry<_AIDictionaryIterator> HandleManager::dictIterators;

// Additional handle registries
HandleRegistry<_AIArray> HandleManager::arrays;
HandleRegistry<_t_AIArtStyle> HandleManager::artStyles;
HandleRegistry<void> HandleManager::patterns;
HandleRegistry<void> HandleManager::gradients;
HandleRegistry<void> HandleManager::customColors;

void HandleManager::InvalidateAll() {
  // Core handles
  art.BumpGeneration();
  layers.BumpGeneration();
  documents.BumpGeneration();
  menuItems.BumpGeneration();
  masks.BumpGeneration();
  tools.BumpGeneration();
  timers.BumpGeneration();
  notifiers.BumpGeneration();

  // Managed (owning) registries
  artboardProperties.BumpGeneration();
  artboardLists.BumpGeneration();

  // Dictionary handles
  dictionaries.BumpGeneration();
  entries.BumpGeneration();
  dictKeys.BumpGeneration();
  dictIterators.BumpGeneration();

  // Additional handles
  arrays.BumpGeneration();
  artStyles.BumpGeneration();
  patterns.BumpGeneration();
  gradients.BumpGeneration();
  customColors.BumpGeneration();
}
