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
The Adobe Text Engine (ATE) uses a fundamentally different architecture than standard AI suites:
- **Different calling convention**: ATE functions use plain C calls (no `ASAPI` marker),
  returning `ATEErr` instead of `ASErr`
- **Different lifecycle**: ATE refs use COM-style `AddRef()`/`Release()` reference counting,
  not the `HandleRegistry<T>` pattern used by AI handles
- **Different header structure**: ATE suites live in `sdk/ate/ATESuites.h` (6,000+ lines),
  a single file with ~40 suite structs — completely different from the one-suite-per-header
  pattern the codegen parser expects

The workaround: manually mirror the vtable layout as local structs and acquire suites
at runtime via `SPBasicSuite::AcquireSuite()` using string names. This avoids including
the ATE headers entirely.

### XMPEndpoints.cpp
Depends on the optional XMP Toolkit SDK (`-DXMP_SDK_PATH=...`). Wrapped in `#ifdef NUXP_HAS_XMP`
guards. The codegen has no concept of optional dependencies.

## Adding a new endpoint

1. Add the route definition to `codegen/src/config/routes.json`
2. Run `./scripts/generate.sh`
3. Add your handler body to `handwritten/NUXPHandlers.cpp` (or a new file in `handwritten/`)
4. Rebuild: `cd plugin && cmake -B build && cmake --build build`

Never edit files in `generated/` — they will be overwritten on the next codegen run.
