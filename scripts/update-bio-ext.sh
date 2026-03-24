#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

INPUT="${1:-docs/bio-ext-content.md}"
OUTPUT_LEGACY="docs/bio-ext.html"
OUTPUT_SITE="site/leadership/index.html"

if [ ! -f "$INPUT" ]; then
  echo "Usage: ./scripts/update-bio-ext.sh <path-to-content-file>"
  echo "Error: '$INPUT' not found."
  exit 1
fi

echo "Encrypting $INPUT -> $OUTPUT_SITE (and $OUTPUT_LEGACY)"
./scripts/encrypt-page.sh "$INPUT" "$OUTPUT_SITE"
cp "$OUTPUT_SITE" "$OUTPUT_LEGACY"

echo ""
echo "Done. To publish:"
echo "  git add site/leadership/index.html docs/bio-ext.html && git commit -m 'Update encrypted page' && git push"
