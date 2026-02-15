# Developer Experience Audit

*Written from the perspective of a developer who just found NUXP on GitHub and wants to build an Illustrator plugin.*

---

## The Good News

This project is impressive. 442 SDK functions auto-generated across 19 suites, typed end-to-end from C++ to TypeScript, with mock mode for frontend development without Illustrator. The architecture (HTTP/JSON bridge, handle management, main thread dispatch) is solid and solves real problems. The tutorial, CI/CD, license, and contributing guide are all professional.

A developer who gets through setup will have a genuinely powerful foundation.

## Where I Got Confused

### 1. The README API Overview is Fiction

Lines 335-354 of README.md show this:

```typescript
import { sdk } from '@/sdk'
const doc = await sdk.document.getActive()
const layers = await sdk.layers.list(doc.id)
await sdk.color.setFill(art.id, { r: 255, g: 0, b: 0 })
```

This API does not exist anywhere in the codebase. There is no `sdk.document`, no `sdk.layers`, no `sdk.color`. The actual API is either:

- **Generated suite functions**: `import { GetArtType } from '@/sdk/generated/AIArtSuite'`
- **Custom route functions**: `import { GetDocumentInfo } from '@/sdk/generated/customRoutes'`
- **Generic bridge call**: `callCpp('demo', 'createRectangle', { x, y, width, height })`

This section will cost every new developer 20+ minutes of confusion trying to find something that doesn't exist.

**Fix:** Replace with the actual calling conventions. Show a real import from a real generated file.

### 2. Three Ways to Call the Plugin, No Guidance on Which to Use

The codebase has three distinct calling patterns:

| Pattern | Where it lives | When to use it |
|---------|---------------|----------------|
| `callCpp(suite, method, args)` | `bridge.ts` | Generic SDK calls routed through CentralDispatcher |
| Generated suite functions | `sdk/generated/AIArtSuite.ts`, etc. | Type-safe wrappers for specific SDK suites |
| Custom route functions | `sdk/generated/customRoutes.ts` | Hand-written endpoints (doc info, view control, queries) |

The tutorial uses `callCpp()`. The examples use `callSdk()` (which wraps `callCpp`). The generated TypeScript SDK uses direct HTTP calls. No documentation explains when to use which, or that they all hit the same plugin.

**Fix:** A short section: "How to Call the Plugin" showing all three patterns and when each is appropriate.

### 3. Adding Custom Endpoints: Which Way?

If I want to add a new feature, the docs point me in three directions:

- **CONTRIBUTING.md** says: "Create YourEndpoints.cpp in `plugin/src/endpoints/`"
- **README "Creating Your Own Plugin"** says: "Create custom endpoint handlers"
- **The actual modern workflow** is: define a route in `codegen/src/config/routes.json`, run codegen, implement the handler in `plugin/src/endpoints/handwritten/`

The `routes.json` + codegen workflow generates both the C++ handler signature AND the TypeScript client. Hand-writing both sides manually is the old way. But the docs don't explain this, and the spec for it (`docs/CUSTOM_ROUTES_CODEGEN_SPEC.md`) is buried in the docs folder with no link from the README.

**Fix:** A "Adding Your First Endpoint" walkthrough that shows the routes.json workflow end-to-end: define route, run codegen, implement handler, call from frontend.

### 4. The Generated vs. Hand-Written Boundary

The project has excellent separation:
- `plugin/src/endpoints/generated/` -- auto-generated, do not edit
- `plugin/src/endpoints/handwritten/` -- custom code lives here
- `plugin/src/endpoints/README.md` explains this clearly

But `endpoints/README.md` is never linked from the main README, CONTRIBUTING.md, or the tutorial. A developer would need to stumble into that directory to find it.

**Fix:** Link to `plugin/src/endpoints/README.md` from the main README and CONTRIBUTING.md.

### 5. No Windows SDK Setup

`scripts/setup-sdk.sh` is macOS-only (it mounts a .dmg). The README mentions Windows support, CI/CD builds on Windows, but there's no setup script or step-by-step guide for Windows SDK extraction.

**Fix:** Either add a Windows section to the getting-started guide, or document the manual steps (extract ZIP, copy headers to `plugin/sdk/`).

### 6. Thread Safety is Critical But Undocumented for Users

Every custom C++ endpoint MUST use `MainThreadDispatch::Run()` because Adobe SDK calls can only happen on Illustrator's main thread. The HTTP server runs on a background thread. If you skip this, your plugin crashes.

