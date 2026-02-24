---
layout: default
title: Architecture
---

# NUXP Architecture

<p align="center">
  <img src="images/nuxp-dance.gif" alt="Captain NUXP" width="200">
</p>

NUXP bridges a TypeScript frontend to Adobe Illustrator's C++ SDK via an HTTP/JSON server embedded in the plugin. Three layers are in play:

```
TypeScript SDK (Bridge + AutoQueue)  <--HTTP/JSON-->  C++ Plugin (MainThreadDispatch)  <--PICA Suites-->  Adobe Illustrator
                                     <--SSE Stream--
```

The TypeScript SDK (`@nuxp/sdk`) serializes outgoing requests and manages event subscriptions. The C++ plugin, loaded as a `.aip` inside Illustrator, runs an embedded HTTP server that marshals requests to Illustrator's main thread and returns JSON responses. The frontend can be a Tauri desktop app, a dev server, or any HTTP client.

---

## 1. Threading Model

This is the most important concept in NUXP. Get this wrong and Illustrator will crash.

**Three layers handle concurrency:**

| Layer | Role |
|-------|------|
| **TypeScript AutoQueue** | Serializes outgoing HTTP requests so only one is in flight at a time. Prevents connection pool exhaustion on the C++ server. |
| **C++ HTTP server thread** | Background `std::thread` running cpp-httplib. Receives HTTP requests and blocks until the main thread processes them. |
| **Illustrator main thread** | The only thread where Adobe SDK calls are permitted. Drains queued work via a timer callback (~16ms). |

Adobe Illustrator's SDK is not thread-safe. Every suite function pointer (`SuitePointers::AIArt()`, `SuitePointers::AIDocument()`, etc.) must be called from Illustrator's main thread. The HTTP server runs on a background thread. `MainThreadDispatch` bridges the gap on the C++ side. The TypeScript `AutoQueue` provides client-side serialization.

### Full Request Lifecycle

A call from TypeScript to the Illustrator SDK passes through both queues:

```mermaid
%%{init: {'theme': 'base', 'themeVariables': {
  'actorBkg': '#1B3A6B',
  'actorBorder': '#F5C518',
  'actorTextColor': '#FFFFFF',
  'actorLineColor': '#F5C518',
  'signalColor': '#F5C518',
  'signalTextColor': '#FFFFFF',
  'noteBkgColor': '#C41E24',
  'noteTextColor': '#FFFFFF',
  'noteBorderColor': '#F5C518',
  'activationBkgColor': '#1B3A6B',
  'activationBorderColor': '#F5C518',
  'sequenceNumberColor': '#FFFFFF'
}}}%%
sequenceDiagram
    participant App as Your TypeScript Code
    participant AQ as AutoQueue
    participant HTTP as HTTP Server Thread
    participant MTD as MainThreadDispatch
    participant AI as Illustrator Main Thread

    App->>AQ: bridge.callSuite('AIArt', 'GetArtName', { art })
    Note over AQ: Waits if another call is in flight
    AQ->>HTTP: POST /api/call { suite, method, args }
    HTTP->>MTD: Run(lambda)
    Note over HTTP: Blocked (cv.wait)
    AI->>MTD: Timer callback (~16ms) drains queue
    MTD->>AI: Execute lambda (SDK call)
    AI-->>MTD: Result (JSON)
    MTD-->>HTTP: cv.notify
    HTTP-->>AQ: HTTP 200 { success, result }
    AQ-->>App: Promise resolves
    Note over AQ: Dequeues next pending call
```

**Why two queues?** They solve different problems:
- **AutoQueue (TypeScript)** prevents connection pool exhaustion. The C++ HTTP server blocks one server thread per in-flight request (waiting on `condition_variable`). Flooding it with concurrent requests would exhaust server threads and cause timeouts.
- **MainThreadDispatch (C++)** ensures thread safety. SDK calls must happen on Illustrator's main thread, period. The work queue + timer callback pattern marshals execution from the HTTP thread to the main thread.

**`batch()` bypasses AutoQueue.** When you have multiple independent calls (e.g., fetching names for 10 art objects), `Bridge.batch()` sends them as parallel HTTP requests. The C++ `MainThreadDispatch` still serializes their execution on the main thread, but you eliminate the HTTP round-trip gap between each call. Use `batch()` when calls don't depend on each other.

### Server-Sent Events (Reverse Channel)

While HTTP request/response handles commands, SSE provides real-time push events from Illustrator to the frontend:

