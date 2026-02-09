/**
 * Document state store
 *
 * Manages the state of the current Illustrator document,
 * including layers and selection information.
 */

import { defineStore } from 'pinia';
import { ref, computed } from 'vue';
import type { DocumentInfo, LayerInfo, ArtInfo } from '@/services/types';
import {
  getActiveDocument,
  getDocumentLayers,
  getSelection,
} from '@/services/illustrator';
import { useConnectionStore } from './connection';

export const useDocumentStore = defineStore('document', () => {
  // State
  const currentDocument = ref<DocumentInfo | null>(null);
  const layers = ref<LayerInfo[]>([]);
  const selection = ref<ArtInfo[]>([]);
  const loading = ref(false);
  const lastRefresh = ref<Date | null>(null);
  const lastApiResponse = ref<unknown>(null);

  // Getters
  const hasDocument = computed(() => currentDocument.value !== null);

  const documentName = computed(() =>
    currentDocument.value?.name || 'No document open'
  );

  const documentDimensions = computed(() => {
    if (!currentDocument.value) return null;
    return {
      width: currentDocument.value.width,
      height: currentDocument.value.height,
    };
  });

  const dimensionsFormatted = computed(() => {
    const dims = documentDimensions.value;
    if (!dims) return 'N/A';
    return `${dims.width.toFixed(1)} x ${dims.height.toFixed(1)} pt`;
  });

  const layerCount = computed(() =>
    currentDocument.value?.layerCount ?? layers.value.length
  );

  const selectionCount = computed(() => selection.value.length);

  const hasSelection = computed(() => selection.value.length > 0);

  const visibleLayers = computed(() =>
    layers.value.filter((layer) => layer.visible)
  );

  const lockedLayers = computed(() =>
    layers.value.filter((layer) => layer.locked)
  );

  // Actions
  async function refreshDocument(): Promise<void> {
    const connectionStore = useConnectionStore();
    if (!connectionStore.connected) {
      currentDocument.value = null;
      layers.value = [];
      return;
    }

    loading.value = true;

    try {
      const doc = await getActiveDocument();
      currentDocument.value = doc;
      lastApiResponse.value = doc;

      if (doc) {
        const docLayers = await getDocumentLayers();
        layers.value = docLayers;
      } else {
        layers.value = [];
      }

      lastRefresh.value = new Date();
    } catch (error) {
      console.error('Failed to refresh document:', error);
      lastApiResponse.value = { error: String(error) };
    } finally {
      loading.value = false;
    }
  }

  async function refreshSelection(): Promise<void> {
    const connectionStore = useConnectionStore();
    if (!connectionStore.connected) {
      selection.value = [];
      return;
    }

    try {
      const sel = await getSelection();
      selection.value = sel;
      lastApiResponse.value = sel;
    } catch (error) {
      console.error('Failed to refresh selection:', error);
      lastApiResponse.value = { error: String(error) };
    }
  }

  async function refreshAll(): Promise<void> {
    await Promise.all([refreshDocument(), refreshSelection()]);
  }

  function setLastApiResponse(response: unknown): void {
    lastApiResponse.value = response;
  }

  function reset(): void {
    currentDocument.value = null;
    layers.value = [];
    selection.value = [];
    loading.value = false;
    lastRefresh.value = null;
    lastApiResponse.value = null;
  }

  return {
    // State
    currentDocument,
    layers,
    selection,
    loading,
    lastRefresh,
    lastApiResponse,

    // Getters
    hasDocument,
    documentName,
    documentDimensions,
    dimensionsFormatted,
    layerCount,
    selectionCount,
    hasSelection,
    visibleLayers,
    lockedLayers,

    // Actions
    refreshDocument,
    refreshSelection,
    refreshAll,
    setLastApiResponse,
    reset,
  };
});
