# NUXP Framework Extraction from Flora-UXP

## Context

Flora-UXP (`/Users/ceres/Desktop/flora/flora-uxp/`) was built as a test fixture to prove the NUXP framework works end-to-end. However, during development, flora-uxp built its own parallel implementations of infrastructure that should have lived in NUXP. This document identifies exactly which files contain reusable framework code that should be extracted back into NUXP.

**Goal:** Make NUXP a self-contained, reusable framework that any Illustrator plugin app (not just Flora) can depend on. Flora-uxp should then import NUXP as a git submodule dependency.

**NUXP repo:** `/Users/ceres/Desktop/flora/nuxp/`
**Flora-uxp repo:** `/Users/ceres/Desktop/flora/flora-uxp/`

---

## Decision Heuristic: NUXP (Open Source) vs Flora (Closed)

Use this rubric to decide where any file belongs:

### NUXP (open source) if ALL of these are true:

1. **No domain knowledge** — The code knows nothing about plants, species, beds, labels, BOM, costs, or any Flora business concept
2. **Any Illustrator plugin could use it** — A hypothetical "fonts plugin" or "color palette plugin" would need the same code
3. **Talks to infrastructure, not business** — It communicates with Illustrator SDK, Tauri APIs, HTTP/SSE transport, or provides generic UI components
4. **Replaceable inputs** — If it takes configuration (routes, settings keys, event names), the configuration is injected, not hardcoded

### Flora (closed) if ANY of these are true:

1. **Mentions plants, species, beds, labels, symbols, BOM, costs, nurseries, or native plants** — Domain-specific by definition
2. **Imports from Flora domain stores or services** — It's coupled to Flora's data model
3. **Contains Flora-specific business rules** — Drawing scale calculations, ref designator sequencing, circle packing for plant placement, etc.
4. **Only Flora would ever need it** — e.g., SVG sprite generation for plant symbols, cost breakdown export

### Gray zone (needs splitting):

- **Generic pattern + Flora config values** — Extract the pattern to NUXP, keep the config in Flora. Example: `SettingsService.ts` has a generic localStorage get/set pattern (NUXP) but stores Flora-specific keys like `drawingScale` and `labelFont` (Flora).
- **Geometry utilities with D3 coupling** — `CoordinateSystemManager` is generic coordinate math (NUXP) but some methods assume D3's Y-down coordinate system (Flora-specific usage).

### Quick test:

> "If I deleted every plant-related line from this file, would it still be useful to someone building a completely different Illustrator plugin?"
>
> **Yes** → NUXP. **No** → Flora. **Partially** → Split it.

---

## Layer 1: TypeScript Client SDK

These files in flora-uxp are **generic Illustrator plugin infrastructure** with zero Flora-specific logic. They should be moved into NUXP's `client/src/sdk/` or similar.

### Communication Adapters (highest priority)

| Flora-uxp File | Purpose | NUXP Target |
|---|---|---|
| `client/adapters/PluginAdapter.ts` | HTTP/SSE bridge abstraction — detects environment (Tauri vs CEP vs browser), creates csInterface singleton | `client/src/sdk/adapters/PluginAdapter.ts` |
| `client/adapters/HttpApiAdapter.ts` | Maps function names to HTTP endpoints, handles request/response serialization | `client/src/sdk/adapters/HttpApiAdapter.ts` |
| `client/adapters/SSEEventAdapter.ts` | Server-sent events handling for real-time Illustrator events (selection change, document switch, etc.) | `client/src/sdk/adapters/SSEEventAdapter.ts` |
| `client/services/HostBridge.ts` | Queued communication with the C++ plugin — serializes concurrent requests, manages timeouts | `client/src/sdk/services/HostBridge.ts` |
| `client/services/AutoQueue.ts` | Generic request serialization queue (used by HostBridge) | `client/src/sdk/services/AutoQueue.ts` |

**Key detail:** `HttpApiAdapter.ts` contains an `ENDPOINT_MAP` that maps function names to HTTP routes. In flora-uxp this map is generated from `codegen/routes.json`. NUXP should provide the map generation infrastructure, with apps providing their own route definitions.

### Orchestration Primitives

| Flora-uxp File | Purpose | NUXP Target |
|---|---|---|
| `client/services/orchestration/primitives.ts` | Thin TypeScript wrappers around C++ SDK endpoints (scanPlants, getLabel, setTextContent, findArtByName, getGroupChildByType, etc.) | `client/src/sdk/primitives.ts` |

