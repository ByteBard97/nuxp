# NUXP ("Not UXP")

**No UXP? No problem.**

Adobe refuses to port UXP to Illustrator, leaving us stuck with the ancient, deprecated CEP framework from 2013.
**NUXP** is a modern alternative that lets you build native-feeling Illustrator plugins using **Vue**, **TypeScript**, and the **Adobe C++ SDK**, completely bypassing the limitations of CEP.

*"If Adobe won't build it, we will."*

## Architecture

NUXP replaces the "CEP Panel" approach with a standalone web application that communicates with Illustrator via a C++ plugin exposing a local HTTP server.

```
┌─────────────────────────────────────────────────────────────┐
│                    Your Application                          │
│  ┌─────────────────────────────────────────────────────┐    │
│  │              Vue 3 Frontend (shell/)                 │    │
│  │   Components │ Pinia Stores │ TypeScript SDK Client  │    │
│  └─────────────────────────┬───────────────────────────┘    │
│                            │ HTTP (localhost:8080)           │
│  ┌─────────────────────────▼───────────────────────────┐    │
│  │              C++ Plugin (plugin/)                    │    │
│  │   HTTP Server │ Handle Manager │ SDK Wrappers        │    │
│  └─────────────────────────┬───────────────────────────┘    │
│                            │ Native API                      │
│  ┌─────────────────────────▼───────────────────────────┐    │
│  │              Adobe Illustrator                       │    │
│  └─────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
```

## Quick Start

### Prerequisites

- **Node.js** v18+
- **CMake** v3.15+ (for building the C++ plugin)
- **C++ Compiler**: Xcode Command Line Tools (macOS) or Visual Studio 2022 (Windows)
- **Adobe Illustrator** 2024+ (for running the plugin)
- **Adobe Illustrator SDK** (see below)

### 1. Clone and Install Dependencies

```bash
git clone https://github.com/your-org/nuxp.git
cd nuxp

# Install codegen dependencies
cd codegen && npm install && cd ..

# Install shell (frontend) dependencies
cd shell && npm install && cd ..
```

### 2. Run in Development Mode (No Illustrator Required)

The shell includes a mock bridge that simulates the C++ plugin, allowing frontend development without Illustrator:

```bash
cd shell
VITE_USE_MOCK=true npm run dev
```

Open http://localhost:5173 to see the debug panel and design system demo.

### 3. Build with Real Illustrator Integration

#### a. Download the Adobe Illustrator SDK

> **Note**: The SDK is proprietary and cannot be included in this repository. You must download it from Adobe (requires free Adobe account).

