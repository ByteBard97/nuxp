# NUXP Plugin - CMake Build Configuration
# Adobe Illustrator Plugin with HTTP/JSON Bridge
#
# Build instructions (Unix Makefiles - default):
#   cmake -B build
#   cmake --build build
#
# Build instructions (Xcode - recommended for macOS):
#   cmake -G Xcode -B build-xcode
#   cmake --build build-xcode --config Release
#
# Install to Illustrator Plug-ins folder:
#   cmake --install build

cmake_minimum_required(VERSION 3.20)

# -----------------------------------------------------------------------------
# Plugin Identity Configuration
# -----------------------------------------------------------------------------
# Users can customize these values when running cmake:
#   cmake -B build -DPLUGIN_NAME="MyPlugin" -DPLUGIN_DISPLAY_NAME="My Plugin"
#
# Or set them in a CMakeUserPresets.json or environment variables.

set(PLUGIN_NAME "NUXPPlugin" CACHE STRING "Plugin library/file name (no spaces)")
set(PLUGIN_DISPLAY_NAME "NUXP" CACHE STRING "Display name shown in Illustrator")
set(PLUGIN_VERSION "1.0.0" CACHE STRING "Plugin version (semver)")
set(PLUGIN_BUNDLE_ID "com.nuxp.illustrator.plugin" CACHE STRING "Bundle identifier (macOS)")
set(PLUGIN_AUTHOR "" CACHE STRING "Plugin author name")
set(PLUGIN_DESCRIPTION "Adobe Illustrator Plugin with HTTP/JSON Bridge" CACHE STRING "Plugin description")

# HTTP Server Configuration
set(NUXP_DEFAULT_PORT 8080 CACHE STRING "Default HTTP server port (1024-65535)")

project(${PLUGIN_NAME} VERSION ${PLUGIN_VERSION} LANGUAGES CXX)

# Require C++17 for modern features (std::optional, std::string_view, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Platform Detection
# -----------------------------------------------------------------------------

if(APPLE)
    set(PLATFORM_NAME "mac")
    set(PLUGIN_EXTENSION ".aip")
    message(STATUS "Building for macOS")
elseif(WIN32)
    set(PLATFORM_NAME "win")
    set(PLUGIN_EXTENSION ".aip")
    message(STATUS "Building for Windows")
else()
    message(FATAL_ERROR "Unsupported platform. NUXP only supports macOS and Windows.")
endif()

# -----------------------------------------------------------------------------
# Path Configuration
# -----------------------------------------------------------------------------

# Adobe Illustrator SDK path - contains SDK headers
set(AI_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sdk"
    CACHE PATH "Path to Adobe Illustrator SDK headers")

# Third-party library path - contains httplib.h, json.hpp, etc.
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    CACHE PATH "Path to third-party libraries")

# Verify SDK exists
if(NOT EXISTS "${AI_SDK_PATH}/AIArt.h")
    message(WARNING "Adobe Illustrator SDK not found at ${AI_SDK_PATH}")
    message(WARNING "Please ensure SDK headers are present in the sdk/ directory")
    message(WARNING "Build may fail without SDK headers")
endif()

# Verify lib directory exists
if(NOT EXISTS "${LIB_PATH}")
    message(WARNING "Third-party library directory not found at ${LIB_PATH}")
    message(WARNING "Please add httplib.h and nlohmann/json.hpp to lib/")
    file(MAKE_DIRECTORY "${LIB_PATH}")
endif()

# -----------------------------------------------------------------------------
# Source Files
# -----------------------------------------------------------------------------

# Core plugin infrastructure sources
set(CORE_SOURCES
    src/Plugin.cpp
    src/HttpServer.cpp
    src/HandleManager.cpp
    src/MainThreadDispatch.cpp
    src/MenuHandler.cpp
    src/SSE.cpp
    src/SuitePointers.cpp
    src/ConfigManager.cpp
    src/Errors.cpp
    src/utils/StringUtils.cpp
    src/utils/LayerUtils.cpp
    src/utils/GeometryUtils.cpp
    src/utils/ColorUtils.cpp
    src/utils/DocumentUtils.cpp
    src/utils/SelectionUtils.cpp
)

# SDK implementation files for ai:: classes
# These require global suite pointers that we define in SuitePointers.cpp:
#   sAIUnicodeString, sAIFilePath, sAIArtboard, sSPBlocks
# Note: AIAssert.cpp excluded - requires Objective-C; we provide our own stub
# Note: IAIAutoBuffer.cpp excluded - ai::SPAlloc symbols already in IAIUnicodeString.cpp
set(SDK_SOURCES
    sdk/IAIUnicodeString.cpp
    sdk/IAIFilePath.cpp
    sdk/IAIArtboards.cpp
)

