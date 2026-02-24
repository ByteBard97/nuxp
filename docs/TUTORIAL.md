---
layout: default
title: "Tutorial: Your First Plugin"
---

# Your First NUXP Plugin

<p align="center">
  <img src="images/nuxp-idle.gif" alt="Captain NUXP" width="200">
</p>

This tutorial walks you through building a simple Illustrator tool using NUXP. By the end, you will have a working component that reads real document information from Adobe Illustrator -- or from mock data during development.

## Prerequisites

- NUXP built and installed (see main README)
- Node.js 18+
- Adobe Illustrator 2024+ (only needed for Step 5)

## Step 1: Start in Mock Mode

You do not need Illustrator running to develop the UI. Mock mode lets you iterate on the frontend independently.

```bash
cd demo
VITE_USE_MOCK=true npm run dev
```

Open http://localhost:5173 in your browser. The demo app will start with a mock Bridge that returns simulated responses for all SDK calls.

## Step 2: Create a Document Info Component

Create a new file `demo/src/components/DocumentInfo.vue`:

{% raw %}
```vue
<template>
  <div class="document-info">
    <h3>Document Info</h3>

    <button @click="fetchInfo" :disabled="loading">
      {{ loading ? 'Loading...' : 'Get Document Info' }}
    </button>

    <div v-if="docInfo" class="info-panel">
      <dl>
        <dt>Name</dt>
        <dd>{{ docInfo.name || '(untitled)' }}</dd>

        <dt>Saved</dt>
        <dd>{{ docInfo.saved ? 'Yes' : 'No' }}</dd>

        <dt>Path</dt>
        <dd>{{ docInfo.path || '(not saved yet)' }}</dd>
      </dl>
    </div>

    <p v-if="errorMsg" class="error">{{ errorMsg }}</p>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { GetDocumentInfo } from '@nuxp/sdk/generated/customRoutes'
import type { GetDocumentInfoResponse } from '@nuxp/sdk/generated/customRoutes'

const loading = ref(false)
const errorMsg = ref('')
const docInfo = ref<GetDocumentInfoResponse | null>(null)

async function fetchInfo() {
  loading.value = true
  errorMsg.value = ''

  try {
    docInfo.value = await GetDocumentInfo()
  } catch (err) {
    errorMsg.value = err instanceof Error ? err.message : 'Unknown error'
    docInfo.value = null
  } finally {
    loading.value = false
  }
}
</script>

<style scoped>
.document-info {
  padding: 1rem;
  border: 1px solid #ccc;
  border-radius: 8px;
  max-width: 360px;
}

.info-panel {
  margin-top: 1rem;
  background: #f8f8f8;
  padding: 0.75rem;
  border-radius: 4px;
}

dl {
  display: grid;
  grid-template-columns: auto 1fr;
  gap: 0.25rem 1rem;
  margin: 0;
}

dt {
  font-weight: 600;
}

dd {
  margin: 0;
  word-break: break-all;
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
  margin-top: 0.5rem;
}
</style>
```
{% endraw %}

Key things to notice:

- **`GetDocumentInfo`** is imported directly from the generated custom routes module. It is a plain `async` function that returns typed data -- no bridge setup needed in the component.
- The response type **`GetDocumentInfoResponse`** gives you full TypeScript autocompletion for fields like `name`, `path`, `saved`, and `artboards`.
- Error handling is straightforward: the function throws on HTTP errors, so a standard `try/catch` is all you need.

## Step 3: Add to a View

Import and use your component in any view. For example, in `demo/src/views/HomeView.vue`:

```vue
<script setup lang="ts">
import DocumentInfo from '@/components/DocumentInfo.vue'
</script>

<template>
  <DocumentInfo />
</template>
```

## Step 4: Test with Mock Mode

When you run with `VITE_USE_MOCK=true`, the SDK's Bridge is replaced with a mock that intercepts HTTP requests. Custom route functions like `GetDocumentInfo()` make `fetch()` calls under the hood, so in mock mode the request goes to the mock server which returns a simulated response.

This lets you build and test the entire UI flow -- loading states, error handling, data rendering -- without Illustrator running. Once your component looks right, you can connect it to the real plugin.

## Step 5: Connect to Real Illustrator

1. Build and install the C++ plugin (see README)
2. Launch Illustrator and open or create a document
3. Stop the dev server and restart without mock mode:

```bash
cd demo
npm run dev
```

4. Click "Get Document Info" -- you will see the real document name, save status, and file path pulled live from Illustrator.

## Understanding the Call Flow

Here is what happens when you call `GetDocumentInfo()`:

```
Your Component
    | GetDocumentInfo()
    v
Generated Custom Route (customRoutes.ts)
    | fetch('GET', 'http://localhost:8080/api/doc/info')
    v
C++ HTTP Server (HttpServer.cpp)
    | Routes to GetDocumentInfoHandler
    v
Main Thread Dispatch
    | Marshals onto Illustrator's main thread
    v
Adobe Illustrator SDK
    | AIDocumentSuite + AIArtboardSuite queries
    v
JSON Response -> TypeScript GetDocumentInfoResponse
```

Custom route functions use `fetch()` directly against the C++ plugin's HTTP server. Generated suite functions (like those in `AIArtSuite.ts`) instead go through `Bridge.callSuite()`, which serializes requests through the AutoQueue. Both patterns end up at the same C++ HTTP server.

## What You Can Do From Here

NUXP ships with **442+ TypeScript functions** covering 19 SDK suites, plus 30+ custom route functions for higher-level operations. You can build most Illustrator tools entirely in TypeScript without touching any C++.

Browse what is available:

- **`sdk/src/generated/`** -- All auto-generated TypeScript functions. Import them via `@nuxp/sdk/generated/` (e.g., `@nuxp/sdk/generated/customRoutes`, `@nuxp/sdk/generated/AIArtSuite`).
- **Custom routes** -- `GetSelection`, `QueryLayers`, `CreateTextFrame`, `GetViewZoom`, `SetViewZoom`, and more in `customRoutes.ts`.
- **Suite functions** -- `NewArt`, `GetArtBounds`, `SetArtName`, `GetLayerTitle`, `SetOpacity`, and hundreds more across 19 suites.
- **Adapters and services** -- Higher-level abstractions like `DocumentAdapter`, `PlacementAdapter`, `SettingsService`, and `FontConfigService` exported from `@nuxp/sdk`.
- Read the [API Reference](api/README.md) for the full endpoint list.
- Look at the Debug Panel in the demo app for real-time event monitoring.

### Advanced: Adding Custom C++ Endpoints

If you need SDK functionality not already covered by the 442+ generated functions, you can write custom C++ endpoints. See [Adding Custom Endpoints](ADDING-ENDPOINTS.md) for a step-by-step guide. Most users will not need this.

## Next Steps

- [Getting Started](getting-started.md) -- Full setup guide including SDK installation and build instructions
- [Architecture](ARCHITECTURE.md) -- Deep dive into the communication layer, threading model, and code generation pipeline
- [Adding Endpoints](ADDING-ENDPOINTS.md) -- How to add new C++ endpoints when the generated functions are not enough

## Troubleshooting

**"Connection refused" errors:**
- Is Illustrator running?
- Is the plugin loaded? (Check Window > Plug-ins)
- Is the port correct? (default: 8080)

**"No document open" errors:**
- Create or open an Illustrator document first

**Changes not appearing:**
- If you modified C++ code, rebuild the plugin and restart Illustrator
- Frontend changes hot-reload automatically