- Selection changes, document lifecycle, layer modifications, art changes
- Persistent EventSource connection to `/events/stream`
- Automatic reconnection with back-off (capped at 5x base delay, max 10 attempts)
- Independent of the HTTP request path — events arrive regardless of pending calls

### How MainThreadDispatch Works

1. An HTTP request arrives on the server thread.
2. The handler calls `MainThreadDispatch::Run(lambda)`, which creates a `WorkItem`, pushes it onto a thread-safe queue, and blocks (via `condition_variable::wait`).
3. On the main thread, `AITimerSuite` fires a periodic callback (~16ms). `ProcessQueue()` drains all pending work items, executing each lambda on the main thread.
4. When a `WorkItem` completes, its `condition_variable` is notified, unblocking the server thread.
5. The handler receives the result and sends the HTTP response.

```mermaid
%%{init: {'theme': 'base', 'themeVariables': {
  'actorBkg': '#1B3A6B',
  'actorBorder': '#F5C518',
  'actorTextColor': '#FFFFFF',
  'actorLineColor': '#F5C518',
  'signalColor': '#F5C518',
  'signalTextColor': '#FFFFFF',
  'noteBkgColor': '#C41E24',
  'noteTextColor': '#FFFFFF',
  'noteBorderColor': '#F5C518',
  'activationBkgColor': '#1B3A6B',
  'activationBorderColor': '#F5C518',
  'sequenceNumberColor': '#FFFFFF'
}}}%%
sequenceDiagram
    participant Client as Frontend
    participant HTTP as HTTP Server Thread
    participant Queue as Work Queue
    participant Timer as AITimerSuite Callback
    participant SDK as Adobe SDK (Main Thread)

    Client->>HTTP: HTTP Request
    HTTP->>Queue: MainThreadDispatch::Run(lambda)
    Note over HTTP: Blocked (cv.wait)
    Timer->>Queue: ProcessQueue()
    Queue->>SDK: Execute lambda
    SDK-->>Queue: Result (json)
    Queue-->>HTTP: cv.notify (result ready)
    HTTP-->>Client: HTTP Response (JSON)
```

### Real Example from NUXPHandlers.cpp

Every handler follows this pattern -- wrap SDK calls inside `MainThreadDispatch::Run()`:

{% raw %}
```cpp
std::string HandleGetSelection() {
    json result = MainThreadDispatch::Run([]() -> json {
        // --- This lambda runs on Illustrator's main thread ---
        AIArtHandle** matches = nullptr;
        ai::int32 numMatches = 0;

        ASErr err = SuitePointers::AIMatchingArt()->GetSelectedArt(
            &matches, &numMatches);
        if (err != kNoErr) {
            return {{"success", false}, {"error", "GetSelectedArt failed"}};
        }

        json handles = json::array();
        for (ai::int32 i = 0; i < numMatches; ++i) {
            handles.push_back(HandleManager::art.Register((*matches)[i]));
        }

        SuitePointers::AIMdMemory()->MdMemoryDisposeHandle(
            reinterpret_cast<AIMdMemoryHandle>(matches));

        return {{"success", true}, {"handles", handles}};
    });
    return result.dump();
}
```
{% endraw %}

There is also `RunWithTimeout()` for cases where you want to avoid blocking indefinitely:

```cpp
auto result = MainThreadDispatch::RunWithTimeout([&]() -> json {
    return callSdkFunction();
}, std::chrono::seconds(5));
// result is std::optional<json> -- empty if timeout
```

> **WARNING**: Every custom endpoint handler **must** wrap SDK calls in `MainThreadDispatch::Run()`. Calling SDK functions directly from an HTTP handler will crash Illustrator. There are no exceptions to this rule.

### Timeout Safety

`WorkItem` uses `shared_ptr` for its mutex, condition variable, and result. If a timeout occurs and the HTTP handler returns early, the main thread can still safely execute the lambda and write to the result without accessing freed memory.

---

## 2. Handle Management

C++ pointers like `AIArtHandle` (which is `ArtObject*`) cannot be serialized as JSON. The frontend needs a way to reference Illustrator objects across HTTP requests.

`HandleRegistry<T>` solves this by assigning stable integer IDs to raw pointers.

