#!/bin/bash
# NUXP Code Generator Script
# Generates C++ and TypeScript wrappers from Adobe SDK headers
#
# Usage:
#   ./scripts/generate.sh [options]
#
# Options are passed directly to the codegen tool:
#   -s, --sdk <path>       Path to SDK headers (default: ../plugin/sdk)
#   -o, --output <path>    Output directory (default: ./output)
#   --suites <names>       Comma-separated list of suites to generate
#   --cpp-only             Only generate C++ code
#   --ts-only              Only generate TypeScript code
#   -v, --verbose          Verbose output

set -e

# Determine script and project directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
CODEGEN_DIR="$ROOT_DIR/codegen"

# Colors for output (with fallback for non-interactive terminals)
if [ -t 1 ]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    NC=''
fi

# Print with color
print_info() {
    echo -e "${GREEN}$1${NC}"
}

print_warn() {
    echo -e "${YELLOW}$1${NC}"
}

print_error() {
    echo -e "${RED}$1${NC}"
}

print_step() {
    echo -e "${BLUE}>>> $1${NC}"
}

# Header
echo ""
print_info "=========================================="
print_info "       NUXP Code Generator"
print_info "=========================================="
echo ""

# Check for Node.js
if ! command -v node &> /dev/null; then
    print_error "Error: Node.js is not installed"
    print_error "Please install Node.js from https://nodejs.org/"
    exit 1
fi

# Check for npm
if ! command -v npm &> /dev/null; then
    print_error "Error: npm is not installed"
    exit 1
fi

# Check if codegen directory exists
if [ ! -d "$CODEGEN_DIR" ]; then
    print_error "Error: codegen directory not found at $CODEGEN_DIR"
    exit 1
fi

# Install dependencies if needed
if [ ! -d "$CODEGEN_DIR/node_modules" ]; then
    print_step "Installing dependencies..."
    cd "$CODEGEN_DIR" && npm install
    if [ $? -ne 0 ]; then
        print_error "Failed to install dependencies"
        exit 1
    fi
    echo ""
fi

# Check if node_modules has the required packages
if [ ! -d "$CODEGEN_DIR/node_modules/commander" ] || [ ! -d "$CODEGEN_DIR/node_modules/tree-sitter" ]; then
    print_step "Dependencies incomplete, reinstalling..."
    cd "$CODEGEN_DIR" && npm install
    if [ $? -ne 0 ]; then
        print_error "Failed to install dependencies"
        exit 1
    fi
    echo ""
fi

# Run the code generator
print_step "Running code generator..."
cd "$CODEGEN_DIR"

# Pass all arguments to the generator
if ! npm run generate -- "$@"; then
    print_error "Code generation failed"
    exit 1
fi

echo ""

# Copy generated files to their target locations
print_step "Copying generated files to targets..."

# Source directories
CPP_SRC="$CODEGEN_DIR/output/cpp"
TS_SRC="$CODEGEN_DIR/output/typescript"

# Destination directories
CPP_DEST="$ROOT_DIR/plugin/src/endpoints/generated"
TS_DEST="$ROOT_DIR/shell/src/sdk/generated"

# Copy C++ files to plugin
if [ -d "$CPP_SRC" ] && [ "$(ls -A "$CPP_SRC" 2>/dev/null)" ]; then
    # Create destination if it doesn't exist
    mkdir -p "$CPP_DEST"

    # Clean old generated files (preserve non-generated files if any)
    # Move old generated files to trash
    find "$CPP_DEST" -maxdepth 1 \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cmake" \) -exec trash {} + 2>/dev/null || true

    # Create marker file to indicate these are generated
    echo "# This directory contains auto-generated files" > "$CPP_DEST/.generated"
    echo "# Do not edit manually - run scripts/generate.sh to regenerate" >> "$CPP_DEST/.generated"
    echo "# Generated: $(date -u +"%Y-%m-%dT%H:%M:%SZ")" >> "$CPP_DEST/.generated"

    # Count files before copy
    cpp_count=0

    # Copy .cpp files
    for file in "$CPP_SRC"/*.cpp; do
        if [ -f "$file" ]; then
            cp "$file" "$CPP_DEST/"
            cpp_count=$((cpp_count + 1))
        fi
    done

    # Copy .h files
    for file in "$CPP_SRC"/*.h; do
        if [ -f "$file" ]; then
            cp "$file" "$CPP_DEST/"
            cpp_count=$((cpp_count + 1))
        fi
    done

    # Copy .hpp files
    for file in "$CPP_SRC"/*.hpp; do
        if [ -f "$file" ]; then
            cp "$file" "$CPP_DEST/"
            cpp_count=$((cpp_count + 1))
        fi
    done

    # Copy CMake include file if it exists
    if [ -f "$CPP_SRC/generated_sources.cmake" ]; then
        cp "$CPP_SRC/generated_sources.cmake" "$CPP_DEST/"
        cpp_count=$((cpp_count + 1))
    fi

    if [ $cpp_count -gt 0 ]; then
        print_info "  Copied $cpp_count C++ files to plugin/src/endpoints/generated/"
    fi
else
    print_warn "  No C++ files to copy"
fi

# Copy TypeScript files to client
if [ -d "$TS_SRC" ] && [ "$(ls -A "$TS_SRC" 2>/dev/null)" ]; then
    # Create destination if it doesn't exist
    mkdir -p "$TS_DEST"

    # Clean old generated files
    # Move old generated files to trash
    find "$TS_DEST" -maxdepth 1 -name "*.ts" -exec trash {} + 2>/dev/null || true

    # Create marker file to indicate these are generated
    echo "// This directory contains auto-generated files" > "$TS_DEST/.generated"
    echo "// Do not edit manually - run scripts/generate.sh to regenerate" >> "$TS_DEST/.generated"
    echo "// Generated: $(date -u +"%Y-%m-%dT%H:%M:%SZ")" >> "$TS_DEST/.generated"

    # Count files before copy
    ts_count=0

    # Copy .ts files
    for file in "$TS_SRC"/*.ts; do
        if [ -f "$file" ]; then
            cp "$file" "$TS_DEST/"
            ts_count=$((ts_count + 1))
        fi
    done

    if [ $ts_count -gt 0 ]; then
        print_info "  Copied $ts_count TypeScript files to client/src/sdk/generated/"
    fi
else
    print_warn "  No TypeScript files to copy"
fi

echo ""
print_info "=========================================="
print_info "       Generation Complete!"
print_info "=========================================="
echo ""

# Print next steps
echo "Next steps:"
echo "  1. Review generated files in:"
echo "     - plugin/src/endpoints/generated/"
echo "     - client/src/sdk/generated/"
echo "  2. Build the plugin: cd plugin && cmake -B build && cmake --build build"
echo "  3. Build the client: cd client && npm run build"
echo ""
