# Adding Custom Endpoints to NUXP

A step-by-step guide for adding your own HTTP endpoints to the NUXP plugin. Time estimate: 15 minutes.

## Overview

NUXP uses a `routes.json` configuration file to generate both C++ handler declarations and TypeScript client functions. The workflow is:

1. **Define** the route once in `codegen/src/config/routes.json`
2. **Run codegen** to produce C++ declarations, HTTP wiring, and a typed TypeScript client
3. **Implement** only the C++ handler body (the business logic)

The codegen guarantees that parameter names, types, and HTTP methods stay in sync between the C++ plugin and the TypeScript frontend. You never manually register an HTTP route or write a fetch wrapper.

---

## Step 1: Define the Route

Open `codegen/src/config/routes.json` and add a new entry to the `routes` array.

**Example:** We want an endpoint that returns the number of artboards in the current document.

```json
{
  "name": "GetArtboardCount",
  "method": "GET",
  "path": "/api/artboard/count",
  "description": "Get the number of artboards in the current document.",
  "response": {
    "count": { "type": "number", "description": "Number of artboards" }
  }
}
```

Add this object inside the existing `routes` array:

```json
{
  "namespace": "NUXP",
  "routes": [
    // ... existing routes ...

    {
      "name": "GetArtboardCount",
      "method": "GET",
      "path": "/api/artboard/count",
      "description": "Get the number of artboards in the current document.",
      "response": {
        "count": { "type": "number", "description": "Number of artboards" }
      }
    }
  ]
}
```

### Route Field Reference

| Field | Required | Description |
|-------|----------|-------------|
| `name` | Yes | Identifier used for the C++ handler name (`Handle{Name}`) and TypeScript function name. Use PascalCase. |
| `method` | Yes | HTTP method: `"GET"`, `"POST"`, `"PUT"`, `"DELETE"`, or `"PATCH"` |
| `path` | Yes | URL path. Use `{param}` placeholders for path parameters (e.g. `/api/art/{id}/style`). |
| `description` | No | Documentation string. Appears in generated code comments. |
| `pathParams` | Only if path has `{param}` | Object mapping each path parameter name to its type definition. |
| `request` | No | Request body fields (for POST/PUT/PATCH). Each field has `type`, optional `description`, and optional `optional: true`. |
| `response` | No | Response fields. Used to generate TypeScript interfaces. |
| `rawBody` | No | When `true`, body is passed as-is instead of being mapped to named fields. Cannot coexist with `request`. |

### Supported Field Types

| Type in JSON | C++ Type | TypeScript Type |
|-------------|----------|-----------------|
| `"string"` | `std::string` | `string` |
| `"number"` | `double` | `number` |
| `"boolean"` | `bool` | `boolean` |
| `"number[]"` | `std::vector<double>` | `number[]` |
| `"string[]"` | `std::vector<std::string>` | `string[]` |
| `"object"` | `nlohmann::json` | `Record<string, unknown>` |

### More Examples

**POST with request body:**

```json
{
  "name": "RenameLayer",
  "method": "POST",
  "path": "/api/layer/rename",
  "description": "Rename a layer by index.",
  "request": {
    "index": { "type": "number", "description": "Layer index (0-based)" },
    "name": { "type": "string", "description": "New layer name" }
  },
  "response": {
    "success": { "type": "boolean" }
  }
}
```

**GET with path parameter:**

```json
{
  "name": "GetArtBounds",
  "method": "GET",
  "path": "/api/art/{id}/bounds",
  "pathParams": {
    "id": { "type": "string", "description": "Art handle ID" }
  },
  "response": {
    "left": { "type": "number" },
    "top": { "type": "number" },
    "right": { "type": "number" },
    "bottom": { "type": "number" }
  }
}
```

**POST with path parameter AND body:**

This produces a handler signature with two parameters: `HandleSetArtName(const std::string& id, const std::string& body)`.

```json
{
  "name": "SetArtName",
  "method": "POST",
  "path": "/api/art/{id}/name",
  "pathParams": {
    "id": { "type": "string", "description": "Art handle ID" }
  },
  "request": {
    "name": { "type": "string", "description": "New name for the art object" }
  },
  "response": {
    "success": { "type": "boolean" }
  }
}
```

---

## Step 2: Run Code Generation

From the codegen directory:

```bash
cd codegen && npm run generate
```

Or from the plugin build directory (if you already have a CMake build configured):

```bash
cd plugin && cmake --build build -t generate
```

Or use the full pipeline script which also copies files to their target directories:

```bash
./scripts/generate.sh
```

### What Gets Generated

Three files are updated:

| File | Purpose |
|------|---------|
| `plugin/src/endpoints/generated/CustomRouteHandlers.h` | Adds the C++ function declaration |
| `plugin/src/endpoints/generated/CustomRouteRegistration.cpp` | Wires the HTTP route to your handler |
| `shell/src/sdk/generated/customRoutes.ts` | Adds a typed TypeScript client function |