# Core plugin headers
set(CORE_HEADERS
    src/Plugin.hpp
    src/HttpServer.hpp
    src/HandleManager.hpp
    src/HandleRegistry.hpp
    src/MainThreadDispatch.hpp
    src/MenuHandler.hpp
    src/SSE.hpp
    src/SuitePointers.hpp
    src/ConfigManager.hpp
    src/Errors.hpp
    src/utils/StringUtils.hpp
    src/utils/LayerUtils.hpp
    src/utils/GeometryUtils.hpp
    src/utils/ColorUtils.hpp
    src/utils/DocumentUtils.hpp
    src/utils/SelectionUtils.hpp
)

# Hand-written endpoint files (manually created endpoints)
file(GLOB HANDWRITTEN_ENDPOINT_SOURCES
    "src/endpoints/*.cpp"
)
file(GLOB HANDWRITTEN_ENDPOINT_HEADERS
    "src/endpoints/*.hpp"
    "src/endpoints/*.h"
)

# Generated endpoint files (from codegen)
# These are populated by running scripts/generate.sh
file(GLOB GENERATED_ENDPOINT_SOURCES
    "src/endpoints/generated/*.cpp"
)
file(GLOB GENERATED_ENDPOINT_HEADERS
    "src/endpoints/generated/*.hpp"
    "src/endpoints/generated/*.h"
)

# Include generated sources CMake file if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated/generated_sources.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated/generated_sources.cmake")
endif()

# Combine all sources
set(PLUGIN_SOURCES
    ${CORE_SOURCES}
    ${SDK_SOURCES}
    ${HANDWRITTEN_ENDPOINT_SOURCES}
    ${GENERATED_ENDPOINT_SOURCES}
)

set(PLUGIN_HEADERS
    ${CORE_HEADERS}
    ${HANDWRITTEN_ENDPOINT_HEADERS}
    ${GENERATED_ENDPOINT_HEADERS}
)

# -----------------------------------------------------------------------------
# Plugin Library Target
# -----------------------------------------------------------------------------
# Only build the standalone plugin when NUXP is the top-level project.
# When included as a subdirectory (e.g., by Flora), downstream projects
# should use the nuxp-core interface library instead.

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # We are the top-level project - build the full plugin
    # Use MODULE (not SHARED) for loadable bundles that are dynamically loaded at runtime

add_library(${PROJECT_NAME} MODULE
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

# Include directories
# SDK headers use SYSTEM to suppress warnings (Adobe SDK has Windows-1252 encoding issues)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${AI_SDK_PATH}
    ${LIB_PATH}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated
)

# Add nlohmann/json include path if using subdirectory structure
if(EXISTS "${LIB_PATH}/nlohmann")
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_PATH}/nlohmann)
endif()

endif() # CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR

# -----------------------------------------------------------------------------
# Interface Library for Downstream Projects
# -----------------------------------------------------------------------------
# This allows projects like Flora to use NUXP's core infrastructure
# without copying code. Usage:
#   add_subdirectory(path/to/nuxp/plugin nuxp-build)
#   target_link_libraries(MyPlugin PRIVATE nuxp-core)
#
# Note: Downstream projects must provide their own:
#   - Adobe SDK include paths
#   - Plugin entry points (Plugin.cpp equivalent)
#   - Endpoint handlers

add_library(nuxp-core INTERFACE)

