#!/bin/bash
#
# clean-history.sh - Prepare NUXP for public release
#
# This script rewrites git history to:
# 1. Remove Adobe SDK content from all commits
# 2. Replace personal author info with generic project identity
# 3. Remove Co-Authored-By lines mentioning Claude
#
# REQUIREMENTS:
#   - git-filter-repo (install: pip install git-filter-repo)
#   - Fresh clone of the repo (filter-repo requires this)
#
# USAGE:
#   1. Clone a fresh copy: git clone <repo> nuxp-clean
#   2. cd nuxp-clean
#   3. Run this script: ../nuxp/scripts/clean-history.sh
#   4. Review changes
#   5. Force push: git push --force origin master
#
# WARNING: This rewrites history! All collaborators must re-clone after.
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== NUXP History Cleaner ===${NC}"
echo ""

# Check for git-filter-repo
if ! command -v git-filter-repo &> /dev/null; then
    echo -e "${RED}Error: git-filter-repo is not installed${NC}"
    echo "Install with: pip install git-filter-repo"
    exit 1
fi

# Verify we're in a git repo
if [ ! -d ".git" ]; then
    echo -e "${RED}Error: Not in a git repository${NC}"
    echo "This script must be run from the root of a fresh git clone"
    exit 1
fi

# Warn about fresh clone requirement
echo -e "${YELLOW}WARNING: git-filter-repo requires a fresh clone.${NC}"
echo "If this is your working copy, abort and create a fresh clone first."
echo ""
read -p "Continue? (yes/no): " confirm
if [ "$confirm" != "yes" ]; then
    echo "Aborted."
    exit 0
fi

# Configuration - customize these before running
NEW_AUTHOR_NAME="NUXP Contributors"
NEW_AUTHOR_EMAIL="nuxp@users.noreply.github.com"

echo ""
echo -e "${GREEN}Step 1: Removing Adobe SDK paths from history...${NC}"

# Remove Adobe proprietary content from all commits
git filter-repo \
    --path plugin/sdk/ \
    --path plugin/tools/pipl/ \
    --path plugin/sdk-docs/ \
    --path-glob 'plugin/sdk/*' \
    --invert-paths \
    --force

echo ""
echo -e "${GREEN}Step 2: Replacing author information...${NC}"

# Create mailmap to replace authors
cat > /tmp/nuxp-mailmap <<EOF
$NEW_AUTHOR_NAME <$NEW_AUTHOR_EMAIL> <your-old-email@example.com>
EOF

# Note: You'll need to add your actual email to the mailmap above
# For now, we'll use a callback to replace ALL authors

git filter-repo \
    --name-callback "return b'$NEW_AUTHOR_NAME'" \
    --email-callback "return b'$NEW_AUTHOR_EMAIL'" \
    --force

echo ""
echo -e "${GREEN}Step 3: Removing Co-Authored-By lines from commit messages...${NC}"

# Remove Claude Co-Authored-By lines from commit messages
git filter-repo \
    --message-callback '
import re
# Remove Co-Authored-By lines mentioning Claude
msg = re.sub(rb"\n\nCo-Authored-By: Claude[^\n]*", b"", message)
msg = re.sub(rb"\nCo-Authored-By: Claude[^\n]*", b"", msg)
return msg
' \
    --force

echo ""
echo -e "${GREEN}=== History cleaning complete ===${NC}"
echo ""
echo "Next steps:"
echo "  1. Review the cleaned history: git log --oneline"
echo "  2. Verify no sensitive content: git log --all --full-history -- 'plugin/sdk/'"
echo "  3. Force push: git push --force origin master"
echo ""
echo -e "${YELLOW}WARNING: Force pushing will require all collaborators to re-clone!${NC}"
