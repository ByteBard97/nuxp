# SDK Documentation Design

## Problem

The entire `sdk/` package (extracted from flora-uxp) is undocumented. The Architecture doc covers C++ threading but not the TypeScript SDK layer. New developers have no quick-start path, no guidance on which class to use, and no understanding of how the two queue systems interact.

## Approach: Layered Documentation

Three documents, each serving a distinct audience entry point.

### 1. `sdk/README.md` — SDK Package Guide

**Audience:** Developer who opened the `sdk/` folder and wants to build something.

**Sections:**
1. One-liner: "TypeScript SDK for building Illustrator plugins on NUXP."
2. Quick Start — 15-line bootstrap: createBridge, setBridgeInstance, health check, SSE subscription, first call.
3. "Which class do I use?" — Decision tree: PluginAdapter (recommended), Bridge (suite calls/batch), HttpAdapter/SSEAdapter (internal), Primitives (convenience).
4. Module Map — Table of directories with purpose, marked as "core" vs "optional."
5. Threading & Request Serialization — AutoQueue purpose, batch() tradeoff, pointer to ARCHITECTURE.md.
6. Events Reference — 5 SSE event types with payloads and triggers.
7. Services Quick Reference — One-liner per service with constructor pattern.
8. Design Principles — Bridge-first, no framework coupling, constructor DI.

### 2. `docs/ARCHITECTURE.md` Updates

**Changes:**
- New Section 1.5: "TypeScript Request Lifecycle" connecting AutoQueue → HTTP → MainThreadDispatch into one diagram.
- Explain WHY AutoQueue exists (connection pool protection, not thread safety).
- Document batch() tradeoff.
- Document SSE as the reverse channel.
- Update intro to acknowledge three layers (TS client, HTTP transport, C++ main thread).
- Fix `Flora::Dispatch` → generic dispatcher reference.

### 3. Root `README.md` Updates

**Changes:**
- Add `sdk/` to project structure tree.
- Update API Overview to mention Bridge/createBridge.
- Fix import paths to show `@nuxp/sdk` alongside shell aliases.
- Mention setBridgeInstance() bootstrap.

## Key Issues Found by Review

1. Bridge vs HttpAdapter vs PluginAdapter — overlapping APIs, no guidance.
2. Two queues (AutoQueue + MainThreadDispatch) — no doc connecting them.
3. SSE endpoint mismatch — docs say `/events`, code says `/events/stream`.
4. setBridgeInstance() bootstrapping invisible to new developers.
5. All examples use shell-internal `@/sdk/` import paths.
6. Flora-specific types mixed with core SDK in barrel exports.
7. `Flora::Dispatch` reference in Architecture doc.
