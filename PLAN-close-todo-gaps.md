# Plan: Close NUXP TODO Gaps via Codegen Routes

## Context

The NUXP TODO.md lists 8 items across 3 priorities plus Tier 3 codegen work. After investigation, **5 of 8 items are already complete** and should be removed from the TODO. The remaining 3 items can all be solved using the existing CustomRouteGenerator pattern: define the API contract in `routes.json`, auto-generate declarations + registration + TypeScript client, hand-write only the business logic body.

### Already Done (remove from TODO.md)

| Item | Evidence |
|------|----------|
| Artboard Bounds Checking (P2) | `HandleCheckBounds()` in NUXPHandlers.cpp:594, route in routes.json |
| Document Query Endpoints (P2) | `HandleQueryTextFrames/QueryLayers/FindArtByName` all implemented |
| Path Area Calculation (P3) | `GeometryUtils::CalculatePathArea()` + `HandleCalculatePathArea` |
| Art Deselection (P3) | `HandleDeselectAll()` + `HandleSelectByHandles()` implemented |
| AIDictionaryRef/AIEntryRef Tier 3 | 37 dictionary + 24 entry functions already generating correctly |

### Still Needed (3 items)

1. **Document Info Endpoint** (P1) — aggregation endpoint missing
2. **View Control Endpoints** (P2) — no custom routes yet, suite wrapper is broken
3. **AIArtStyleHandle Tier 3** — ~8 BlendStyle focal functions blocked

---

## Approach: Everything Through routes.json

**Zero new hand-maintained API contracts.** Every HTTP endpoint gets defined in `routes.json`, which generates:
- C++ handler declarations (`CustomRouteHandlers.h`)
- Route registration (`CustomRouteRegistration.cpp`)
- TypeScript client (`customRoutes.ts`)

Hand-written code is limited to handler **bodies** in `NUXPHandlers.cpp` — pure business logic calling SuitePointers, no HTTP plumbing.

---

## Step 1: Update TODO.md

Remove the 5 completed items. Keep Document Info, View Control, and AIArtStyleHandle (deferred).

## Step 2: Add 8 New Routes to routes.json

**File:** `codegen/src/config/routes.json`

### Document Info (1 route)

```json
{
  "name": "GetDocumentInfo",
  "method": "GET",
  "path": "/api/doc/info",
  "description": "Aggregated document info: name (no extension), directory path, saved state, artboard dimensions in points and inches.",
  "response": {
    "name": { "type": "string", "description": "Document name without extension" },
    "path": { "type": "string", "description": "Directory path" },
    "fullPath": { "type": "string", "description": "Full file path including filename" },
    "saved": { "type": "boolean", "description": "Whether document has been saved" },
    "artboards": { "type": "object", "description": "Array of artboard info with bounds, widthPoints, heightPoints, widthInches, heightInches" }
  }
}
```

### View Control (5 routes)

```json
{
  "name": "GetViewZoom",
  "method": "GET",
  "path": "/api/view/zoom",
  "description": "Get current view zoom level as percentage.",
  "response": {
    "zoom": { "type": "number", "description": "Zoom percentage (100 = 100%)" }
  }
},
{
  "name": "SetViewZoom",
  "method": "POST",
  "path": "/api/view/zoom",
  "description": "Set view zoom level.",
  "request": {
    "zoom": { "type": "number", "description": "Zoom percentage (100 = 100%)" }
  },
  "response": {
    "success": { "type": "boolean" },
    "zoom": { "type": "number", "description": "Actual zoom after setting" }
  }
},
{
  "name": "SetViewCenter",
  "method": "POST",
  "path": "/api/view/center",
  "description": "Center the view on a point in artwork coordinates.",
  "request": {
    "x": { "type": "number", "description": "X coordinate in points" },
    "y": { "type": "number", "description": "Y coordinate in points" }
  },
  "response": { "success": { "type": "boolean" } }
},
{
  "name": "FitArtboardInView",
  "method": "POST",
  "path": "/api/view/fit-artboard",
  "description": "Zoom to fit the active artboard in the view.",
  "response": { "success": { "type": "boolean" } }
},
{
  "name": "FitSelectionInView",
  "method": "POST",
  "path": "/api/view/fit-selection",
  "description": "Zoom to fit the current selection in the view.",
  "response": { "success": { "type": "boolean" } }
}
```

