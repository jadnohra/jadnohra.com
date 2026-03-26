# NEXT — learn-rust-2 Context Recovery

## What Is This

A new version of the interactive graph reader at jadnohra.com/learn-rust. Same engine as learn-rust (fan menus, EA anchors, textbook walk, sidebar), new data format and new content.

## Source Content

`learn-rust-full-nodes.md` — 58 nodes, 3 parts. No edges defined yet.

### Node Inventory

**Part 1: The Problem** (22 nodes — 21 original + 1 restored)
| ID | Title | Status |
|---|---|---|
| P1.1 | Why This Course Exists | done |
| **P1.1b** | **Familiar Syntax** | **restore from v1 `familiar`** |
| P1.2 | Programs That Don't Compile | done |
| P1.3 | Thread Safety Errors |
| P1.4 | The Dangling Reference |
| P1.5 | Why Memory Is Hard |
| P1.6 | What Happens When Two Cores Touch the Same Address |
| P1.7 | This Is a Distributed System |
| P1.8 | The Coherence Constraint |
| P1.9 | Deriving the States |
| P1.10 | The Protocol |
| P1.11 | Shared Memory Is Message Passing |
| P1.12 | Distance Forces Copies |
| P1.13 | Three Primitives |
| P1.14 | Bugs as Interaction Failures |
| P1.15 | Use-After-Free |
| P1.16 | Dangling Pointer |
| P1.17 | Double Free |
| P1.18 | Data Race |
| P1.19 | Other Memory Bugs |
| P1.20 | Paradigms and Sync Strategies | **expand: weave in v1 `features` S×T×C table with dates** |
| P1.21 | The Lineage | **style-fix punchy ending** |

**Part 2: The Compiler** (29 nodes — 28 original + 1 restored)
| ID | Title | Status |
|---|---|---|
| P2.1 | Bindings and Addresses | done |
| P2.2 | How Programs Manage Space |
| P2.3 | What the Compiler Needs to See |
| P2.4 | Detecting Access to Reclaimed Space |
| P2.5 | Why C++ Compilers Skip This Analysis |
| P2.6 | RAM Is Flat |
| P2.7 | Data Flow Crosses Scope Boundaries |
| P2.8 | Invisible Coordinate Copying | done |
| **P2.8b** | **How Other Languages Approach These Obstacles** | **restore from v1 `other_langs`** |
| P2.9 | Where Branches Break the Analysis | done |
| P2.10 | Why Perfect Analysis Is Impossible |
| P2.11 | Sound or Complete |
| P2.12 | Names Are Exact, Data Flow Is Approximated |
| P2.13 | Constrained Coordinates |
| P2.14 | Ownership |
| P2.15 | The Notation |
| P2.16 | Lifetime Annotations |
| P2.17 | Desugaring and Lifetime Elision |
| P2.18 | Using Data, Managing Space |
| P2.19 | Why Rust Moves by Default |
| P2.20 | What mut Means on a Binding |
| P2.21 | Shared and Exclusive Coordinates |
| P2.22 | Composing the Vocabulary |
| P2.23 | How a Coordinate Can Dangle While Its Owner Lives |
| P2.24 | Why Derived Data Needs a Sync Strategy |
| P2.25 | Why the Compiler Cannot Distinguish Safe Methods |
| P2.26 | The Borrowing Rule |
| P2.27 | Why the Borrowing Rule Applies to Every Type |
| P2.28 | The Chapter 1 Programs and the Borrowing Rule | **expand: strengthen C++ UB side-by-side from v1** |

**Part 3: The Consequences** (10 nodes — 9 original + 1 restored)
| ID | Title | Status |
|---|---|---|
| P3.1 | What the Compiler Can Distinguish | done |
| P3.2 | Why Compilers Skip Decidable Cases | done |
| P3.3 | Signatures Lose Field-Level Information | done |
| P3.4 | The Mirror | done |
| P3.5 | Relaxing Space | done |
| P3.6 | Relaxing Time | **expand: add BFS + build-new-collections from v1 `algo_patterns`** |
| P3.7 | Relaxing Coordinates | done |
| P3.8 | Data Shapes and the Table | done |
| P3.9 | When to Reach for Unsafe | done |
| **P3.10** | **Hints and Patterns** | **restore from v1 `hints_patterns`** |

