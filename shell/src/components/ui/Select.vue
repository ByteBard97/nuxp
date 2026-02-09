<script setup lang="ts">
/**
 * Select Component
 *
 * Dropdown select with Flora theme styling.
 * Supports labels, placeholders, and custom options.
 */

export interface SelectOption {
  value: string | number;
  label: string;
  disabled?: boolean;
}

export interface SelectProps {
  modelValue?: string | number;
  options: SelectOption[];
  placeholder?: string;
  label?: string;
  disabled?: boolean;
}

withDefaults(defineProps<SelectProps>(), {
  modelValue: '',
  placeholder: 'Select an option',
  label: '',
  disabled: false,
});

const emit = defineEmits<{
  'update:modelValue': [value: string | number];
  change: [value: string | number];
}>();

function handleChange(event: Event): void {
  const target = event.target as HTMLSelectElement;
  const value = target.value;
  emit('update:modelValue', value);
  emit('change', value);
}
</script>

<template>
  <div class="select-wrapper">
    <label v-if="label" class="select-label">
      {{ label }}
    </label>
    <div class="select-container">
      <select
        class="select"
        :value="modelValue"
        :disabled="disabled"
        @change="handleChange"
      >
        <option v-if="placeholder" value="" disabled>
          {{ placeholder }}
        </option>
        <option
          v-for="option in options"
          :key="option.value"
          :value="option.value"
          :disabled="option.disabled"
        >
          {{ option.label }}
        </option>
      </select>
      <div class="select-icon">
        <svg
          width="12"
          height="12"
          viewBox="0 0 12 12"
          fill="none"
          xmlns="http://www.w3.org/2000/svg"
        >
          <path
            d="M2.5 4.5L6 8L9.5 4.5"
            stroke="currentColor"
            stroke-width="1.5"
            stroke-linecap="round"
            stroke-linejoin="round"
          />
        </svg>
      </div>
    </div>
  </div>
</template>

<style scoped>
.select-wrapper {
  display: flex;
  flex-direction: column;
  gap: var(--space-1);
}

.select-label {
  font-size: var(--text-sm);
  font-weight: var(--weight-medium);
  color: var(--text-secondary);
}

.select-container {
  position: relative;
}

.select {
  width: 100%;
  height: 40px;
  padding: 0 var(--space-7) 0 var(--space-3);
  font-family: var(--font-sans);
  font-size: var(--text-sm);
  color: var(--text-primary);
  background-color: var(--surface-3);
  border: 1px solid var(--border-default);
  border-radius: var(--radius-md);
  cursor: pointer;
  appearance: none;
  -webkit-appearance: none;
  -moz-appearance: none;
  transition:
    border-color var(--transition-base),
    background-color var(--transition-base),
    box-shadow var(--transition-base);
}

.select:hover:not(:disabled) {
  border-color: var(--border-strong);
  background-color: var(--surface-4);
}

.select:focus {
  outline: none;
  border-color: var(--flora-accent);
  background-color: var(--surface-4);
  box-shadow: 0 0 0 3px var(--flora-accent-muted);
}

.select:disabled {
  opacity: 0.5;
  cursor: not-allowed;
  background-color: var(--surface-2);
}

/* Dropdown icon */
.select-icon {
  position: absolute;
  top: 50%;
  right: var(--space-3);
  transform: translateY(-50%);
  color: var(--text-muted);
  pointer-events: none;
  transition: color var(--transition-base);
}

.select:hover:not(:disabled) ~ .select-icon,
.select:focus ~ .select-icon {
  color: var(--text-primary);
}

/* Option styling (limited browser support) */
.select option {
  background-color: var(--surface-4);
  color: var(--text-primary);
  padding: var(--space-2);
}

.select option:disabled {
  color: var(--text-muted);
}
</style>
