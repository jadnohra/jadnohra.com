# Bootstrap — learn-rust-2

Read this file first. It has everything needed for a session. Do not read BOOTSTRAP-COMPACT.md or the old NEXT.md unless explicitly asked.

## Mission

Build `course.md` — a single markdown file that encodes a graph-structured Rust course. 61 nodes, ~200 card edges, ~250 inline edges, 1 textbook walk. The file renders as readable prose AND encodes the full interactive graph for the jadnohra.com/learn-rust reader.

## Audience

Experienced C++ programmers. They know pointers, move semantics, RAII, smart pointers, undefined behavior. Teach by diff. Skip what doesn't differ. Do not explain basic programming concepts.

## Pedagogy

Derive rules from the framework, never declare them. Build the problem until the rule falls out as the only possible answer. Framework: SPACE x TIME x COORDINATES. Every memory bug is a coherence failure between these primitives.

## Current State

| File | What |
|---|---|
| `learn-rust-full-nodes.md` | 58 nodes as flat markdown. No edges, no walks, no graph format. Source content. |
| `NEXT.md` | Full context recovery: node inventory, format spec, content recovery plan, TODO phases. |
| `course.md` | **Does not exist yet.** This is what we're building. |

Old course (v1) lives at `/Users/jad_irred/repos/jad/jadnohra.com/site/learn-rust/` — 69 nodes, 158 edges, JS format. Read node files (`nodes/ch01.js` through `ch08.js`) when restoring content or studying edge patterns.

## course.md Format

### Nodes = `## id | Title`

```markdown
## p1_1 | Why This Course Exists

This course exists because I cannot memorize rules...
```

Everything below `##` until the next `##` is the body. Standard markdown.

### Context line = blockquote after heading

```markdown
## p1_4 | The Dangling Reference
> One rejected program examined in detail.

`r` refers to `x`...
```

Shown when the node is not the first rendered. One sentence positioning the reader.

### Card edges = `<!-- edges -->` comment block after each node

```markdown
<!-- edges
-> p1_2 | What doesn't compile? | The compiler rejects code that looks right.
-> p1_5 | Why is memory hard? | Three primitives rooted in physics.
-->
```

Format: `-> target_id | link_text | tooltip_question`
- `from` is implicit (the preceding node)
- 2-4 per node. First card edge = textbook walk successor.
- `link_text` = a question the reader might ask after reading this node
- `tooltip_question` = a short teaser, not an answer

### Inline edges = `[text](!id "tooltip")` links in prose

```markdown
The [coherence constraint](!p1_8 "The same algebra governs hardware and software.") sits underneath.
```

- Renders as underlined text with tooltip on hover. Click navigates to target.
- Regular markdown links `[text](url)` stay as regular links.
- 1-5 per node. Cross-references to nodes the text mentions or builds on.

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

Chapter labels come from the walk, not the node. The engine looks up which chapter a node belongs to based on the active walk.

## Content Recovery (from v1)

Don't remove content. Weave it in at the right place.

| New ID | Title | Source | Placement |
|---|---|---|---|
| p1_1b | Familiar Syntax | v1 `familiar` | Between P1.1 and P1.2. Hello world, structs, loops — warm-up before rejection. |
| (expand P1.20) | Features x S*T*C table with dates | v1 `features` | Weave into P1.20. GC Lisp 1959, RAII C++ 1980s, lexical scope ALGOL 1958, etc. |
| p2_8b | How Other Languages Approach These Obstacles | v1 `other_langs` | Between P2.8 and P2.9. Cyclone/ATS, Clang SA/Coverity/Infer, GC, ownership. |
| (expand P3.6) | Algorithm patterns | v1 `algo_patterns` | Add BFS with arena indices, build-new-collections functional pattern to P3.6. |
| p3_10 | Hints and Patterns | v1 `hints_patterns` | After P3.9. Maps reactive hints to proactive patterns by dimension. |
| (expand P2.28) | C++ UB side-by-side | v1 `cpp_threads_rule` | Strengthen P2.28 with explicit C++ code that compiles and is UB. |

Also: trim P1.10 (The Protocol) by ~15% — merge near-duplicate write transitions.

## Edge Design (from v1 — replicate this density)

v1 had 158 edges across 69 nodes (~2.3 edges/node). Two types:

