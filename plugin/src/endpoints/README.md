# Endpoints Directory Structure

This directory has a strict boundary between auto-generated and hand-written code.

```
endpoints/
├── generated/      ← AUTO-GENERATED — do not edit
│   ├── CentralDispatcher.h         — routes SDK suite calls by name
│   ├── CustomRouteHandlers.h       — declares all handler function signatures
│   ├── CustomRouteRegistration.cpp — wires HTTP routes to handler functions
│   ├── FloraAI*Wrapper.cpp/h       — SDK suite wrappers (from header parsing)
│   └── .generated                  — marker file
├── handwritten/    ← MANUAL — business logic lives here
│   ├── NUXPHandlers.cpp            — handler bodies for custom routes
│   ├── TextEndpoints.cpp/hpp       — ATE text frame operations
│   ├── XMPEndpoints.cpp/hpp        — XMP metadata (optional SDK)
│   └── DemoEndpoints.cpp/hpp       — example/demo endpoints
└── README.md       ← you are here
```

## How the boundary works

1. **Define your API** in `codegen/src/config/routes.json` (name, method, path, request/response schema)
2. **Run codegen** (`./scripts/generate.sh`) — this produces the files in `generated/`
3. **Write only the handler body** in `handwritten/NUXPHandlers.cpp`

The generated code declares the function signature and wires the HTTP route.
Your hand-written code implements the business logic (SDK calls, data aggregation, math).

## Why some code must be hand-written

### NUXPHandlers.cpp
Standard SDK handler bodies. The codegen generates the *declarations* and *route registration*,
but the actual SDK call sequences are business logic that can't be auto-generated.

### TextEndpoints.cpp
The Adobe Text Engine (ATE) uses a fundamentally different architecture than standard AI suites
(different calling convention, COM-style ref counting, ~6,000-line monolithic header). The ATE
headers (`ATESuites.h`, `AIFont.h`) conflict with `AITypes.h` when included alongside
`IllustratorSDK.h` in the same translation unit.

**Solution: ATEBridge** (`src/bridges/ATEBridge.cpp`) is compiled as an isolated translation
unit that includes the real ATE headers directly. It exposes a clean API through `ATEBridge.h`
using only `AITypes.h` types — no ATE types leak out. TextEndpoints.cpp simply calls
`ATEBridge::NewPointText()`, `ATEBridge::GetTextContent()`, and `ATEBridge::SetTextContent()`.

This uses the real SDK struct types and version constants (`kAITextFrameSuiteVersion`,
`kTextRangeSuiteVersion`), eliminating the fragile hand-rolled vtable structs that previously
lived here with hardcoded version numbers.

### XMPEndpoints.cpp
Depends on the optional XMP Toolkit SDK (`-DXMP_SDK_PATH=...`). Wrapped in `#ifdef NUXP_HAS_XMP`
guards. The codegen has no concept of optional dependencies.

## Adding a new endpoint

1. Add the route definition to `codegen/src/config/routes.json`
2. Run `./scripts/generate.sh`
3. Add your handler body to `handwritten/NUXPHandlers.cpp` (or a new file in `handwritten/`)
4. Rebuild: `cd plugin && cmake -B build && cmake --build build`

Never edit files in `generated/` — they will be overwritten on the next codegen run.
