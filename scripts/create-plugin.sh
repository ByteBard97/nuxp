#!/bin/bash
# create-plugin.sh — Scaffold a new NUXP-based Illustrator plugin
#
# Usage:
#   ./nuxp/scripts/create-plugin.sh MyPlugin "My Plugin Display Name" /path/to/output
#
# Creates a complete, buildable Illustrator plugin project with:
#   - CMakeLists.txt (configured for NUXP integration)
#   - Plugin source files (header, implementation, suites)
#   - Info.plist (macOS bundle metadata)
#   - plugin.pipl (binary PiPL resource)
#   - .rsrc file (empty resource fork — required by Illustrator)
#   - IDToFile.txt (resource ID mapping)
#   - Build script
#
# The generated plugin loads in Illustrator out of the box.
# Add NUXP HTTP/SSE capabilities by uncommenting the NUXP sections.

set -e

# --- Argument parsing ---
PLUGIN_NAME="${1:?Usage: create-plugin.sh <PluginName> [DisplayName] [OutputDir]}"
DISPLAY_NAME="${2:-$PLUGIN_NAME}"
OUTPUT_DIR="${3:-./$PLUGIN_NAME}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NUXP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m'

# Validate plugin name (no spaces, alphanumeric + underscore)
if [[ ! "$PLUGIN_NAME" =~ ^[a-zA-Z][a-zA-Z0-9_]*$ ]]; then
    echo -e "${RED}ERROR: Plugin name must be alphanumeric (no spaces). Got: '$PLUGIN_NAME'${NC}"
    exit 1
fi

if [ -d "$OUTPUT_DIR" ]; then
    echo -e "${RED}ERROR: Directory already exists: $OUTPUT_DIR${NC}"
    exit 1
fi

# Compute uppercase name for include guard (macOS bash 3.x lacks ${VAR^^})
PLUGIN_NAME_UPPER=$(echo "$PLUGIN_NAME" | tr '[:lower:]' '[:upper:]')
PLUGIN_NAME_LOWER=$(echo "$PLUGIN_NAME" | tr '[:upper:]' '[:lower:]')

echo -e "${CYAN}Creating plugin: $PLUGIN_NAME ($DISPLAY_NAME)${NC}"
echo -e "${CYAN}Output: $OUTPUT_DIR${NC}"
echo ""

# --- Create directory structure ---
mkdir -p "$OUTPUT_DIR/Source"

# --- Generate Header ---
cat > "$OUTPUT_DIR/Source/${PLUGIN_NAME}Plugin.h" << HEADER_EOF
#ifndef __${PLUGIN_NAME_UPPER}PLUGIN_H__
#define __${PLUGIN_NAME_UPPER}PLUGIN_H__

#include "Plugin.hpp"
#include "SDKAboutPluginsHelper.h"
#include "SDKDef.h"
#include "VTableSupport.hpp"

#define k${PLUGIN_NAME}PluginName "${PLUGIN_NAME}"

Plugin *AllocatePlugin(SPPluginRef pluginRef);
void FixupReload(Plugin *plugin);

class ${PLUGIN_NAME}Plugin : public Plugin {
public:
    ${PLUGIN_NAME}Plugin(SPPluginRef pluginRef);
    virtual ~${PLUGIN_NAME}Plugin() {}

    FIXUP_VTABLE_EX(${PLUGIN_NAME}Plugin, Plugin);

protected:
    ASErr StartupPlugin(SPInterfaceMessage *message) override;
    ASErr PostStartupPlugin() override;
    ASErr ShutdownPlugin(SPInterfaceMessage *message) override;
    ASErr GoMenuItem(AIMenuMessage *message) override;
    ASErr GoTimer(AITimerMessage *message) override;
    ASErr Notify(AINotifierMessage *message) override;

private:
    AIMenuItemHandle fAboutMenuItem;
};

#endif
HEADER_EOF

# --- Generate Implementation ---
cat > "$OUTPUT_DIR/Source/${PLUGIN_NAME}Plugin.cpp" << IMPL_EOF
#include "${PLUGIN_NAME}Plugin.h"
#include "IllustratorSDK.h"
#include "SDKErrors.h"