```mermaid
%%{init: {'theme': 'base', 'themeVariables': {
  'primaryColor': '#1B3A6B',
  'primaryTextColor': '#FFFFFF',
  'primaryBorderColor': '#F5C518',
  'lineColor': '#F5C518',
  'secondaryColor': '#C41E24',
  'secondaryTextColor': '#FFFFFF',
  'tertiaryColor': '#2A5298',
  'tertiaryTextColor': '#FFFFFF',
  'edgeLabelBackground': 'transparent'
}}}%%
flowchart TD
    A["SDK returns AIArtHandle*"] --> B["HandleManager::art.Register(ptr)"]
    B --> C["Returns integer ID (e.g. 42)"]
    C --> D["ID sent to frontend as JSON"]
    D --> E["Frontend sends ID back in later request"]
    E --> F["HandleManager::art.Get(42)"]
    F --> G{Generation matches?}
    G -- "Yes" --> H["Returns original AIArtHandle*"]
    G -- "No (stale)" --> I["Returns nullptr"]
```

### HandleRegistry Internals

```cpp
template <typename T>
class HandleRegistry {
    struct Entry {
        T* ptr;
        uint32_t generation;  // generation when registered
    };
    std::unordered_map<int32_t, Entry> entries_;
    int32_t nextId_ = 1;      // 0 is reserved for null
    uint32_t generation_ = 0;
    // ...
};
```

Key behaviors:

- **Register**: Assigns a new integer ID (monotonically increasing, never reused). Thread-safe via `shared_mutex`.
- **Get**: Returns the pointer only if the entry's generation matches the current generation. Returns `nullptr` for stale handles.
- **BumpGeneration**: Increments the generation counter and clears all entries. Called on document switch, undo, redo, or document close.

### Pre-built Registries

`HandleManager` provides typed registries for every handle type the SDK uses:

```cpp
class HandleManager {
public:
    static HandleRegistry<ArtObject> art;           // AIArtHandle
    static HandleRegistry<_t_AILayerOpaque> layers;  // AILayerHandle
    static HandleRegistry<_t_AIDocument> documents;  // AIDocumentHandle
    static HandleRegistry<void> patterns;            // AIPatternHandle (void*)
    static HandleRegistry<void> gradients;           // AIGradientHandle (void*)
    // ... plus masks, tools, timers, dictionaries, artStyles, etc.

    static void InvalidateAll();  // BumpGeneration on all registries
};
```

### Usage Pattern

Register on the main thread (inside `MainThreadDispatch::Run`), send the ID to the frontend:

{% raw %}
```cpp
// Registering
int32_t id = HandleManager::art.Register(artHandle);
responseJson["handle"] = id;

// Later, looking up
AIArtHandle art = HandleManager::art.Get(id);
if (!art) {
    return {{"success", false}, {"error", "Invalid or stale art handle"}};
}
```
{% endraw %}

---

## 3. Code Generation Pipeline

NUXP auto-generates C++ wrappers and TypeScript clients from Adobe's SDK headers. This avoids writing boilerplate for each of the hundreds of SDK functions.

```mermaid
%%{init: {'theme': 'base', 'themeVariables': {
  'primaryColor': '#1B3A6B',
  'primaryTextColor': '#FFFFFF',
  'primaryBorderColor': '#F5C518',
  'lineColor': '#C41E24',
  'secondaryColor': '#F5C518',
  'tertiaryColor': '#2A5298',
  'tertiaryTextColor': '#FFFFFF',
  'clusterBkg': '#0D2240',
  'clusterBorder': '#F5C518',
  'clusterTextColor': '#F5C518'
}}}%%
flowchart TB
    subgraph Input [" Input "]
        SDK["SDK Headers (.h files)"]
        Routes["routes.json"]
    end

    subgraph Codegen [" Codegen (Node.js + Tree-sitter) "]
        Parser["Tree-sitter Parser"]
        TC["TypeClassifier"]
        CppGen["CppGenerator"]
        TsGen["TypeScriptGenerator"]
        CRGen["CustomRouteGenerator"]
    end

    subgraph Output_CPP [" C++ Output (plugin/src/endpoints/generated/) "]
        Wrappers["AI*SuiteWrapper.h (one per suite)"]
        Central["CentralDispatcher.h"]
        CRHandlers["CustomRouteHandlers.h"]
        CRReg["CustomRouteRegistration.cpp"]
    end

    subgraph Output_TS [" TypeScript Output (sdk/src/generated/) "]
        TsClients["ai*.ts (one per suite)"]
        TsCustom["customRoutes.ts"]
    end

    SDK --> Parser
    Parser --> TC
    TC --> CppGen
    TC --> TsGen
    CppGen --> Wrappers
    CppGen --> Central
    TsGen --> TsClients

    Routes --> CRGen
    CRGen --> CRHandlers
    CRGen --> CRReg
    CRGen --> TsCustom
```

