---
layout: default
title: Getting Started
---

# Getting Started with NUXP

<p align="center">
  <img src="images/nuxp-destruction.gif" alt="Captain NUXP" width="200">
</p>

This guide covers how to set up your development environment for NUXP.

## Prerequisites

-   **Node.js**: v18+
-   **CMake**: v3.20+
-   **C++ Compiler**: Xcode Command Line Tools (`xcode-select --install`)
-   **Adobe Illustrator**: 2024 or later (for testing the plugin)

## 1. Download the Adobe Illustrator SDK

> **Important**: The SDK is proprietary and cannot be included in this repository. You must download it from Adobe (requires free Adobe account).

1.  **Go to Adobe Developer Console**:
    [https://developer.adobe.com/console/servicesandapis/ai](https://developer.adobe.com/console/servicesandapis/ai)

2.  **Sign in** with your Adobe ID (create one for free if needed)

3.  **Download** the Illustrator SDK for your version (e.g., `AI_2026_SDK_Mac.dmg`)

4.  **Save the file** somewhere accessible (e.g., `~/Downloads/`)

## 2. Setup the SDK

Run the setup script with your downloaded DMG:

```bash
./scripts/setup-sdk.sh ~/Downloads/AI_2026_SDK_Mac.dmg
```

This will:
- Mount the DMG
- Extract all required headers to `plugin/sdk/`
- Create the `IllustratorSDK.h` convenience header
- Unmount the DMG

## 3. Install Dependencies

```bash
# Install codegen dependencies
cd codegen && npm install && cd ..

# Install shell (frontend) dependencies
cd shell && npm install && cd ..
```

## 4. Build & Run

### Frontend Only (Mock Mode - No Illustrator Required)

For UI development without the C++ plugin:

```bash
cd shell
VITE_USE_MOCK=true npm run dev
```

Open http://localhost:5173 to see the app with simulated SDK responses.

### Full Stack (With Illustrator)

#### a. Build the Plugin

```bash
cd plugin
cmake -B build
cmake --build build
```

#### b. Install the Plugin

```bash
cmake --install build
```

This copies the plugin to your Illustrator plug-ins folder.

#### c. Start Illustrator

Launch Illustrator - the plugin will start its HTTP server on port 8080.

#### d. Run the Frontend

```bash
cd shell
npm run dev
```

The frontend will connect to the real plugin.

## 5. Generate SDK Wrappers (Optional)

To regenerate the C++ and TypeScript bindings from SDK headers:

```bash
./scripts/generate.sh
```

This creates:
- `plugin/src/endpoints/generated/` - C++ wrapper functions
- `shell/src/sdk/generated/` - TypeScript client SDK

## CI/CD Setup

For GitHub Actions:

1. Host the SDK somewhere accessible (S3, private release, etc.)
2. Set the `ILLUSTRATOR_SDK_URL` repository secret
3. The CI workflow will download and extract it automatically

## Troubleshooting

### "SDK not found" errors
Ensure `plugin/sdk/AIArt.h` exists. Re-run `setup-sdk.sh`.

### Plugin doesn't load in Illustrator
Check Illustrator's console/log for errors. Ensure the plugin is in the correct Plug-ins folder.

### HTTP connection refused
Ensure Illustrator is running with the plugin loaded. Check port 8080 is not in use.
