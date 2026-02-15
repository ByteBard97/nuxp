<script setup lang="ts">
import { ref, reactive } from 'vue'
import type { Script, ScriptResult } from '@/modules/scripts/types'
import { Button } from '@/components/ui'

const props = defineProps<{
  script: Script
}>()

const running = ref(false)
const result = ref<ScriptResult | null>(null)
const paramValues = reactive<Record<string, unknown>>({})

// Initialize param defaults
if (props.script.params) {
  for (const p of props.script.params) {
    paramValues[p.name] = p.default ?? (p.type === 'number' ? 0 : p.type === 'boolean' ? false : '')
  }
}

async function runScript() {
  running.value = true
  result.value = null
  try {
    result.value = await props.script.run({ ...paramValues })
  } catch (err) {
    result.value = {
      success: false,
      message: err instanceof Error ? err.message : String(err),
    }
  } finally {
    running.value = false
  }
}
</script>

<template>
  <div class="script-card">
    <div class="script-header">
      <div class="script-info">
        <h4 class="script-name">{{ script.name }}</h4>
        <p class="script-desc">{{ script.description }}</p>
        <p v-if="script.inspiredBy" class="script-inspired">
          Inspired by: {{ script.inspiredBy }}
        </p>
      </div>
      <Button
        variant="primary"
        size="sm"
        :loading="running"
        @click="runScript"
      >
        Run
      </Button>
    </div>

    <!-- Parameters -->
    <div v-if="script.params?.length" class="script-params">
      <div v-for="param in script.params" :key="param.name" class="param-row">
        <label class="param-label">{{ param.label }}</label>
        <input
          v-if="param.type === 'string'"
          v-model="paramValues[param.name]"
          type="text"
          class="param-input"
        />
        <input
          v-else-if="param.type === 'number'"
          v-model.number="paramValues[param.name]"
          type="number"
          class="param-input param-input--number"
          :min="param.min"
          :max="param.max"
          :step="param.step ?? 1"
        />
        <label v-else-if="param.type === 'boolean'" class="param-checkbox">
          <input
            v-model="paramValues[param.name]"
            type="checkbox"
          />
          {{ param.label }}
        </label>
      </div>
    </div>

    <!-- Result -->
    <div v-if="result" class="script-result" :class="result.success ? 'result-ok' : 'result-err'">
      <div class="result-message">{{ result.message }}</div>
      <pre v-if="result.data" class="result-data">{{ JSON.stringify(result.data, null, 2) }}</pre>
    </div>
  </div>
</template>

<style scoped>
.script-card {
  padding: var(--spacing-md);
  background-color: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-lg);
  margin-bottom: var(--spacing-sm);
}

.script-header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: var(--spacing-md);
}

.script-info { flex: 1; }

.script-name {
  margin: 0;
  font-size: var(--font-size-sm);
  font-weight: 600;
  color: var(--text-bright);
}

.script-desc {
  margin: var(--spacing-xs) 0 0;
  font-size: var(--font-size-xs);
  color: var(--text-secondary);
}

.script-inspired {
  margin: var(--spacing-xs) 0 0;
  font-size: var(--font-size-xs);
  color: var(--text-muted);
  font-style: italic;
}

.script-params {
  margin-top: var(--spacing-md);
  padding-top: var(--spacing-sm);
  border-top: 1px solid var(--border-color);
  display: flex;
  flex-direction: column;
  gap: var(--spacing-sm);
}

.param-row {
  display: flex;
  align-items: center;
  gap: var(--spacing-md);
}

.param-label {
  font-size: var(--font-size-xs);
  color: var(--text-secondary);
  min-width: 80px;
}

.param-input {
  flex: 1;
  padding: var(--spacing-xs) var(--spacing-sm);
  background-color: var(--bg-primary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
  color: var(--text-primary);
  font-size: var(--font-size-xs);
  font-family: inherit;
}

.param-input--number { max-width: 100px; }

.param-input:focus {
  outline: none;
  border-color: var(--accent-blue);
}

.script-result {
  margin-top: var(--spacing-md);
  padding: var(--spacing-sm) var(--spacing-md);
  border-radius: var(--radius-md);
  font-size: var(--font-size-xs);
}

.result-ok {
  background-color: rgba(60, 154, 95, 0.15);
  border: 1px solid var(--accent-green);
  color: var(--accent-green);
}

.result-err {
  background-color: rgba(231, 76, 60, 0.15);
  border: 1px solid var(--accent-red);
  color: var(--accent-red);
}

.result-message { font-weight: 500; }

.result-data {
  margin: var(--spacing-sm) 0 0;
  padding: var(--spacing-sm);
  background-color: var(--bg-primary);
  border-radius: var(--radius-sm);
  color: var(--text-primary);
  font-size: var(--font-size-xs);
  overflow-x: auto;
  max-height: 200px;
  overflow-y: auto;
  white-space: pre-wrap;
}
</style>