${PLUGIN_NAME}Plugin *gPlugin = NULL;

Plugin *AllocatePlugin(SPPluginRef pluginRef) {
    return new ${PLUGIN_NAME}Plugin(pluginRef);
}

void FixupReload(Plugin *plugin) {
    ${PLUGIN_NAME}Plugin::FixupVTable(static_cast<${PLUGIN_NAME}Plugin *>(plugin));
}

${PLUGIN_NAME}Plugin::${PLUGIN_NAME}Plugin(SPPluginRef pluginRef)
    : Plugin(pluginRef),
      fAboutMenuItem(NULL) {
    strncpy(fPluginName, k${PLUGIN_NAME}PluginName, kMaxStringLength);
}

ASErr ${PLUGIN_NAME}Plugin::StartupPlugin(SPInterfaceMessage *message) {
    ASErr error = Plugin::StartupPlugin(message);
    if (error) return error;

    SDKAboutPluginsHelper aboutHelper;
    error = aboutHelper.AddAboutPluginsMenuItem(
        message,
        kSDKDefAboutSDKCompanyPluginsGroupName,
        ai::UnicodeString(kSDKDefAboutSDKCompanyPluginsGroupNameString),
        "${DISPLAY_NAME}...",
        &fAboutMenuItem
    );

    return kNoErr;
}

ASErr ${PLUGIN_NAME}Plugin::PostStartupPlugin() {
    return kNoErr;
}

ASErr ${PLUGIN_NAME}Plugin::ShutdownPlugin(SPInterfaceMessage *message) {
    return Plugin::ShutdownPlugin(message);
}

ASErr ${PLUGIN_NAME}Plugin::GoMenuItem(AIMenuMessage *message) {
    if (message->menuItem == fAboutMenuItem) {
        SDKAboutPluginsHelper aboutHelper;
        aboutHelper.PopAboutBox(message, "${DISPLAY_NAME}",
            "${DISPLAY_NAME} - Illustrator Plugin");
    }
    return kNoErr;
}

ASErr ${PLUGIN_NAME}Plugin::GoTimer(AITimerMessage *message) {
    return kNoErr;
}

ASErr ${PLUGIN_NAME}Plugin::Notify(AINotifierMessage *message) {
    return kNoErr;
}
IMPL_EOF

# --- Generate Suites ---
cat > "$OUTPUT_DIR/Source/${PLUGIN_NAME}Suites.cpp" << SUITES_EOF
#include "IllustratorSDK.h"
#include "Suites.hpp"

AIUnicodeStringSuite *sAIUnicodeString = nullptr;
SPBlocksSuite *sSPBlocks = nullptr;
AIFilePathSuite *sAIFilePath = nullptr;

ImportSuite gImportSuites[] = {
    {kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks},
    {kAIUnicodeStringSuite, kAIUnicodeStringSuiteVersion, &sAIUnicodeString},
    {kAIFilePathSuite, kAIFilePathSuiteVersion, &sAIFilePath},
    {nullptr, 0, nullptr}
};
SUITES_EOF

# --- Generate Info.plist ---
cat > "$OUTPUT_DIR/Info.plist" << PLIST_EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>English</string>
	<key>CFBundleDisplayName</key>
	<string>${DISPLAY_NAME}</string>
	<key>CFBundleExecutable</key>
	<string>${PLUGIN_NAME}</string>
	<key>CFBundleGetInfoHTML</key>
	<string>${DISPLAY_NAME} version 1.0.0</string>
	<key>CFBundleGetInfoString</key>
	<string>${DISPLAY_NAME} version 1.0.0</string>
	<key>CFBundleIdentifier</key>
	<string>com.nuxp.illustrator.${PLUGIN_NAME_LOWER}</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>${PLUGIN_NAME}</string>
	<key>CFBundlePackageType</key>
	<string>ARPI</string>
	<key>CFBundleShortVersionString</key>
	<string>1.0.0</string>
	<key>CFBundleSignature</key>
	<string>ART5</string>
	<key>CFBundleVersion</key>
	<string>1.0.0</string>
	<key>CSResourcesFileMapped</key>
	<true/>
	<key>LSRequiresCarbon</key>
	<true/>
