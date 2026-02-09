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
- [ ] Suite pointer management for essential suites
- [ ] Demo endpoints (document info, layers, selection, create rectangle)
- [ ] Artboard support

### Phase 2: Developer Experience
- [ ] Better error messages from SDK calls
- [ ] TypeScript SDK improvements
- [ ] Hot reload improvements

## Planned

### Phase 3: Extended SDK Support
- [ ] Text frame manipulation
- [ ] Symbol library access
- [ ] Gradient and pattern support
- [ ] Color management

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
