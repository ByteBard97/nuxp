# NUXP — Remaining Work

Consolidated from 5 previous planning docs (Feb 2026). Completed items have been removed.

---

## SDK Gaps

These are generic Illustrator SDK capabilities that NUXP should expose as reusable infrastructure.

### P1 — HIGH

#### Document Info Endpoint Completeness

**Files:** `plugin/src/endpoints/DocumentEndpoints.cpp`

The document info response is missing fields that plugins commonly need:

- Strip file extension from document name (return `"MyDoc"` not `"MyDoc.ai"`)
- Add `saved` boolean field (has the document been saved at least once?)
- Add `widthInches` / `heightInches` to artboard info (in addition to points)
- Add `widthPoints` / `heightPoints` to document bounds
- Ensure `path` returns the directory path, not the full file path
- Target response shape:
  ```json
  {
    "name": "MyDoc",
    "path": "/Users/me/Documents/",
    "fullPath": "/Users/me/Documents/MyDoc.ai",
    "saved": true,
    "artboards": [{
      "index": 0,
      "name": "Artboard 1",
      "bounds": { "left": 0, "top": 792, "right": 612, "bottom": 0 },
      "widthPoints": 612,
      "heightPoints": 792,
      "widthInches": 8.5,
      "heightInches": 11
    }]
  }
  ```

### P2 — MEDIUM

#### Artboard Bounds Checking Utility

**Files:** `plugin/src/utils/GeometryUtils.cpp`

No utility to check if a rect/point fits within the active artboard. Plugins that place art need to validate positions before placement.

- `GeometryUtils::FitsInArtboard(AIRealRect itemBounds, AIRealRect artboardBounds) -> bool`
- `GeometryUtils::ClampToArtboard(AIRealRect itemBounds, AIRealRect artboardBounds) -> AIRealRect`
- Optional HTTP endpoint: `POST /api/artboard/check-bounds`

#### Document Query Endpoints

**Files:** `plugin/src/endpoints/QueryEndpoints.cpp` (new)

Plugins need to introspect document contents. NUXP wraps low-level art/layer suites but doesn't provide high-level query endpoints.

- `GET /api/query/text-frames` — all text frames with content, position, font info
- `GET /api/query/path-items` — all path items with bounds, fill, stroke
- `GET /api/query/find?name=PATTERN` — find art items by name (wildcards or regex)
- `GET /api/query/count?layer=LAYERNAME` — count items on a specific layer
- `GET /api/query/layers` — full layer hierarchy with visibility, lock state, item counts
- All responses should include `handle` IDs for further manipulation

#### View Control Endpoints

**Files:** `plugin/src/endpoints/ViewEndpoints.cpp` (new)

No way to control the Illustrator viewport programmatically.

- Wrap `AIDocumentViewSuite`:
  - `POST /api/view/fit-artboard` — zoom to fit active artboard
  - `POST /api/view/fit-selection` — zoom to fit current selection
  - `POST /api/view/zoom` — `{level: number}` set zoom percentage
  - `GET /api/view/zoom` — get current zoom level
  - `POST /api/view/center` — `{x, y}` center view on a point

### P3 — LOW

#### Path Area Calculation

**Files:** `plugin/src/utils/GeometryUtils.cpp`

Calculating the actual area of a path (not bounding box area) requires walking path segments.

- `GeometryUtils::CalculatePathArea(AIArtHandle path) -> AIReal`
- Use `AIPathSuite` to walk segments, compute via shoelace formula
- Handle both straight segments and bezier curves (linearize for approximation)
- Optional endpoint: `GET /api/path/:handle/area`

#### Art Deselection Utility

**Files:** `plugin/src/utils/SelectionUtils.cpp`

After programmatic art creation, items may remain selected.

- Ensure `SelectionUtils::DeselectAll()` exists and works reliably
- Expose as: `POST /api/selection/deselect-all`
- Also useful: `POST /api/selection/select` — `{handles: [id1, id2, ...]}`

---

## Codegen Type Expansion (Tier 3 Remaining)

Tiers 1-2 are complete (UnicodeString, FilePath, void params, bool mapping, non-standard returns, managed handles). Remaining Tier 3 items:

#### AIDictionaryRef + AIEntryRef as Handles (~18 functions)

Dictionary and entry types have AddRef/Release lifecycle semantics. Can be treated as handles, but the code generator would also need to handle `ConstAIDictionaryRef` (const variant). Registries exist in HandleManager but not all functions are unblocked.

- ~10 functions in AIBlendStyleSuite (Get/SetBlendStyleAttrs family)
- ~8 functions in AIDictionarySuite (CreateDictionary, Clone, etc.)

#### AIArtStyleHandle as Handle (~8 functions)

Art style handles appear in BlendStyleSuite focal functions. Straightforward handle addition but low independent value — most functions also need AIDictionaryRef.

- GetFocalFillBlendStyleAttrs, SetFocalFillBlendStyleAttrs, and 6 similar focal functions

---

## Completed (for reference)

These items from the original plans are done and have been removed:

- Text Frame Creation (AITextFrameSuite wrapper, `POST /api/text/create`)
- XMP Metadata Read/Write (AIXMPSuite wrapper, GET/POST/dump endpoints)
- documentNew SSE Event (kAIDocumentNewNotifier registered in EventMapper)
- Route Pattern Flexibility (CustomRouteGenerator supports per-route regex via `pathParams.pattern`)
- All codegen type fixes (handle types, primitives, blocked functions, dead code removal)
- Codegen Tier 1: UnicodeString unblock (~40 functions)
- Codegen Tier 2: FilePath support, non-standard return types
- Codegen Tier 3A: Managed handles for ArtboardProperties/ArtboardList
- CustomRouteGenerator (fully implemented and in use by Flora)
