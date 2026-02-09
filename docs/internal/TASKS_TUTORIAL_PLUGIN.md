# The "NUXP DevTools" Showcase Plugin 🛠️

A developer-focused tutorial plugin that acts as a live API explorer for the Illustrator SDK.
This is designed to be useful for **ANY** Illustrator plugin developer using NUXP. It demonstrates how to call SDK functions, inspect the results, and understand the data flow.

## Why this is useful:
1.  **Code Playground**: Test SDK calls live (e.g., `sAIArt->GetArtBounds(art)`) without recompiling C++.
2.  **Selection Inspector**: See the JSON representation of the currently selected artwork.
3.  **Handle Visualizer**: Understand how NUXP maps `AIArtHandle` (C++ pointer) to `int32` (JSON ID).

---

## Task 9: Selection Inspector Module

**Objective**: Create a panel that shows live details about the currently selected artwork.

**Inputs**:
-   `shell/src/sdk/generated/AIArtSuite.ts`: The generated bindings.
-   `shell/src/sdk/generated/AIPathSuite.ts`.

**Outputs**:
-   `shell/src/modules/devtools/SelectionInspector.vue`.
-   `shell/src/modules/devtools/InspectorStore.ts`.

**Instructions**:
1.  **Polling Loop**: In the store, poll `AIArtSuite.GetArtUserAttr` or similar every 500ms (or hook into `kAIArtChanged` later).
2.  **Inspector UI**:
    -   Display the `Handle ID`.
    -   Call `GetArtName` and display the name.
    -   Call `GetArtBounds` and display `top, left, width, height`.
    -   Call `GetArtType` and show the enum value.
3.  **Code Snippet**: Below the details, show the actual TypeScript code that fetched this data (e.g., `await AIArtSuite.GetArtBounds(123)`).

**Acceptance Criteria**:
-   Select an object in Illustrator.
-   The Inspector updates to show its Name, Bounds, and Type.
-   The "Code Snippet" box updates to reflect the calls made.

---

## Task 10: API Console (REPL)

**Objective**: A "Console" where developers can manually trigger SDK functions with custom parameters.

**Inputs**:
-   `shell/src/sdk/generated/*.ts`.

**Outputs**:
-   `shell/src/modules/devtools/ApiConsole.vue`.

**Instructions**:
1.  **Function Selector**: A dropdown list of all available generated functions (e.g., `AIArtSuite.SetArtName`).
2.  **Argument Inputs**: Dynamically generate inputs based on the selected function's parameters.
    -   `String` -> Text Input.
    -   `AIReal` -> Number Input.
    -   `AIArtHandle` -> "Use Selection" button (grabs ID from Task 9).
3.  **Execute**: Button that calls the function and prints the JSON result to a log window.

**Acceptance Criteria**:
-   Developer can select `SetArtName`, type "My New Name", click Execute.
-   The selected object in Illustrator is renamed.
-   The log window shows: `CreateArtName({ art: 123, name: "My New Name" }) -> Success`.

---

## Task 11: "How It Works" Walkthrough

**Objective**: Interactive tutorial explaining the NUXP architecture.

**Inputs**:
-   `driver.js` (Vue tour library).

**Outputs**:
-   `shell/src/modules/tutorial/ArchitectureTour.vue`.

**Instructions**:
1.  **Guided Tour**: Create a tour that highlights:
    -   **The Frontend (Vue)**: "This is where your UI lives."
    -   **The Bridge (IPC)**: "We send JSON messages over localhost."
    -   **The Backend (C++)**: "Our generated wrappers catch the JSON and call the SDK."
2.  **Visual Diagram**: A simple SVG diagram of the architecture that animates when an API call is made.

**Acceptance Criteria**:
-   A readable, step-by-step guide that helps new plugin developers understand the stack.
