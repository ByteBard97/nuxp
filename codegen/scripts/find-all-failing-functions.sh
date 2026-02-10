#!/bin/bash
#
# Find ALL failing functions by iteratively compiling until success
#
# This script:
# 1. Regenerates code with current blocklist
# 2. Attempts to build
# 3. Extracts failing function names from errors
# 4. Outputs ALL functions that need to be blocked
#
# Usage: cd nuxp && ./codegen/scripts/find-all-failing-functions.sh
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CODEGEN_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
PLUGIN_DIR="$(cd "$CODEGEN_DIR/../plugin" && pwd)"
GENERATED_DIR="$PLUGIN_DIR/src/endpoints/generated"

ALL_FAILING=""

extract_failing_functions() {
    local build_output="$1"

    # Extract error lines that reference generated wrapper files
    local error_lines=$(echo "$build_output" | grep -E "Flora[A-Za-z]+Wrapper\.cpp:[0-9]+:[0-9]+: error:" || true)

    if [ -z "$error_lines" ]; then
        return 1  # No errors
    fi

    local funcs=""
    while IFS= read -r line; do
        # Extract file path and line number
        local file=$(echo "$line" | sed -E 's/^([^:]+):([0-9]+):.*/\1/')
        local line_num=$(echo "$line" | sed -E 's/^([^:]+):([0-9]+):.*/\2/')

        if [ -f "$file" ] && [ -n "$line_num" ]; then
            # Find the function name by looking backwards from error line
            local func=$(head -n "$line_num" "$file" 2>/dev/null | grep -E "^nlohmann::json [A-Za-z_]+\(" | tail -1 | sed -E 's/nlohmann::json ([A-Za-z_]+)\(.*/\1/')

            if [ -n "$func" ] && [ "$func" != "Dispatch" ]; then
                funcs="$funcs$func\n"
            fi
        fi
    done <<< "$error_lines"

    echo -e "$funcs" | sort -u | grep -v '^$'
}

echo "=== Finding ALL failing functions ===" >&2
echo "This will do a single build pass and extract all errors" >&2
echo "" >&2

# Step 1: Regenerate code
echo "Step 1: Regenerating code..." >&2
cd "$CODEGEN_DIR"
npm run generate > /dev/null 2>&1

# Step 2: Copy to plugin
echo "Step 2: Copying to plugin..." >&2
rm -f "$GENERATED_DIR"/*.cpp "$GENERATED_DIR"/*.h "$GENERATED_DIR"/*.hpp "$GENERATED_DIR"/*.cmake 2>/dev/null || true
cp -r "$CODEGEN_DIR/output/cpp/"* "$GENERATED_DIR/"

# Step 3: Build and collect errors
echo "Step 3: Building to find errors..." >&2
cd "$PLUGIN_DIR"

# Use make with -k to keep going after errors (collect all errors, not just first)
BUILD_OUTPUT=$(cmake --build build -- -k 2>&1 || true)

# Step 4: Extract all failing functions
echo "Step 4: Extracting failing functions..." >&2
FAILING=$(extract_failing_functions "$BUILD_OUTPUT")

if [ -z "$FAILING" ]; then
    echo "" >&2
    echo "=== SUCCESS! No failing functions found ===" >&2
    echo "The build completed without errors in generated wrappers." >&2
    exit 0
fi

echo "" >&2
echo "=== FUNCTIONS TO BLOCK ===" >&2
echo "" >&2
echo "Add these to BLOCKED_FUNCTIONS in codegen/src/index.ts:" >&2
echo ""

# Output in format ready to copy-paste
echo "$FAILING" | while read func; do
    if [ -n "$func" ]; then
        echo "    '$func',"
    fi
done

echo "" >&2
COUNT=$(echo "$FAILING" | wc -l | tr -d ' ')
echo "=== Total: $COUNT functions need blocking ===" >&2
