# NUXP Plugin - CMake Build Configuration
# Adobe Illustrator Plugin with HTTP/JSON Bridge
#
# Build instructions:
#   cmake -B build
#   cmake --build build
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

add_library(${PROJECT_NAME} SHARED
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${AI_SDK_PATH}
    ${LIB_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints
    ${CMAKE_CURRENT_SOURCE_DIR}/src/endpoints/generated
)

# Add nlohmann/json include path if using subdirectory structure
if(EXISTS "${LIB_PATH}/nlohmann")
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_PATH}/nlohmann)
endif()

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

target_sources(nuxp-core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HttpServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainThreadDispatch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SSE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SuitePointers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HandleManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Errors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/StringUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/LayerUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/GeometryUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/ColorUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/DocumentUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/SelectionUtils.cpp
)

target_include_directories(nuxp-core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# nlohmann/json for downstream projects
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann")
    target_include_directories(nuxp-core INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann
    )
endif()

# -----------------------------------------------------------------------------
# Platform-Specific Configuration
# -----------------------------------------------------------------------------

if(APPLE)
    # macOS Bundle Configuration
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "aip"
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "${PLUGIN_DISPLAY_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${PLUGIN_BUNDLE_ID}"
    )

    # Use custom Info.plist if available
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
        )
    endif()

    # macOS compile definitions
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

    # Deployment target for modern macOS features
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version")

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
if(APPLE)
    # macOS: User's Plug-ins folder or system Plug-ins folder
    set(AI_PLUGINS_DEFAULT_PATH "$ENV{HOME}/Library/Application Support/Adobe/Adobe Illustrator 2024/Plug-ins")
    if(NOT EXISTS "${AI_PLUGINS_DEFAULT_PATH}" AND EXISTS "$ENV{HOME}/Library/Application Support/Adobe/Adobe Illustrator 2025/Plug-ins")
        set(AI_PLUGINS_DEFAULT_PATH "$ENV{HOME}/Library/Application Support/Adobe/Adobe Illustrator 2025/Plug-ins")
    endif()
elseif(WIN32)
    # Windows: Common Plug-ins folder
    set(AI_PLUGINS_DEFAULT_PATH "C:/Program Files/Adobe/Adobe Illustrator 2024/Plug-ins")
    if(NOT EXISTS "${AI_PLUGINS_DEFAULT_PATH}" AND EXISTS "C:/Program Files/Adobe/Adobe Illustrator 2025/Plug-ins")
        set(AI_PLUGINS_DEFAULT_PATH "C:/Program Files/Adobe/Adobe Illustrator 2025/Plug-ins")
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
message(STATUS "")
message(STATUS "Source files:")
message(STATUS "  Core sources:     ${CORE_SOURCES}")
list(LENGTH HANDWRITTEN_ENDPOINT_SOURCES HANDWRITTEN_COUNT)
message(STATUS "  Handwritten endpoints: ${HANDWRITTEN_COUNT} files")
list(LENGTH GENERATED_ENDPOINT_SOURCES GENERATED_COUNT)
message(STATUS "  Generated endpoints:   ${GENERATED_COUNT} files")
message(STATUS "")
message(STATUS "Build commands:")
message(STATUS "  cmake --build build            # Build plugin")
message(STATUS "  cmake --build build -t generate    # Regenerate code")
message(STATUS "  cmake --install build          # Install to Illustrator")
message(STATUS "========================================")
message(STATUS "")