For our `GetArtboardCount` example, the codegen produces:

**In `CustomRouteHandlers.h`** -- the declaration you must implement:

```cpp
/**
 * GET /api/artboard/count - Get the number of artboards in the current document.
 * @returns JSON: { count: number }
 */
std::string HandleGetArtboardCount();
```

**In `CustomRouteRegistration.cpp`** -- the HTTP wiring (you never edit this):

```cpp
// GET /api/artboard/count - Get the number of artboards in the current document.
HttpServer::Get("/api/artboard/count", [](const std::string&) {
    return HandleGetArtboardCount();
});
```

**In `customRoutes.ts`** -- the TypeScript client:

```typescript
export interface GetArtboardCountResponse {
  /** Number of artboards */
  count: number;
}

export async function GetArtboardCount(): Promise<GetArtboardCountResponse> {
  return fetchRoute<GetArtboardCountResponse>('GET', getApiUrl('/api/artboard/count'));
}
```

### Handler Signature Rules

The codegen determines the C++ function signature from the route definition:

| Route Characteristics | Generated C++ Signature |
|----------------------|------------------------|
| GET, no path params | `std::string HandleFoo()` |
| POST with body | `std::string HandleFoo(const std::string& body)` |
| Path params only, no body | `std::string HandleFoo(const std::string& id)` |
| Path params + body | `std::string HandleFoo(const std::string& id, const std::string& body)` |

---

## Step 3: Implement the Handler

Create your handler implementation in a `.cpp` file inside `plugin/src/endpoints/handwritten/`. You can add it to the existing `NUXPHandlers.cpp` or create a new file -- the choice is yours.

Here is the complete, working implementation for `GetArtboardCount`:

```cpp
// In plugin/src/endpoints/handwritten/NUXPHandlers.cpp
// (or a new file like plugin/src/endpoints/handwritten/ArtboardHandlers.cpp)

#include "CustomRouteHandlers.h"
#include "IllustratorSDK.h"
#include "SuitePointers.hpp"
#include "MainThreadDispatch.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace NUXP {

std::string HandleGetArtboardCount() {
    json result = MainThreadDispatch::Run([]() -> json {
        // 1. Check that the required suite is available
        if (!SuitePointers::AIArtboard()) {
            return {{"success", false},
                    {"error", "AIArtboard suite not available"}};
        }

        // 2. Get the artboard list
        ai::ArtboardList artboardList;
        ASErr err = SuitePointers::AIArtboard()->GetArtboardList(artboardList);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "Failed to get artboard list"},
                    {"errorCode", static_cast<int>(err)}};
        }

        // 3. Get the count
        ai::ArtboardID count = 0;
        SuitePointers::AIArtboard()->GetCount(artboardList, count);

        // 4. Clean up SDK resources
        SuitePointers::AIArtboard()->ReleaseArtboardList(artboardList);

        // 5. Return JSON result
        return {{"count", static_cast<int>(count)}};
    });
    return result.dump();
}

} // namespace NUXP
```

### Critical Pattern: MainThreadDispatch::Run()

Every handler that calls the Adobe SDK **must** wrap those calls in `MainThreadDispatch::Run()`. This is non-negotiable.

**Why:** The HTTP server runs on a background thread, but all Adobe Illustrator SDK calls must execute on the main thread. `MainThreadDispatch::Run()` queues your lambda onto the main thread (via `AITimerSuite` callbacks) and blocks the HTTP thread until the work completes.

```cpp
json result = MainThreadDispatch::Run([]() -> json {
    // Everything inside this lambda runs on Illustrator's main thread.
    // SDK calls are safe here.
    return {{"key", "value"}};
});
return result.dump();  // Back on HTTP thread -- convert JSON to string
```

If you skip `MainThreadDispatch::Run()`, your SDK calls will execute on the HTTP thread and crash Illustrator.

### Error Handling Pattern

Follow the pattern used throughout `NUXPHandlers.cpp`:

1. **Check suite availability** before calling any SDK function
2. **Check `ASErr` return codes** after every SDK call
3. **Return structured errors** with `success: false`, an `error` message, and optionally `errorCode`
4. **Clean up SDK resources** (release artboard lists, dispose handles, free memory blocks)

```cpp
if (!SuitePointers::AILayer()) {
    return {{"success", false},
            {"error", "AILayer suite not available"}};
}

ASErr err = SuitePointers::AILayer()->CountLayers(&count);
if (err != kNoErr) {
    return {{"success", false},
            {"error", "CountLayers failed"},
            {"errorCode", static_cast<int>(err)}};
}
```

### POST Handler Example (with request body)

For a POST endpoint, the handler receives the request body as a JSON string. Parse it, validate required fields, then pass data into the `MainThreadDispatch::Run()` lambda:

