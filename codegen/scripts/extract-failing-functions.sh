#!/bin/bash
#
# Extract all failing function names from compiler errors
#
# This script:
# 1. Attempts to build the plugin
# 2. Parses all compiler errors from generated wrappers
# 3. Extracts function names that need to be blocked
#
# Usage: cd plugin && ../codegen/scripts/extract-failing-functions.sh
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PLUGIN_DIR="$(cd "$SCRIPT_DIR/../../plugin" && pwd)"
GENERATED_DIR="$PLUGIN_DIR/src/endpoints/generated"

cd "$PLUGIN_DIR"

echo "=== Building plugin to collect errors ===" >&2
echo "" >&2

# Run cmake build and capture stderr (where errors go)
BUILD_OUTPUT=$(cmake --build build 2>&1 || true)

# Extract error lines that reference generated wrapper files
# Format: /path/FloraAISuiteSuiteWrapper.cpp:123:45: error: ...
ERROR_LINES=$(echo "$BUILD_OUTPUT" | grep -E "Flora[A-Za-z]+Wrapper\.cpp:[0-9]+:[0-9]+: error:" || true)

if [ -z "$ERROR_LINES" ]; then
    echo "=== No errors found in generated wrappers ===" >&2
    echo "Build successful!" >&2
    exit 0
fi

echo "=== Found errors, extracting function names ===" >&2
echo "" >&2

# Process each error to extract file:line
FUNCTIONS=()
while IFS= read -r line; do
    # Extract file path and line number
    FILE=$(echo "$line" | sed -E 's/^([^:]+):([0-9]+):.*/\1/')
    LINE_NUM=$(echo "$line" | sed -E 's/^([^:]+):([0-9]+):.*/\2/')

    if [ -f "$FILE" ] && [ -n "$LINE_NUM" ]; then
        # Find the function name by looking backwards from error line
        # Pattern: "nlohmann::json FunctionName(const nlohmann::json& params)"
        FUNC=$(head -n "$LINE_NUM" "$FILE" | grep -E "^nlohmann::json [A-Za-z_]+\(" | tail -1 | sed -E 's/nlohmann::json ([A-Za-z_]+)\(.*/\1/')

        if [ -n "$FUNC" ] && [ "$FUNC" != "Dispatch" ]; then
            FUNCTIONS+=("$FUNC")
        fi
    fi
done <<< "$ERROR_LINES"

# Deduplicate and sort
UNIQUE_FUNCTIONS=$(printf '%s\n' "${FUNCTIONS[@]}" | sort -u)

echo "=== Functions to add to BLOCKED_FUNCTIONS ===" >&2
echo "" >&2

# Output in TypeScript format for easy copy-paste
echo "// Auto-detected from build errors - add to BLOCKED_FUNCTIONS in codegen/src/index.ts"
while IFS= read -r func; do
    if [ -n "$func" ]; then
        echo "    '$func',"
    fi
done <<< "$UNIQUE_FUNCTIONS"

echo "" >&2
echo "=== Summary ===" >&2
COUNT=$(echo "$UNIQUE_FUNCTIONS" | grep -c . || echo "0")
echo "Found $COUNT functions with compilation errors" >&2
