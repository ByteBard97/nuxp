---
layout: default
title: "Tutorial: Your First Plugin"
---

# Your First NUXP Plugin

This tutorial walks you through creating a simple Illustrator tool using NUXP. By the end, you'll have a working button that creates a rectangle in your Illustrator document.

## Prerequisites

- NUXP built and installed (see main README)
- Adobe Illustrator 2024+ running
- Node.js 18+

## Step 1: Start in Mock Mode

First, let's develop the UI without needing Illustrator. This is faster and lets you iterate on the frontend independently.

```bash
cd shell
VITE_USE_MOCK=true npm run dev
```

Open http://localhost:5173 in your browser.

## Step 2: Create a Simple Component

Create a new file `shell/src/components/CreateRectangle.vue`:

{% raw %}
```vue
<template>
  <div class="create-rectangle">
    <h3>Create Rectangle</h3>

    <div class="inputs">
      <label>
        Width:
        <input v-model.number="width" type="number" min="1" />
      </label>
      <label>
        Height:
        <input v-model.number="height" type="number" min="1" />
      </label>
    </div>

    <button @click="createRectangle" :disabled="loading">
      {{ loading ? 'Creating...' : 'Create Rectangle' }}
    </button>

    <p v-if="message" :class="{ error: isError }">{{ message }}</p>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { callCpp } from '@/sdk/bridge'

const width = ref(100)
const height = ref(100)
const loading = ref(false)
const message = ref('')
const isError = ref(false)

async function createRectangle() {
  loading.value = true
  message.value = ''
  isError.value = false

  try {
    const result = await callCpp('demo', 'createRectangle', {
      x: 100,
      y: 100,
      width: width.value,
      height: height.value
    })

    if (result.success) {
      message.value = 'Rectangle created!'
    } else {
      message.value = result.error || 'Failed to create rectangle'
      isError.value = true
    }
  } catch (err) {
    message.value = err instanceof Error ? err.message : 'Unknown error'
    isError.value = true
  } finally {
    loading.value = false
  }
}
</script>

<style scoped>
.create-rectangle {
  padding: 1rem;
  border: 1px solid #ccc;
  border-radius: 8px;
  max-width: 300px;
}

.inputs {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  margin: 1rem 0;
}

.inputs label {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.inputs input {
  width: 100px;
  padding: 0.25rem;
}

button {
  width: 100%;
  padding: 0.5rem;
  background: #0066cc;
  color: white;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}

button:disabled {
  background: #ccc;
  cursor: not-allowed;
}

button:hover:not(:disabled) {
  background: #0052a3;
}

.error {
  color: red;
}
</style>
```
{% endraw %}

## Step 3: Add to a View

Import and use your component in `shell/src/views/HomeView.vue` or create a new view.

## Step 4: Test with Mock Mode

In mock mode, the `callCpp` function routes to `MockBridge.ts` which returns simulated responses. This lets you develop the UI flow without the real plugin.

## Step 5: Connect to Real Illustrator

1. Build and install the plugin (see README)
2. Launch Illustrator
3. Stop the dev server and restart without mock mode:

```bash
npm run dev
```

4. Click your button - a rectangle should appear in Illustrator!

## Understanding the Call Flow

```
Your Component
    ↓ callCpp('demo', 'createRectangle', {...})
SDK Bridge (bridge.ts)
    ↓ HTTP POST to localhost:8080
C++ HTTP Server
    ↓ Routes to DemoEndpoints::CreateRectangle
Adobe SDK
    ↓ Creates actual rectangle
Illustrator Document
```

## What You Can Do From Here

NUXP ships with **442+ TypeScript functions** covering 19 SDK suites. You can build most Illustrator tools entirely in TypeScript without touching any C++.

Browse what's available:
- Check `shell/src/sdk/generated/` for all auto-generated TypeScript functions
- Read the [API Reference](api/README.md) for the full endpoint list
- Look at the Debug Panel in the shell for real-time event monitoring

### Advanced: Adding Custom C++ Endpoints

If you need SDK functionality not already covered by the 442+ generated functions, you can write custom C++ endpoints. See [Adding Custom Endpoints](ADDING-ENDPOINTS.md) for a step-by-step guide. Most users won't need this.

## Troubleshooting

**"Connection refused" errors:**
- Is Illustrator running?
- Is the plugin loaded? (Check Window → Plug-ins)
- Is the port correct? (default: 8080)

**"No document open" errors:**
- Create or open an Illustrator document first

**Changes not appearing:**
- If you modified C++ code, rebuild the plugin and restart Illustrator
- Frontend changes hot-reload automatically
