<script setup lang="ts">
/**
 * Input Component
 *
 * Text input with premium focus states and Flora theme integration.
 * Supports labels, placeholders, and error states.
 */

export interface InputProps {
  modelValue?: string;
  type?: 'text' | 'password' | 'email' | 'number' | 'search';
  placeholder?: string;
  label?: string;
  error?: string;
  disabled?: boolean;
  readonly?: boolean;
}

withDefaults(defineProps<InputProps>(), {
  modelValue: '',
  type: 'text',
  placeholder: '',
  label: '',
  error: '',
  disabled: false,
  readonly: false,
});

const emit = defineEmits<{
  'update:modelValue': [value: string];
  focus: [event: FocusEvent];
  blur: [event: FocusEvent];
}>();

function handleInput(event: Event): void {
  const target = event.target as HTMLInputElement;
  emit('update:modelValue', target.value);
}
</script>

<template>
  <div class="input-wrapper" :class="{ 'input-wrapper--error': error }">
    <label v-if="label" class="input-label">
      {{ label }}
    </label>
    <div class="input-container">
      <input
        class="input"
        :type="type"
        :value="modelValue"
        :placeholder="placeholder"
        :disabled="disabled"
        :readonly="readonly"
        @input="handleInput"
        @focus="$emit('focus', $event)"
        @blur="$emit('blur', $event)"
      />
      <div class="input-focus-ring" />
    </div>
    <span v-if="error" class="input-error">
      {{ error }}
    </span>
  </div>
</template>

<style scoped>
.input-wrapper {
  display: flex;
  flex-direction: column;
  gap: var(--space-1);
}

.input-label {
  font-size: var(--text-sm);
  font-weight: var(--weight-medium);
  color: var(--text-secondary);
}

.input-container {
  position: relative;
}

.input {
  width: 100%;
  height: 40px;
  padding: 0 var(--space-3);
  font-family: var(--font-sans);
  font-size: var(--text-sm);
  color: var(--text-primary);
  background-color: var(--surface-3);
  border: 1px solid var(--border-default);
  border-radius: var(--radius-md);
  transition:
    border-color var(--transition-base),
    background-color var(--transition-base),
    box-shadow var(--transition-base);
}

.input::placeholder {
  color: var(--text-muted);
}

.input:hover:not(:disabled):not(:focus) {
  border-color: var(--border-strong);
  background-color: var(--surface-4);
}

.input:focus {
  outline: none;
  border-color: var(--flora-accent);
  background-color: var(--surface-4);
  box-shadow: 0 0 0 3px var(--flora-accent-muted);
}

.input:disabled {
  opacity: 0.5;
  cursor: not-allowed;
  background-color: var(--surface-2);
}

.input:read-only {
  cursor: default;
  background-color: var(--surface-2);
}

/* Focus Ring for accessibility */
.input-focus-ring {
  position: absolute;
  inset: -2px;
  border-radius: calc(var(--radius-md) + 2px);
  pointer-events: none;
  opacity: 0;
  border: 2px solid var(--flora-accent);
  transition: opacity var(--transition-base);
}

.input:focus-visible ~ .input-focus-ring {
  opacity: 1;
}

/* Error State */
.input-wrapper--error .input {
  border-color: var(--status-error);
}

.input-wrapper--error .input:focus {
  box-shadow: 0 0 0 3px rgba(248, 113, 113, 0.2);
}

.input-error {
  font-size: var(--text-xs);
  color: var(--status-error);
}
</style>
