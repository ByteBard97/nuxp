#include "HandleManager.hpp"

// Define the static members
HandleRegistry<AIArtHandle> HandleManager::art;
HandleRegistry<AILayerHandle> HandleManager::layers;
HandleRegistry<AIDocumentHandle> HandleManager::documents;

void HandleManager::InvalidateAll() {
  art.BumpGeneration();
  layers.BumpGeneration();
  documents.BumpGeneration();
}
