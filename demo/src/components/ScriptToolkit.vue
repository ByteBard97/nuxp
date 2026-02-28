<script setup lang="ts">
import { ref, computed } from 'vue'
import ScriptCard from '@/components/ScriptCard.vue'
import { getAllScripts, getCategories } from '@/modules/scripts/registry'
import { CATEGORY_LABELS, CATEGORY_ICONS } from '@/modules/scripts/types'
import type { ScriptCategory } from '@/modules/scripts/types'

const activeCategory = ref<ScriptCategory | null>(null)
const categories = computed(() => getCategories())
const scripts = computed(() => {
  if (!activeCategory.value) return getAllScripts()
  return getAllScripts().filter(s => s.category === activeCategory.value)
})

// Select first category on mount
if (categories.value.length > 0) {
  activeCategory.value = categories.value[0]
}
</script>

<template>
  <div class="toolkit">
    <!-- Category tabs -->
    <div class="category-bar">
      <button
        v-for="cat in categories"
        :key="cat"
        class="category-tab"
        :class="{ active: activeCategory === cat }"
        @click="activeCategory = cat"
      >
        <span class="cat-icon">{{ CATEGORY_ICONS[cat] }}</span>
        {{ CATEGORY_LABELS[cat] }}
      </button>
    </div>

    <!-- Scripts list -->
    <div class="scripts-list">
      <div v-if="scripts.length === 0" class="empty-state">
        No scripts registered yet.
      </div>
      <ScriptCard
        v-for="script in scripts"
        :key="script.id"
        :script="script"
      />
    </div>
  </div>
</template>

<style scoped>
.toolkit {
  height: 100%;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.category-bar {
  display: flex;
  gap: var(--spacing-xs);
  padding: var(--spacing-md);
  border-bottom: 1px solid var(--border-color);
  background-color: var(--bg-secondary);
  overflow-x: auto;
  flex-shrink: 0;
}

.category-tab {
  display: flex;
  align-items: center;
  gap: var(--spacing-xs);
  padding: var(--spacing-xs) var(--spacing-md);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
  background-color: transparent;
  color: var(--text-secondary);
  font-size: var(--font-size-xs);
  cursor: pointer;
  white-space: nowrap;
  transition: all 0.15s ease;
}

.category-tab:hover {
  background-color: var(--bg-hover);
  color: var(--text-primary);
}

.category-tab.active {
  background-color: var(--bg-active);
  color: var(--text-bright);
  border-color: var(--accent-blue);
}

.cat-icon { font-size: var(--font-size-sm); }

.scripts-list {
  flex: 1;
  overflow-y: auto;
  padding: var(--spacing-md);
}

.empty-state {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 200px;
  color: var(--text-muted);
  font-size: var(--font-size-sm);
}
</style>