## Markdown Graph Format

Everything lives in ONE markdown file: `course.md`. Nodes are sections. Edges and walks are metadata. The file renders as readable prose AND encodes the full graph.

### Nodes = markdown sections

```markdown
## p1_1 | Why This Course Exists

This course exists because I cannot memorize rules...
```

`## id | Title` — the id is the node key, the title is what the reader sees. Everything below until the next `##` is the body (standard markdown: paragraphs, code blocks, tables).

### Card edges = comment blocks after each node

Card edges appear in the fan menu when the reader hovers "next." at the end of a section. They go in an HTML comment right before the next `##`:

```markdown
## p1_1 | Why This Course Exists

This course exists because...

<!-- edges
-> p1_2 | What doesn't compile? | The compiler rejects code that looks right.
-> p1_5 | Why is memory hard? | Three primitives rooted in physics.
-> p2_15 | What's that notation? | A companion crate that makes every operation explicit.
-->

## p1_2 | Programs That Don't Compile
```

Format: `-> target_id | link_text | tooltip_question`
All edges in the block are card edges (fan menu choices). The `from` is implicit (the preceding node).

### Inline edges = special links in prose

Cross-references within the text (EA anchors: underlined words with tooltips). Use markdown link syntax with `!` prefix on the target:

```markdown
The [coherence constraint](!p1_8 "The same constraint from hardware.") sits underneath.
```

Renders as underlined "coherence constraint" with tooltip on hover. Click navigates to p1_8. The engine creates an edge from the current node to p1_8.

Regular markdown links (`[text](url)`) remain regular links.

### Walks = YAML frontmatter

```yaml
---
walks:
  textbook:
    chapters:
      - title: "Part 1: The Problem"
        nodes: [p1_1, p1_1b, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7, p1_8,
                p1_9, p1_10, p1_11, p1_12, p1_13, p1_14, p1_15,
                p1_16, p1_17, p1_18, p1_19, p1_20, p1_21]
      - title: "Part 2: The Compiler"
        nodes: [p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7, p2_8, p2_8b,
                p2_9, p2_10, p2_11, p2_12, p2_13, p2_14, p2_15,
                p2_16, p2_17, p2_18, p2_19, p2_20, p2_21, p2_22,
                p2_23, p2_24, p2_25, p2_26, p2_27, p2_28]
      - title: "Part 3: The Consequences"
        nodes: [p3_1, p3_2, p3_3, p3_4, p3_5, p3_6, p3_7, p3_8, p3_9, p3_10]
---
```

Chapter labels come from the walk, not the node. A node can appear in different walks under different chapter names. The engine looks up which chapter a node belongs to based on the active walk.

### Context line (optional)

A blockquote immediately after the heading serves as the context line (shown when the node is not the first rendered):

```markdown
## p1_4 | The Dangling Reference
> One rejected program examined in detail.

`r` refers to `x`. The block ends...
```

### Complete example (3 nodes)