1. Go to **[Adobe Illustrator SDK Download](https://developer.adobe.com/console/servicesandapis/ai)**
2. Sign in with your Adobe ID
3. Download the **Illustrator 2026 SDK** (or matching your Illustrator version)
4. You'll get a `.dmg` file (macOS) or `.zip` file (Windows)

#### b. Setup the SDK

**macOS:**
```bash
# Run the setup script with your downloaded DMG
./scripts/setup-sdk.sh ~/Downloads/AI_2026_SDK_Mac.dmg
```

**Windows:**
```bash
# Extract the ZIP to plugin/sdk/ manually, then:
# Ensure these directories exist:
#   plugin/sdk/AIArt.h (and other headers)
#   plugin/sdk/SPFiles.h (PICA headers)
```

The setup script extracts and organizes:
- Illustrator API headers
- PICA/Sweet Pea headers (platform types)
- ATE (Adobe Text Engine) headers
- Creates `IllustratorSDK.h` convenience header

#### c. Generate SDK Wrappers (Optional)

```bash
./scripts/generate.sh
```

This parses SDK headers and generates:
- C++ endpoint handlers → `plugin/src/endpoints/generated/`
- TypeScript SDK client → `shell/src/sdk/generated/`

#### d. Build the C++ Plugin

```bash
cd plugin
cmake -B build
cmake --build build
```

**Customizing the Plugin Name:**

You can customize your plugin's identity when configuring CMake:

```bash
cmake -B build \
  -DPLUGIN_NAME="MyPlugin" \
  -DPLUGIN_DISPLAY_NAME="My Awesome Plugin" \
  -DPLUGIN_VERSION="1.0.0" \
  -DPLUGIN_BUNDLE_ID="com.mycompany.illustrator.myplugin"
```

Or use CMake presets (see `plugin/CMakePresets.json`):

```bash
cmake --preset custom-example
cmake --build build
```

| Variable | Default | Description |
|----------|---------|-------------|
| `PLUGIN_NAME` | `NUXPPlugin` | File name (no spaces) |
| `PLUGIN_DISPLAY_NAME` | `NUXP` | Name shown in Illustrator |
| `PLUGIN_VERSION` | `1.0.0` | Semantic version |
| `PLUGIN_BUNDLE_ID` | `com.nuxp.illustrator.plugin` | macOS bundle identifier |

#### e. Install the Plugin

```bash
cmake --install build
```

Or manually copy to your Illustrator plugins folder:
- **macOS**: `~/Library/Application Support/Adobe/Adobe Illustrator 2024/Plug-ins/`
- **Windows**: `C:\Program Files\Adobe\Adobe Illustrator 2024\Plug-ins\`

#### f. Run the Frontend

```bash
cd shell
npm run dev
```

With Illustrator running and the plugin loaded, the frontend will connect to the real SDK.

## Project Structure

```
nuxp/
├── plugin/                 # C++ Illustrator plugin
│   ├── CMakeLists.txt
│   ├── sdk/                # Adobe SDK headers (gitignored, you provide)
│   ├── lib/                # Third-party deps (httplib.h, json.hpp)
│   └── src/
│       ├── Plugin.cpp      # Entry point
│       ├── HttpServer.cpp  # Local HTTP server
│       ├── HandleManager.* # SDK object reference management
│       └── endpoints/      # HTTP endpoint handlers
│           ├── *.cpp       # Hand-written endpoints
│           └── generated/  # Auto-generated from codegen
│
├── shell/                  # Vue 3 frontend
│   ├── src/
│   │   ├── components/     # Vue components
│   │   ├── services/       # API client, MockBridge
│   │   ├── sdk/            # TypeScript SDK bindings
│   │   ├── stores/         # Pinia state management
│   │   └── views/          # Page components
│   └── package.json
│
├── codegen/                # SDK header parser & code generator
│   ├── src/
│   │   ├── parser/         # tree-sitter based header parser
│   │   ├── generator/      # C++ and TypeScript generators
│   │   └── config/         # Type mappings, skip lists
│   └── package.json
│
├── scripts/
│   └── generate.sh         # Run codegen and copy outputs
│
└── docs/
    └── getting-started.md  # Detailed setup guide
```

## Development Workflows

### Frontend Development (Mock Mode)

For rapid UI iteration without Illustrator:

```bash
cd shell
VITE_USE_MOCK=true npm run dev
```

The mock bridge provides simulated responses for all SDK calls.

### Full Stack Development

1. Start Illustrator with the plugin loaded
2. Run the frontend: `cd shell && npm run dev`
3. Changes to Vue components hot-reload automatically

### Extending the SDK

To add support for new Illustrator SDK features:

1. **Automatic** (recommended): Add headers to the SDK and run `./scripts/generate.sh`
2. **Manual**: Create custom endpoint handlers in `plugin/src/endpoints/`

## API Overview

The TypeScript SDK provides typed access to Illustrator functionality:

```typescript
import { sdk } from '@/sdk'

// Document operations
const doc = await sdk.document.getActive()
const layers = await sdk.layers.list(doc.id)

// Art manipulation
const art = await sdk.art.create({
  type: 'rectangle',
  bounds: { x: 0, y: 0, width: 100, height: 100 }
})

// Colors
await sdk.color.setFill(art.id, { r: 255, g: 0, b: 0 })
```

## Creating Your Own Plugin

NUXP is designed as a foundation for building your own Illustrator plugins. Here's how to customize it for your project:

### 1. Plugin Identity

Configure your plugin's name and branding when building:

```bash
cd plugin
cmake -B build \
  -DPLUGIN_NAME="AcmeTools" \
  -DPLUGIN_DISPLAY_NAME="Acme Design Tools" \
  -DPLUGIN_VERSION="1.0.0" \
  -DPLUGIN_BUNDLE_ID="com.acme.illustrator.tools" \
  -DPLUGIN_AUTHOR="Acme Inc." \
  -DPLUGIN_DESCRIPTION="Professional design tools for Illustrator"
cmake --build build
```

| Option | Description | Example |
|--------|-------------|---------|
| `PLUGIN_NAME` | Output filename (no spaces) | `AcmeTools` |
| `PLUGIN_DISPLAY_NAME` | Name in Illustrator UI | `Acme Design Tools` |
| `PLUGIN_VERSION` | Semantic version | `1.0.0` |
| `PLUGIN_BUNDLE_ID` | macOS identifier | `com.acme.illustrator.tools` |
| `PLUGIN_AUTHOR` | Your name/company | `Acme Inc.` |

For persistent configuration, create `plugin/CMakeUserPresets.json`:

```json
{
  "version": 6,
  "configurePresets": [{
    "name": "my-plugin",
    "inherits": "default",
    "cacheVariables": {
      "PLUGIN_NAME": "AcmeTools",
      "PLUGIN_DISPLAY_NAME": "Acme Design Tools",
      "PLUGIN_BUNDLE_ID": "com.acme.illustrator.tools"
    }
  }]
}
```

Then build with: `cmake --preset my-plugin && cmake --build build`

### 2. Frontend Branding

Update `shell/index.html` and `shell/src/App.vue` with your branding. The Tauri configuration in `shell/src-tauri/tauri.conf.json` controls the desktop app name and window title.

### 3. Adding Custom Endpoints

Create new SDK bindings in `plugin/src/endpoints/`:

```cpp
// plugin/src/endpoints/MyFeatureEndpoints.cpp
void RegisterMyFeatureEndpoints(httplib::Server& server) {
    server.Post("/api/my-feature/do-something", [](const auto& req, auto& res) {
        // Your SDK code here
        res.set_content("{\"success\": true}", "application/json");
    });
}
```

Register in `plugin/src/endpoints/RegisterAll.cpp` and add corresponding TypeScript types in `shell/src/sdk/`.

## Using NUXP as a Library

If you want to keep NUXP as a separate upstream dependency (rather than forking), you can link against NUXP's core infrastructure from your own project. This allows you to:

- Pull in NUXP updates without merge conflicts
- Keep your plugin code separate from NUXP infrastructure
- Contribute fixes back to NUXP easily

### Project Structure

```
your-workspace/
├── nuxp/                    # Clone of NUXP (upstream)
│   └── plugin/
└── your-plugin/             # Your project
    ├── CMakeLists.txt
    ├── plugin/
    │   └── src/
    │       ├── YourPlugin.cpp       # Your plugin entry point
    │       └── endpoints/           # Your endpoint handlers
    └── frontend/                    # Your frontend (optional)
```

### CMakeLists.txt Example

```cmake
cmake_minimum_required(VERSION 3.20)
project(YourPlugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include NUXP's core infrastructure
# This provides: HttpServer, SSE, SuitePointers, HandleManager, utils
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../nuxp/plugin nuxp-build)

# Your plugin sources
set(YOUR_SOURCES
    src/YourPlugin.cpp
    src/endpoints/FeatureOneEndpoints.cpp
    src/endpoints/FeatureTwoEndpoints.cpp
)

# Create your plugin
add_library(YourPlugin SHARED ${YOUR_SOURCES})

# Link NUXP core infrastructure
target_link_libraries(YourPlugin PRIVATE nuxp-core)

# Add YOUR Adobe SDK location (NUXP doesn't provide this)
set(AI_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sdk")
target_include_directories(YourPlugin PRIVATE
    ${AI_SDK_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Platform-specific settings (see NUXP's CMakeLists.txt for full example)
if(APPLE)
    set_target_properties(YourPlugin PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "aip"
        MACOSX_BUNDLE TRUE
    )
    target_compile_definitions(YourPlugin PRIVATE MAC_ENV)
    target_link_libraries(YourPlugin PRIVATE
        "-framework CoreFoundation"
        "-framework Cocoa"
    )
endif()
```

### What NUXP Core Provides

When you link against `nuxp-core`, you get:

| Component | Description |
|-----------|-------------|
| `HttpServer` | Background HTTP server with CORS support |
| `SSE` | Server-Sent Events for real-time push notifications |
| `MainThreadDispatch` | Safe SDK calls from HTTP thread |
| `SuitePointers` | Adobe SDK suite acquisition (12 essential suites) |
| `HandleManager` | Thread-safe handle lifecycle management |
| `StringUtils` | String conversion utilities |
| `ColorUtils` | Color manipulation helpers |
| `GeometryUtils` | Geometry and transform utilities |
| `LayerUtils` | Layer management helpers |
| `DocumentUtils` | Document operations |
| `SelectionUtils` | Selection handling |

### What You Provide

Your project must provide:

- **Adobe SDK headers** - Download from Adobe, add to your include paths
- **Plugin entry point** - Your own `Plugin.cpp` with `StartupPlugin()`, `ShutdownPlugin()`, etc.
- **Endpoint handlers** - Your feature-specific HTTP endpoints
- **Route registration** - Call your handlers from `HttpServer::ConfigureRoutes()`

### Example Plugin Entry Point

```cpp
// YourPlugin.cpp
#include "SuitePointers.hpp"
#include "HttpServer.hpp"
#include "SSE.hpp"

extern "C" SPBasicSuite* sSPBasic;

ASErr StartupPlugin(SPInterfaceMessage* message) {
    sSPBasic = message->d.basic;

    // Initialize NUXP infrastructure
    SuitePointers::Acquire();
    HttpServer::Start(8080);

    return kNoErr;
}

ASErr ShutdownPlugin(SPInterfaceMessage* message) {
    HttpServer::Stop();
    SuitePointers::Release();
    return kNoErr;
}

// ... other plugin callbacks
```

### Workflow

1. **Clone NUXP** alongside your project
2. **Create your CMakeLists.txt** referencing NUXP via `add_subdirectory()`
3. **Write your plugin code** using NUXP's infrastructure
4. **Find a bug in NUXP?** Fix it in the nuxp/ folder, commit, push upstream
5. **Pull NUXP updates** with `git pull` in the nuxp/ folder

## Building for Production

### Frontend

```bash
cd shell
npm run build
```

Output: `shell/dist/`

### Plugin

```bash
cd plugin
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## CI/CD

GitHub Actions workflows are included for:
- Building the C++ plugin (macOS + Windows)
- Building the Vue frontend
- Running codegen tests

Set the `ILLUSTRATOR_SDK_URL` secret to a URL where the SDK can be downloaded.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Run tests: `cd codegen && npm test`
4. Submit a pull request

## License

MIT

---

*NUXP is not affiliated with or endorsed by Adobe. Adobe, Illustrator, and related marks are trademarks of Adobe Inc.*
