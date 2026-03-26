#!/usr/bin/env node
// build.js — parse course.md → course.json
// Usage: node build.js

const fs = require('fs');
const { marked } = require('marked');

const src = fs.readFileSync(__dirname + '/course.md', 'utf8');

// ═══ 1. YAML FRONTMATTER ═══
const fmMatch = src.match(/^---\n([\s\S]*?)\n---\n/);
if (!fmMatch) { console.error('No YAML frontmatter found'); process.exit(1); }
const walks = parseWalksYaml(fmMatch[1]);
const body = src.slice(fmMatch[0].length);

// ═══ 2. SPLIT INTO SECTIONS ═══
const sections = body.split(/(?=^## )/m).filter(s => s.trim());

const nodes = {};
const edges = [];

for (const section of sections) {
  // Parse heading: ## id | Title
  const headMatch = section.match(/^## (\S+) \| (.+)\n/);
  if (!headMatch) continue;
  const id = headMatch[1];
  const title = headMatch[2].trim();
  let rest = section.slice(headMatch[0].length);

  // Extract context line (blockquote immediately after heading)
  let ctx = '';
  const ctxMatch = rest.match(/^> (.+)\n/);
  if (ctxMatch) {
    ctx = ctxMatch[1];
    rest = rest.slice(ctxMatch[0].length);
  }

  // Extract <!-- edges --> block → card edges
  const edgeBlockRe = /<!-- edges\n([\s\S]*?)-->/g;
  let edgeBlock;
  while ((edgeBlock = edgeBlockRe.exec(rest)) !== null) {
    const lines = edgeBlock[1].trim().split('\n');
    for (const line of lines) {
      const m = line.match(/^-> (\S+) \| (.+?) \| (.+)$/);
      if (m) edges.push({ from: id, to: m[1], t: m[2].trim(), w: m[3].trim(), card: true });
    }
  }
  rest = rest.replace(/<!-- edges\n[\s\S]*?-->/g, '').trim();

  // Extract inline EA links: [text](!id "tooltip") → collect edges
  const eaRe = /\[([^\]]+)\]\(!(\S+)\s+"([^"]+)"\)/g;
  let eaMatch;
  while ((eaMatch = eaRe.exec(rest)) !== null) {
    edges.push({ from: id, to: eaMatch[2], t: eaMatch[1], w: eaMatch[3], card: false });
  }

  // Transform EA links to HTML spans BEFORE marked processing
  // (marked would try to parse them as links)
  let mdBody = rest.replace(eaRe, (_, text, target, tooltip) => {
    return `<span class="ea" data-t="${esc(target)}">${esc(text)}<span class="tt"><span class="em-q">${esc(tooltip)}</span></span></span>`;
  });

  // Convert markdown body to HTML
  const html = renderMarkdown(mdBody);
  nodes[id] = { t: title, ctx, body: html };
}

// ═══ 3. OUTPUT ═══
const output = { nodes, edges, walks };
const json = JSON.stringify(output);
fs.writeFileSync(__dirname + '/course.json', json);

// Stats
const cardEdges = edges.filter(e => e.card).length;
const inlineEdges = edges.filter(e => !e.card).length;
const nodeIds = Object.keys(nodes);
const walkNodeCount = Object.values(walks).reduce((sum, w) =>
  sum + w.chapters.reduce((s, ch) => s + ch.nodes.length, 0), 0);

console.log(`Nodes: ${nodeIds.length}`);
console.log(`Edges: ${edges.length} (${cardEdges} card + ${inlineEdges} inline)`);
console.log(`Walk nodes: ${walkNodeCount}`);
console.log(`Output: course.json (${(json.length / 1024).toFixed(0)} KB)`);

// Validate: all edge targets exist
let broken = 0;
for (const e of edges) {
  if (!nodes[e.to]) { console.error(`  BROKEN edge: ${e.from} -> ${e.to}`); broken++; }
  if (!nodes[e.from]) { console.error(`  BROKEN edge from: ${e.from}`); broken++; }
}
// Validate: all walk nodes exist
for (const [wid, w] of Object.entries(walks)) {
  for (const ch of w.chapters) {
    for (const nid of ch.nodes) {
      if (!nodes[nid]) { console.error(`  BROKEN walk node: ${wid}/${ch.title}/${nid}`); broken++; }
    }
  }
}
if (broken) { console.error(`${broken} broken references!`); process.exit(1); }
else console.log('All references valid.');


// ═══════════════════════════════════════════════════
// HELPERS
// ═══════════════════════════════════════════════════

function esc(s) {
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
}

// ═══ YAML PARSER (minimal, for walks only) ═══
function parseWalksYaml(yamlStr) {
  // Parse the specific walks format:
  // walks:
  //   textbook:
  //     chapters:
  //       - title: "..."
  //         nodes: [a, b, c]
  const walks = {};
  const walkBlocks = yamlStr.split(/^  (\w+):\s*$/m).slice(1);
  for (let i = 0; i < walkBlocks.length; i += 2) {
    const walkId = walkBlocks[i];
    const walkBody = walkBlocks[i + 1];
    const chapters = [];
    // Split on "- title:"
    const chParts = walkBody.split(/^\s*- title:\s*/m).slice(1);
    for (const chPart of chParts) {
      const titleMatch = chPart.match(/^"(.+?)"\s*\n/);
      if (!titleMatch) continue;
      const title = titleMatch[1];
      // Extract nodes array — may span multiple lines
      const nodesMatch = chPart.match(/nodes:\s*\[([\s\S]*?)\]/);
      if (!nodesMatch) continue;
      const nodeList = nodesMatch[1].replace(/\s+/g, ' ').trim().split(/,\s*/).filter(Boolean);
      chapters.push({ title, nodes: nodeList });
    }
    walks[walkId] = { chapters };
  }
  return walks;
}

// ═══ MARKDOWN → HTML ═══
function renderMarkdown(md) {
  const renderer = new marked.Renderer();

  renderer.code = function({ text, lang }) {
    const highlighted = (lang === 'rust' || lang === 'rs') ? hlRust(text)
      : lang === 'cpp' ? hlCpp(text)
      : escCode(text);
    return `<div class="cb"><code>${highlighted}</code></div>`;
  };

  renderer.paragraph = function({ tokens }) {
    const text = this.parser.parseInline(tokens);
    // Error blocks: **error[E0...]:** ...
    if (text.match(/^<strong>error\[/)) {
      return `<div class="er">${text}</div>`;
    }
    return `<p>${text}</p>`;
  };

  renderer.blockquote = function({ tokens }) {
    const inner = this.parser.parse(tokens);
    return `<div class="co">${inner}</div>`;
  };

  renderer.table = function({ header, rows }) {
    let h = '<table><thead><tr>';
    for (const cell of header) {
      h += `<th>${this.parser.parseInline(cell.tokens)}</th>`;
    }
    h += '</tr></thead><tbody>';
    for (const row of rows) {
      h += '<tr>';
      for (const cell of row) {
        h += `<td>${this.parser.parseInline(cell.tokens)}</td>`;
      }
      h += '</tr>';
    }
    h += '</tbody></table>';
    return h;
  };

  renderer.list = function({ items, ordered }) {
    const tag = ordered ? 'ol' : 'ul';
    let h = `<${tag}>`;
    for (const item of items) {
      // parse block-level tokens, strip wrapping <p> for tight lists
      let inner = this.parser.parse(item.tokens);
      inner = inner.replace(/^<p>([\s\S]*)<\/p>\n?$/, '$1');
      h += `<li>${inner}</li>`;
    }
    h += `</${tag}>`;
    return h;
  };

  renderer.codespan = function({ text }) {
    return `<code>${text}</code>`;
  };

  renderer.link = function({ href, text }) {
    return `<a href="${href}">${text}</a>`;
  };

  return marked.parse(md, { renderer, breaks: false, gfm: true });
}

// ═══ SYNTAX HIGHLIGHTING ═══
function escCode(s) {
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

function hlRust(code) {
  return highlight(code, {
    keywords: /\b(fn|let|mut|struct|impl|use|pub|mod|if|else|for|while|loop|match|return|move|ref|where|type|trait|enum|const|static|unsafe|async|await|dyn|in|as|self|Self|super|crate|extern|true|false)\b/g,
    types: /\b(i8|i16|i32|i64|i128|u8|u16|u32|u64|u128|f32|f64|bool|char|str|usize|isize|String|Vec|Box|Rc|Arc|Option|Result|Some|None|Ok|Err|RefCell|Cell|Mutex|HashMap|BTreeMap|Send|Sync|Copy|Clone|Drop|Fn|FnMut|FnOnce|Iterator|Display|Debug)\b/g,
    macros: /\b(\w+!)/g,
  });
}

function hlCpp(code) {
  return highlight(code, {
    keywords: /\b(int|void|char|float|double|long|short|unsigned|signed|const|static|extern|struct|class|template|typename|auto|return|if|else|for|while|do|switch|case|break|continue|new|delete|nullptr|true|false|using|namespace|std|include|define|printf|cout|endl)\b/g,
    types: /\b(std::string|std::vector|std::thread|std::mutex|size_t)\b/g,
    macros: null,
  });
}

function highlight(code, rules) {
  // Tokenize to preserve strings and comments
  const tokens = [];
  let rest = escCode(code);

  // Extract strings and comments first (replace with placeholders)
  const placeholders = [];
  let phIdx = 0;

  // Comments: // to end of line
  rest = rest.replace(/\/\/.*$/gm, m => {
    const ph = `\x00PH${phIdx++}\x00`;
    placeholders.push({ ph, html: `<span class="c">${m}</span>` });
    return ph;
  });

  // Strings: "..." (handle escaped quotes)
  rest = rest.replace(/"(?:[^"\\]|\\.)*"/g, m => {
    const ph = `\x00PH${phIdx++}\x00`;
    placeholders.push({ ph, html: `<span class="s">${m}</span>` });
    return ph;
  });

  // Numbers
  rest = rest.replace(/\b(\d+(?:\.\d+)?)\b/g, '<span class="n">$1</span>');

  // Macros (before keywords, since macros contain word chars)
  if (rules.macros) {
    rest = rest.replace(rules.macros, '<span class="m">$1</span>');
  }

  // Keywords
  rest = rest.replace(rules.keywords, '<span class="k">$1</span>');

  // Types/built-in names
  if (rules.types) {
    rest = rest.replace(rules.types, '<span class="m">$1</span>');
  }

  // Function definitions: fn name
  rest = rest.replace(/<span class="k">fn<\/span> (\w+)/g,
    '<span class="k">fn</span> <span class="m">$1</span>');

  // Restore placeholders
  for (const { ph, html } of placeholders) {
    rest = rest.replace(ph, html);
  }

  return rest;
}