```markdown
---
walks:
  textbook:
    chapters:
      - title: "Part 1: The Problem"
        nodes: [p1_1, p1_2, p1_3]
---

## p1_1 | Why This Course Exists

This course exists because I cannot memorize rules and never could.

[Rust](https://www.rust-lang.org/) tutorials gave me rules such as "one mutable reference or many immutable." The [coherence constraint](!p1_8 "The same algebra governs hardware and software.") sits underneath these rules.

<!-- edges
-> p1_2 | What doesn't compile? | The compiler rejects code that looks right.
-> p1_5 | Why is memory hard? | Cores, caches, and stale data.
-->

## p1_2 | Programs That Don't Compile
> Most Rust syntax matches C++. But some code that compiles in C++ does not compile in Rust.

These programs contain memory bugs. Rust rejects them.

```rust
let v = vec![1, 2, 3];
let w = v;
println!("{:?}", v);
```

**error[E0382]:** borrow of moved value: `v`

The value moved. `v` is gone. This is [ownership](!p2_14 "Why does assigning a value invalidate the original?") in action.

<!-- edges
-> p1_3 | Does the compiler catch thread bugs too? | Does the type system prevent concurrency bugs?
-> p1_4 | What's a dangling reference? | C++ compiles it. Rust refuses.
-> p1_5 | Why do these bugs exist at all? | Three primitives rooted in physics.
-->

## p1_3 | Thread Safety Errors
> The compiler prevents data races too.

```rust
use std::thread;
fn main() {
    let v = vec![1, 2, 3];
    thread::spawn(|| {
        println!("{:?}", v);
    });
}
```

**error[E0373]:** closure may outlive the current function

<!-- edges
-> p1_4 | What does a dangling reference look like? | The same bug, Rust vs C++, side by side.
-> p1_5 | Why do all these bugs keep happening? | Three primitives rooted in physics.
-->
```

### How the engine processes it

A build script (or runtime loader) reads `course.md` and produces:
- `N` registry: node id → {title, ctx, body_html}
- `ED` registry: edge id → {from, to, t, w, card}
- `W` registry: walk id → {chapters: [{title, nodes}]}

Markdown body is converted to HTML. `[text](!id "tooltip")` links become EA anchors. `<!-- edges -->` blocks become card edges. YAML frontmatter becomes walk definitions.

### Edge design principles (from v1)

Each node should have:
- **2-4 card edges** (fan menu "next." choices): the textbook-walk successor + 2-3 alternatives that jump forward/sideways to related content
- **1-5 inline edges** (EA anchors in prose): cross-references to nodes the text mentions or builds on

Card edge `t` (link text) = a question the reader might ask after reading this node.
Card edge `w` (tooltip) = a short teaser question, not an answer.
Inline edge tooltip = a short question about the connection.

Like v1: edges support many theoretical walks. The textbook walk is one linear path. A reader exploring freely follows different card edges and builds a different document.

## Content Recovery from v1

v1 had 69 nodes and 158 edges. v2 has 58 nodes, new MESI content, but dropped several substantive sections. All must be restored — don't remove content, weave it in at the right place.

### Must Restore

| v1 node | Content | Where to weave in v2 |
|---|---|---|
| `other_langs` | How Other Languages Approach These Obstacles — Cyclone/ATS region annotations, Clang SA/Coverity/Infer static analyzers, GC, ownership. Five strategies with tradeoffs. | New node between P2.8 and P2.9. C++ programmers will ask "has anyone else tried this at compile time?" |
| `features` | Features as Interaction Solutions — full S×T×C feature table with historical dates (GC Lisp 1959, RAII C++ 1980s, lexical scope ALGOL 1958, ref counting 1960s, closures Lisp 1958, etc.). Feature-by-dimension breakdown. | Weave into P1.20 (Paradigms and Sync Strategies). P1.20 has paradigm-level view but drops the feature-by-feature S×T×C mapping with dates. |
| `familiar` | Familiar Syntax — hello world, arithmetic, add function, Vec, struct Point, while loop. Shows what works identically before showing what breaks. | New node between P1.1 and P1.2. P1.2 jumps straight to rejection with no warm-up. |
| `algo_patterns` | Algorithm Patterns — 5 patterns with full code: separate read/write passes, build new collections (functional), index-based graph BFS, drain and rebuild, swap to extract. | Expand P3.6 (Relaxing Time) with the missing BFS example and "build new collections" pattern. |
| `hints_patterns` | Hints-Patterns Correspondence Table — maps reactive hints to proactive patterns by dimension (Duplication→Clone&Diverge, Ownership Transfer→Message Passing, Reordering→Separate Read/Write, Partitioning→Split&Partition, Mutability Upgrade→Interior Mutability). | New closing node after P3.9, or weave into P3.8. |
| `cpp_threads_rule` | C++ comparison + threads — explicit C++ code showing push-while-borrowing compiles and is UB, side-by-side. Thread data races need the same rule. | Strengthen P2.28. The C++ UB examples are lighter in v2. |