### Missing Query Endpoints (2 routes)

```json
{
  "name": "QueryPathItems",
  "method": "GET",
  "path": "/api/query/path-items",
  "description": "Get all path items with bounds, fill, stroke info.",
  "response": {
    "items": { "type": "object", "description": "Array of path item info with handles" },
    "count": { "type": "number" }
  }
},
{
  "name": "CountItemsOnLayer",
  "method": "POST",
  "path": "/api/query/count",
  "description": "Count art items on a specific layer.",
  "request": {
    "layer": { "type": "string", "description": "Layer name" }
  },
  "response": {
    "count": { "type": "number" },
    "layer": { "type": "string" }
  }
}
```

## Step 3: Regenerate

```bash
cd codegen && npm run generate
# Copy to plugin + shell
cp output/cpp/*.cpp output/cpp/*.h output/cpp/*.hpp output/cpp/generated_sources.cmake plugin/src/endpoints/generated/
```

This produces the 8 new declarations in `CustomRouteHandlers.h`, 8 new registrations, and 8 new TypeScript functions — all type-safe, both sides matching.

## Step 4: Implement 8 Handler Bodies

**File:** `plugin/src/endpoints/NUXPHandlers.cpp`

All follow the same pattern: `MainThreadDispatch::Run([]() -> json { ... })`, call SuitePointers, return JSON.

| Handler | SDK Calls | Est. Lines |
|---------|-----------|------------|
| `HandleGetDocumentInfo` | AIDocumentSuite (filename, path, modified), AIArtboardSuite (bounds) | ~60 |
| `HandleGetViewZoom` | AIDocumentViewSuite::GetDocumentViewZoom | ~15 |
| `HandleSetViewZoom` | AIDocumentViewSuite::SetDocumentViewZoom | ~20 |
| `HandleSetViewCenter` | AIDocumentViewSuite::SetDocumentViewCenter | ~20 |
| `HandleFitArtboardInView` | AIArtboardSuite (bounds) + SetDocumentViewCenter + SetDocumentViewZoom | ~35 |
| `HandleFitSelectionInView` | AIMatchingArtSuite (selection bounds) + view calls | ~35 |
| `HandleQueryPathItems` | AIMatchingArtSuite + AIPathSuite + AIPathStyleSuite | ~50 |
| `HandleCountItemsOnLayer` | AILayerSuite + AIArtSuite tree walk | ~30 |

**Total hand-written: ~265 lines of business logic.** Zero HTTP plumbing — all generated.

## Step 5: Build and Verify

```bash
cd codegen && npm test                    # Codegen tests pass
cd codegen && npm run generate            # Regenerate
cd plugin && cmake --build build          # Plugin compiles+links
```

Verify TypeScript client has all 30 functions (22 existing + 8 new):
```bash
grep "export async function" shell/src/sdk/generated/customRoutes.ts | wc -l
```

## Step 6: Update TODO.md

Remove all completed items. Note AIArtStyleHandle as deferred (low value, ~8 functions, requires codegen enhancement).

---

## What Stays Hand-Written and Why

| Component | Why Not Codegen |
|-----------|----------------|
| Handler bodies (~265 lines) | Business logic — SDK call sequences, data aggregation, math. No code generator can produce this. |
| TextEndpoints.cpp (existing) | ATE forward-declared vtable — compiler-level workaround for header conflicts |
| XMPEndpoints.cpp (existing) | Optional SDK dependency with `#ifdef` guards |

Everything else — declarations, route registration, TypeScript client, request/response types — is generated from `routes.json`.

## What About AIArtStyleHandle (Tier 3)?

**Recommendation: Defer.** Only ~8 BlendStyle focal functions depend on it. The codegen change requires:
- Adding `artStyles` registry to HandleManager
- Extending TypeClassifier to recognize AIArtStyleHandle
- Most affected functions ALSO need AIDictionaryRef (which IS working)

The ROI is low. If needed later, it's a contained codegen enhancement, not an architectural change.