### Step by Step

1. **Parse**: Tree-sitter walks SDK C headers and extracts `SuiteInfo` JSON -- suite names, function signatures, parameter types.

2. **Classify**: `TypeClassifier` categorizes each parameter:
   - **Handle** -- `AIArtHandle`, `AILayerHandle`, etc. Maps to integer IDs via `HandleManager`.
   - **ManagedHandle** -- RAII objects like `ai::ArtboardProperties`. Plugin owns lifetime.
   - **String** -- `ai::UnicodeString`, `const char*`. Serialized as JSON strings.
   - **Primitive** -- `AIBoolean`, `AIReal`, `ai::int32`. Direct JSON types.
   - **Struct** -- `AIRealRect`, `AIRealPoint`. Serialized as JSON objects with named fields.
   - **Enum** -- `AIArtType`, etc. Mapped to integers.
   - **Error** -- `ASErr` return values. Checked for `kNoErr`.

3. **Generate C++**: `CppGenerator` produces an `AI*SuiteWrapper.h` for each suite. Each wrapper has a `Dispatch(method, params)` function that switches on method name, extracts params from JSON, calls the SDK, and returns JSON.

4. **Generate TypeScript**: `TypeScriptGenerator` produces a matching `ai*.ts` file for each suite, with typed function signatures that call `callCpp(suite, method, args)`.

5. **CentralDispatcher**: A generated `if/else` chain that routes `(suite, method)` pairs to the correct suite wrapper's `Dispatch` function.

6. **Custom Routes**: `routes.json` defines hand-written REST endpoints. The generator produces:
   - `CustomRouteHandlers.h` -- function declarations (you implement the bodies)
   - `CustomRouteRegistration.cpp` -- calls `HttpServer::Get/Post/...` to wire routes
   - `customRoutes.ts` -- TypeScript client functions

### Running the Pipeline

```bash
./scripts/generate.sh
```

This script:
1. Installs codegen npm dependencies if needed
2. Runs `npm run generate` in `codegen/`
3. Copies generated C++ to `plugin/src/endpoints/generated/`
4. Copies generated TypeScript to `sdk/src/generated/`

CMake also provides targets:
```bash
cmake --build build -t generate      # regenerate wrappers
cmake --build build -t regenerate    # regenerate + rebuild plugin
```

---

## 4. Request Routing

HTTP requests reach the plugin through two distinct paths.

```mermaid
%%{init: {'theme': 'base', 'themeVariables': {
  'primaryColor': '#1B3A6B',
  'primaryTextColor': '#FFFFFF',
  'primaryBorderColor': '#F5C518',
  'lineColor': '#F5C518',
  'secondaryColor': '#C41E24',
  'secondaryTextColor': '#FFFFFF',
  'tertiaryColor': '#2A5298',
  'tertiaryTextColor': '#FFFFFF',
  'edgeLabelBackground': 'transparent'
}}}%%
flowchart TB
    Req["HTTP Request"] --> Type{Route type?}

    Type -- "POST /api/call" --> APICall["Generic Suite Dispatch"]
    Type -- "POST /AIArt/NewArt" --> SuiteShort["Suite Shorthand (regex catch-all)"]
    Type -- "GET /api/selection etc." --> Custom["Custom Route (registered handlers)"]

    APICall --> MTD1["MainThreadDispatch::Run()"]
    SuiteShort --> MTD2["MainThreadDispatch::Run()"]
    Custom --> Handler["Hand-written Handler"]
    Handler --> MTD3["MainThreadDispatch::Run()"]

    MTD1 --> CD["CentralDispatcher::Dispatch(suite, method, args)"]
    MTD2 --> CD
    CD --> Wrapper["Generated Suite Wrapper"]
    Wrapper --> SDKCall["SDK Function Call"]

    MTD3 --> SDKCall2["SDK Function Calls (via SuitePointers)"]

    SDKCall --> JSON["JSON Response"]
    SDKCall2 --> JSON
```

### Path 1: Generated Suite Dispatch

Two equivalent entry points:

```
POST /api/call
Body: { "suite": "AIArtSuite", "method": "NewArt", "args": { "type": 1 } }
```

```
POST /AIArtSuite/NewArt
Body: { "type": 1 }
```

