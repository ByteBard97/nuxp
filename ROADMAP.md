# NUXP Roadmap

This document outlines what's complete, what's in progress, and what's planned for NUXP.

## Current Status: Alpha

NUXP's core architecture, code generation pipeline, and SDK coverage are solid. The auto-generated bindings cover 442 functions across 19 suites. However, three critical hand-written wrappers (selection access, fill/stroke colors, path geometry) are still needed before NUXP is usable for real-world Illustrator automation.

## What's Complete

### Core Infrastructure
- [x] C++ plugin framework with HTTP/JSON server (cpp-httplib)
- [x] Vue 3 frontend with TypeScript SDK
- [x] Handle management for safe cross-thread operations (18 handle types + 2 managed)
- [x] ManagedHandleRegistry for RAII objects (ai::ArtboardProperties, ai::ArtboardList)
- [x] Server-Sent Events for real-time Illustrator notifications
- [x] Main thread dispatch (work queue + AITimer integration)
- [x] Mock mode for development without Illustrator
- [x] Path parameter routing with regex support

### Code Generation (4 generators, 346 tests)
- [x] **CppGenerator** - SDK header -> C++ HTTP/JSON wrappers
- [x] **TypeScriptGenerator** - SDK header -> typed async TS client functions
- [x] **SSEGenerator** - Event definitions -> C++ emitters + TS event bus
- [x] **CustomRouteGenerator** - Route JSON -> C++ handlers + TS clients (with config inheritance)
- [x] Tree-sitter based SDK header parser
- [x] TypeClassifier with Handle, ManagedHandle, Primitive, String, Struct, Enum categories

### Type System Coverage
- [x] Handles: AIArtHandle, AILayerHandle, AIDocumentHandle, AIMaskRef, AIToolHandle, AITimerHandle, AINotifierHandle, AIDictionaryRef, ConstAIDictionaryRef, AIEntryRef, AIDictKey, AIDictionaryIterator, AIArrayRef, AIArtStyleHandle, AIPatternHandle, AIGradientHandle, AICustomColorHandle, AIMenuItemHandle
- [x] Managed handles: ai::ArtboardProperties, ai::ArtboardList
- [x] Strings: ai::UnicodeString, const char*, ai::FilePath
- [x] Structs: AIRealRect, AIRealPoint, AIRealMatrix
- [x] Primitives: AIBoolean, AIReal, ai::int32, ai::int16, ai::uint32, size_t, short, long, ai::ArtboardID, AIEntryType
- [x] Non-standard return types: AIReal, AIArtHandle, enum, ai::UnicodeString, ai::FilePath, const char*

### SDK Suite Coverage (442 functions, 19 suites)
- [x] AIArtSuite (72) - Core art object manipulation
- [x] AIDocumentSuite (68) - Document management
- [x] AIArtboardSuite (40) - Artboard properties and layout
- [x] AILayerSuite (39) - Layer management
- [x] AIDictionarySuite (36) - Dictionary/metadata access
- [x] AIToolSuite (32) - Tool management
- [x] AIEntrySuite (23) - Dictionary entry read/write
- [x] AIBlendStyleSuite (23) - Opacity, blending modes
- [x] AIUserSuite (20) - User interaction, dialogs
- [x] AIArtSetSuite (16) - Art set operations
- [x] AIUndoSuite (16) - Undo/redo transactions
- [x] AIMaskSuite (15) - Clipping mask operations
- [x] AILayerListSuite (12) - Layer list traversal
- [x] AITimerSuite (8) - Timer callbacks
- [x] AIAppContextSuite (7) - Application context
- [x] AINotifierSuite (5) - Event notifications
- [x] AIMdMemorySuite (5) - Memory management
- [x] AIGroupSuite (4) - Group operations
- [x] AITransformArtSuite (1) - Transform operations

### Frontend
- [x] Debug Panel with event logging
- [x] Test Runner framework (4 test suites)
- [x] Design System showcase
- [x] Connection status management
- [x] Pinia stores for document/connection state

### Build & CI
- [x] CMake build system with customization options
- [x] Xcode generator support
- [x] GitHub Actions for automated builds
- [x] SDK setup script (macOS DMG extraction)

