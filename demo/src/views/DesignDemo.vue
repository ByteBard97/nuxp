<script setup lang="ts">
/**
 * Design Demo View
 *
 * Showcases all Flora design system components.
 * Navigate to /design to view this demo.
 */

import { ref } from 'vue';
import { Button, Input, Panel, Select } from '@/components/ui';
import type { SelectOption } from '@/components/ui';

// Demo state
const inputValue = ref('');
const inputWithError = ref('');
const selectedOption = ref('');
const isLoading = ref(false);

const selectOptions: SelectOption[] = [
  { value: 'option1', label: 'Layer Style' },
  { value: 'option2', label: 'Blend Mode' },
  { value: 'option3', label: 'Opacity' },
  { value: 'option4', label: 'Transform', disabled: true },
];

function simulateLoading(): void {
  isLoading.value = true;
  setTimeout(() => {
    isLoading.value = false;
  }, 2000);
}
</script>

<template>
  <div class="design-demo">
    <header class="demo-header">
      <div class="demo-header-content">
        <h1 class="demo-title">Flora Design System</h1>
        <p class="demo-subtitle">Premium dark-mode components for NUXP</p>
      </div>
      <a href="/" class="demo-back-link">Back to App</a>
    </header>

    <main class="demo-content">
      <!-- Color Palette Section -->
      <section class="demo-section">
        <h2 class="section-title">Color Palette</h2>
        <div class="color-grid">
          <div class="color-swatch">
            <div class="swatch" style="background: var(--flora-deep)" />
            <span class="swatch-label">Flora Deep</span>
            <span class="swatch-value">#0d3320</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--flora-forest)" />
            <span class="swatch-label">Flora Forest</span>
            <span class="swatch-value">#1a5c3a</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--flora-mid)" />
            <span class="swatch-label">Flora Mid</span>
            <span class="swatch-value">#2a7a4a</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--flora-accent)" />
            <span class="swatch-label">Flora Accent</span>
            <span class="swatch-value">#4ade80</span>
          </div>
        </div>

        <h3 class="subsection-title">Surface Colors</h3>
        <div class="color-grid">
          <div class="color-swatch">
            <div class="swatch" style="background: var(--surface-1)" />
            <span class="swatch-label">Surface 1</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--surface-2)" />
            <span class="swatch-label">Surface 2</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--surface-3)" />
            <span class="swatch-label">Surface 3</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--surface-4)" />
            <span class="swatch-label">Surface 4</span>
          </div>
          <div class="color-swatch">
            <div class="swatch" style="background: var(--surface-5)" />
            <span class="swatch-label">Surface 5</span>
          </div>
        </div>
      </section>

      <!-- Button Section -->
      <section class="demo-section">
        <h2 class="section-title">Buttons</h2>

        <Panel title="Button Variants" class="demo-panel">
          <div class="button-row">
            <Button variant="primary">Primary</Button>
            <Button variant="secondary">Secondary</Button>
            <Button variant="ghost">Ghost</Button>
          </div>
        </Panel>

        <Panel title="Button Sizes" class="demo-panel">
          <div class="button-row">
            <Button variant="primary" size="sm">Small</Button>
            <Button variant="primary" size="md">Medium</Button>
            <Button variant="primary" size="lg">Large</Button>
          </div>
        </Panel>

        <Panel title="Button States" class="demo-panel">
          <div class="button-row">
            <Button variant="primary" disabled>Disabled</Button>
            <Button variant="primary" :loading="isLoading" @click="simulateLoading">
              {{ isLoading ? 'Loading...' : 'Click to Load' }}
            </Button>
            <Button variant="secondary" full-width>Full Width</Button>
          </div>
        </Panel>
      </section>

      <!-- Input Section -->
      <section class="demo-section">
        <h2 class="section-title">Inputs</h2>

        <Panel title="Text Inputs" class="demo-panel">
          <div class="input-grid">
            <Input
              v-model="inputValue"
              label="Layer Name"
              placeholder="Enter layer name..."
            />
            <Input
              v-model="inputWithError"
              label="With Error"
              placeholder="This has an error"
              error="This field is required"
            />
            <Input
              label="Disabled"
              placeholder="Cannot edit"
              disabled
            />
            <Input
              model-value="Read only value"
              label="Read Only"
              readonly
            />
          </div>
        </Panel>
      </section>

      <!-- Select Section -->
      <section class="demo-section">
        <h2 class="section-title">Select</h2>

        <Panel title="Dropdown Select" class="demo-panel">
          <div class="input-grid">
            <Select
              v-model="selectedOption"
              :options="selectOptions"
              label="Property Type"
              placeholder="Choose a property..."
            />
            <Select
              :options="selectOptions"
              label="Disabled Select"
              disabled
            />
          </div>
          <p v-if="selectedOption" class="selected-value">
            Selected: {{ selectedOption }}
          </p>
        </Panel>
      </section>

      <!-- Panel Section -->
      <section class="demo-section">
        <h2 class="section-title">Panels</h2>

        <div class="panel-grid">
          <Panel title="Glass Panel" variant="glass">
            <p>This panel uses glassmorphism with backdrop blur and transparency. Perfect for overlays and floating UI elements.</p>
          </Panel>

          <Panel title="Default Panel" variant="default">
            <p>A solid panel with subtle transparency. Good for content sections and cards.</p>
          </Panel>

          <Panel title="Solid Panel" variant="solid">
            <p>Fully opaque panel for maximum contrast and readability.</p>
          </Panel>
        </div>

        <Panel title="Panel with Footer" variant="glass" class="demo-panel">
          <template #header>
            <div class="custom-header">
              <span>Custom Header</span>
              <Button variant="ghost" size="sm">Action</Button>
            </div>
          </template>
          <p>This panel has a custom header and footer.</p>
          <template #footer>
            <div class="panel-footer-actions">
              <Button variant="ghost" size="sm">Cancel</Button>
              <Button variant="primary" size="sm">Save Changes</Button>
            </div>
          </template>
        </Panel>
      </section>

      <!-- Typography Section -->
      <section class="demo-section">
        <h2 class="section-title">Typography</h2>

        <Panel title="Text Styles" class="demo-panel">
          <div class="typography-demo">
            <p class="text-3xl">Heading 3XL - 32px</p>
            <p class="text-2xl">Heading 2XL - 24px</p>
            <p class="text-xl">Heading XL - 18px</p>
            <p class="text-lg">Text Large - 16px</p>
            <p class="text-base">Text Base - 14px</p>
            <p class="text-sm">Text Small - 12px</p>
            <p class="text-xs">Text Extra Small - 11px</p>
          </div>
        </Panel>
      </section>

      <!-- Spacing Section -->
      <section class="demo-section">
        <h2 class="section-title">Spacing Scale</h2>

        <Panel title="4px Base Unit System" class="demo-panel">
          <div class="spacing-demo">
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-1); height: var(--space-1)" />
              <span>--space-1 (4px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-2); height: var(--space-2)" />
              <span>--space-2 (8px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-3); height: var(--space-3)" />
              <span>--space-3 (12px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-4); height: var(--space-4)" />
              <span>--space-4 (16px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-5); height: var(--space-5)" />
              <span>--space-5 (20px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-6); height: var(--space-6)" />
              <span>--space-6 (24px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-7); height: var(--space-7)" />
              <span>--space-7 (32px)</span>
            </div>
            <div class="spacing-item">
              <div class="spacing-box" style="width: var(--space-8); height: var(--space-8)" />
              <span>--space-8 (40px)</span>
            </div>
          </div>
        </Panel>
      </section>
    </main>
  </div>
