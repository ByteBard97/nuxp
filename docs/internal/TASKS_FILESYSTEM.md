# NUXP: Filesystem Access (Tauri)

The user requires local filesystem access for the NUXP Shell (e.g., for caching thumbnails or reading config). Since the NUXP Shell is a Tauri application, we will use the `@tauri-apps/plugin-fs` system.

## Task 21: Initialize Tauri

**Status**: [ ] Pending

**Objective**: Convert the existing `nuxp-shell` (Vite) into a Tauri application.

**Inputs**:
-   `shell/` directory (Vite app).

**Outputs**:
-   `shell/src-tauri/` (Rust backend).
-   `shell/src-tauri/tauri.conf.json`.

**Instructions**:
1.  **Install Tauri CLI**:
    ```bash
    npm install --save-dev @tauri-apps/cli
    ```
2.  **Initialize**:
    Run `npx tauri init` (or manually create `src-tauri`).
    -   Package Manager: `npm`
    -   Frontend Url: `http://localhost:1420` (Vite dev)
3.  **Configure**: Ensure `tauri.conf.json` allows `http://localhost:8080` (for our C++ Plugin bridge) in the permissions/CSP if needed, though `plugin-fs` handles local files natively.

## Task 22: Integrate Filesystem Plugin

**Status**: [ ] Pending

**Objective**: Add the `@tauri-apps/plugin-fs` to allow reading/writing files from the Vue app.

**Inputs**:
-   `shell/package.json`.
-   `shell/src-tauri/Cargo.toml`.

**Outputs**:
-   `shell/src/services/filesystem.ts` (Wrapper).

**Instructions**:
1.  **Install Plugin**:
    ```bash
    cd shell
    npm run tauri add fs
    # Or:
    # npm install @tauri-apps/plugin-fs
    # cargo add tauri-plugin-fs --path src-tauri
    ```
2.  **Configure Capabilities (`src-tauri/capabilities/default.json`)**:
    Allow access to `$APPDATA` and `$HOME/Documents/NUXP/`.
    ```json
    {
      "permissions": [
        "fs:default",
        "fs:allow-read-text-file",
        "fs:allow-write-text-file",
        {
          "identifier": "fs:allow-write",
          "allow": [{ "path": "$APPDATA/nuxp/**" }]
        }
      ]
    }
    ```
3.  **Create Wrapper (`filesystem.ts`)**:
    Create a service that wraps `plugin-fs` commands.
    ```typescript
    import { exists, writeTextFile, BaseDirectory } from '@tauri-apps/plugin-fs';
    // ...
    ```

## Task 23: Verify File Access

**Status**: [ ] Pending

**Objective**: Prove we can write a file to disk.

**Instructions**:
1.  Add a "Save Log" button to the Debug Panel.
2.  When clicked, write the current Log content to `$APPDATA/nuxp/debug.log`.
3.  Read it back and display a success message.
