#!/bin/bash
# NUXP SDK Setup Script
# Downloads/extracts the Adobe Illustrator SDK and sets up the plugin/sdk directory
#
# Usage:
#   ./scripts/setup-sdk.sh /path/to/AI_2026_SDK_Mac.dmg
#   ./scripts/setup-sdk.sh ~/Downloads/AI_2026_SDK_Mac.dmg

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
SDK_DEST="$ROOT_DIR/plugin/sdk"

# Colors
if [ -t 1 ]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m'
else
    RED='' GREEN='' YELLOW='' BLUE='' NC=''
fi

print_info() { echo -e "${GREEN}$1${NC}"; }
print_warn() { echo -e "${YELLOW}$1${NC}"; }
print_error() { echo -e "${RED}$1${NC}"; }
print_step() { echo -e "${BLUE}>>> $1${NC}"; }

echo ""
print_info "=========================================="
print_info "       NUXP SDK Setup"
print_info "=========================================="
echo ""

# Check for DMG argument
if [ -z "$1" ]; then
    print_error "Usage: $0 /path/to/AI_2026_SDK_Mac.dmg"
    echo ""
    echo "Download the SDK from Adobe:"
    echo "  https://developer.adobe.com/console/servicesandapis"
    echo ""
    echo "Look for 'Adobe Illustrator' -> 'Downloads' -> 'Illustrator 2026 SDK'"
    exit 1
fi

DMG_PATH="$1"

# Verify DMG exists
if [ ! -f "$DMG_PATH" ]; then
    print_error "Error: DMG file not found: $DMG_PATH"
    exit 1
fi

# Create temp mount point
MOUNT_POINT=$(mktemp -d)
trap "hdiutil detach '$MOUNT_POINT' 2>/dev/null || true; trash '$MOUNT_POINT'" EXIT

print_step "Mounting SDK DMG..."
hdiutil attach "$DMG_PATH" -mountpoint "$MOUNT_POINT" -nobrowse -quiet

# Find the SDK folder (might be named differently across versions)
SDK_SOURCE=""
for dir in "$MOUNT_POINT"/*SDK* "$MOUNT_POINT"/Adobe*; do
    if [ -d "$dir/illustratorapi" ]; then
        SDK_SOURCE="$dir"
        break
    fi
done

if [ -z "$SDK_SOURCE" ]; then
    print_error "Error: Could not find illustratorapi in mounted DMG"
    print_error "Contents of DMG:"
    ls -la "$MOUNT_POINT"
    exit 1
fi

print_info "Found SDK at: $SDK_SOURCE"

# Verify expected structure
if [ ! -d "$SDK_SOURCE/illustratorapi/illustrator" ]; then
    print_error "Error: Expected directory not found: illustratorapi/illustrator"
    exit 1
fi

if [ ! -d "$SDK_SOURCE/illustratorapi/pica_sp" ]; then
    print_error "Error: Expected directory not found: illustratorapi/pica_sp"
    exit 1
fi

# Clear and recreate destination
print_step "Setting up plugin/sdk directory..."
trash "$SDK_DEST"
mkdir -p "$SDK_DEST"

# Copy Illustrator API headers
print_step "Copying Illustrator API headers..."
cp "$SDK_SOURCE/illustratorapi/illustrator/"*.h "$SDK_DEST/" 2>/dev/null || true
cp "$SDK_SOURCE/illustratorapi/illustrator/"*.hpp "$SDK_DEST/" 2>/dev/null || true
cp "$SDK_SOURCE/illustratorapi/illustrator/"*.cpp "$SDK_DEST/" 2>/dev/null || true
cp "$SDK_SOURCE/illustratorapi/illustrator/"*.inl "$SDK_DEST/" 2>/dev/null || true

# Copy config subdirectory
if [ -d "$SDK_SOURCE/illustratorapi/illustrator/config" ]; then
    cp -r "$SDK_SOURCE/illustratorapi/illustrator/config" "$SDK_DEST/"
fi

# Copy actions subdirectory
if [ -d "$SDK_SOURCE/illustratorapi/illustrator/actions" ]; then
    cp -r "$SDK_SOURCE/illustratorapi/illustrator/actions" "$SDK_DEST/"
fi

# Copy PICA/Sweet Pea headers (critical for platform types)
print_step "Copying PICA/Sweet Pea headers..."
cp "$SDK_SOURCE/illustratorapi/pica_sp/"*.h "$SDK_DEST/" 2>/dev/null || true

# Copy ATE (Adobe Text Engine) headers if present (directly to sdk, not subdirectory)
if [ -d "$SDK_SOURCE/illustratorapi/ate" ]; then
    print_step "Copying ATE headers..."
    cp "$SDK_SOURCE/illustratorapi/ate/"*.h "$SDK_DEST/" 2>/dev/null || true
fi

# Create IllustratorSDK.h convenience header
print_step "Creating IllustratorSDK.h convenience header..."
cat > "$SDK_DEST/IllustratorSDK.h" << 'HEADER_EOF'
/**
 * IllustratorSDK.h - Common Illustrator SDK Header
 * NOTE: This is a NUXP convenience header, not part of Adobe's SDK.
 */
#ifndef __IllustratorSDK__
#define __IllustratorSDK__

#include "SPBasic.h"
#include "SPPlugs.h"
#include "SPFiles.h"
#include "SPInterf.h"
#include "SPAccess.h"
#include "SPProps.h"
#include "AITypes.h"
#include "AIBasicTypes.h"
#include "AIPlugin.h"
#include "AITimer.h"
#include "AINotifier.h"
#include "AIDocument.h"
#include "AILayer.h"
#include "AILayerList.h"
#include "AIArt.h"
#include "AIArtSet.h"
#include "AIPath.h"
#include "AIGroup.h"
#include "AIRaster.h"
#include "AIColor.h"
#include "AIPathStyle.h"
#include "AIGradient.h"
#include "AIPattern.h"
#include "AISwatchList.h"
#include "AIRealMath.h"
#include "AITransformArt.h"
#include "AIMatchingArt.h"
#include "IAIUnicodeString.h"

#endif // __IllustratorSDK__
HEADER_EOF

# Create version marker
SDK_VERSION=$(cat "$SDK_SOURCE/Version.txt" 2>/dev/null || echo "Unknown")
cat > "$SDK_DEST/SDK_VERSION.txt" << EOF
Adobe Illustrator SDK
Version: $SDK_VERSION
Source: $DMG_PATH
Extracted: $(date -u +"%Y-%m-%dT%H:%M:%SZ")
EOF

# Count files
HEADER_COUNT=$(find "$SDK_DEST" -name "*.h" | wc -l | tr -d ' ')
HPP_COUNT=$(find "$SDK_DEST" -name "*.hpp" | wc -l | tr -d ' ')

echo ""
print_info "=========================================="
print_info "       SDK Setup Complete!"
print_info "=========================================="
echo ""
echo "Installed to: $SDK_DEST"
echo "Headers: $HEADER_COUNT .h files, $HPP_COUNT .hpp files"
echo "Version: $SDK_VERSION"
echo ""
echo "Next steps:"
echo "  1. Run code generator:  ./scripts/generate.sh"
echo "  2. Build the plugin:    cd plugin && cmake -B build && cmake --build build"
echo ""
