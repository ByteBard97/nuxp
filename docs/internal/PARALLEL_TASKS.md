# NUXP Parallel Agent Tasks (Phase 2)

These tasks are independent of the Code Generator and can be started immediately by separate agents.

## Task 4: Frontend Shell Skeleton

**Objective**: Initialize the Vue/Vite + Tauri application in `shell/`.

**Inputs**:
-   `REPO_STRUCTURE.md`: Defines the folder structure.

**Outputs**:
-   `shell/package.json`, `shell/vite.config.ts`, `shell/src/App.vue`.
-   Basic routing and state management (Pinia).

**Instructions**:
1.  **Initialize**: Use `create-tauri-app` or manual setup for Vue 3 + TypeScript + Vite.
2.  **Architecture**:
    -   Set up a `services/` folder for the API layer (which will eventually use the generated code).
    -   Create a `store/` validation layer using Pinia.
3.  **Mock UI**: Create a simple "Debug Panel" that will eventually list active documents and selection.

**Acceptance Criteria**:
-   `npm run dev` in `shell/` launches a functional Vue app.
-   The app has a basic layout (sidebar + content area).

---

## Task 5: Plugin Infrastructure (HTTP Server)

**Objective**: Implement the `Plugin.cpp` entry point and the core HTTP server loop.

**Inputs**:
-   `plugin/lib/httplib.h`: The HTTP library.
-   `PROPOSAL_HANDLE_MANAGEMENT.md`: For context on threading.

**Outputs**:
-   `plugin/src/Plugin.cpp`: The main plugin entry.
-   `plugin/src/Server.cpp`: The HTTP server implementation.

**Instructions**:
1.  **Entry Point**: Implement standard Illustrator plugin entry points (`PluginMain`).
2.  **Server Loop**:
    -   Start an HTTP server on a background thread (port 8080 or configurable).
    -   Implement a basic `/health` endpoint returning `{"status": "ok"}`.
    -   Implement a generic `/api/call` endpoint that accepts JSON `{"suite": "...", "method": "...", "args": {...}}` (this will later dispatch to generated code).
3.  **Thread Safety**: Ensure all SDK calls from the server thread are dispatched to the main thread (using `MainThreadDispatch` mechanism defined in `PROPOSAL_HANDLE_MANAGEMENT.md`).

**Acceptance Criteria**:
-   The plugin compiles (mocking the SDK entry points if necessary).
-   The HTTP server starts and responds to `curl localhost:8080/health`.

---

# Phase 3: Integration & Features (Post-Codegen)

Once the Generators and Parallel Tasks are complete, the next phase is:

1.  **Wire Up**: Connect the `Server`'s `/api/call` endpoint to the generated `HandleManager` and Wrappers.
2.  **First Feature**: Implement "Create Rectangle" end-to-end (Frontend Button -> API Call -> C++ Wrapper -> Illustrator SDK -> Rectangle on Canvas).
3.  **Event System**: Capture `kAIArtChanged` notifier and push an event to the Frontend via WebSocket/SSE.