</template>

<style scoped>
.design-demo {
  min-height: 100vh;
  background: linear-gradient(
    180deg,
    var(--flora-deep) 0%,
    var(--surface-1) 30%
  );
}

.demo-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: var(--space-6) var(--space-8);
  border-bottom: 1px solid var(--border-subtle);
  background: var(--glass-bg);
  backdrop-filter: blur(var(--glass-blur));
  position: sticky;
  top: 0;
  z-index: var(--z-sticky);
}

.demo-title {
  margin: 0;
  font-size: var(--text-2xl);
  font-weight: var(--weight-bold);
  color: var(--flora-accent);
}

.demo-subtitle {
  margin: var(--space-1) 0 0;
  font-size: var(--text-sm);
  color: var(--text-secondary);
}

.demo-back-link {
  color: var(--flora-accent);
  font-size: var(--text-sm);
  text-decoration: none;
  padding: var(--space-2) var(--space-4);
  border: 1px solid var(--flora-accent);
  border-radius: var(--radius-md);
  transition: all var(--transition-base);
}

.demo-back-link:hover {
  background: var(--flora-accent);
  color: var(--text-on-accent);
}

.demo-content {
  max-width: 1200px;
  margin: 0 auto;
  padding: var(--space-8);
}

.demo-section {
  margin-bottom: var(--space-8);
}