# Collect generated endpoint sources
file(GLOB NUXP_GENERATED_ENDPOINTS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated/*.cpp"
)

target_sources(nuxp-core INTERFACE
    # Core infrastructure
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HttpServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainThreadDispatch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SSE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SuitePointers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HandleManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Errors.cpp
    # Utility functions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/StringUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/LayerUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/GeometryUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/ColorUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/DocumentUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/SelectionUtils.cpp
    # Endpoints
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/DemoEndpoints.cpp
    ${NUXP_GENERATED_ENDPOINTS}
)

# Third-party libs use SYSTEM to suppress warnings
target_include_directories(nuxp-core SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann")
    target_include_directories(nuxp-core SYSTEM INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann
    )
endif()

# NUXP source directories (our code - show warnings)
target_include_directories(nuxp-core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated
)

# -----------------------------------------------------------------------------
# Platform-Specific Configuration
# -----------------------------------------------------------------------------
# Only configure the plugin target when building standalone

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

if(APPLE)
    # -------------------------------------------------------------------------
    # macOS Bundle Configuration
    # -------------------------------------------------------------------------
    # Adobe Illustrator plugins are loadable bundles (.aip) with:
    # - Compiled PiPL (Plugin Property List) resource
    # - Info.plist with CFBundlePackageType = "BNDL"
    # - Universal binary (arm64 + x86_64)

    # Deployment target for modern macOS features
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version")

    # Build Universal Binary (Apple Silicon + Intel)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")

    # Bundle properties
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "aip"
        BUNDLE TRUE
        BUNDLE_EXTENSION "aip"
        MACOSX_BUNDLE TRUE
    )

    # Generate Info.plist from template with proper variable substitution
    # This is critical for Unix Makefiles which don't substitute $(PRODUCT_NAME) variables
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mac/resources/Info.plist.in")
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/mac/resources/Info.plist.in"
            "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
            @ONLY
        )
        set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
        )
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mac/resources/Info.plist")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/mac/resources/Info.plist"
        )
    endif()

    # -------------------------------------------------------------------------
    # Xcode Generator Attributes
    # -------------------------------------------------------------------------
    # These only take effect when using: cmake -G Xcode
    set_target_properties(${PROJECT_NAME} PROPERTIES
        # Universal Binary
        XCODE_ATTRIBUTE_ARCHS "arm64 x86_64"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
        # Code Signing (use "-" for ad-hoc, or "Apple Development" for dev cert)
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
        # Other settings
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${PLUGIN_BUNDLE_ID}"
        XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleDisplayName "${PLUGIN_DISPLAY_NAME}"
    )

    # -------------------------------------------------------------------------
    # PiPL Generation (New Python-based method)
    # -------------------------------------------------------------------------
    # The PiPL (Plugin Property List) tells Illustrator how to load the plugin.
    # Since Illustrator 2022, Adobe recommends using create_pipl.py instead of Rez.
    # The generated plugin.pipl goes in Contents/Resources/pipl/

    find_program(PYTHON_EXECUTABLE python3
        HINTS /opt/homebrew/bin /usr/local/bin /usr/bin
    )

    # Path to Adobe's PIPL tool (check project first, then SDK)
    set(PIPL_TOOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tools/pipl")
    if(NOT EXISTS "${PIPL_TOOL_DIR}/create_pipl.py")
        # Fallback: check in SDK installation
        set(PIPL_TOOL_DIR "${AI_SDK_PATH}/../tools/pipl")
    endif()

    if(PYTHON_EXECUTABLE AND EXISTS "${PIPL_TOOL_DIR}/create_pipl.py")
        message(STATUS "Found Python: ${PYTHON_EXECUTABLE}")
        message(STATUS "PiPL tool dir: ${PIPL_TOOL_DIR}")

        # Generate plugin.pipl in build directory (pre-build)
        set(PIPL_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/plugin.pipl")
        add_custom_command(
            OUTPUT "${PIPL_OUTPUT}"
            COMMAND ${PYTHON_EXECUTABLE} "${PIPL_TOOL_DIR}/create_pipl.py"
                -input "[{\"name\":\"${PLUGIN_NAME}\", \"entry_point\":\"PluginMain\"}]"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Generating PiPL with Python tool"
            VERBATIM
        )

        # Add as dependency to ensure it's generated before build
        add_custom_target(generate_pipl DEPENDS "${PIPL_OUTPUT}")
        add_dependencies(${PROJECT_NAME} generate_pipl)

        # Copy plugin.pipl to bundle after build
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}>/Resources/pipl"
            COMMAND ${CMAKE_COMMAND} -E copy
                "${PIPL_OUTPUT}"
                "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}>/Resources/pipl/plugin.pipl"
            COMMENT "Copying plugin.pipl to bundle"
        )
    else()
        # Fallback to old Rez method if Python tool not available
        find_program(REZ_EXECUTABLE Rez
            HINTS /usr/bin /Applications/Xcode.app/Contents/Developer/usr/bin
        )
        set(PIPL_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mac/resources")

        if(REZ_EXECUTABLE AND EXISTS "${PIPL_RESOURCES_DIR}/Plugin.r")
            message(STATUS "Using legacy Rez method for PiPL")
            message(STATUS "Found Rez compiler: ${REZ_EXECUTABLE}")

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}>/Resources"
                COMMAND ${REZ_EXECUTABLE}
                    "-d" "PIPL_PLUGIN_NAME=\"${PLUGIN_NAME}\""
                    "-d" "TargetOS_Mac=1"
                    "-i" "${PIPL_RESOURCES_DIR}"
                    "-i" "${AI_SDK_PATH}"
                    "-useDF"
                    "-o" "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}>/Resources/${PROJECT_NAME}.rsrc"
                    "${PIPL_RESOURCES_DIR}/Plugin.r"
                COMMENT "Compiling PiPL resource with Rez (legacy)"
                VERBATIM
            )
        else()
            message(WARNING "Neither Python PIPL tool nor Rez compiler found")
            message(WARNING "PiPL resource is required for Illustrator to load the plugin")
            message(WARNING "Copy Adobe SDK tools/pipl directory to ${CMAKE_CURRENT_SOURCE_DIR}/tools/")
        endif()
    endif()

    # -------------------------------------------------------------------------
    # macOS Compile Definitions
    # -------------------------------------------------------------------------
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        MAC_ENV
        __MACH__
        ILLUSTRATOR_PLUGIN
    )

    # Link macOS frameworks
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework CoreFoundation"
        "-framework CoreServices"
        "-framework ApplicationServices"
        "-framework Cocoa"
    )

elseif(WIN32)
    # Windows Configuration
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".aip"
        PREFIX ""
    )

    # Windows compile definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN_ENV
        _WINDOWS
        WIN32
        _WIN32
        ILLUSTRATOR_PLUGIN
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )

    # Link Windows libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32  # Winsock for HTTP server
        user32
        gdi32
    )
endif()

# Common compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NUXP_VERSION="${PROJECT_VERSION}"
    NUXP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    NUXP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    NUXP_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    NUXP_DEFAULT_PORT=${NUXP_DEFAULT_PORT}
    PLUGIN_NAME="${PLUGIN_NAME}"
    PLUGIN_DISPLAY_NAME="${PLUGIN_DISPLAY_NAME}"
)

# -----------------------------------------------------------------------------
# Compiler Warnings and Options
# -----------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -Wno-missing-field-initializers
        -Wno-unknown-pragmas
        # Suppress warnings from Adobe SDK (Windows-1252 encoding, ObjC #import)
        -Wno-invalid-utf8
        -Wno-import-preprocessor-directive-pedantic
    )

    # Enable debug symbols in debug builds
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2>
    )

elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W3
        /wd4100  # unreferenced formal parameter
        /wd4201  # nonstandard extension: nameless struct/union
        /wd4244  # conversion from 'type1' to 'type2', possible loss of data
        /wd4267  # conversion from 'size_t' to 'type', possible loss of data
        /wd4996  # deprecated function
        /EHsc    # Enable C++ exceptions
        /utf-8   # Source and execution character sets are UTF-8
    )

    # Enable debug symbols
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Zi /Od>
        $<$<CONFIG:Release>:/O2>
    )
endif()

# -----------------------------------------------------------------------------
# Installation
# -----------------------------------------------------------------------------

# Default installation paths for Adobe Illustrator plugins
# Checks for 2026, 2025, 2024 in that order
if(APPLE)
    # macOS: User's Plug-ins folder (doesn't require sudo)
    set(AI_PLUGINS_DEFAULT_PATH "")
    foreach(YEAR 2026 2025 2024)
        set(CHECK_PATH "$ENV{HOME}/Library/Application Support/Adobe/Adobe Illustrator ${YEAR}/Plug-ins")
        if(EXISTS "${CHECK_PATH}" AND AI_PLUGINS_DEFAULT_PATH STREQUAL "")
            set(AI_PLUGINS_DEFAULT_PATH "${CHECK_PATH}")
        endif()
    endforeach()
    # Fallback to 2026 even if it doesn't exist yet
    if(AI_PLUGINS_DEFAULT_PATH STREQUAL "")
        set(AI_PLUGINS_DEFAULT_PATH "$ENV{HOME}/Library/Application Support/Adobe/Adobe Illustrator 2026/Plug-ins")
    endif()
elseif(WIN32)
    # Windows: Program Files Plug-ins folder
    set(AI_PLUGINS_DEFAULT_PATH "")
    foreach(YEAR 2026 2025 2024)
        set(CHECK_PATH "C:/Program Files/Adobe/Adobe Illustrator ${YEAR}/Plug-ins")
        if(EXISTS "${CHECK_PATH}" AND AI_PLUGINS_DEFAULT_PATH STREQUAL "")
            set(AI_PLUGINS_DEFAULT_PATH "${CHECK_PATH}")
        endif()
    endforeach()
    if(AI_PLUGINS_DEFAULT_PATH STREQUAL "")
        set(AI_PLUGINS_DEFAULT_PATH "C:/Program Files/Adobe/Adobe Illustrator 2026/Plug-ins")
    endif()
endif()

set(AI_PLUGINS_PATH "${AI_PLUGINS_DEFAULT_PATH}"
    CACHE PATH "Adobe Illustrator Plug-ins folder")

# Install target
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION "${AI_PLUGINS_PATH}"
    LIBRARY DESTINATION "${AI_PLUGINS_PATH}"
    RUNTIME DESTINATION "${AI_PLUGINS_PATH}"
)

# macOS: Remove quarantine attribute after install (prevents "damaged" errors)
if(APPLE)
    install(CODE "
        execute_process(
            COMMAND xattr -cr \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.aip\"
            ERROR_QUIET
        )
        message(STATUS \"Removed quarantine attributes from ${PROJECT_NAME}.aip\")
    ")
endif()

# -----------------------------------------------------------------------------
# Custom Targets
# -----------------------------------------------------------------------------

# Target to regenerate code from SDK headers
add_custom_target(generate
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/generate.sh"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
    COMMENT "Regenerating C++ and TypeScript code from SDK headers"
    VERBATIM
)

# Target to clean generated files
add_custom_target(clean-generated
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated"
    COMMENT "Cleaning generated endpoint files"
    VERBATIM
)

# Target to rebuild with fresh generated code
add_custom_target(regenerate
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean-generated
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target generate
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Regenerating code and rebuilding plugin"
    VERBATIM
)

endif() # CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR (standalone plugin)

# -----------------------------------------------------------------------------
# Generate TypeScript Defaults
# -----------------------------------------------------------------------------
# This generates shell/src/sdk/defaults.ts from the template with CMake values
# So that TypeScript uses the same defaults as C++

set(SHELL_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../shell/src/sdk")
if(EXISTS "${SHELL_SDK_DIR}/defaults.ts.in")
    configure_file(
        "${SHELL_SDK_DIR}/defaults.ts.in"
        "${SHELL_SDK_DIR}/defaults.ts"
        @ONLY
    )
    message(STATUS "Generated TypeScript defaults: ${SHELL_SDK_DIR}/defaults.ts")
endif()

# -----------------------------------------------------------------------------
# Configuration Summary
# -----------------------------------------------------------------------------

message(STATUS "")
message(STATUS "========================================")
message(STATUS "NUXP Plugin Configuration Summary")
message(STATUS "========================================")
message(STATUS "Plugin Identity:")
message(STATUS "  Name:             ${PLUGIN_NAME}")
message(STATUS "  Display Name:     ${PLUGIN_DISPLAY_NAME}")
message(STATUS "  Version:          ${PROJECT_VERSION}")
message(STATUS "  Bundle ID:        ${PLUGIN_BUNDLE_ID}")
message(STATUS "")
message(STATUS "Build Settings:")
message(STATUS "  Platform:         ${PLATFORM_NAME}")
message(STATUS "  C++ Standard:     C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Default Port:     ${NUXP_DEFAULT_PORT}")
message(STATUS "  SDK Path:         ${AI_SDK_PATH}")
message(STATUS "  Library Path:     ${LIB_PATH}")
message(STATUS "  Install Path:     ${AI_PLUGINS_PATH}")
if(APPLE)
message(STATUS "")
message(STATUS "macOS Settings:")
message(STATUS "  Architectures:    ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "  Deployment Target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
if(REZ_EXECUTABLE)
message(STATUS "  Rez Compiler:     ${REZ_EXECUTABLE}")
else()
message(STATUS "  Rez Compiler:     NOT FOUND (PiPL will not be compiled)")
endif()
endif()
message(STATUS "")
message(STATUS "Source files:")
list(LENGTH CORE_SOURCES CORE_COUNT)
message(STATUS "  Core sources:          ${CORE_COUNT} files")
list(LENGTH HANDWRITTEN_ENDPOINT_SOURCES HANDWRITTEN_COUNT)
message(STATUS "  Handwritten endpoints: ${HANDWRITTEN_COUNT} files")
list(LENGTH GENERATED_ENDPOINT_SOURCES GENERATED_COUNT)
message(STATUS "  Generated endpoints:   ${GENERATED_COUNT} files")
message(STATUS "")
message(STATUS "Build commands:")
if(APPLE)
message(STATUS "  # Recommended: Use Xcode generator for proper bundle/signing")
message(STATUS "  cmake -G Xcode -B build-xcode")
message(STATUS "  cmake --build build-xcode --config Release")
message(STATUS "")
message(STATUS "  # Alternative: Unix Makefiles (simpler, but no code signing)")
endif()
message(STATUS "  cmake -B build")
message(STATUS "  cmake --build build")
message(STATUS "  cmake --install build          # Install to Illustrator")
message(STATUS "========================================")
message(STATUS "")
