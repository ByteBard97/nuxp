# NUXP Mac Build Support

This directory contains Xcode build configuration files for building Adobe Illustrator plugins on macOS.

## Why Xcode?

Adobe Illustrator plugins require a **PIPL (Plugin Property List)** resource that tells Illustrator how to load and identify the plugin. On macOS, this resource is compiled from `.r` (Rez) files using Apple's `Rez` compiler.

**Xcode handles PIPL compilation automatically**, while CMake requires manual configuration. For the most reliable Mac builds, we recommend using Xcode.

## Directory Structure

```
mac/
├── xcconfig/
│   ├── AIPluginCommon.xcconfig   # Common build settings
│   ├── AIPluginDebug.xcconfig    # Debug configuration
│   └── AIPluginRelease.xcconfig  # Release configuration
├── resources/
│   ├── PiPL.r                    # PIPL resource type definitions
│   ├── Plugin.r                  # Plugin PIPL resource template
│   └── Info.plist                # Bundle Info.plist template
└── README.md                     # This file
```

## Quick Start

### Option 1: Use with Existing Xcode Project

1. Copy the `xcconfig/` and `resources/` directories to your plugin directory
2. In Xcode, go to Project → Info → Configurations
3. Set Debug configuration to use `AIPluginDebug.xcconfig`
4. Set Release configuration to use `AIPluginRelease.xcconfig`

### Option 2: Create New Xcode Project

1. Create new Xcode project (macOS → Bundle)
2. Set bundle extension to `aip`
3. Add your source files and NUXP sources
4. Add the `.r` resource files to your project
5. Configure using the xcconfig files

## Creating Your Plugin's .r File

Each plugin needs its own resource file to define its PIPL. Create `YourPlugin.r`:

```c
//========================================================================================
// YourPlugin Resource File
//========================================================================================

// Define your plugin name BEFORE including Plugin.r
#define PIPL_PLUGIN_NAME "YourPluginName"

// Include the base PIPL template
#include "Plugin.r"
```

For HiDPI support, also create `YourPlugin2x.r`:

```c
//========================================================================================
// YourPlugin HiDPI Resource File
//========================================================================================

#define PIPL_PLUGIN_NAME "YourPluginName"

// HiDPI resource definitions go here
// (Can be empty if you don't have HiDPI icons)
```

## Build Settings

The xcconfig files set these key paths (customize as needed):

| Setting | Default | Description |
|---------|---------|-------------|
| `NUXP_SDK_PATH` | `$(PROJECT_DIR)/../sdk` | Path to Illustrator SDK headers |
| `NUXP_LIB_PATH` | `$(PROJECT_DIR)/../lib` | Path to third-party libs (httplib, json) |
| `NUXP_SRC_PATH` | `$(PROJECT_DIR)/../src` | Path to NUXP plugin sources |

Override in your project settings or environment:

```bash
# Example: Set via environment
export NUXP_SDK_PATH=/path/to/illustrator/sdk
```

## Troubleshooting

### Plugin Not Loading

1. **Check PIPL resource**: Use `DeRez` to verify PIPL was compiled:
   ```bash
   DeRez -only 'PiPL' YourPlugin.aip/Contents/Resources/plugin.pipl
   ```

2. **Check bundle structure**: Ensure your .aip contains:
   ```
   YourPlugin.aip/
   ├── Contents/
   │   ├── Info.plist
   │   ├── MacOS/
   │   │   └── YourPlugin       # Binary
   │   ├── PkgInfo              # Contains "BNDL????"
   │   └── Resources/
   │       └── pipl/
   │           └── plugin.pipl  # PIPL resource
   ```

3. **Check CFBundlePackageType**: Must be `BNDL` (not `APPL`)

### Rez Compilation Errors

- Ensure `.r` files have `#include "PiPL.r"`
- Define `PIPL_PLUGIN_NAME` before the include
- Check `REZ_SEARCH_PATHS` in xcconfig includes the path to `PiPL.r`

## CMake Alternative

If you prefer CMake, you can add Rez compilation:

```cmake
# Find Rez compiler
find_program(REZ_COMPILER Rez HINTS /usr/bin)

if(REZ_COMPILER AND APPLE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plugin.pipl
        COMMAND ${REZ_COMPILER}
            -d PIPL_PLUGIN_NAME=\\"${PLUGIN_NAME}\\"
            -I ${CMAKE_CURRENT_SOURCE_DIR}/mac/resources
            -o ${CMAKE_CURRENT_BINARY_DIR}/plugin.pipl
            ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Mac/${PLUGIN_NAME}.r
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Mac/${PLUGIN_NAME}.r
        COMMENT "Compiling PIPL resource"
    )
endif()
```

## References

- [Adobe Illustrator SDK Documentation](https://developer.adobe.com/illustrator/)
- [Apple Rez Resource Compiler](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/MachOTopics/1-Articles/building_files.html)
