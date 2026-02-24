# @nuxp/sdk

TypeScript SDK for building Adobe Illustrator plugins on NUXP.

NUXP provides 442+ pre-built TypeScript functions that talk directly to the Illustrator SDK through a C++ plugin running a local HTTP server. This package is the client side of that communication — it handles request serialization, event streaming, and provides higher-level services for common plugin operations.

The SDK is framework-agnostic. It has no Vue, React, or Pinia dependencies. Use it with any JavaScript framework, or none at all.

## Quick Start

```typescript
import { createBridge, setBridgeInstance, connectSSE } from '@nuxp/sdk'

// 1. Create and register a bridge (wires up all generated suite functions)
const bridge = createBridge({ port: 8080 })
setBridgeInstance(bridge)

// 2. Check that the plugin is running
const available = await bridge.isAvailable()
if (!available) throw new Error('Plugin not running — open Illustrator first')

// 3. Subscribe to real-time events
const sse = connectSSE({ serverUrl: 'http://localhost:8080' })
sse.on('selection', (data) => {
  console.log(`${data.count} items selected:`, data.selectedIds)
})

// 4. Call an SDK function
const name = await bridge.callSuite<string>('AIArt', 'GetArtName', { art: 1 })
```

The bridge talks to the C++ plugin's HTTP server (default `localhost:8080`). The plugin must be loaded in Illustrator for calls to succeed. For frontend-only development, use mock mode (`useMock: true` in `BridgeConfig`).

## Which Class Do I Use?

The SDK provides several communication layers. Here is when to use each:

| Class | Use When | Example |
|-------|----------|---------|
| **Bridge** | Most application code. Suite calls, endpoint-mapped calls, batch operations. | `bridge.callSuite('AIArt', 'GetArtName', { art })` |
| **PluginAdapter** | You want HTTP calls + SSE events composed in one object. | `new PluginAdapter({ serverUrl })` |
| **Primitives** | Convenience wrappers for common art/text operations. | `getArtBounds(bridge.call, uuid)` |
| **HttpAdapter** | Building a custom adapter. Internal building block. | Rarely used directly. |
| **SSEAdapter** | You need event streaming without PluginAdapter. | `connectSSE({ serverUrl })` |

**For most projects:** use `Bridge` for calling Illustrator and `SSEAdapter` (or `PluginAdapter`) for events. The generated suite files all use `Bridge` internally via `setBridgeInstance()`.

## Module Map

```
sdk/src/
├── bridge/          Core — request serialization and HTTP transport
├── adapters/        Core — HTTP, SSE, Plugin facade, Document, Placement
├── primitives/      Core — low-level art/text manipulation functions
├── geometry/        Core — coordinate transforms, artboard bounds
├── generated/       Core — auto-generated suite clients (442+ functions)
├── types/           Core — shared type definitions
├── utils/           Utilities — async safety, environment detection, unit conversions
├── services/        Utilities — settings, fonts, appearance, SVG, symbols, document index
├── tauri/           Utilities — desktop filesystem and dialogs (Tauri only)
└── schemas/         Utilities — Zod validation schemas
```

**Core** modules are needed by every NUXP plugin. **Utility** modules provide higher-level functionality you can adopt as needed.

## Threading and Request Serialization

NUXP's C++ plugin is single-threaded — every SDK call must execute on Illustrator's main thread. To protect against overwhelming the host, the Bridge serializes requests through an `AutoQueue`:

```
Your code             SDK                    C++ Plugin
   │                   │                       │
   ├─ callSuite() ───► AutoQueue ──► HTTP ───► MainThreadDispatch ──► SDK call
   │                   (one at a time)         (main thread only)
   │
   ├─ callSuite() ───► queued...
   │                   waits for first to complete
```

- **AutoQueue** is enabled by default. Each call waits for the previous one to complete before sending the next HTTP request. This prevents connection pool exhaustion on the C++ server.
- **`batch()`** bypasses the queue. Use it when you have multiple independent calls that don't depend on each other — they'll execute in parallel.
- **Retries** are automatic. Transient failures (5xx, timeouts, network errors) retry up to 3 times with 1-second delay.

For the full threading story (including the C++ `MainThreadDispatch` mechanics), see [Architecture](../docs/ARCHITECTURE.md).

## Events

The plugin pushes real-time events to the frontend via Server-Sent Events:

| Event | Payload | Fires When |
|-------|---------|------------|
| `selection` | `{ count, selectedIds }` | User selects or deselects art objects |
| `document` | `{ type, documentName }` | Document opened, closed, or switched |
| `layers` | `{ layerCount }` | Layers added, removed, or reordered |
| `artChanged` | `{ artIds, changeType }` | Art objects created, modified, or deleted |
| `version` | `{ version, build }` | Sent once on initial connection |

```typescript
import { connectSSE } from '@nuxp/sdk'

const sse = connectSSE({ serverUrl: 'http://localhost:8080' })

sse.on('selection', (data) => {
  // data is typed as SelectionEvent — { count: number, selectedIds: number[] }
  console.log(`Selected: ${data.selectedIds}`)
})

sse.on('document', (data) => {
  if (data.type === 'closed') {
    // Handle IDs from previous document are now invalid
  }
})

// Wildcard — receive all events
sse.onAll((event) => console.log(event.type, event.data))

// Clean up
sse.disconnect()
```

