# NUXP Code Generator Agent Tasks

This document defines the tasks required to complete the NUXP Code Generator. These tasks are designed to be handed off to independent coding agents.

## Project Context
The goal is to generate C++ wrappers and TypeScript interfaces for the Adobe Illustrator C++ SDK. We have already implemented:
-   `SuiteParser`: Parses SDK headers into JSON `SuiteInfo`.
-   `TypeClassifier`: Enriches parameter types with semantic meaning (Handle, String, Primitive).
-   `config/type-map.json`: Configuring the mapping.

## Task 1: Implement C++ Generator Logic

**Objective**: Create the `CppGenerator` class that transforms enriched `SuiteInfo` JSON into C++ source code. The generated C++ code must marshal JSON inputs to SDK types, call the SDK function, and marshal the result back to JSON.

**Inputs**:
-   `src/parser/types.ts`: TypeScript interfaces for the parser output.
-   `src/config/type-map.json`: The type mapping configuration.
-   `CODEGEN_STRATEGY.md`: The architectural blueprint for the marshaling logic.

**Output**:
-   `src/generator/CppGenerator.ts`: The generator implementation.

**Instructions**:
1.  **Create generic templates**: Use `mustache` (already in `package.json`) to create templates for:
    -   Header file (`Flora[Suite]Wrapper.h`)
    -   Source file (`Flora[Suite]Wrapper.cpp`)
    -   Function wrapper logic (inside the source file).
2.  **Implement `generate(suite: SuiteInfo): GeneratedFile[]`**: This method should:
    -   Load templates.
    -   Iterate over functions in the suite.
    -   For each function parameter, use the `classification` data to determine the correct marshaling code (refer to `CODEGEN_STRATEGY.md` for specific logic).
    -   Generate the full C++ file content.
3.  **Handle Marshalling Spec**:
    -   **Input Handles**: `HandleManager::[registry].Get(json_id)` -> `SDKHandle`.
    -   **Output Handles**: `SDKHandle` -> `HandleManager::[registry].Register(ptr)` -> `json_id`.
    -   **Strings**: usage of `ai::UnicodeString` constructors and `.as_UTF8()`.
    -   **Primitives**: direct casting.
4.  **Error Handling**: Ensure all SDK calls wrap the return `AIErr` and throw a JSON-compatible error if non-zero.


**Acceptance Criteria**:
-   The generator produces valid C++ code for `AIArtSuite` (test with `AIArt.h`).
-   The generated code compiles (run a mock compile test or verify syntax).
-   It correctly handles `AIArtHandle` input/output and `ai::UnicodeString` output.
-   **Verification**: Create a test script `src/test-cpp-gen.ts` that mocks a `SuiteInfo` object and asserts the generated string contains expected patterns (e.g., `HandleManager::art.Get`, `sAIArt->GetArtName`).

---

## Task 2: Implement TypeScript Generator Logic

**Objective**: Create the `TypeScriptGenerator` class that produces typed TypeScript interfaces and API calls for the frontend.

**Inputs**:
-   `src/parser/types.ts`: Parser output interfaces.

**Output**:
-   `src/generator/TypeScriptGenerator.ts`: The generator implementation.

**Instructions**:
1.  **Create templates**: Use `mustache` for the TypeScript file structure.
2.  **Implement `generate(suite: SuiteInfo): GeneratedFile`**:
    -   Generate an `interface` for the Suite (e.g., `AIArtApis`).
    -   Generate functions that call `feral.callCpp(...)`.
3.  **Type Mapping**:
    -   Map C++ primitives to TS primitives (`ai::int32` -> `number`, `AIBoolean` -> `boolean`).
    -   Map Handles to `number` (IDs).
    -   Map Structs to `any` or generates specific interfaces if possible (bonus).

**Acceptance Criteria**:
-   Generates a `.ts` file that exports a class/interface matching the SDK suite.
-   Functions correctly map parameters to a JSON object for the IPC call.
-   **Verification**: Create `src/test-ts-gen.ts` to mock a `SuiteInfo` and verify the output contains valid TypeScript syntax and correct API calls.

---

## Task 3: Build & Integrate

**Objective**: Create the build script to run the generator and integrate the output into the C++ plugin.

**Inputs**:
-   `src/index.ts` (Main entry point for codegen).

**Outputs**:
-   `scripts/generate.sh`: Shell script to run the codegen.
-   `plugin/CMakeLists.txt`: Update to include generated files.

**Instructions**:
1.  **Main Entry Point**: Update `src/index.ts` to orchestrate `SuiteParser` -> `CppGenerator` -> `TypeScriptGenerator` -> File Write.
2.  **Script**: Create `scripts/generate.sh` that runs `npm start` in `codegen` and copies output to `plugin/src/generated` and `client/src/generated`.
3.  **CMake**: Update `plugin/CMakeLists.txt` to verify it includes the generated `.cpp` files.


-   **Verification**: Run the full build process and check for compilation errors. If possible, create a dummy test plugin that links against the generated code.



