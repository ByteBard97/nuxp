# NUXP: Repository Structure

**Status**: Defined
**Context**: Monorepo containing Tauri shell, C++ Illustrator plugin, SDK code generator, and Vue example app

---

## Overview

```
nuxp/
├── README.md
├── LICENSE
├── .gitignore
│
├── plugin/                     # C++ Illustrator plugin (.aip)
│   ├── CMakeLists.txt
│   ├── sdk/                    # Adobe Illustrator SDK headers (gitignored, user provides)
│   │   └── .gitkeep
│   ├── lib/                    # Third-party C++ deps
│   │   ├── httplib.h           # cpp-httplib (header-only)
│   │   └── json.hpp            # nlohmann/json (header-only)
│   ├── src/
│   │   ├── Plugin.cpp          # Entry point: StartupPlugin, ShutdownPlugin, main notifier dispatch
│   │   ├── Plugin.hpp
│   │   ├── HttpServer.cpp      # cpp-httplib server lifecycle (start/stop on background thread)
│   │   ├── HttpServer.hpp
│   │   ├── MainThreadDispatch.cpp   # Timer-queue: packaged_task queue + AITimerSuite callback
│   │   ├── MainThreadDispatch.hpp
│   │   ├── HandleManager.cpp   # Static registry instances
│   │   ├── HandleManager.hpp   # HandleRegistry<T> template + HandleManager wrapper
│   │   ├── Errors.cpp          # AIErr → string mapping
│   │   ├── Errors.hpp
│   │   └── endpoints/          # HTTP endpoint handlers, organized by suite
│   │       ├── RegisterAll.cpp # Calls all Register*Endpoints(server) functions
│   │       ├── RegisterAll.hpp
│   │       ├── DocumentEndpoints.cpp
│   │       ├── LayerEndpoints.cpp
│   │       ├── ArtEndpoints.cpp
│   │       ├── ColorEndpoints.cpp
│   │       ├── ToolEndpoints.cpp
│   │       ├── ApplicationEndpoints.cpp
│   │       └── generated/      # Auto-generated endpoints from codegen (don't hand-edit)
│   │           ├── .generated  # Marker file so codegen knows this is its output dir
│   │           ├── GenDocumentEndpoints.cpp
│   │           ├── GenLayerEndpoints.cpp
│   │           ├── GenArtEndpoints.cpp
│   │           └── ...
│   └── build/                  # Build output (gitignored)
│
├── codegen/                    # Node.js SDK header parser + code generator
│   ├── package.json
│   ├── tsconfig.json           # TypeScript for the generator itself
│   ├── src/
│   │   ├── index.ts            # CLI entry: parse headers → generate endpoints
│   │   ├── parser/
│   │   │   ├── SuiteParser.ts      # tree-sitter: extract suite structs from headers
│   │   │   ├── FunctionParser.ts   # Parse AIAPI function pointer signatures
│   │   │   ├── TypeClassifier.ts   # Classify params as serializable / handle / skip
│   │   │   └── types.ts            # AST types: SuiteInfo, FunctionInfo, ParamInfo
│   │   ├── generator/
│   │   │   ├── CppGenerator.ts     # Emit C++ endpoint handlers
│   │   │   ├── TypeScriptGenerator.ts  # Emit TS client functions
│   │   │   ├── MarkdownGenerator.ts    # Emit API docs
│   │   │   └── templates/          # Mustache/string templates for codegen output
│   │   │       ├── endpoint.cpp.tmpl
│   │   │       ├── client.ts.tmpl
│   │   │       └── docs.md.tmpl
│   │   └── config/
│   │       ├── type-map.json       # SDK type → JSON type mapping (AIReal→number, etc.)
│   │       └── skip-list.json      # Functions to explicitly skip (dangerous, deprecated)
│   ├── test/
│   │   ├── fixtures/               # Sample SDK headers for testing parser
│   │   │   ├── AILayer.h
│   │   │   └── AIDocument.h
│   │   ├── parser.test.ts
│   │   └── generator.test.ts
│   └── output/                 # Generated files land here, then get copied to targets
│       ├── cpp/                # → copied to plugin/src/endpoints/generated/
│       ├── typescript/         # → copied to client/src/sdk/generated/
│       └── docs/               # → copied to docs/api/
│
├── shell/                      # Tauri app (Rust + webview)
│   ├── Cargo.toml
│   ├── tauri.conf.json
│   ├── src/
│   │   └── main.rs             # Tauri entry point (minimal — most logic is in the webview)
│   ├── icons/
│   └── build/                  # Build output (gitignored)
│
├── client/                     # Vue 3 frontend (loads in Tauri webview)
│   ├── package.json
│   ├── vite.config.ts
│   ├── tsconfig.json
│   ├── index.html
│   ├── src/
│   │   ├── main.ts
│   │   ├── App.vue
│   │   ├── sdk/                # TypeScript SDK client (talks to C++ plugin over HTTP)
│   │   │   ├── client.ts       # Base fetch wrapper, error handling, StaleHandleError
│   │   │   ├── layers.ts       # Hand-written convenience wrappers
│   │   │   ├── documents.ts
│   │   │   ├── art.ts
│   │   │   └── generated/      # Auto-generated from codegen (don't hand-edit)
│   │   │       ├── .generated
│   │   │       ├── genLayers.ts
│   │   │       ├── genDocuments.ts
│   │   │       ├── genArt.ts
│   │   │       └── ...
│   │   ├── stores/             # Pinia stores
│   │   │   ├── document.ts
│   │   │   └── layers.ts
│   │   ├── components/         # Example Vue components
│   │   │   ├── LayerPanel.vue
│   │   │   ├── ArtInfo.vue
│   │   │   └── DocumentStatus.vue
│   │   └── composables/        # Vue composables for SDK interaction patterns
│   │       └── useStaleRecovery.ts   # Auto-refetch on StaleHandleError
│   └── public/
│
├── docs/
│   ├── getting-started.md      # Clone → build → run tutorial
│   ├── architecture.md         # How the pieces fit together (diagram)
│   ├── adding-endpoints.md     # How to hand-write a custom endpoint
│   ├── codegen.md              # How to run the code generator
│   ├── handle-management.md    # The handle registry design (from our proposals)
│   └── api/                    # Auto-generated API reference
│       └── generated/          # From codegen MarkdownGenerator
│
└── scripts/
    ├── setup.sh                # Install deps for all packages (npm install, cargo check)
    ├── build-plugin.sh         # CMake build for the .aip
    ├── build-app.sh            # Tauri build (compiles Rust + bundles Vue)
    ├── generate.sh             # Run codegen: parse SDK → generate → copy to targets
    ├── dev.sh                  # Start Tauri dev server with hot reload
    └── install-plugin.sh       # Copy built .aip to Illustrator's Plug-ins folder
```

