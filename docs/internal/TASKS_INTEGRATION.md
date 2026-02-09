# NUXP Integration Tasks (Phase 3)

These tasks bridge the gap between the Generated Code, the C++ Plugin Infrastructure, and the Frontend Shell.

## Task 12: Upgrade Generator for Dispatching

**Status**: [x] Complete

**Objective**: Update the `CppGenerator` to automatically generate the `Dispatch(method, params)` function for each suite...

**Inputs**:
-   `codegen/src/generator/CppGenerator.ts`: Source to modify.
-   `codegen/output/cpp/`: Target output location.

**Outputs**:
-   `plugin/src/endpoints/generated/Flora[Suite]Wrapper.h`: Add a `Dispatch(method, params)` function to the namespace.
-   `plugin/src/endpoints/generated/CentralDispatcher.h`: A master dispatcher that routes by Suite Name.

**Instructions**:
1.  **Suite Dispatcher**: Update `CppGenerator` to add a `json Dispatch(string method, json args)` function to each Suite's namespace.
    -   It should switch on `method` string and call the appropriate wrapper (e.g., `if (method == "NewArt") return NewArt(args);`).
2.  **Central Dispatcher**: Generate a new file `CentralDispatcher.h` that:
    -   Includes all Suite Headers.
    -   Exposes `json Dispatch(string suite, string method, json args)`.
    -   Switches on `suite` string (e.g., `if (suite == "AIArt") return Flora::AIArtSuite::Dispatch(method, args);`).

**Acceptance Criteria**:
-   Running `scripts/generate.sh` produces `CentralDispatcher.h`.
-   The generated code compiles.

---

## Task 13: Wire Up C++ Server

**Objective**: Connect the HTTP Server to the generated `CentralDispatcher`.

**Inputs**:
-   `plugin/src/HttpServer.cpp`: The file to modify.
-   `plugin/src/endpoints/generated/CentralDispatcher.h`: The generated file from Task 12.

**Outputs**:
-   Modified `HttpServer.cpp`.

**Instructions**:
1.  **Include**: Add `#include "endpoints/generated/CentralDispatcher.h"`.
2.  **Route**: In the `/api/call` and `/{suite}/{method}` endpoints, replace the dummy return with:
    ```cpp
    return Flora::CentralDispatcher::Dispatch(suite, method, args);
    ```
3.  **Error Handling**: Ensure exceptions from the Dispatcher are caught and returned as JSON errors.

**Acceptance Criteria**:
-   The Plugin compiles with the generated dispatcher.
-   A `curl` POST to `/api/call` with valid suite/method triggers the wrapper (even if it fails inside the SDK, it should reach the wrapper).

---

## Task 14: TypeScript Bridge Implementation

**Objective**: Implement the frontend communication layer to talk to the C++ server.

**Inputs**:
-   `shell/src/sdk/`: Location for the bridge.
-   `shell/src/services/`: Integration point.

**Outputs**:
-   `shell/src/sdk/bridge.ts`: The transport layer.
-   `shell/src/sdk/config.ts`: Configuration (port, timeouts).

**Instructions**:
1.  **Bridge**: Create `bridge.ts` that exports `callCpp(suite, method, args)`.
    -   It should `fetch('http://localhost:8080/api/call', ...)`
    -   Handle network errors and JSON parsing.
2.  **Environment**: Respect `VITE_USE_MOCK` env var. If true, route to `MockBridge` (from Task 7) instead of localhost.
3.  **Injection**: Ensure generated TS files import `callCpp` from this bridge. (You may need to tweak `TypeScriptGenerator` imports if they are hardcoded).

**Acceptance Criteria**:
-   Calling `callCpp('AIArt', 'GetArtName', {...})` sends a correct POST request to port 8080.

---

## Task 15: "Hello World" Feature (Create Rectangle)

**Objective**: Prove the end-to-end pipeline by creating a simple rectangle in Illustrator.

**Inputs**:
-   `shell/src/modules/debug/DebugPanel.vue`: The UI to add the button to.
-   `shell/src/sdk/generated/AIArtSuite.ts`.

**Outputs**:
-   A working "Create Rectangle" button in the UI.

**Instructions**:
1.  **UI**: Add a button "Create Test Rectangle".
2.  **Logic**:
    -   Call `AIArtSuite.NewArt({ type: kPathArt, ... })`.
    -   Call `AIArtSuite.SetArtName({ art: id, name: "Hello NUXP" })`.
    -   Call `AIArtSuite.SetArtBounds(...)` to give it size (100x100).
3.  **Verify**: Log the result.

**Acceptance Criteria**:
-   Clicking the button causes a rectangle to appear in the active Illustrator document.
-   The rectangle is named "Hello NUXP".