The SSE connection automatically reconnects on network interruption (linear back-off, max 10 attempts).

## Services

Higher-level services for common plugin operations. All services that talk to Illustrator accept a bridge callable via constructor injection.

| Service | Purpose | Constructor |
|---------|---------|-------------|
| `LoggerService` | Per-module logging with ring buffer | `createLogger('myModule')` |
| `SettingsService<T>` | Type-safe localStorage persistence | `new SettingsService(storageKey, defaults)` |
| `DocumentIndexService<T>` | Typed item index stored in document XMP metadata | `new DocumentIndexService(bridge.call, options)` |
| `FontConfigService` | Font enumeration and caching | `new FontConfigService(fontConfig)` |
| `AppearanceConfigService` | UI theme and preset management | `new AppearanceConfigService(options)` |
| `SymbolManagementService` | Illustrator symbol import and lifecycle | `new SymbolManagementService(bridge.call)` |
| `AssetCache` | In-memory asset cache with LRU eviction | `createAssetCache(options)` |
| `SvgPlacementService` | SVG metadata extraction and placement | `new SvgPlacementService(bridge.call)` |

## Primitives

Low-level functions for direct art/text manipulation. Every primitive takes a `BridgeCallFn` as its first argument -- no global state, fully testable.

### Art and Text

```typescript
import { getArtBounds, transformArt, setTextContent } from '@nuxp/sdk'

const bounds = await getArtBounds(bridge.call, 'art-uuid-123')
await transformArt(bridge.call, 'art-uuid-123', { tx: 50, ty: -30 })
await setTextContent(bridge.call, 'text-uuid-456', 'Hello Illustrator')
```

### Groups

Create groups, move art into them, iterate children, and ungroup:

```typescript
import { createGroup, addToGroup, getGroupChildren, ungroup } from '@nuxp/sdk'

// Create an empty group and populate it
const group = await createGroup(bridge.call)
await addToGroup(bridge.call, group, artHandle1)
await addToGroup(bridge.call, group, artHandle2)

// Iterate children
const children = await getGroupChildren(bridge.call, group)

// Ungroup — moves children out and disposes the empty group shell
const formerChildren = await ungroup(bridge.call, group)
```

### Layers

Query, create, and enumerate art on layers:

```typescript
import { getLayerByName, createLayer, getLayerArt, getAllLayers } from '@nuxp/sdk'

// Find a layer by its display name
const layer = await getLayerByName(bridge.call, 'Background')
if (layer) {
  const artOnLayer = await getLayerArt(bridge.call, layer)
}

// Create a new layer
const newLayer = await createLayer(bridge.call, 'Annotations')

// List all layers with handles and names
const layers = await getAllLayers(bridge.call)
// => [{ handle: 1, name: 'Layer 1' }, { handle: 2, name: 'Background' }]
```

### Duplication

Clone art objects and optionally reposition the copy:

```typescript
import { duplicateArt, duplicateArtToPosition } from '@nuxp/sdk'

// Simple duplicate (placed above the original)
const copy = await duplicateArt(bridge.call, artHandle)

// Duplicate and reposition relative to a target
const repositioned = await duplicateArtToPosition(bridge.call, artHandle, targetHandle, 0)
```

### Full Primitives Reference

| Module | Functions |
|--------|-----------|
| **art.ts** | `getArtBounds`, `transformArt`, `setArtVisibility`, `getArtChildren`, `getGroupChildByType`, `setArtName`, `deleteArt`, `findArtByName`, `getArtboardInfo` |
| **text.ts** | `setTextFont`, `setTextFontSize`, `setTextContent`, `getPathSegments` |
| **group.ts** | `createGroup`, `addToGroup`, `ungroup`, `getGroupChildren` |
| **layer.ts** | `getLayerByName`, `createLayer`, `getLayerArt`, `getAllLayers` |
| **duplication.ts** | `duplicateArt`, `duplicateArtToPosition` |

## Generated Suites

The 442+ auto-generated functions are organized by SDK suite (AIArtSuite, AIDocumentSuite, etc.). They use `callCpp()` internally, which delegates to whichever Bridge instance you registered with `setBridgeInstance()`.

```typescript
import { setBridgeInstance, createBridge } from '@nuxp/sdk'
import { GetArtType, GetArtBounds } from '@nuxp/sdk/generated/AIArtSuite'

setBridgeInstance(createBridge({ port: 8080 }))

const artType = await GetArtType(handleId)
const bounds = await GetArtBounds(handleId)
```

## Design Principles

- **Bridge-first**: Functions accept a bridge callable as the first parameter. No hidden singletons (except `setBridgeInstance` for generated code compatibility).
- **Framework-agnostic**: No Vue, React, or Pinia. Use the SDK from any JavaScript environment.
- **Constructor DI**: Services receive their dependencies in the constructor. Easy to test, easy to swap.