**Note:** Some primitives in this file are Flora-specific (e.g., `scanPlants`). The generic ones (art manipulation, text operations, layer operations) should move to NUXP. Flora-specific ones stay.

### Tauri Wrappers

| Flora-uxp File | Purpose | NUXP Target |
|---|---|---|
| `client/services/TauriFsWrapper.ts` | File system operations via Tauri APIs (read/write/exists) | `client/src/sdk/tauri/TauriFsWrapper.ts` |
| `client/services/TauriDialogService.ts` | Native file picker dialogs via Tauri | `client/src/sdk/tauri/TauriDialogService.ts` |

### Logging

| Flora-uxp File | Purpose | NUXP Target |
|---|---|---|
| `client/services/LoggerService.ts` | Logging abstraction with categories (build, api, error, etc.) | `client/src/sdk/services/LoggerService.ts` |

### Generic UI Components

Flora-uxp uses shadcn-vue components that are completely generic:

| Flora-uxp Directory | Purpose | NUXP Target |
|---|---|---|
| `client/components/ui/*` | shadcn/radix Vue components (Button, Slider, Accordion, Dialog, Tabs, etc.) | `client/src/components/ui/*` |

These are standard shadcn-vue components. NUXP could either bundle them or document them as a peer dependency.

### Generic Composables & Stores

| Flora-uxp File | Purpose | NUXP Target |
|---|---|---|
| `client/stores/useThemeStore.ts` | Generic UI theme state management | `client/src/sdk/stores/useThemeStore.ts` |
| `client/composables/useTabManagement.ts` | Tab switching/routing logic (if exists) | `client/src/sdk/composables/useTabManagement.ts` |

### Geometry Utilities

| Flora-uxp File | Purpose | NUXP Target |
|---|---|---|
| `shared/geometry/CoordinateSystemManager.ts` | Coordinate transformations between D3/screen/Illustrator spaces | `client/src/sdk/geometry/CoordinateSystemManager.ts` |
| `shared/geometry/ArtboardBoundsManager.ts` | Artboard bounds calculation and caching | `client/src/sdk/geometry/ArtboardBoundsManager.ts` |

---

## Layer 2: Rust/Tauri Shell

Both projects have minimal Tauri shells. Flora-uxp's is slightly richer.

| Flora-uxp File | What's Generic | What's Flora-Specific |
|---|---|---|
| `src-tauri/src/lib.rs` | Tauri plugin setup (fs, shell, opener, dialog, window-state), CLI bridge startup | `get_asset_path` and `list_assets` commands are Flora-specific asset resolution |
| `src-tauri/Cargo.toml` | Tauri plugin dependencies | Flora-specific metadata (app name, identifier) |
| `src-tauri/tauri.conf.json` | Window config, CSP policy pattern | App-specific values (title, identifier, icon, resources) |

**Recommendation:** NUXP's `demo/` should provide a base Tauri setup that apps extend. The pattern from flora-uxp's `lib.rs` (plugin registration, CLI bridge, window setup) should become NUXP's default shell template.

---

## Layer 3: C++ Plugin

The C++ architectures differ fundamentally:

- **NUXP** (`nuxp/plugin/`): Uses codegen to auto-generate C++ endpoint wrappers from Adobe SDK suite definitions (AIArtSuite, AIDocumentSuite, etc.)
- **Flora-uxp** (`flora-uxp/cpp-plugin/`): Hand-built 27+ handler files under `Handlers/` directory

**What flora-uxp proved works:**
- HTTP server architecture (request routing, JSON serialization, SSE event streaming)
- Handler pattern (one file per domain, registered in a route table)
- The specific thin primitives that work well: `getArtBounds`, `setTextContent`, `findArtByName`, `getGroupChildByType`, `createLabel`, `deleteLabel`, `scanPlants`

**What NUXP should absorb:**
- The HTTP server + SSE infrastructure (if not already present)
- The handler registration pattern
- Generic art manipulation primitives (get/set bounds, text operations, layer operations, selection operations)

**What stays Flora-specific:**
- All 27 handler files in `Handlers/` (FloraPlacementHandlers, FloraSymbolHandlers, FloraLabelHandlers, etc.)
- The 200+ routes in `codegen/routes.json`

---

## Layer 4: Codegen Infrastructure

