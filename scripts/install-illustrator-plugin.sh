#!/bin/bash
# Install the built NUXP plugin into Illustrator's app-level plug-ins folder
# and relaunch Illustrator.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

SOURCE_BUNDLE="${1:-$REPO_ROOT/plugin/build-xcode-verify/Release/NUXPPlugin.aip}"
TARGET_BUNDLE="/Applications/Adobe Illustrator 2026/Plug-ins.localized/NUXPPlugin.aip"
ILLUSTRATOR_APP="/Applications/Adobe Illustrator 2026/Adobe Illustrator.app"

if [[ ! -d "$SOURCE_BUNDLE" ]]; then
    echo "Source bundle not found: $SOURCE_BUNDLE" >&2
    exit 1
fi

echo "Installing:"
echo "  from: $SOURCE_BUNDLE"
echo "  to:   $TARGET_BUNDLE"

osascript -e 'tell application "Adobe Illustrator" to quit' >/dev/null 2>&1 || true
sleep 2

sudo rm -rf "$TARGET_BUNDLE"
sudo cp -R "$SOURCE_BUNDLE" "$TARGET_BUNDLE"

open "$ILLUSTRATOR_APP"

echo "Installed and relaunched Illustrator."
