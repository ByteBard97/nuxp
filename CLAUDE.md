# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## CRITICAL SAFETY RULES

**These rules are MANDATORY and override all other instructions:**

1. **NEVER COPY .aip FILES** to system locations (like Illustrator's Plug-ins folder) without explicit user permission. Always tell the user the output path and let them copy it themselves.

2. **NEVER USE `rm` OR `rm -rf` COMMANDS - EVER.** If you need to delete files, use `mv` to move them to `~/.Trash/` instead. This allows recovery.
   - Example: `mv /path/to/file ~/.Trash/` (NOT `rm /path/to/file`)

3. **NEVER WRITE ANONYMOUS/INLINE SCRIPTS.** Don't run multi-line bash, python, or node scripts inline. Instead:
   - Write the script to a file first (e.g., `scripts/my-script.sh`)
   - Then run the file: `bash scripts/my-script.sh`
   - This allows the user to grant permission once instead of repeatedly for each inline script

4. **ALWAYS ASK FIRST** before moving or deleting anything, showing the exact path.

## Project Overview

NUXP ("Not UXP") is a modern alternative to Adobe's deprecated CEP framework for Illustrator plugins. It combines a C++ plugin (HTTP/JSON server) with a Tauri desktop app (Vue 3 frontend), completely bypassing CEP/ExtendScript limitations.

**Architecture**: Frontend (Vue/TypeScript) ↔ HTTP/JSON ↔ C++ Plugin (.aip) ↔ Adobe Illustrator SDK

## Build Commands

### Plugin (C++)
```bash
cd plugin
cmake -B build
cmake --build build
cmake --install build  # installs to Illustrator Plug-ins folder
```

### Shell (Vue Frontend)
```bash
cd shell
npm install
npm run dev                    # dev server with hot reload
npm run build                  # production build
npm run type-check             # TypeScript checking
VITE_USE_MOCK=true npm run dev # dev without C++ plugin
```

### Code Generator
```bash
cd codegen
npm install
npm test                       # run all tests
npm test -- --testPathPattern="CppGenerator"  # run single test file
npm run generate               # generate wrappers
```

### Full Code Generation Pipeline
```bash
./scripts/generate.sh          # parse SDK → generate C++/TypeScript → copy to targets
```

CMake custom targets:
```bash
cmake --build build -t generate      # regenerate from SDK headers
cmake --build build -t regenerate    # regenerate + rebuild plugin
```

## Project Structure

```
nuxp/
├── plugin/           # C++ Illustrator plugin (.aip)
│   ├── src/          # Core: Plugin.cpp, HttpServer.cpp, HandleManager.cpp
│   ├── src/endpoints/           # Hand-written endpoint handlers
│   ├── src/endpoints/generated/ # Auto-generated (DO NOT EDIT)
│   ├── sdk/          # Adobe SDK headers (user-provided, gitignored)
│   └── lib/          # Third-party: httplib.h, nlohmann/json.hpp
├── shell/            # Vue 3 frontend
│   └── src/sdk/      # TypeScript bridge layer
├── codegen/          # SDK parser + code generators
│   ├── src/parser/   # Tree-sitter based SDK header parser
│   └── src/generator/ # CppGenerator.ts, TypeScriptGenerator.ts
└── scripts/          # Build automation (generate.sh)
```

## Key Architectural Patterns

### Handle Management (Cross-Thread Safety)
C++ handles (AIArtHandle, etc.) cannot be safely passed between threads. The `HandleRegistry<T>` template provides:
- Integer IDs for cross-thread handle passing
- Generation counters to detect stale handles after undo/document switch
- Thread-safe registration and lookup

```cpp
// Register on main thread
int32_t id = HandleManager::art.Register(artHandle);
// Send ID to frontend, later retrieve
AIArtHandle* handle = HandleManager::art.Get(id);
// Invalidate on document close/switch
HandleManager::art.BumpGeneration();
```

### Code Generation Flow
1. **Parse**: Tree-sitter walks SDK headers → `SuiteInfo` JSON
2. **Classify**: `TypeClassifier` marks params as Handle/String/Primitive/Struct
3. **Generate**: Templates produce C++ wrappers and TypeScript clients
4. **Dispatch**: `CentralDispatcher.h` routes `/api/{suite}/{method}` calls

Generated files go to:
- `plugin/src/endpoints/generated/` (C++ wrappers)
- `client/src/sdk/generated/` (TypeScript SDK)

### HTTP/JSON Bridge
- Plugin runs cpp-httplib server on localhost:8080
- Frontend calls `callCpp(suite, method, args)` from `@/sdk/bridge`
- Mock mode (`VITE_USE_MOCK=true`) routes to `mockBridge.ts` for development without plugin

### Main Thread Dispatch
All Adobe SDK calls must happen on Illustrator's main thread. `MainThreadDispatch` provides:
- Task queue for work from HTTP server thread
- `AITimerSuite` periodic callback drains queue on main thread

## Important Conventions

### Generated Code
Never manually edit files in:
- `plugin/src/endpoints/generated/`
- `shell/src/sdk/generated/`
- `client/src/sdk/generated/`

Regenerate using `./scripts/generate.sh`

### SDK Setup
The Adobe Illustrator SDK is NOT included. Use the setup script:
```bash
# Download SDK DMG from Adobe Developer Console
./scripts/setup-sdk.sh ~/Downloads/AI_2026_SDK_Mac.dmg
```
This extracts SDK headers, PICA/Sweet Pea headers, ATE headers, and creates `IllustratorSDK.h`.

### Type Mappings
See `codegen/src/config/type-map.json` for C++ → JSON type mappings:
- Handles: AIArtHandle → "art" registry
- Primitives: AIBoolean → bool, AIReal → double
- Strings: ai::UnicodeString, const char*

## Testing

```bash
# Codegen tests (Jest)
cd codegen && npm test

# Run specific test
npm test -- --testPathPattern="TypeScriptGenerator"

# Type-check shell
cd shell && npm run type-check
```

Test files are in `codegen/src/**/__tests__/**/*.test.ts`

## CI/CD Notes

The `ILLUSTRATOR_SDK_URL` secret must point to a hosted SDK zip for CI builds (Adobe login required to download SDK, so host privately for CI).