---

## Key Design Decisions

### 1. Naming Convention
*   **`plugin/`**: Clear purpose (Illustrator plugin).
*   **`shell/`**: The host environment (Tauri/Rust). Implementation detail.
*   **`client/`**: The HTTP client / UI logic (Vue).

### 2. Generated Code Strategy
*   **Separation**: Hand-written endpoints (`src/endpoints/`) vs Generated (`src/endpoints/generated/`). Allows custom workflows alongside 1:1 mappings.
*   **Decoupled Codegen**: Generator writes to `codegen/output/`, shell script copies to targets.
*   **Version Control**: **DO NOT Check In Generated Code**.
    *   *Reason*: `client` and `codegen` both require `npm`. Users must have `npm` to build the full app anyway. Checking in generated code risks merge conflicts and drift.
    *   *Mechanism*: `scripts/generate.sh` is the source of truth.

### 3. Build Dependencies
*   **npm/Node**: Required for `codegen` and `client`.
*   **Cargo/Rust**: Required for `shell`.
*   **CMake**: Required for `plugin`.

---

## .gitignore Rules

```gitignore
# Build outputs
plugin/build/
shell/build/
shell/target/
client/dist/

# SDK headers (checked in for reproducibility)
# plugin/sdk/VERSION contains the SDK version (e.g. "Adobe Illustrator 2026 SDK")

# Generated code
plugin/src/endpoints/generated/*
plugin/src/endpoints/generated/*
!plugin/src/endpoints/generated/.generated
client/src/sdk/generated/*
!client/src/sdk/generated/.generated
docs/api/generated/

# Codegen intermediate output
codegen/output/

# Dependencies
node_modules/
```
