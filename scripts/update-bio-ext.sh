#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

INPUT="${1:-bio-ext-content.md}"
OUTPUT="site/leadership/index.html"

if [ ! -f "$INPUT" ]; then
  echo "Usage: ./scripts/update-bio-ext.sh <path-to-content-file>"
  echo "Error: '$INPUT' not found."
  exit 1
fi

echo "Encrypting $INPUT -> $OUTPUT"
./scripts/encrypt-page.sh "$INPUT" "$OUTPUT"

echo ""
echo "Done. To publish:"
echo "  git add site/leadership/index.html && git commit -m 'Update encrypted page' && git push"
echo "  wrangler pages deploy site/ --project-name=jadnohra-com --commit-dirty=true"
