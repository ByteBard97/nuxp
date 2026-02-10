#include "HandleManager.hpp"

// Define the static members using underlying struct types
// (see HandleManager.hpp for explanation)
HandleRegistry<ArtObject> HandleManager::art;
HandleRegistry<_t_AILayerOpaque> HandleManager::layers;
HandleRegistry<_t_AIDocument> HandleManager::documents;

void HandleManager::InvalidateAll() {
  art.BumpGeneration();
  layers.BumpGeneration();
  documents.BumpGeneration();
}
