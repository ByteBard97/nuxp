# NUXP Phase 4-6: Platform Hardening & Migration

This document outlines the roadmap for verifying the platform with DevTools, hardening it for production, and finally migrating the legacy Flora application.

---

## Phase 4: The Showcase (DevTools)
*See [TASKS_TUTORIAL_PLUGIN.md](./TASKS_TUTORIAL_PLUGIN.md) for details.*

**Goal**: Prove the platform works by building a developer tool.
-   **Task 9**: Selection Inspector (Read Capability)
-   **Task 10**: API Console (Write Capability)
-   **Task 11**: Architecture Tour (Documentation)

---

## Phase 5: Platform Hardening

Before migrating the complex Flora app, we need to ensure the NUXP core can handle real-world scenarios like generic events and large data transfer.

### Task 16: Generic Events System

**Objective**: Allow the frontend to subscribe to any Illustrator event (e.g., `kAIArtSelectionChanged`) without polling.

**Inputs**:
-   `plugin/src/MainThreadDispatch.cpp`.
-   `shell/src/sdk/bridge.ts`.

**Outputs**:
-   **C++**: `EventMapper` class that translates `AINotifierMessage` to JSON and pushes it to the generic `/events` SSE (Server-Sent Events) endpoint or WebSocket.
-   **TS**: `sdk.on('selectionChanged', callback)` wrapper.

**Instructions**:
1.  **C++**: Implement a `std::vector<json> eventQueue` protected by a mutex.
2.  **C++**: Update `HandleNotifier` in `Plugin.cpp` to push events to this queue.
3.  **TS**: Implement long-polling or SSE in `bridge.ts` to receive these events in near real-time.

### Task 17: Binary Data Support

**Objective**: Efficiently transfer image data (for Plant thumbnails) without base64 overhead.

**Inputs**:
-   `plugin/src/HttpServer.cpp`.

**Outputs**:
-   New endpoint `/api/binary/{handle_id}`.

**Instructions**:
1.  **C++**: Implement an endpoint that takes an `AIArtHandle`, gets its raster data (if valid), and streams it as `image/png`.
2.  **TS**: `AIArtSuite.GetRasterImage(handle)` returns a `Blob` URL.

---

## Phase 6: Flora App Migration

**Goal**: Port the legacy Flora logic to the new NUXP architecture.

### Task 18: Flora Data Layer Port

**Objective**: Rewrite the business logic to use generated SDK suites instead of CSInterface/ExtendScript.

**Inputs**:
-   Legacy `host/` scripts (reference).
-   `shell/src/sdk/generated/AIArtSuite.ts`.

**Outputs**:
-   `shell/src/modules/flora/stores/plantStore.ts`.
-   `shell/src/modules/flora/models/Plant.ts`.

**Instructions**:
1.  **Model**: Define `Plant` as a class that wraps an `AIArtHandle`.
2.  **Methods**: Implement `plant.grow()` -> calls `AIArtSuite.TransformArt(...)`.
3.  **Sync**: Implement `syncFromArt(handle)` -> reads `GetArtName`, `GetArtUserAttr` to populate local state.

### Task 19: Flora UI Port

**Objective**: Move the Vue components into the Shell.

**Inputs**:
-   Legacy `client/` components.
-   `shell/src/modules/flora/components/`.

**Outputs**:
-   A working Flora panel in the NUXP shell.

**Instructions**:
1.  **Migrate**: Copy legacy `.vue` files to `shell/src/modules/flora/`.
2.  **Refactor**: Replace `csInterface.evalScript` calls with `usePlantStore` actions.
3.  **Styling**: Update CSS to use the new `variables.css` design system (Task 6).

### Task 20: End-to-End Verification

**Objective**: Verify the full lifecycle.
1.  Open Illustrator.
2.  Open NUXP Flora Panel.
3.  Click "Add Plant".
4.  See a new ArtItem appear on the canvas.
5.  Modify it in Illustrator (move/rename).
6.  See the changes reflect in the Vue UI (via Events).

**Acceptance Criteria**:
-   The Flora App works faster and more reliably than the CEP version.
-   No "Silent Failures" (errors are caught and displayed in the UI).
