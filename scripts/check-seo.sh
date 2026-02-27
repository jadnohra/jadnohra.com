#!/usr/bin/env bash
# SEO verification script for notes.jadnohra.com
# Checks SOURCE files directly — no Jekyll build required.
# Usage: ./scripts/check-seo.sh

set -uo pipefail

DOCS="/Users/jad_irred/repos/jad/jadnohra.com/docs"
PASS=0
FAIL=0

green() { printf '\033[32m  PASS: %s\033[0m\n' "$1"; }
red()   { printf '\033[31m  FAIL: %s\033[0m\n' "$1"; }

check() {
  local label="$1" file="$2" pattern="$3"
  if grep -q "$pattern" "$file" 2>/dev/null; then
    green "$label"; PASS=$((PASS + 1))
  else
    red "$label"; FAIL=$((FAIL + 1))
  fi
}

check_no() {
  local label="$1" file="$2" pattern="$3"
  if grep -q "$pattern" "$file" 2>/dev/null; then
    red "$label"; FAIL=$((FAIL + 1))
  else
    green "$label"; PASS=$((PASS + 1))
  fi
}

echo "=== SEO Source Verification ==="
echo ""

echo "--- 1. Bio page noscript fallback ---"
check "noscript block in bio.md" "$DOCS/_tabs/bio.md" "<noscript>"
check "career content in noscript" "$DOCS/_tabs/bio.md" "Irreducible"
check "side projects in noscript" "$DOCS/_tabs/bio.md" "vram.run"
check "patents in noscript" "$DOCS/_tabs/bio.md" "patents.google.com"
echo ""

echo "--- 2. Front matter descriptions (tabs) ---"
for f in bio quotes compilers complexity better-intuitions abstractor rust about; do
  check "$f has description" "$DOCS/_tabs/$f.md" "^description:"
done
echo ""

echo "--- 3. Front matter descriptions (posts) ---"
for f in "$DOCS"/_posts/*.md; do
  name=$(basename "$f" .md)
  check "post $name has description" "$f" "^description:"
done
echo ""

echo "--- 4. Standalone pages: meta tags ---"
check "cv.html has meta description" "$DOCS/_tabs/cv.html" 'meta name="description"'
check "cv.html has og:title" "$DOCS/_tabs/cv.html" "og:title"
check "cv.html has og:description" "$DOCS/_tabs/cv.html" "og:description"
check "cv.html has twitter:card" "$DOCS/_tabs/cv.html" "twitter:card"
check "cv.html has canonical" "$DOCS/_tabs/cv.html" 'rel="canonical"'
check "music has meta description" "$DOCS/music/index.html" 'meta name="description"'
check "music has og:title" "$DOCS/music/index.html" "og:title"
check "music has og:description" "$DOCS/music/index.html" "og:description"
check "music has twitter:card" "$DOCS/music/index.html" "twitter:card"
check "music has canonical" "$DOCS/music/index.html" 'rel="canonical"'
echo ""

echo "--- 5. Canonical URLs are absolute ---"
check "index.html canonical absolute" "$DOCS/index.html" 'https://notes.jadnohra.com/bio/'
check "portfolio canonical absolute" "$DOCS/portfolio.html" 'https://notes.jadnohra.com/bio/'
echo ""

echo "--- 6. Title tags on redirect pages ---"
check "index.html has title" "$DOCS/index.html" "<title>"
check "portfolio has title" "$DOCS/portfolio.html" "<title>"
echo ""

echo "--- 7. No accidental noindex on public pages ---"
check_no "bio.md has no noindex" "$DOCS/_tabs/bio.md" "noindex"
check_no "cv.html has no noindex" "$DOCS/_tabs/cv.html" "noindex"
check_no "music has no noindex" "$DOCS/music/index.html" "noindex"
check_no "quotes has no noindex" "$DOCS/_tabs/quotes.md" "noindex"
echo ""

echo "=== Summary ==="
printf '\033[32mPASS: %d\033[0m\n' "$PASS"
if [ "$FAIL" -gt 0 ]; then
  printf '\033[31mFAIL: %d\033[0m\n' "$FAIL"
  exit 1
else
  printf 'FAIL: 0\n'
  exit 0
fi
