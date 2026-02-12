#include "HandleManager.hpp"

// Define the static members using underlying struct types
// (see HandleManager.hpp for explanation)
HandleRegistry<ArtObject> HandleManager::art;
HandleRegistry<_t_AILayerOpaque> HandleManager::layers;
HandleRegistry<_t_AIDocument> HandleManager::documents;
HandleRegistry<_AIMask> HandleManager::masks;
HandleRegistry<_t_AIToolOpaque> HandleManager::tools;
HandleRegistry<_t_AITimerOpaque> HandleManager::timers;
HandleRegistry<_t_AINotifierOpaque> HandleManager::notifiers;

void HandleManager::InvalidateAll() {
  art.BumpGeneration();
  layers.BumpGeneration();
  documents.BumpGeneration();
  masks.BumpGeneration();
  tools.BumpGeneration();
  timers.BumpGeneration();
  notifiers.BumpGeneration();
}
