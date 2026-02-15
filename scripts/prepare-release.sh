#!/usr/bin/env bash
# prepare-release.sh — Copy NUXP to a clean folder ready for a fresh git repo.
# Usage: ./scripts/prepare-release.sh [destination]
#   Default destination: ~/Desktop/nuxp-release

set -euo pipefail

SRC="$(cd "$(dirname "$0")/.." && pwd)"
DEST="${1:-$HOME/Desktop/nuxp-release}"

if [ -d "$DEST" ]; then
    echo "ERROR: Destination already exists: $DEST"
    echo "Remove it first or choose a different path."
    exit 1
fi

echo "Source:      $SRC"
echo "Destination: $DEST"
echo ""

rsync -a --progress \
    --exclude='.git' \
    --exclude='.DS_Store' \
    --exclude='.claude' \
    --exclude='.playwright-mcp' \
    --exclude='CLAUDE.md' \
    --exclude='DEVELOPER-EXPERIENCE-AUDIT.md' \
    --exclude='TODO.md' \
    --exclude='PLAN-*.md' \
    --exclude='docs/internal' \
    --exclude='node_modules' \
    --exclude='plugin/sdk' \
    --exclude='plugin/sdk-docs' \
    --exclude='plugin/tools/pipl' \
    --exclude='plugin/build' \
    --exclude='plugin/build-xcode' \
    --exclude='plugin/.cache' \
    --exclude='shell/build' \
    --exclude='shell/dist' \
    --exclude='shell/target' \
    --exclude='shell/src-tauri/target' \
    --exclude='shell/src-tauri/gen' \
    --exclude='client/dist' \
    --exclude='codegen/output' \
    --exclude='codegen/dist' \
    --exclude='*.token' \
    --exclude='*.bak' \
    --exclude='*.backup' \
    --exclude='*_backup' \
    --exclude='*.swp' \
    --exclude='*.swo' \
    --exclude='*.pyc' \
    --exclude='__pycache__' \
    --exclude='*.tsbuildinfo' \
    --exclude='test-runner-results.*' \
    "$SRC/" "$DEST/"

echo ""
echo "--- Verifying exclusions ---"

PROBLEMS=0
for CHECK in \
    ".git" \
    ".claude" \
    ".playwright-mcp" \
    "CLAUDE.md" \
    "DEVELOPER-EXPERIENCE-AUDIT.md" \
    "TODO.md" \
    "docs/internal" \
    "plugin/sdk/AIArt.h" \
    "plugin/tools/pipl" \
    "node_modules" \
; do
    if [ -e "$DEST/$CHECK" ]; then
        echo "  FAIL: $CHECK was NOT excluded"
        PROBLEMS=$((PROBLEMS + 1))
    else
        echo "  OK:   $CHECK excluded"
    fi
done

echo ""

if [ "$PROBLEMS" -gt 0 ]; then
    echo "WARNING: $PROBLEMS item(s) were not properly excluded."
    exit 1
fi

# Count what we copied
FILE_COUNT=$(find "$DEST" -type f | wc -l | tr -d ' ')
DIR_SIZE=$(du -sh "$DEST" | cut -f1)

echo "Release folder ready: $DEST"
echo "  Files: $FILE_COUNT"
echo "  Size:  $DIR_SIZE"
echo ""
echo "Next steps:"
echo "  cd $DEST"
echo "  git init"
echo "  git add ."
echo "  git commit -m 'Initial release'"
echo "  git remote add origin <your-new-repo-url>"
echo "  git push -u origin main"