### Utility Modules
- [x] SelectionUtils - Get/set/clear selection
- [x] GeometryUtils - Create shapes, get bounds, move/scale/rotate
- [x] LayerUtils - Layer manipulation
- [x] DocumentUtils - Document metadata
- [x] ColorUtils - Color conversion
- [x] StringUtils - Encoding utilities

## In Progress

### Hand-Written Wrappers for Complex Types
These SDK patterns can't be auto-generated and need manual C++ endpoints:

#### 1. Selection Access (AIMatchingArtSuite) - CRITICAL
**Blocks:** Most real-world automation scripts start with `app.selection`
**Endpoint:** `GET /api/selection` -> array of art handle IDs
**Why codegen can't handle it:** Triple pointer output (`AIArtHandle***`) - SDK allocates array internally
**Manual wrapper needs to:**
- Call `GetSelectedArt(&matches, &count)`
- Iterate returned array, register each handle with HandleManager
- Free SDK-allocated memory with AIMdMemorySuite
- Return JSON array of handle IDs

#### 2. Fill/Stroke Colors (AIPathStyleSuite) - HIGH
**Blocks:** Color-related workflows
**Endpoints:** `GET/POST /api/art/{id}/pathStyle`
**Why codegen can't handle it:** Complex nested structs with union types
- `AIPathStyle` contains `AIFillStyle` and `AIStrokeStyle`
- `AIColor` is a tagged union (gray/RGB/CMYK/pattern/gradient)
**Manual wrapper needs to:**
- Switch on `color.kind` to determine active union member
- Recursively serialize each color variant to JSON
- Handle pattern/gradient references (additional handle registration)

#### 3. Path Geometry (AIPathSuite) - MEDIUM
**Blocks:** Path manipulation workflows
**Endpoints:** `GET/POST /api/path/{id}/segments`
**Why codegen can't handle it:** Array parameters with separate count
- `GetPathSegments(art, segNumber, count, AIPathSegment* segments)`
**Manual wrapper needs to:**
- Allocate segment array buffer based on count
- Serialize/deserialize `AIPathSegment[]` to JSON array
- Each segment has: anchor point, in-handle, out-handle, corner flag

### Developer Experience
- [ ] Better error messages from SDK calls (map AIErr codes to descriptive strings)
- [ ] Hot reload improvements for generated code

## Planned

### Phase 1: Extended SDK Support
- [ ] Text frame manipulation (blocked by ATE header conflicts with Illustrator SDK)
- [ ] Symbol library access (AISymbolSuite)
- [ ] Swatch management (AISwatchListSuite)
- [ ] Additional struct support in codegen (AIColor, AIPathStyle, AIGradient)

### Phase 2: Advanced Features
- [ ] Compound path operations
- [ ] Live effects integration
- [ ] Plugin preferences persistence
- [ ] XMP metadata support (requires separate Adobe XMP SDK)

### Phase 3: Production Readiness
- [ ] Performance optimization
- [ ] Memory leak detection
- [ ] Comprehensive error handling with error code database
- [ ] Production build optimization
- [ ] Windows build support

### Phase 4: Ecosystem
- [ ] Plugin templates for common use cases
- [ ] Community extensions directory
- [ ] Visual Studio Code integration
- [ ] Example plugins

## Contributing

We welcome contributions! Here are some areas where help is especially appreciated:

### Good First Issues
- Improving documentation
- Adding TypeScript types for SDK methods
- Writing tests for the code generator
- UI improvements in the shell

### Medium Complexity
- Adding support for additional SDK suites in the parser
- Implementing hand-written wrappers for complex types (see above)
- Adding struct definitions to type-map.json

### Advanced
- Windows build support
- ATE (text engine) header compatibility
- Complex struct auto-marshaling (union types, variable-length arrays)

See [CONTRIBUTING.md](CONTRIBUTING.md) for how to get started.

## Release Schedule

We don't have fixed release dates. Releases happen when significant features are complete and stable.

### Versioning

NUXP follows semantic versioning:
- **Major**: Breaking API changes
- **Minor**: New features, backward compatible
- **Patch**: Bug fixes, backward compatible

## Feedback

Have suggestions for the roadmap? Open an issue or start a discussion on GitHub.
