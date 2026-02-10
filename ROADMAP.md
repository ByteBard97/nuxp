# NUXP Roadmap

This document outlines what's complete, what's in progress, and what's planned for NUXP.

## Current Status: Alpha

NUXP is in alpha stage - the core architecture is complete and functional, but some features are still being developed.

## What's Complete

### Core Infrastructure
- [x] C++ plugin framework with HTTP/JSON server
- [x] Vue 3 frontend with TypeScript SDK
- [x] Code generator for SDK bindings
- [x] Handle management for safe cross-thread operations
- [x] Event system for Illustrator notifications
- [x] Mock mode for development without Illustrator

### Frontend
- [x] Debug Panel with event logging
- [x] Test Runner framework
- [x] Design System showcase
- [x] Connection status management

### Build & CI
- [x] CMake build system with customization options
- [x] GitHub Actions for automated builds
- [x] SDK setup script

### Documentation
- [x] README with architecture overview
- [x] Getting started guide
- [x] Contribution guidelines

## In Progress

### Phase 1: SDK Integration
- [x] Suite pointer management for essential suites (12 suites)
- [x] Demo endpoints (document info, layers, selection, create rectangle)
- [x] Utility modules (Color, Document, Geometry, Layer, Selection, String)
- [ ] Artboard support

### Phase 2: Developer Experience
- [x] SSE real-time push notifications
- [ ] Better error messages from SDK calls
- [ ] TypeScript SDK improvements
- [ ] Hot reload improvements

## Planned

### Community Priority: Critical SDK Gaps

These three manual wrappers would unblock ~80% of real-world Illustrator automation scripts.
They require manual implementation because the code generator can't handle their parameter types.

#### 1. Selection Access (AIMatchingArtSuite) - CRITICAL
**Blocks:** ~95% of all scripts (everything starts with `app.selection`)
**Endpoint:** `GET /api/selection` → returns array of art handle IDs
**Why codegen fails:** Triple pointer output (`AIArtHandle***`) - SDK allocates array internally
**Manual wrapper needs to:**
- Call `GetSelectedArt(&matches, &count)`
- Iterate returned array, register each handle with HandleManager
- Free SDK-allocated memory with AIMdMemorySuite
- Return JSON array of handle IDs

#### 2. Fill/Stroke Colors (AIPathStyleSuite) - HIGH
**Blocks:** ~20 color-related scripts (ColorCorrector, SelectBySwatches, etc.)
**Endpoints:** `GET/POST /api/art/{id}/pathStyle`
**Why codegen fails:** Complex nested structs with union types
- `AIPathStyle` contains `AIFillStyle` and `AIStrokeStyle`
- `AIColor` is a tagged union (gray/RGB/CMYK/pattern/gradient)
**Manual wrapper needs to:**
- Switch on `color.kind` to determine active union member
- Recursively serialize each color variant to JSON
- Handle pattern/gradient references (additional handle registration)

#### 3. Path Geometry (AIPathSuite) - MEDIUM
**Blocks:** ~12 path manipulation scripts (SplitPath, DrawPath, etc.)
**Endpoints:** `GET/POST /api/path/{id}/segments`
**Why codegen fails:** Array parameters with separate count
- `GetPathSegments(art, segNumber, count, AIPathSegment* segments)`
**Manual wrapper needs to:**
- Allocate segment array buffer based on count
- Serialize/deserialize `AIPathSegment[]` to JSON array
- Each segment has: anchor point, in-handle, out-handle, corner flag

---

### Phase 3: Extended SDK Support
- [ ] Text frame manipulation (blocked by ATE header conflicts)
- [ ] Symbol library access
- [ ] Gradient and pattern support (needs AIGradientSuite, AIPatternSuite)
- [ ] Swatch management (needs AISwatchListSuite)

### Phase 4: Advanced Features
- [ ] Undo/redo transaction support
- [ ] Compound path operations
- [ ] Live effects integration
- [ ] Plugin preferences persistence

### Phase 5: Production Readiness
- [ ] Performance optimization
- [ ] Memory leak detection
- [ ] Comprehensive error handling
- [ ] Production build optimization

### Phase 6: Ecosystem
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
- Adding support for additional SDK suites
- Implementing demo endpoints
- Improving error handling

### Advanced
- Windows build support
- Performance optimization
- Memory management improvements

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
