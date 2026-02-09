<script setup lang="ts">
/**
 * Panel Component
 *
 * Glassmorphism container with subtle blur and transparency.
 * Provides a premium surface for grouping content.
 */

export interface PanelProps {
  title?: string;
  padding?: 'none' | 'sm' | 'md' | 'lg';
  variant?: 'default' | 'glass' | 'solid';
}

withDefaults(defineProps<PanelProps>(), {
  title: '',
  padding: 'md',
  variant: 'glass',
});
</script>

<template>
  <div class="panel" :class="[`panel--${variant}`, `panel--padding-${padding}`]">
    <header v-if="title || $slots.header" class="panel__header">
      <slot name="header">
        <h3 class="panel__title">{{ title }}</h3>
      </slot>
    </header>
    <div class="panel__content">
      <slot />
    </div>
    <footer v-if="$slots.footer" class="panel__footer">
      <slot name="footer" />
    </footer>
  </div>
</template>

<style scoped>
.panel {
  border-radius: var(--radius-lg);
  overflow: hidden;
}

/* Variant: Glass (default) - Glassmorphism effect */
.panel--glass {
  background: var(--glass-bg);
  backdrop-filter: blur(var(--glass-blur));
  -webkit-backdrop-filter: blur(var(--glass-blur));
  border: 1px solid var(--glass-border);
  box-shadow: var(--shadow-lg);
}

.panel--glass:hover {
  background: var(--glass-bg-hover);
  border-color: var(--glass-border-hover);
}

/* Variant: Default - Solid with subtle transparency */
.panel--default {
  background-color: var(--surface-3);
  border: 1px solid var(--border-default);
  box-shadow: var(--shadow-md);
}

/* Variant: Solid - Fully opaque */
.panel--solid {
  background-color: var(--surface-4);
  border: 1px solid var(--border-strong);
  box-shadow: var(--shadow-sm);
}

/* Padding Variants */
.panel--padding-none .panel__content {
  padding: 0;
}

.panel--padding-sm .panel__content {
  padding: var(--space-3);
}

.panel--padding-md .panel__content {
  padding: var(--space-4);
}

.panel--padding-lg .panel__content {
  padding: var(--space-6);
}

/* Header */
.panel__header {
  padding: var(--space-3) var(--space-4);
  border-bottom: 1px solid var(--border-subtle);
  background: rgba(0, 0, 0, 0.2);
}

.panel--padding-none .panel__header {
  padding: var(--space-3) var(--space-4);
}

.panel__title {
  margin: 0;
  font-size: var(--text-sm);
  font-weight: var(--weight-semibold);
  color: var(--text-primary);
  letter-spacing: 0.025em;
}

/* Content */
.panel__content {
  position: relative;
}

/* Footer */
.panel__footer {
  padding: var(--space-3) var(--space-4);
  border-top: 1px solid var(--border-subtle);
  background: rgba(0, 0, 0, 0.1);
}

.panel--padding-none .panel__footer {
  padding: var(--space-3) var(--space-4);
}
</style>