</dict>
</plist>
PLIST_EOF

# --- Generate PiPL (binary) ---
# Uses NUXP's create_pipl.py if available, otherwise generates directly
PIPL_TOOL="$NUXP_ROOT/plugin/tools/pipl/create_pipl.py"
if [ -f "$PIPL_TOOL" ] && command -v python3 &>/dev/null; then
    cd "$OUTPUT_DIR"
    python3 "$PIPL_TOOL" -input "[{\"name\":\"${PLUGIN_NAME}\", \"entry_point\":\"PluginMain\"}]"
    cd - > /dev/null
else
    # Generate PiPL with inline Python
    python3 -c "
import struct, sys
name = '${PLUGIN_NAME}'
name_bytes = name.encode('ascii') + b'\x00'
while len(name_bytes) % 4 != 0:
    name_bytes += b'\x00'
props = []
props.append(b'ADBE' + b'kind' + struct.pack('>II', 0, 4) + b'SPEA')
props.append(b'ADBE' + b'ivrs' + struct.pack('>II', 0, 4) + struct.pack('>I', 2))
props.append(b'ADBE' + b'mi32' + struct.pack('>II', 0, 0))
props.append(b'ADBE' + b'pinm' + struct.pack('>II', 0, len(name_bytes)) + name_bytes)
header = struct.pack('>III', 1, 0, len(props))
with open('plugin.pipl', 'wb') as f:
    f.write(header + b''.join(props))
" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo -e "${RED}WARNING: Could not generate PiPL. Copy one from an existing plugin.${NC}"
    fi
fi

# --- Copy .rsrc template ---
RSRC_TEMPLATE="$NUXP_ROOT/plugin/resources/empty.rsrc"
if [ -f "$RSRC_TEMPLATE" ]; then
    cp "$RSRC_TEMPLATE" "$OUTPUT_DIR/${PLUGIN_NAME}.rsrc"
else
    # Generate empty resource fork
    python3 -c "
import struct, sys
hdr = struct.pack('>IIII', 0x100, 0x100, 0, 0x1e)
data = hdr + b'\x00' * 240
map_hdr = hdr + b'\x00' * 4 + struct.pack('>HH', 0x1c, 0x1e) + b'\xff\xff'
open(sys.argv[1], 'wb').write(data + map_hdr)
" "$OUTPUT_DIR/${PLUGIN_NAME}.rsrc"
fi

# --- Create IDToFile.txt ---
printf '\n' > "$OUTPUT_DIR/IDToFile.txt"

# --- Generate CMakeLists.txt ---
cat > "$OUTPUT_DIR/CMakeLists.txt" << 'CMAKE_HEADER'
cmake_minimum_required(VERSION 3.20)

# CRITICAL: Set deployment target BEFORE project() or Xcode ignores it
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS version")
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build universal binary")

CMAKE_HEADER

