# NUXP Test Plugin (QA App)

The goal of this plugin is to serve as a comprehensive **Verification Suite** for the NUXP framework. It should exercise every supported feature of the adapter, ensuring that both the C++ SDK bridge and the Tauri system integrations are working correctly.

## Overview

The Test Plugin will be a single panel with a **sidebar navigation** that switches between different "Test Suites". Each suite focuses on one specific domain (e.g., "Filesystem", "Art Creation", "Events").

## Test Structure

Each test case should follow this pattern:
1.  **Description**: What is being tested.
2.  **Action**: Button to trigger the test.
3.  **Result**: Visual feedback (Success/Fail icon + JSON output).

---

## Suite 1: System & Connectivity

**Goal**: Verify the bridge is alive and performance is acceptable.

-   [ ] **Health Check**: Call `/health` endpoint.
-   [ ] **Round Trip Latency**: Measure time for a simple `NoOp` call.
-   [ ] **Version Info**: Display C++ Plugin Version, wrapper version, and Tauri version.

## Suite 2: Filesystem (Tauri)

**Goal**: Verify sandboxed file I/O works.

-   [ ] **Write Config**: Write a JSON object to `$APPDATA/config.json`.
-   [ ] **Read Config**: Read it back and verify contents match.
-   [ ] **List Directory**: List files in `$APPDATA`.
-   [ ] **Delete File**: Remove the test file.

## Suite 3: Art Creation (SDK)

**Goal**: Verify `AIArtSuite` and `AIPathSuite` bindings.

-   [ ] **Create Rectangle**: The "Hello World" test (Path Art).
-   [ ] **Create Text**: Create a text frame with specific content (Text Art).
-   [ ] **Create Layer**: Add a new layer and activate it.
-   [ ] **Modify Selection**: Get current selection, move it `+10pt` right.

## Suite 4: Hierarchy & Traversal

**Goal**: Verify handle management and iteration.

-   [ ] **Count Items**: Recursive count of all items in the document.
-   [ ] **Get Selection Details**: detailed JSON dump of the selected item's properties (bounds, type, name).

## Task 24: Scaffold Test App

**Objective**: Create the sidebar layout and the "Test Runner" component.

**Inputs**:
-   `shell/src/components/ui/` (Button, Panel).

**Outputs**:
-   `shell/src/modules/tests/TestRunner.vue`.
-   `shell/src/modules/tests/suites/*.ts`.

**Instructions**:
1.  Create a `useTestRunner` composable that handles `runTest(name, callback)`.
2.  Implement the UI to show a list of tests with status indicators.

## Task 25: Implement Test Suites

**Objective**: Write the actual test logic using `callCpp` and `filesystem`.

**Instructions**:
1.  Implement `SystemSuite.ts` (Health, Latency).
2.  Implement `FilesystemSuite.ts` (Write/Read).
3.  Implement `ArtSuite.ts` (Create Rect, Text).
