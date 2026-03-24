#!/usr/bin/env bash
set -euo pipefail

usage() {
  echo "Usage: $0 <input.md> <output.html> [--title TITLE] [--page PAGE] [--css FILE]... [--js FILE]... [--wide]"
  echo ""
  echo "Converts markdown to a standalone HTML page with the editor theme."
  echo ""
  echo "Options:"
  echo "  --title TITLE   Page title (default: derived from filename)"
  echo "  --page PAGE     Tab identifier for shell.js (default: 'writing')"
  echo "  --css FILE      Additional CSS file to include (can repeat)"
  echo "  --js FILE       Additional JS file to include at end (can repeat)"
  echo "  --wide          Use content-wide class instead of content"
  echo ""
  echo "Example:"
  echo "  $0 docs/_tabs/compilers.md site/writing/tech/compilers.html --title Compilers"
  exit 1
}

if [ $# -lt 2 ]; then
  usage
fi

INPUT="$1"
OUTPUT="$2"
shift 2

TITLE=""
PAGE="writing"
EXTRA_CSS=""
EXTRA_JS=""
CONTENT_CLASS="content"

while [ $# -gt 0 ]; do
  case "$1" in
    --title) TITLE="$2"; shift 2 ;;
    --page) PAGE="$2"; shift 2 ;;
    --css) EXTRA_CSS="$EXTRA_CSS $2"; shift 2 ;;
    --js) EXTRA_JS="$EXTRA_JS $2"; shift 2 ;;
    --wide) CONTENT_CLASS="content-wide"; shift ;;
    *) echo "Unknown option: $1"; usage ;;
  esac
done

if [ ! -f "$INPUT" ]; then
  echo "Error: Input file '$INPUT' not found."
  exit 1
fi

# Default title from filename
if [ -z "$TITLE" ]; then
  TITLE=$(basename "$INPUT" .md | sed 's/-/ /g')
fi

# Convert markdown to HTML using Python
# - Strips YAML frontmatter
# - Passes through raw HTML blocks
# - Supports tables, fenced code, footnotes
BODY_HTML=$(python3 << 'PYEOF'
import sys, os, re
import markdown

input_file = os.environ.get('MD_INPUT', '')
with open(input_file, 'r') as f:
    text = f.read()

# Strip YAML frontmatter
text = re.sub(r'^---\s*\n.*?\n---\s*\n', '', text, count=1, flags=re.DOTALL)

# Convert markdown to HTML
# extensions: tables, fenced_code, and allow raw HTML
html = markdown.markdown(text, extensions=['tables', 'fenced_code', 'md_in_html'])
print(html)
PYEOF
)

# Build CSS includes
CSS_LINKS='<link rel="stylesheet" href="/css/editor.css">'
for css_file in $EXTRA_CSS; do
  CSS_LINKS="$CSS_LINKS
<link rel=\"stylesheet\" href=\"$css_file\">"
done

# Build JS includes
JS_SCRIPTS=""
for js_file in $EXTRA_JS; do
  JS_SCRIPTS="$JS_SCRIPTS
<script src=\"$js_file\"></script>"
done

# Ensure output directory exists
mkdir -p "$(dirname "$OUTPUT")"

# Write the HTML page
cat > "$OUTPUT" << HTMLEOF
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>${TITLE} — jadnohra.com</title>
${CSS_LINKS}
</head>
<body data-page="${PAGE}" data-toc="true">

<div class="${CONTENT_CLASS}">
${BODY_HTML}
</div>

<script src="/js/shell.js"></script>${JS_SCRIPTS}
</body>
</html>
HTMLEOF

echo "Built: $OUTPUT"
