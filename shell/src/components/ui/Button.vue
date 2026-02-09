<script setup lang="ts">
/**
 * Button Component
 *
 * Premium button with Primary, Secondary, and Ghost variants.
 * Features smooth hover transitions and accessible focus states.
 */

export interface ButtonProps {
  variant?: 'primary' | 'secondary' | 'ghost';
  size?: 'sm' | 'md' | 'lg';
  disabled?: boolean;
  loading?: boolean;
  fullWidth?: boolean;
}

withDefaults(defineProps<ButtonProps>(), {
  variant: 'secondary',
  size: 'md',
  disabled: false,
  loading: false,
  fullWidth: false,
});

defineEmits<{
  click: [event: MouseEvent];
}>();
</script>

<template>
  <button
    class="btn"
    :class="[
      `btn--${variant}`,
      `btn--${size}`,
      { 'btn--loading': loading, 'btn--full-width': fullWidth }
    ]"
    :disabled="disabled || loading"
    @click="$emit('click', $event)"
  >
    <span v-if="loading" class="btn__spinner" />
    <span class="btn__content" :class="{ 'btn__content--hidden': loading }">
      <slot />
    </span>
  </button>
</template>

<style scoped>
.btn {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: var(--space-2);
  font-family: var(--font-sans);
  font-weight: var(--weight-medium);
  border: 1px solid transparent;
  cursor: pointer;
  position: relative;
  transition:
    background-color var(--transition-base),
    border-color var(--transition-base),
    color var(--transition-base),
    box-shadow var(--transition-base),
    transform var(--transition-fast);
}

.btn:focus-visible {
  outline: none;
  box-shadow: 0 0 0 2px var(--surface-2), 0 0 0 4px var(--flora-accent);
}

.btn:active:not(:disabled) {
  transform: translateY(1px);
}

.btn:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

/* Size Variants */
.btn--sm {
  height: 28px;
  padding: 0 var(--space-3);
  font-size: var(--text-xs);
  border-radius: var(--radius-sm);
}

.btn--md {
  height: 36px;
  padding: 0 var(--space-4);
  font-size: var(--text-sm);
  border-radius: var(--radius-md);
}

.btn--lg {
  height: 44px;
  padding: 0 var(--space-6);
  font-size: var(--text-base);
  border-radius: var(--radius-md);
}

/* Primary Variant - Flora accent */
.btn--primary {
  background: linear-gradient(
    135deg,
    var(--flora-accent) 0%,
    var(--flora-mid) 100%
  );
  color: var(--text-on-accent);
  border-color: var(--flora-accent);
}

.btn--primary:hover:not(:disabled) {
  background: linear-gradient(
    135deg,
    var(--flora-accent-hover) 0%,
    var(--flora-accent) 100%
  );
  box-shadow: var(--shadow-glow);
}

/* Secondary Variant - Subtle surface */
.btn--secondary {
  background-color: var(--surface-4);
  color: var(--text-primary);
  border-color: var(--border-default);
}

.btn--secondary:hover:not(:disabled) {
  background-color: var(--surface-hover);
  border-color: var(--border-strong);
}

/* Ghost Variant - Transparent */
.btn--ghost {
  background-color: transparent;
  color: var(--text-secondary);
  border-color: transparent;
}

.btn--ghost:hover:not(:disabled) {
  background-color: var(--surface-4);
  color: var(--text-primary);
}

/* Full Width */
.btn--full-width {
  width: 100%;
}

/* Loading State */
.btn--loading {
  pointer-events: none;
}

.btn__content {
  display: inline-flex;
  align-items: center;
  gap: var(--space-2);
}

.btn__content--hidden {
  visibility: hidden;
}

.btn__spinner {
  position: absolute;
  width: 16px;
  height: 16px;
  border: 2px solid currentColor;
  border-top-color: transparent;
  border-radius: 50%;
  animation: spin 0.8s linear infinite;
}

@keyframes spin {
  to {
    transform: rotate(360deg);
  }
}
</style>