**Card edges** (fan menu): question the reader asks after finishing the node. The textbook-walk successor is always one of them. 2-3 alternatives jump forward/sideways to related content. This enables many theoretical walks — a reader exploring freely follows different card edges and builds a different document.

**Inline edges** (EA anchors): cross-references inside the prose. Underlined words with tooltips. When the text mentions a concept explained elsewhere, link it. When the text says "the coherence constraint," link to the node that derives it.

Study the v1 edge patterns in `nodes/ch01.js` through `ch08.js` for density and style. The edges were great — plentiful, supporting many theoretical walks, enabling nice navigation.

## Style (compressed)

Inspired by Steven Pinker. Smart reader who doesn't know this particular thing. Professorial warmth. Slightly elevated vocabulary. Longer sentences with clear syntax. Dry wit through word choice.

**Two modes.** Journey mode: "we analyze..." (real shared activity). Description mode: "Rust requires..." (thing as subject).

**Section titles.** Active or questioning. Never topic labels or slogans.

**20 forbidden patterns** (full tables in `/Users/jad_irred/repos/jad/abstractor-learn-rust/course/common-v1/STYLE.md` — read on demand):

1. Punctuation crutches (em dashes, semicolons, colons before explanations)
2. Contrastive framing ("not X but Y", "X, not Y")
3. False signposting ("the key insight is", "in other words")
4. Hedging ("essentially", "basically", "actually")
5. Assumed agreement ("of course", "naturally", "obviously")
6. Rhetorical questions (question then answer)
7. False collaboration ("let's examine", "we can see that")
8. Emphasis flags ("in fact", "note that", "importantly")
9. Inverted conditional (main clause leads, not "When X, Y")
10. Passive voice hiding agent (name the actor)
11. Throat-clearing ("it turns out that")
12. Meta-commentary ("this section covers")
13. Filler intensifiers ("very", "just", "simply")
14. Nominalizations (use verb forms)
15. Distancing language ("one might say")
16. False precision ("exactly" when not exact)
17. Anticipatory "it" ("It is X that Y")
18. Pretentious naming ("call this X")
19. Ungrounded abstractions (ground questions in scenarios)
20. Language reference voice (no "through" — use "at" or show code)

**Top pitfalls:** "X, not Y" contrastive framing. Passive voice. Colons before explanations. Short punchy dramatic sentences. Inverted conditionals. "through."

**Gold style reference:** read ch03 from the old course (`/Users/jad_irred/repos/jad/abstractor-learn-rust/course/chapters-v4/ch03.md`) before writing prose.

## TODO

### Phase 0: Content recovery
- [ ] Add p1_1b (familiar syntax warm-up) from v1
- [ ] Weave v1 features table with dates into P1.20
- [ ] Add p2_8b (other languages) from v1
- [ ] Expand P3.6 with BFS + build-new-collections from v1
- [ ] Add p3_10 (hints-patterns correspondence) from v1
- [ ] Strengthen P2.28 with C++ UB side-by-side from v1
- [ ] Trim P1.10 by ~15%

### Phase 1: Convert to course.md
- [ ] Create course.md with YAML frontmatter
- [ ] Convert all ~61 nodes to `## id | Title` format
- [ ] Apply content recovery edits

### Phase 2: Add edges
- [ ] Card edges: 2-4 per node (~150-200 total)
- [ ] Inline EA edges: 1-5 per node (~200-300 total)
- [ ] Validate: all targets exist, all links resolve

### Phase 3: Build script
- [ ] Write build.js: parse course.md -> N/ED/W registries
- [ ] Adapt engine.js for walk-based chapters
- [ ] Copy style.css, index.html from v1

### Phase 4: Polish
- [ ] Test textbook walk
- [ ] Static export (full.html)
- [ ] Mobile + noscript
- [ ] Validate edge coverage

## Key Files

| File | Location |
|---|---|
| This bootstrap | `site/learn-rust-2/bootstrap.md` |
| Context recovery | `site/learn-rust-2/NEXT.md` |
| Source content | `site/learn-rust-2/learn-rust-full-nodes.md` |
| Old course nodes | `site/learn-rust/nodes/ch01.js` through `ch08.js` |
| Old course engine | `site/learn-rust/engine.js` |
| Old course walks | `site/learn-rust/walks.js` |
| Style guide (full) | `abstractor-learn-rust/course/common-v1/STYLE.md` |
| Gold style reference | `abstractor-learn-rust/course/chapters-v4/ch03.md` |