### Review Recommendations

1. **Trim P1.10** (The Protocol) by ~15%. Some transitions are near-duplicates.
2. **Style check**: expand three-sentence punches at P1.8 and P1.21 ("Same constraint. Same algebra. Different names."). Check for "through" usage, passive voice.

## TODO

### Phase 0: Content recovery — DONE
- [x] Add p1_1b (familiar syntax warm-up) from v1
- [x] Weave v1 features table with dates into P1.20
- [x] Add p2_8b (other languages) from v1
- [x] Expand P3.6 with BFS + build-new-collections from v1
- [x] Add p3_10 (hints-patterns correspondence) from v1
- [x] Strengthen P2.28 with C++ UB side-by-side from v1
- [x] Trim P1.10 by ~15%

### Phase 1: Convert to course.md — DONE
- [x] Create course.md with YAML frontmatter
- [x] Convert all 61 nodes to `## id | Title` format
- [x] Apply content recovery edits

### Phase 2: Add edges — DONE
- [x] Card edges: 138 total (2-3 per node), first = textbook successor
- [x] Inline EA edges: 80 total, cross-references in prose
- [x] Validated: all 218 edge targets resolve to existing node IDs

### Phase 3: Build script — NEXT
- [ ] Write `build.js` (Node): parse `course.md` → `course.json`
- [ ] Adapt `engine.js` to load from `course.json` instead of N/ED/W JS globals
- [ ] Copy engine.js, style.css, index.html from learn-rust v1 as starting point
- [ ] `course.json` format: `{nodes: {id: {t, ctx, body_html}}, edges: [{from, to, t, w, card}], walks: {textbook: {chapters: [{title, nodes}]}}}`
- [ ] Markdown body → HTML conversion (marked.js or similar)
- [ ] `[text](!id "tooltip")` → EA anchor spans
- [ ] `<!-- edges -->` blocks → card edge objects

### Phase 4: Polish
- [ ] Test full textbook walk renders correctly
- [ ] Static export (full.html) via export.js
- [ ] Mobile styles + noscript fallback
- [ ] Validate edge coverage: every node reachable, no orphans

## Key Differences from learn-rust v1

| | learn-rust (v1) | learn-rust-2 |
|---|---|---|
| Content | 8 chapters, 69 nodes | 3 parts, ~61 nodes (58 new + 3 restored), coherence-first |
| Source format | JS files per chapter | Single `course.md` (markdown + graph) |
| Data format | `N.x={ch,t,b()}`, `ED.e={...}` | Parsed from markdown sections + edge comments |
| Chapter source | Baked into node (`n.ch`) | Walk metadata (YAML frontmatter) |
| New content | — | MESI protocol, coherence constraint, distributed systems, lineage |
| Restored | — | familiar syntax, other-languages approaches, features×S×T×C table, algo patterns, hints-patterns table, C++ UB comparison |

## Files

| File | Purpose | Status |
|---|---|---|
| `learn-rust-full-nodes.md` | Original source content (58 nodes, flat markdown) | exists, read-only |
| `course.md` | **THE source of truth** — 61 nodes + 218 edges + walk in one file | **DONE** (2137 lines) |
| `NEXT.md` | This file — context recovery | exists |
| `bootstrap.md` | Session startup instructions | exists |
| `course.json` | Build output: parsed course.md as JSON | **TODO** |
| `build.js` | Node script: parses course.md → course.json | **TODO** |
| `engine.js` | Interactive engine (adapt from v1 to load course.json) | **TODO** |
| `style.css` | Styles (copy from v1, adapt) | **TODO** |
| `index.html` | Shell (copy from v1, adapt) | **TODO** |

### v1 engine files (source to adapt)
Located at `/Users/jad_irred/repos/jad/jadnohra.com/site/learn-rust/`:
`engine.js`, `style.css`, `index.html`, `data.js`, `walks.js`, `nodes/ch01-08.js`