```cpp
std::string HandleRenameLayer(const std::string& body) {
    // Parse the JSON body
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    // Validate required fields
    if (!params.contains("index") || !params.contains("name")) {
        return json{{"success", false},
                    {"error", "Missing required fields: index, name"}}.dump();
    }

    // Run SDK calls on main thread
    json result = MainThreadDispatch::Run([&params]() -> json {
        if (!SuitePointers::AILayer()) {
            return {{"success", false},
                    {"error", "AILayer suite not available"}};
        }

        ai::int32 index = params["index"].get<int>();
        std::string newName = params["name"].get<std::string>();

        AILayerHandle layer = nullptr;
        ASErr err = SuitePointers::AILayer()->GetNthLayer(index, &layer);
        if (err != kNoErr || !layer) {
            return {{"success", false},
                    {"error", "Layer not found at index"}};
        }

        ai::UnicodeString nameUni(newName);
        err = SuitePointers::AILayer()->SetLayerTitle(layer, nameUni);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "SetLayerTitle failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        return {{"success", true}};
    });
    return result.dump();
}
```

### Path Parameter Handler Example

For routes with `{id}` in the path, the extracted parameter is passed as a `const std::string&` argument. Typically you parse it to an integer handle ID:

```cpp
std::string HandleGetArtBounds(const std::string& id) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

    json result = MainThreadDispatch::Run([artId]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }

        AIRealRect bounds;
        ASErr err = SuitePointers::AIArt()->GetArtBounds(art, &bounds);
        if (err != kNoErr) {
            return {{"success", false},
                    {"error", "GetArtBounds failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        return {{"left", bounds.left},
                {"top", bounds.top},
                {"right", bounds.right},
                {"bottom", bounds.bottom}};
    });
    return result.dump();
}
```

### Including Your New File in CMake

If you added a new `.cpp` file (rather than appending to `NUXPHandlers.cpp`), make sure it is included in the build. Files in `plugin/src/endpoints/handwritten/` should already be picked up by the CMake glob in `plugin/CMakeLists.txt`. If not, add it explicitly:

```cmake
# In plugin/CMakeLists.txt, in the source file list:
src/endpoints/handwritten/ArtboardHandlers.cpp
```

---

## Step 4: Build and Test

Build the plugin:

```bash
cd plugin && cmake -B build && cmake --build build
```

If this is an incremental change (no new files), just:

```bash
cd plugin && cmake --build build
```

Restart Illustrator so it loads the updated `.aip` plugin, then test from the frontend:

```typescript
import { GetArtboardCount } from '@/sdk/generated/customRoutes'

const result = await GetArtboardCount()
console.log(result.count)  // e.g. 3
```

Or test directly with `curl`:

```bash
curl http://localhost:8080/api/artboard/count
# => {"count":3}
```

---

## Quick Reference: Full Route JSON Schema

```json
{
  "name": "RouteName",
  "description": "What this route does",
  "method": "GET | POST | PUT | DELETE | PATCH",
  "path": "/api/your/path/{optionalParam}",
  "pathParams": {
    "optionalParam": {
      "type": "string",
      "pattern": "[a-zA-Z0-9_.-]+",
      "description": "Parameter description"
    }
  },
  "request": {
    "fieldName": {
      "type": "string | number | boolean | number[] | string[] | object",
      "description": "Field description",
      "optional": true,
      "enum": ["allowed", "values"]
    }
  },
  "response": {
    "fieldName": {
      "type": "string | number | boolean | number[] | string[] | object",
      "description": "Field description"
    }
  },
  "rawBody": false
}
```

**Validation rules enforced by codegen:**

- Every `{param}` in `path` must have a matching key in `pathParams`
- `rawBody: true` and `request` cannot coexist on the same route
- No duplicate route names across the config
- No duplicate method + path combinations

---

## When NOT to Use Custom Routes

NUXP's auto-generated SDK wrappers already cover 442+ functions across all standard Illustrator suites. If you just need to call a single SDK function (get a property, set a value), check `shell/src/sdk/generated/` first -- it is probably already there.

Custom routes are the right choice when you need:

- **Multi-step operations** -- combining several SDK calls into one atomic request (e.g., `GetArtboardCount` uses `GetArtboardList` then `GetCount` then `ReleaseArtboardList`)
- **Complex data transformations** -- serializing SDK structs like `AIColor` (a tagged union with 7 variants) into clean JSON
- **Collection iteration** -- walking the art tree, iterating layers, gathering matching objects
- **Aggregated queries** -- returning document info that pulls from 3-4 different suites in one call
- **Special error handling** -- operations that need cleanup logic, retries, or fallback behavior
- **Custom business logic** -- anything that does not map 1:1 to a single SDK function

If your endpoint just wraps `SuitePointers::SomeSuite()->SomeFunction(arg)` and returns the result, the auto-generated wrappers are simpler and require zero hand-written code.
