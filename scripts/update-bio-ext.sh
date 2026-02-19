#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

INPUT="${1:-docs/bio-ext-content.md}"
OUTPUT="docs/bio-ext.html"

if [ ! -f "$INPUT" ]; then
  echo "Usage: ./scripts/update-bio.sh <path-to-content-file>"
  echo "Error: '$INPUT' not found."
  exit 1
fi

echo "Encrypting $INPUT -> $OUTPUT"
./scripts/encrypt-page.sh "$INPUT" "$OUTPUT"

echo ""
echo "Done. To publish:"
echo "  git add docs/bio-ext.html && git commit -m 'Update bio-ext' && git push"