This is explained in CLAUDE.md (developer instructions for AI assistants) but not in any user-facing documentation. A developer looking at `DemoEndpoints.cpp` will see the pattern but won't understand why it's required.

**Fix:** Add a "Threading" section to the endpoint development guide. One paragraph: "All Adobe SDK calls must execute on Illustrator's main thread. Wrap your handler body in `MainThreadDispatch::Run([&]() { ... })`. The HTTP server runs on a background thread -- calling SDK functions directly from a handler will crash."

### 7. Handle Management is Invisible

The HandleManager system (integer IDs instead of raw C++ pointers, generation counters for undo safety) is architecturally important and well-implemented. But a new developer seeing `HandleManager::art.Register(handle)` in handler code has no context for why this exists.

CLAUDE.md explains it well. User-facing docs don't.

**Fix:** A brief "Handles" section explaining: "C++ pointers can't be sent over HTTP as JSON. NUXP assigns integer IDs to SDK handles. The frontend sends these IDs back, and the plugin looks up the real handle. Generation counters detect stale handles after undo or document switches."

### 8. The Example File Uses a Non-Obvious Pattern

`examples/basic-api.js` calls `callSdk('demo', 'getDocumentInfo')` which routes through the `CentralDispatcher` as suite="demo", method="getDocumentInfo". This works because DemoEndpoints registers under the "demo" suite name.

But a developer reading this would think "demo" is an actual Adobe SDK suite. It's not -- it's a hand-written endpoint registered with a custom suite name. This conflation of "real SDK suites" and "custom endpoints registered as fake suites" is confusing.

The custom routes system (REST endpoints like `GET /api/doc/info`) is cleaner and doesn't have this confusion. The example should probably use REST endpoints instead.

**Fix:** Update the example to use REST-style custom routes, or add comments explaining that "demo" is a custom handler, not an Adobe SDK suite.

## What's Great (Keep This)

- **The tutorial** (`docs/TUTORIAL.md`) is genuinely good. Real code, works with mock mode, shows the actual development experience.
- **Mock mode** (`VITE_USE_MOCK=true`) lets you build UI without Illustrator. This is huge for developer experience.
- **The codegen pipeline** is well-tested (346 tests) and produces correct, typed code on both sides.
- **CUSTOMIZATION.md** with CMake cache variables is the right pattern for a C++ skeleton project.
- **CI/CD** builds on both macOS and Windows, creates releases from tags. Professional.
- **The endpoints/README.md** clearly explains the generated vs. hand-written boundary. Just needs to be linked.

## Suggested Documentation Structure

The existing docs are good individually but scattered. Here's a suggested reading order that a new developer could actually follow:

1. **README.md** -- What is NUXP, why does it exist, quick start
2. **docs/getting-started.md** -- Detailed setup (needs Windows section)
3. **docs/TUTORIAL.md** -- Build your first feature (already exists, already good)
4. **NEW: "Adding Custom Endpoints"** -- routes.json workflow end-to-end
5. **plugin/src/endpoints/README.md** -- Generated vs. hand-written code (exists, needs linking)
6. **CUSTOMIZATION.md** -- Making the project your own (exists, needs package.json fields added)
7. **docs/CUSTOM_ROUTES_CODEGEN_SPEC.md** -- Deep dive on the codegen system (exists)

### What's Missing from That List

- **"How to Call the Plugin"** -- a short reference showing all three calling patterns
- **"Adding Custom Endpoints"** -- the routes.json workflow from definition to working feature
- **"Architecture: Threading and Handles"** -- why MainThreadDispatch and HandleManager exist
- **Windows SDK setup steps**
- **"Extending the SDK"** -- how to add support for a new Adobe SDK suite (add header to `plugin/sdk/`, run `generate.sh`, explains the namespace and how to avoid collisions with existing wrappers)

## Summary

The codebase is ready for release. The architecture is sound, the tests pass, the CI works. The documentation gaps are real but not blocking -- they'll cost new developers hours of confusion, not days. The highest-impact fixes:

1. **Replace the fictional API Overview** in README with real code (~5 minutes)
2. **Link to endpoints/README.md** from main docs (~2 minutes)
3. **Add "Threading" and "Handles" paragraphs** to the tutorial or a new concepts page (~15 minutes)
4. **Write "Adding Custom Endpoints" guide** showing routes.json workflow (~30 minutes)
5. **Add Windows SDK setup section** (~10 minutes)

None of these require code changes. They're all documentation.