Both go through `MainThreadDispatch::Run()`, then `CentralDispatcher::Dispatch()`, then the generated wrapper. The `/api/call` form is preferred for the TypeScript SDK. The `/{suite}/{method}` shorthand is a regex catch-all (`R"(/(\w+)/(\w+))"`) registered last so it does not shadow custom routes.

### Path 2: Custom Routes

Defined in `routes.json`, wired by generated `CustomRouteRegistration.cpp`, implemented in hand-written `.cpp` files under `plugin/src/endpoints/handwritten/`.

Custom routes are registered **before** the generic suite dispatcher to ensure specific paths like `/api/selection` are not captured by the `/{suite}/{method}` regex.

Planned custom routes (defined in `routes.json`, C++ handlers not yet implemented) include shape creation endpoints (`CreateRectangle`, `CreateEllipse`, `CreatePath`, `CreateLine`) and a native file save dialog (`ShowFileSaveDialog`). Once their C++ handlers are written, the TypeScript clients will be auto-generated.

There are two sub-types:

**Static routes** (exact path match):
```
GET  /api/selection
POST /api/selection/select
GET  /api/doc/info
```

**Pattern routes** (regex with capture groups):
```
GET  /api/art/{id}/style   -->  R"(/api/art/([a-zA-Z0-9_.-]+)/style)"
POST /api/art/{id}/segments -->  R"(/api/art/([a-zA-Z0-9_.-]+)/segments)"
```

Captured groups are passed to the handler as a `std::vector<std::string>`:

```cpp
// Generated registration (CustomRouteRegistration.cpp)
HttpServer::GetWithPattern(
    R"(/api/art/([a-zA-Z0-9_.-]+)/style)",
    [](const std::string& body, const std::vector<std::string>& params) {
        return HandleGetPathStyle(params[0]);  // params[0] = the {id}
    });
```

### Suite Pointers

All SDK suite function pointers are acquired once at plugin startup via `SuitePointers::Acquire()` and accessed through static methods:

```cpp
SuitePointers::AIArt()->GetArtType(art, &type);
SuitePointers::AIDocument()->GetDocumentModified(&modified);
SuitePointers::AILayer()->CountLayers(&count);
```

Suites are released during `SuitePointers::Release()` at plugin shutdown. Check `SuitePointers::IsValid()` if you need to verify acquisition succeeded.

---

## 5. Writing a Custom Endpoint

To add a new hand-written endpoint:

1. **Add the route to `codegen/src/config/routes.json`**:
```json
{
  "name": "MyNewEndpoint",
  "method": "POST",
  "path": "/api/my-feature",
  "description": "Does something useful.",
  "request": { "param1": { "type": "string" } },
  "response": { "result": { "type": "number" } }
}
```

2. **Regenerate** (`./scripts/generate.sh`). This updates `CustomRouteHandlers.h` with the new declaration and `CustomRouteRegistration.cpp` with the route wiring.

3. **Implement the handler** in a `.cpp` file under `plugin/src/endpoints/handwritten/`:
{% raw %}
```cpp
#include "CustomRouteHandlers.h"
#include "MainThreadDispatch.hpp"
#include "SuitePointers.hpp"
#include "HandleManager.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace NUXP {

std::string HandleMyNewEndpoint(const std::string& body) {
    json params = json::parse(body);

    json result = MainThreadDispatch::Run([&params]() -> json {
        // SDK calls go here -- this runs on the main thread
        return {{"success", true}, {"result", 42}};
    });
    return result.dump();
}

} // namespace NUXP
```
{% endraw %}

4. **Build**: `cd plugin && cmake --build build`

The TypeScript client function is generated automatically in `sdk/src/generated/customRoutes.ts`.

---

## 6. TypeScript SDK (`@nuxp/sdk`)

The SDK package (`sdk/`) is a framework-agnostic TypeScript library that provides the complete communication and utility layer between any frontend and the C++ plugin. It is consumed by the demo app via `workspace:*` and by downstream projects (like flora-uxp) via git submodule.

### Module Overview

```
sdk/src/
├── bridge/         # Bridge, AutoQueue — core communication
├── adapters/       # HTTP, SSE, Plugin, Document, Placement
├── services/       # Settings, logging, fonts, appearance, assets, SVG, symbols, document index
├── geometry/       # Coordinate transforms, artboard bounds
├── primitives/     # Low-level art/text/group/layer/duplication (dependency-injected)
├── generated/      # Auto-generated suite clients (19 suites, 442+ functions)
├── tauri/          # Desktop filesystem and dialog wrappers
├── utils/          # Async safety, environment detection, unit conversions
├── schemas/        # Zod validation schemas for document responses
└── types/          # Shared type definitions
```