cat >> "$OUTPUT_DIR/CMakeLists.txt" << CMAKE_EOF
project(${PLUGIN_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SDK Paths ---
# Adjust these paths to point to your Illustrator SDK installation
set(AI_SDK_PATH "\${CMAKE_CURRENT_SOURCE_DIR}/../illustratorapi"
    CACHE PATH "Path to Illustrator SDK (illustratorapi directory)")
set(COMMON_PATH "\${CMAKE_CURRENT_SOURCE_DIR}/../common"
    CACHE PATH "Path to common SDK helpers")

if(NOT EXISTS "\${AI_SDK_PATH}")
    message(FATAL_ERROR "Illustrator SDK not found at \${AI_SDK_PATH}. "
        "Set AI_SDK_PATH to your illustratorapi directory.")
endif()

# --- Plugin Sources ---
set(PLUGIN_SOURCES
    Source/${PLUGIN_NAME}Plugin.cpp
    Source/${PLUGIN_NAME}Suites.cpp
)

# SDK common sources (required for Plugin class hierarchy)
set(SDK_COMMON_SOURCES
    \${COMMON_PATH}/source/AppContext.cpp
    \${COMMON_PATH}/source/Plugin.cpp
    \${COMMON_PATH}/source/Suites.cpp
    \${COMMON_PATH}/source/IllustratorSDK.cpp
    \${COMMON_PATH}/source/Main.cpp
    \${COMMON_PATH}/source/SDKErrors.cpp
    \${COMMON_PATH}/source/SDKAboutPluginsHelper.cpp
)

# SDK implementation files
set(SDK_IMPL_SOURCES
    \${AI_SDK_PATH}/illustrator/IAIUnicodeString.cpp
    \${AI_SDK_PATH}/illustrator/IAIFilePath.cpp
)

add_library(\${PROJECT_NAME} MODULE
    \${PLUGIN_SOURCES}
    \${SDK_COMMON_SOURCES}
    \${SDK_IMPL_SOURCES}
)

target_include_directories(\${PROJECT_NAME} PRIVATE
    \${CMAKE_CURRENT_SOURCE_DIR}/Source
    \${COMMON_PATH}/includes
    \${COMMON_PATH}/includes/legacy
)

# ATE (Adobe Text Engine) headers — may be in a different location than the main SDK
set(ATE_PATH "\${AI_SDK_PATH}/../ate" CACHE PATH "Path to ATE headers (ATETypesDef.h)")
if(NOT EXISTS "\${ATE_PATH}/ATETypesDef.h")
    # Try common alternative locations
    foreach(ATE_CANDIDATE
        "\${AI_SDK_PATH}/ate"
        "\${AI_SDK_PATH}/../illustratorapi/ate"
        "\${AI_SDK_PATH}/../../ate"
    )
        if(EXISTS "\${ATE_CANDIDATE}/ATETypesDef.h")
            set(ATE_PATH "\${ATE_CANDIDATE}")
            break()
        endif()
    endforeach()
    if(NOT EXISTS "\${ATE_PATH}/ATETypesDef.h")
        message(WARNING "ATETypesDef.h not found. Set ATE_PATH to the directory containing it.")
    endif()
endif()

target_include_directories(\${PROJECT_NAME} SYSTEM PRIVATE
    \${AI_SDK_PATH}
    \${AI_SDK_PATH}/illustrator
    \${AI_SDK_PATH}/pica_sp
    \${ATE_PATH}
)

target_compile_definitions(\${PROJECT_NAME} PRIVATE
    MAC_ENV
    __MACH__
    ILLUSTRATOR_PLUGIN
)

if(APPLE)
    set_target_properties(\${PROJECT_NAME} PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "aip"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.nuxp.illustrator.${PLUGIN_NAME_LOWER}"
        MACOSX_BUNDLE_BUNDLE_NAME "\${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "\${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "\${PROJECT_VERSION}"
        MACOSX_BUNDLE_INFO_PLIST "\${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )

    set_target_properties(\${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_ARCHS "arm64 x86_64"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
        XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17"
        XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
        XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES
        XCODE_ATTRIBUTE_WRAPPER_EXTENSION "aip"
    )

    target_link_libraries(\${PROJECT_NAME} PRIVATE
        "-framework CoreFoundation"
        "-framework CoreServices"
        "-framework ApplicationServices"
        "-framework Cocoa"
    )

    # --- Bundle Resources (ALL REQUIRED by Illustrator) ---

    # PiPL — Plugin Property List (tells Illustrator how to load the plugin)
    if(EXISTS "\${CMAKE_CURRENT_SOURCE_DIR}/plugin.pipl")
        add_custom_command(TARGET \${PROJECT_NAME} POST_BUILD
            COMMAND \${CMAKE_COMMAND} -E make_directory
                "\$<TARGET_BUNDLE_CONTENT_DIR:\${PROJECT_NAME}>/Resources/pipl"
            COMMAND \${CMAKE_COMMAND} -E copy
                "\${CMAKE_CURRENT_SOURCE_DIR}/plugin.pipl"
                "\$<TARGET_BUNDLE_CONTENT_DIR:\${PROJECT_NAME}>/Resources/pipl/plugin.pipl"
            COMMENT "Copying plugin.pipl to bundle"
        )
    else()
        message(FATAL_ERROR "plugin.pipl not found. Run the NUXP create_pipl.py tool.")
    endif()

    # .rsrc — Empty resource fork (Illustrator REQUIRES this file to load the plugin)
    add_custom_command(TARGET \${PROJECT_NAME} POST_BUILD
        COMMAND \${CMAKE_COMMAND} -E copy
            "\${CMAKE_CURRENT_SOURCE_DIR}/\${PROJECT_NAME}.rsrc"
            "\$<TARGET_BUNDLE_CONTENT_DIR:\${PROJECT_NAME}>/Resources/\${PROJECT_NAME}.rsrc"
        COMMENT "Copying .rsrc to bundle (required by Illustrator)"
    )

    # IDToFile.txt — Resource ID mapping
    add_custom_command(TARGET \${PROJECT_NAME} POST_BUILD
        COMMAND \${CMAKE_COMMAND} -E make_directory
            "\$<TARGET_BUNDLE_CONTENT_DIR:\${PROJECT_NAME}>/Resources/txt"
        COMMAND \${CMAKE_COMMAND} -E copy
            "\${CMAKE_CURRENT_SOURCE_DIR}/IDToFile.txt"
            "\$<TARGET_BUNDLE_CONTENT_DIR:\${PROJECT_NAME}>/Resources/txt/IDToFile.txt"
        COMMENT "Copying IDToFile.txt to bundle"
    )

    # Strip extended attributes
    add_custom_command(TARGET \${PROJECT_NAME} POST_BUILD
        COMMAND xattr -cr "\$<TARGET_BUNDLE_DIR:\${PROJECT_NAME}>"
        COMMENT "Stripping extended attributes"
    )
endif()

target_compile_options(\${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
)
CMAKE_EOF

# --- Generate build script ---
cat > "$OUTPUT_DIR/build.sh" << 'BUILD_EOF'
#!/bin/bash
# Build script for the plugin
set -e
cd "$(dirname "${BASH_SOURCE[0]}")"

BUILD_DIR="build-xcode"
CONFIG="${1:-Debug}"

if [ ! -d "$BUILD_DIR" ]; then
    echo "=== Configuring (Xcode) ==="
    cmake -G Xcode -B "$BUILD_DIR"
fi

echo "=== Building $CONFIG ==="
cmake --build "$BUILD_DIR" --config "$CONFIG"

echo ""
echo "=== Built ==="
PLUGIN=$(find "$BUILD_DIR/$CONFIG" -name "*.aip" -type d | head -1)
echo "Plugin: $PLUGIN"
echo ""
echo "Deploy with:"
echo "  cp -R '$PLUGIN' /path/to/addl-plugins-folder/"
echo "  xattr -cr /path/to/addl-plugins-folder/$(basename "$PLUGIN")"
echo "  codesign --force --deep --sign - -o runtime /path/to/addl-plugins-folder/$(basename "$PLUGIN")"
BUILD_EOF
chmod +x "$OUTPUT_DIR/build.sh"

echo ""
echo -e "${GREEN}Plugin scaffolded successfully!${NC}"
echo ""
echo "  Directory: $OUTPUT_DIR"
echo "  Files:"
find "$OUTPUT_DIR" -type f | sort | while read f; do
    echo "    $(echo "$f" | sed "s|$OUTPUT_DIR/||")"
done
echo ""
echo -e "${CYAN}Build:${NC}"
echo "  cd $OUTPUT_DIR"
echo "  ./build.sh          # Debug build"
echo "  ./build.sh Release  # Release build"
echo ""
echo -e "${CYAN}Deploy:${NC}"
echo "  cp -R $OUTPUT_DIR/build-xcode/Debug/${PLUGIN_NAME}.aip /path/to/addl-plugins-folder/"
echo ""
echo -e "${CYAN}NOTE: Set AI_SDK_PATH in CMakeLists.txt to your Illustrator SDK location${NC}"