.section-title {
  font-size: var(--text-xl);
  font-weight: var(--weight-semibold);
  color: var(--text-bright);
  margin: 0 0 var(--space-4);
  padding-bottom: var(--space-2);
  border-bottom: 1px solid var(--border-subtle);
}

.subsection-title {
  font-size: var(--text-base);
  font-weight: var(--weight-medium);
  color: var(--text-secondary);
  margin: var(--space-4) 0 var(--space-3);
}

.demo-panel {
  margin-bottom: var(--space-4);
}

/* Color Grid */
.color-grid {
  display: flex;
  flex-wrap: wrap;
  gap: var(--space-4);
}

.color-swatch {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: var(--space-2);
}

.swatch {
  width: 80px;
  height: 80px;
  border-radius: var(--radius-md);
  border: 1px solid var(--border-default);
  box-shadow: var(--shadow-md);
}

.swatch-label {
  font-size: var(--text-sm);
  font-weight: var(--weight-medium);
  color: var(--text-primary);
}

.swatch-value {
  font-size: var(--text-xs);
  font-family: var(--font-mono);
  color: var(--text-muted);
}

/* Button Row */
.button-row {
  display: flex;
  flex-wrap: wrap;
  gap: var(--space-3);
  align-items: center;
}

/* Input Grid */
.input-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: var(--space-4);
}

.selected-value {
  margin-top: var(--space-3);
  font-size: var(--text-sm);
  color: var(--flora-accent);
}

/* Panel Grid */
.panel-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: var(--space-4);
  margin-bottom: var(--space-4);
}

.custom-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-weight: var(--weight-semibold);
}

.panel-footer-actions {
  display: flex;
  justify-content: flex-end;
  gap: var(--space-2);
}

/* Typography Demo */
.typography-demo {
  display: flex;
  flex-direction: column;
  gap: var(--space-2);
}

.text-3xl { font-size: var(--text-3xl); color: var(--text-bright); margin: 0; }
.text-2xl { font-size: var(--text-2xl); color: var(--text-bright); margin: 0; }
.text-xl { font-size: var(--text-xl); color: var(--text-bright); margin: 0; }
.text-lg { font-size: var(--text-lg); color: var(--text-primary); margin: 0; }
.text-base { font-size: var(--text-base); color: var(--text-primary); margin: 0; }
.text-sm { font-size: var(--text-sm); color: var(--text-secondary); margin: 0; }
.text-xs { font-size: var(--text-xs); color: var(--text-muted); margin: 0; }

/* Spacing Demo */
.spacing-demo {
  display: flex;
  flex-wrap: wrap;
  gap: var(--space-4);
  align-items: flex-end;
}

.spacing-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: var(--space-2);
}

.spacing-box {
  background: var(--flora-accent);
  border-radius: var(--radius-sm);
}

.spacing-item span {
  font-size: var(--text-xs);
  font-family: var(--font-mono);
  color: var(--text-muted);
}
</style>