### Adapters

Adapters wrap communication patterns into reusable, composable layers:

| Adapter | Purpose |
|---------|---------|
| **HttpAdapter** | HTTP transport with configurable base URL, retry logic, and timeout handling |
| **SSEAdapter** | Typed Server-Sent Events with automatic reconnection and backoff |
| **PluginAdapter** | Composes HttpAdapter + SSEAdapter into a unified interface with environment detection |
| **DocumentAdapter** | 5 generic document operations: `getDocumentInfo`, `getArtboards`, `getSelection`, `getLayers`, `getDocumentItems` |
| **PlacementAdapter** | 7 placement workflow functions: `placeItem`, `positionItem`, `scaleItem`, `rotateItem`, `groupItems`, `ungroupItems`, `deleteItems` |

### Services

| Service | Purpose |
|---------|---------|
| **LoggerService** | Per-module logging with ring buffer, console interception, and persistent config |
| **SettingsService** | Type-safe localStorage persistence with schema validation |
| **FontConfigService** | Font enumeration, caching, and lookup |
| **AppearanceConfigService** | UI theme and preset management |
| **AssetCache** | In-memory asset caching with LRU eviction |
| **SvgLoader** | SVG loading from URLs and local paths |
| **SvgPlacementService** | SVG metadata extraction and placement coordinate calculation |
| **SymbolManagementService** | Illustrator symbol import and lifecycle management |
| **DocumentIndexService** | Typed item indexing stored in document XMP metadata |

### Primitives

Low-level art and text manipulation functions that use dependency injection via `BridgeCallFn` — they accept a bridge call function as a parameter rather than importing a global bridge, making them testable and composable.

| Module | Functions | Description |
|--------|-----------|-------------|
| **art.ts** | `getArtBounds`, `transformArt`, `setArtVisibility`, `getArtChildren`, `getGroupChildByType`, `setArtName`, `deleteArt`, `findArtByName`, `getArtboardInfo` | Core art object queries and transforms |
| **text.ts** | `setTextFont`, `setTextFontSize`, `setTextContent`, `getPathSegments` | Text styling and path segment access |
| **group.ts** | `createGroup`, `addToGroup`, `ungroup`, `getGroupChildren` | Group creation, child iteration, ungrouping |
| **layer.ts** | `getLayerByName`, `createLayer`, `getLayerArt`, `getAllLayers` | Layer queries, creation, art enumeration |
| **duplication.ts** | `duplicateArt`, `duplicateArtToPosition` | Art cloning and repositioning |

The group, layer, and duplication primitives compose auto-generated `AIArtSuite` and `AILayerSuite` functions internally, providing higher-level operations while maintaining the same dependency-injection pattern.

### Geometry

| Module | Purpose |
|--------|---------|
| **CoordinateSystemManager** | 30+ static methods for coordinate system conversions (D3 ↔ Illustrator), bounds operations (get, inset, expand, intersect, validate), point transforms (translate, rotate, scale), polygon area/centroid, circle overlap detection, and unit conversions (inches ↔ points ↔ feet, drawing scale application) |
| **ArtboardBoundsManager** | Artboard bounds calculations, artboard-relative positioning |

### Tauri Desktop Integration

| Module | Purpose |
|--------|---------|
| **tauriFs** | Filesystem wrappers: `exists`, `mkdir`, `readTextFile`, `writeTextFile`, `remove` |
| **TauriDialogService** | Save dialogs: `saveTextFile`, `saveBinaryFile` |

### Utilities

| Module | Purpose |
|--------|---------|
| **apiHelpers** | HTTP result parsing and error extraction |
| **asyncErrorHandling** | Safe async wrappers to prevent unhandled rejections |
| **contrastChecker** | Color contrast ratio calculation for accessibility |
| **sizeCalculator** | Unit conversions (inches, points, feet, drawing scales) |
| **environment** | Runtime environment detection (Tauri, browser, mock mode) |

### Schemas

Zod validation schemas for document responses, used to validate data at the boundary between the SDK and the C++ plugin:

`InitializeDocumentResponseSchema`, `ArtboardInfoSchema`, `CreateArtboardResponseSchema`, `SuccessResponseSchema`, `ViewZoomResponseSchema`, and associated parsing helpers.