| Flora-uxp File | Purpose | NUXP Action |
|---|---|---|
| `codegen/routes.json` | Route definitions (200+ Flora routes) | **Pattern is reusable**, actual routes are Flora-specific |
| `codegen/generate.cjs` | Generates C++ route registration from routes.json | Generalize for NUXP — apps provide their routes.json, codegen produces registration code |
| `codegen/generate-endpoint-map.cjs` | Generates TypeScript endpoint map from routes.json | Same — generalize the script, keep it in NUXP |

---

## Files That MUST Stay in Flora-UXP

These are 100% Flora business logic:

### Services
- `client/services/PlantManager.ts` — Plant CRUD
- `client/services/PlantReportService.ts` — Reporting
- `client/services/SymbolManagementService.ts` — Illustrator symbol management for plant symbols
- `client/services/SvgPlacementService.ts` — SVG placement
- `client/services/BOMExportService.ts` — Bill of Materials export
- `client/services/CostBreakdownService.ts` — Cost calculations
- `client/services/ReAnnotationService.ts` — Plant re-annotation
- `client/services/SitePlanImportService.ts` — Site plan importing
- `client/services/SpriteGeneratorService.ts` — Sprite generation
- `client/services/AppearanceConfigService.ts` — Flora theme/appearance config
- `client/services/SettingsService.ts` — Flora settings (drawingScale, labelFont, etc.)

### Orchestrators
- `client/services/orchestration/PlantAppearanceOrchestrator.ts`
- `client/services/orchestration/BatchPlacementOrchestrator.ts`
- `client/services/orchestration/LabelOrchestrator.ts`

### Stores
- `client/stores/useDocumentIndexStore.ts`
- `client/stores/usePlacementStore.ts`
- `client/stores/usePlantLabelsStore.ts`
- `client/stores/useBedStore.ts`
- `client/stores/usePaletteStore.ts`

### Composables
- `client/composables/usePlantsTab.ts`
- `client/composables/useInspectTab.ts`
- `client/composables/usePlantLabelActions.ts`
- `client/composables/useReportsData.ts`
- `client/composables/useSiteImport.ts`

### All Tab/Page Components
- `client/tabs/*` — Every tab is Flora-specific

### Domain Logic
- `client/domain/services/*` — Plant geometry, grid fill, curve flattening
- `client/types/core.d.ts` — Flora domain types
- `shared/schemas/PlantSchemas.ts`, `DocumentSchemas.ts` — Flora Zod schemas

---

## Needs Splitting (Generic Pattern, Flora Values)

| File | Generic Part | Flora Part |
|---|---|---|
| `client/services/SettingsService.ts` | localStorage persistence pattern, get/set/subscribe API | Setting keys and defaults (drawingScale, labelFont, etc.) |
| `client/config/fonts.json` | Font loading infrastructure | Specific font selections |
| `client/config/backgroundImages.ts` | Image asset management pattern | Specific botanical images |

---

## Recommended Extraction Order

1. **Communication layer first** (PluginAdapter, HttpApiAdapter, SSEEventAdapter, HostBridge, AutoQueue) — this is the core SDK
2. **Tauri wrappers** (TauriFsWrapper, TauriDialogService) — needed for any Tauri-based plugin
3. **LoggerService** — used everywhere
4. **Generic primitives** from `primitives.ts` — art manipulation, text, layers
5. **Geometry utilities** (CoordinateSystemManager, ArtboardBoundsManager)
6. **Codegen scripts** — generalize route generation
7. **Rust/Tauri shell template** — base setup from flora-uxp's lib.rs
8. **UI components** — shadcn-vue components (lowest priority, easy to add later)

---

## Integration Architecture

Once extraction is complete, flora-uxp should consume NUXP as a git submodule:

```
flora-uxp/
  nuxp/                    # git submodule → github.com/user/nuxp
    client/src/sdk/        # TypeScript SDK
    plugin/                # C++ plugin framework
    demo/                 # Rust/Tauri shell template
    codegen/               # Route generation scripts
  client/
    adapters/              # Thin re-exports or extensions of nuxp/client/src/sdk/adapters/
    services/              # Flora-specific services (import generic ones from nuxp)
  cpp-plugin/
    FloraBridge/           # Flora handlers, links against nuxp-core
  src-tauri/               # Extends nuxp shell template
```

**TypeScript:** npm workspace or path aliases in `tsconfig.json` to resolve `@nuxp/*` imports
**Rust:** `Cargo.toml` path dependency to `nuxp/demo/src-tauri`
**C++:** Already works via CMakeLists.txt `add_subdirectory(${NUXP_PLUGIN_DIR})`
