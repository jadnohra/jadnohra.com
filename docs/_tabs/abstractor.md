---
layout: post
title: Abstractor
icon: fas fa-layer-group
order: 3
toc: true
---

<style>
/* Compact layout */
.content p { margin-bottom: 0.5rem; }
.content h2 { margin-top: 1.5rem; margin-bottom: 0.5rem; display: block !important; visibility: visible !important; }
.content h3 { margin-top: 1rem; margin-bottom: 0.3rem; display: block !important; visibility: visible !important; }
.content ul, .content ol { margin-bottom: 0.5rem; }
.content li { margin-bottom: 0.2rem; }
.content blockquote { margin: 0.5rem 0; }
.content hr { margin: 1rem 0; }

/* System Primitives */
#system-primitives {
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 14px;
  margin-bottom: 2rem;
}
.primitives-intro {
  margin-bottom: 1rem;
  line-height: 1.6;
}
.category-card {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  margin-bottom: 8px;
  overflow: hidden;
  border-left: 3px solid var(--category-color, #64748b);
}
.category-header {
  padding: 14px 16px;
  font-weight: 600;
  color: var(--category-color, #e2e8f0);
  font-size: 15px;
  border-bottom: 1px solid rgba(255,255,255,0.05);
}
.primitive-item {
  padding: 6px 16px;
  border-bottom: 1px solid rgba(255,255,255,0.03);
}
.primitive-item:last-child {
  border-bottom: none;
}
.primitive-item.has-explanation {
  cursor: pointer;
}
.primitive-item.has-explanation:hover {
  background: rgba(255,255,255,0.02);
}
.primitive-name {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  font-weight: 600;
  font-size: 14px;
  color: #f1f5f9;
  transition: color 0.15s;
}
.primitive-item:hover .primitive-name {
  color: var(--category-color);
}
.primitive-item:hover .primitive-implication {
  color: var(--category-color);
  filter: brightness(1.4) saturate(0.7);
}
.primitive-name .expand-icon {
  font-size: 10px;
  color: #64748b;
  transition: transform 0.2s;
}
.primitive-item.expanded .primitive-name .expand-icon {
  transform: rotate(90deg);
}
.primitive-implications {
  margin-top: 4px;
  padding-left: 12px;
}
.primitive-implication {
  color: #e2e8f0;
  font-size: 13px;
  line-height: 1.7;
}
.primitive-implication::before {
  content: "→ ";
  color: var(--category-color);
}
/* Popover bubble */
.primitive-popover {
  position: fixed;
  max-width: 520px;
  padding: 14px 16px;
  background: #0f172a;
  border: 1px solid var(--popover-color, #475569);
  border-radius: 6px;
  color: #e2e8f0;
  font-size: 15px;
  line-height: 1.7;
  z-index: 9999;
  pointer-events: none;
  opacity: 0;
  transition: opacity 0.12s ease;
  box-shadow: 0 8px 32px rgba(0,0,0,0.5);
  white-space: pre-line;
}
.primitive-popover.visible {
  opacity: 1;
}

/* Tradeoff Cards */
#tradeoff-cards {
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 14px;
  margin-bottom: 2rem;
}
.tradeoff-grid {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
}
.tradeoff-item {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 6px;
  padding: 8px 12px;
  cursor: pointer;
  display: flex;
  align-items: center;
  gap: 8px;
  transition: all 0.15s;
  border: 1px solid transparent;
}
.tradeoff-item:hover {
  background: linear-gradient(135deg, #1e293b 0%, #334155 100%);
  border-color: rgba(255,255,255,0.2);
}
.tradeoff-name {
  color: #f1f5f9;
  font-weight: 500;
}
.tradeoff-count {
  color: #64748b;
  font-size: 11px;
  background: rgba(255,255,255,0.05);
  padding: 2px 6px;
  border-radius: 3px;
}
.tradeoff-popover {
  position: fixed;
  max-width: 520px;
  padding: 14px 16px;
  background: #0f172a;
  border: 1px solid #475569;
  border-radius: 6px;
  color: #e2e8f0;
  font-size: 14px;
  line-height: 1.6;
  z-index: 9999;
  pointer-events: none;
  opacity: 0;
  transition: opacity 0.12s ease;
  box-shadow: 0 8px 32px rgba(0,0,0,0.5);
}
.tradeoff-popover.visible {
  opacity: 1;
}
.tradeoff-popover-title {
  font-weight: 600;
  font-size: 15px;
  color: #f1f5f9;
  margin-bottom: 12px;
  padding-bottom: 8px;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.tradeoff-popover-item {
  margin-bottom: 10px;
  font-size: 13px;
  line-height: 1.6;
}
.tradeoff-popover-item:last-child {
  margin-bottom: 0;
}
.tradeoff-popover-item b {
  color: #94a3b8;
}

/* Derived Data */
.derived-intro {
  font-size: 16px;
  color: #f1f5f9;
  margin-bottom: 1rem;
  line-height: 1.6;
}
.derived-intro b {
  color: #f97316;
}
.derived-box {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 6px;
  padding: 12px 16px;
  margin: 1rem 0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 13px;
  color: #e2e8f0;
  border-left: 3px solid #f97316;
}
.derived-box b {
  color: #f97316;
}
.derived-grid {
  display: flex;
  flex-direction: column;
  gap: 12px;
  margin: 1.5rem 0;
}
.derived-card {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px 18px;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 13px;
}
.derived-card-title {
  font-weight: 600;
  color: #f1f5f9;
  margin-bottom: 12px;
  font-size: 14px;
}
.derived-card-item {
  color: #e2e8f0;
  margin-bottom: 8px;
  line-height: 1.6;
}
.derived-card-item b {
  color: #94a3b8;
}
.derived-explains {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin: 1rem 0;
}
.derived-explains span {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 4px;
  padding: 6px 10px;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  color: #cbd5e1;
}
.derived-list {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px 16px 16px 36px;
  margin: 1rem 0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 13px;
  list-style: disc;
}
.derived-list > li {
  color: #f1f5f9;
  margin-bottom: 12px;
  line-height: 1.6;
  list-style: none;
  margin-left: -18px;
}
.derived-list > li:last-child {
  margin-bottom: 0;
}
.derived-list ul {
  margin-top: 6px;
  padding-left: 20px;
  list-style: disc;
}
.derived-list ul li {
  color: #cbd5e1;
  margin-bottom: 4px;
  line-height: 1.5;
}
.derived-list b {
  color: #f1f5f9;
}
.derived-list ul b {
  color: #94a3b8;
}

/* Derived Data Table */
.derived-table {
  width: 100%;
  border-collapse: collapse;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  margin: 1rem 0;
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  overflow: hidden;
}
.derived-table th {
  background: rgba(255,255,255,0.05);
  color: #f1f5f9;
  font-weight: 600;
  padding: 10px 12px;
  text-align: left;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.derived-table td {
  padding: 8px 12px;
  color: #cbd5e1;
  border-bottom: 1px solid rgba(255,255,255,0.03);
}
.derived-table tr:last-child td {
  border-bottom: none;
}
.derived-table tr:hover td {
  background: rgba(255,255,255,0.02);
}

#concept-grid {
  position: relative;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  overflow-x: auto;
}
#concept-grid svg {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  pointer-events: none;
  z-index: 10;
}
.grid-container {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(140px, 1fr));
  gap: 2px;
}
.layer-group {
  background: rgba(255,255,255,0.03);
  border-radius: 4px;
  padding: 8px;
  min-width: 140px;
}
.layer-header {
  font-size: 10px;
  font-weight: 600;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-bottom: 6px;
  padding-bottom: 4px;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.concept {
  padding: 3px 6px;
  margin: 2px 0;
  border-radius: 3px;
  cursor: pointer;
  transition: all 0.15s ease;
  color: #64748b;
  background: transparent;
  border: 1px solid transparent;
}
.concept:hover {
  background: rgba(255,255,255,0.05);
}
.concept.selected {
  background: #f97316;
  color: #fff;
  font-weight: 600;
}
.concept.active {
  color: #e2e8f0;
  border-color: currentColor;
}
.concept.inactive {
  opacity: 0.15;
}
.layer-group.highlighted {
  border: 1px solid currentColor;
  box-shadow: 0 0 8px currentColor;
}

/* Layer Cards */
#layer-cards {
  margin-top: 3rem;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
}
.layer-card {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  margin-bottom: 8px;
  overflow: hidden;
  border-left: 3px solid var(--domain-color, #64748b);
}
.card-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 12px 16px;
  cursor: pointer;
  user-select: none;
}
.card-header:hover {
  background: rgba(255,255,255,0.03);
}
.card-title {
  display: flex;
  align-items: center;
  gap: 12px;
}
.card-name {
  font-weight: 600;
  color: var(--domain-color, #e2e8f0);
}
.card-level {
  font-size: 10px;
  color: #64748b;
  background: rgba(255,255,255,0.05);
  padding: 2px 6px;
  border-radius: 3px;
}
.card-stats {
  display: flex;
  gap: 12px;
  font-size: 10px;
  color: #64748b;
}
.card-stat {
  display: flex;
  align-items: center;
  gap: 4px;
}
.card-stat-dot {
  width: 6px;
  height: 6px;
  border-radius: 50%;
}
.card-toggle {
  color: #64748b;
  transition: transform 0.2s;
}
.layer-card.expanded .card-toggle {
  transform: rotate(180deg);
}
.card-body {
  display: none;
  padding: 0 16px 16px 16px;
  border-top: 1px solid rgba(255,255,255,0.05);
}
.layer-card.expanded .card-body {
  display: block;
}
.card-section {
  margin-top: 12px;
}
.card-section-title {
  font-size: 10px;
  font-weight: 600;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-bottom: 8px;
  display: flex;
  align-items: center;
  gap: 6px;
}
.section-abstracts .card-section-title { color: #f97316; }
.section-exposes .card-section-title { color: #22c55e; }
.section-leaks .card-section-title { color: #ef4444; }
.section-escapes .card-section-title { color: #a855f7; }
.card-items {
  display: flex;
  flex-wrap: wrap;
  gap: 6px;
}
.card-item {
  background: rgba(255,255,255,0.05);
  padding: 4px 10px;
  border-radius: 4px;
  color: #94a3b8;
  font-size: 11px;
  cursor: default;
  transition: all 0.15s;
  border: 1px solid transparent;
}
.card-item:hover {
  background: rgba(255,255,255,0.08);
  color: #e2e8f0;
}
.card-item[title]:hover {
  border-color: rgba(255,255,255,0.1);
}
.card-item.item-abstract { border-left: 2px solid #f97316; }
.card-item.item-expose { border-left: 2px solid #22c55e; }
.card-item.item-leak { border-left: 2px solid #ef4444; }
.card-item.item-escape { border-left: 2px solid #a855f7; }
.card-from {
  font-size: 9px;
  color: #64748b;
  margin-left: 4px;
}

/* Engineers Bar */
#engineers-bar {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px 8px 0 0;
  margin-bottom: 0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
}
#engineers-bar.expanded + #concept-grid {
  border-radius: 0 0 8px 8px;
}
.engineers-header {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 10px 16px;
  cursor: pointer;
  user-select: none;
  color: #64748b;
}
.engineers-header:hover {
  color: #94a3b8;
}
.engineers-toggle {
  transition: transform 0.2s;
}
#engineers-bar.expanded .engineers-toggle {
  transform: rotate(180deg);
}
.engineers-content {
  display: none;
  padding: 0 16px 12px 16px;
  border-top: 1px solid rgba(255,255,255,0.05);
}
#engineers-bar.expanded .engineers-content {
  display: flex;
  align-items: center;
  gap: 10px;
  flex-wrap: wrap;
}
.mode-toggle {
  background: rgba(255,255,255,0.05);
  border: 1px solid rgba(255,255,255,0.1);
  border-radius: 4px;
  padding: 6px 10px;
  cursor: pointer;
  color: #94a3b8;
  font-size: 14px;
  font-family: inherit;
  transition: all 0.15s;
  min-width: 32px;
  text-align: center;
}
.mode-toggle:hover {
  background: rgba(255,255,255,0.08);
  color: #e2e8f0;
}
.mode-toggle.union {
  color: #22c55e;
  border-color: #22c55e;
}
.mode-toggle.intersection {
  color: #f97316;
  border-color: #f97316;
}
.profile-btn {
  background: rgba(255,255,255,0.05);
  border: 1px solid transparent;
  border-radius: 4px;
  padding: 6px 12px;
  cursor: pointer;
  color: #64748b;
  font-size: 11px;
  font-family: inherit;
  transition: all 0.15s;
  display: flex;
  align-items: center;
  gap: 6px;
}
.profile-btn:hover {
  background: rgba(255,255,255,0.08);
  color: #94a3b8;
}
.profile-btn.selected {
  background: rgba(59, 130, 246, 0.2);
  border-color: #3b82f6;
  color: #e2e8f0;
}

/* Engineer mode concept styling */
.concept.engineer-highlight {
  border-color: transparent;
}

/* Visual separation between engineers bar and grid */
#engineers-bar {
  border-bottom: 1px solid rgba(255,255,255,0.1);
  margin-bottom: 2px;
}
#engineers-bar.expanded {
  border-bottom: none;
  margin-bottom: 0;
}

/* Language Derived Data Section */
.lang-derived-popover {
  position: fixed;
  max-width: 520px;
  padding: 14px 16px;
  background: #0f172a;
  border: 1px solid #475569;
  border-radius: 6px;
  color: #e2e8f0;
  font-size: 14px;
  line-height: 1.7;
  z-index: 9999;
  pointer-events: none;
  opacity: 0;
  transition: opacity 0.12s ease;
  box-shadow: 0 8px 32px rgba(0,0,0,0.5);
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
}
.lang-derived-popover.visible {
  opacity: 1;
}
.lang-derived-popover-title {
  font-weight: 600;
  font-size: 15px;
  color: #f1f5f9;
  margin-bottom: 4px;
}
.lang-derived-popover-section {
  font-size: 11px;
  color: #64748b;
  margin-bottom: 10px;
  padding-bottom: 8px;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.lang-derived-popover-content {
  color: #cbd5e1;
  font-size: 13px;
}

/* Hoverable elements */
[data-hover].hoverable {
  color: #f97316;
  cursor: help;
  border-bottom: 1px dotted #f97316;
  transition: all 0.15s;
}
[data-hover].hoverable:hover {
  color: #fb923c;
  border-bottom-style: solid;
}

/* Triangle diagram */
.triangle-diagram {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  overflow-x: auto;
}
.triangle-diagram pre {
  margin: 0;
  color: #f97316;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  line-height: 1.4;
}

/* Tradeoff spectrum diagram */
.tradeoff-spectrum {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  overflow-x: auto;
}
.tradeoff-spectrum pre {
  margin: 0;
  color: #e2e8f0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  line-height: 1.5;
}
</style>

<div></div>

## System Primitives

These constraints are rooted in physics and fundamental CS. They apply at every layer. Abstractions can defer them, transform them, or trade one for another — but never eliminate them.

<div id="system-primitives"></div>

<script src="{{ '/assets/js/primitives.js' | relative_url }}"></script>

---

## Derived Data - Systems

<div class="derived-box">
<b>Physics creates distance.</b> Distance forces copies. <b>Copies require coherence.</b>
</div>

Computation and storage are separated by distance — in the memory hierarchy, across the network, even in time. When crossing that distance repeatedly is too expensive, you store a copy closer. That copy is **derived data**.

<div class="derived-box">
Source → transform → store closer → faster access → two representations → <b>sync obligation</b>
</div>

Every cache, replica, index, materialized view, denormalized table, and memoized result is the same pattern: **store a transform of source data closer to consumption. Pay for sync.**

### The Three Choices

<ul class="derived-list">
<li><b>1. What transform?</b>
  <ul>
    <li><b>Identity</b> → CPU cache, CDN, replica</li>
    <li><b>Projection</b> → covering index, column store</li>
    <li><b>Aggregation</b> → materialized view, rollup</li>
    <li><b>Structure change</b> → B-tree, hash, inverted index</li>
    <li><b>Lossy</b> → bloom filter, HyperLogLog, sketch</li>
  </ul>
</li>
<li><b>2. Where to store?</b>
  <ul>
    <li><b>Memory</b> → register → L1 → L2 → L3 → RAM → SSD → disk</li>
    <li><b>Network</b> → same-process → machine → rack → DC → region → edge</li>
    <li><b>Time</b> → precomputed → on-demand → lazy</li>
  </ul>
</li>
<li><b>3. How to sync?</b>
  <ul>
    <li><b>Sync on write</b> → strong consistency, write pays RTT</li>
    <li><b>Invalidate</b> → strong consistency, invalidation fanout</li>
    <li><b>TTL</b> → bounded staleness, no coordination</li>
    <li><b>Version on read</b> → strong consistency, read pays check</li>
    <li><b>Never</b> → immutable source, no cost</li>
  </ul>
</li>
</ul>

### Unification

<table class="derived-table">
<tr><th>Name</th><th>Transform</th><th>Location</th><th>Sync</th></tr>
<tr><td>CPU L1 cache</td><td>identity</td><td>RAM → L1</td><td>coherence protocol</td></tr>
<tr><td>CDN</td><td>identity</td><td>origin → edge</td><td>TTL / purge</td></tr>
<tr><td>Redis cache</td><td>identity / projection</td><td>disk → RAM</td><td>TTL / invalidate</td></tr>
<tr><td>Database replica</td><td>identity</td><td>primary → secondary</td><td>sync / async</td></tr>
<tr><td>Materialized view</td><td>aggregation</td><td>compute → storage</td><td>refresh / periodic</td></tr>
<tr><td>Index</td><td>projection + structure</td><td>scan → lookup</td><td>sync on write</td></tr>
<tr><td>Denormalized table</td><td>join</td><td>join-time → storage</td><td>dual write</td></tr>
<tr><td>Memoization</td><td>full result</td><td>compute → lookup</td><td>none (pure)</td></tr>
<tr><td>Bloom filter</td><td>lossy projection</td><td>set → bits</td><td>rebuild</td></tr>
</table>

### What This Explains

<ul class="derived-list">
<li><b>Cache invalidation is hard</b> — it's distributed coordination</li>
<li><b>Immutability is powerful</b> — no sync needed, copies are forever valid</li>
<li><b>Indexes slow writes</b> — sync obligation on every mutation</li>
<li><b>Eventual consistency exists</b> — coordination is expensive, defer it</li>
<li><b>CDNs use TTL</b> — bounded staleness avoids coordination</li>
<li><b>Denormalization is dangerous</b> — multiple sync points, easy to diverge</li>
<li><b>Memoization is easy</b> — pure functions have immutable inputs</li>
</ul>

---

## Concrete Tradeoff Examples

Real-world technology choices mapped to the primitives above. Hover to see which constraints apply.

<div id="tradeoff-cards"></div>

<script src="{{ '/assets/js/tradeoffs.js' | relative_url }}"></script>

---

## Derived Data - Languages

<div class="derived-box">
<b>Physics creates distance.</b> Distance forces copies. <b>Copies require coherence.</b>
</div>

This pattern applies at every layer of the stack.

<div class="derived-box">
<b>SYSTEM:</b>&nbsp;&nbsp;&nbsp;Source DB → replica → CDN edge → browser cache<br>
<b>LANGUAGE:</b>&nbsp;Value → alias → copy → register
</div>

### Isomorphism

<table class="derived-table">
<tr><th>Derived Data (System)</th><th>Derived Data (Language)</th></tr>
<tr><td>Source</td><td>Original value / memory location</td></tr>
<tr><td>Transform</td><td>Copy / reference / move / borrow</td></tr>
<tr><td>Store closer</td><td>Register, stack, local variable, cache line</td></tr>
<tr><td>Two representations</td><td>Multiple references to same data</td></tr>
<tr><td>Sync obligation</td><td>Coherence: who can read/write when?</td></tr>
</table>

### Layers

<table class="derived-table">
<tr><th>Layer</th><th>Source</th><th>Derived Copy</th><th>Sync Strategy</th></tr>
<tr><td><span data-hover="layer-cpu-cache">CPU cache</span></td><td>RAM</td><td>L1/L2/L3 line</td><td>MESI protocol</td></tr>
<tr><td><span data-hover="layer-compiler">Compiler</span></td><td>Memory</td><td>Register</td><td>Register allocation</td></tr>
<tr><td><span data-hover="layer-language">Language</span></td><td>Original binding</td><td>Alias / reference</td><td>Borrow checker / locks / GC</td></tr>
<tr><td><span data-hover="layer-thread">Thread</span></td><td>Shared heap</td><td>Thread-local</td><td>Mutex / channels / atomics</td></tr>
<tr><td><span data-hover="layer-process">Process</span></td><td>Shared memory</td><td>Process-local</td><td>IPC / message passing</td></tr>
<tr><td><span data-hover="layer-database">Database</span></td><td>Primary</td><td>Replica</td><td>Replication protocol</td></tr>
<tr><td><span data-hover="layer-network">Network</span></td><td>Origin server</td><td>CDN edge</td><td>TTL / invalidation</td></tr>
<tr><td><span data-hover="layer-geo">Geo-distributed</span></td><td>Region A</td><td>Region B</td><td>Eventual consistency</td></tr>
</table>

### Triangle

Three primitives rooted in physics:

<div class="triangle-diagram">
<pre>
                      TIME
                     (when)
                       △
                      /|\
                     / | \
                    /  |  \
                   / STATE \
                  /    |    \
                 /     |     \
                /      |      \
             SPACE ----+---- IDENTITY
            (where)         (which)
</pre>
</div>

<table class="derived-table">
<tr><th>Primitive</th><th>Physical Root</th><th>Question</th><th>System Equivalent</th></tr>
<tr><td><span data-hover="primitive-space">SPACE</span></td><td>Locality, memory hierarchy</td><td>Where does data reside?</td><td>Source vs edge location</td></tr>
<tr><td><span data-hover="primitive-time">TIME</span></td><td>Causality, sequence</td><td>When does it exist/change?</td><td>Sync timing</td></tr>
<tr><td><span data-hover="primitive-identity">IDENTITY</span></td><td>Equivalence, sameness</td><td>Are these the same data?</td><td>Replica or independent?</td></tr>
</table>

<span data-hover="primitive-state">STATE</span> emerges from the triangle:

<div class="derived-box">
STATE = f(SPACE, TIME)
</div>

### Coherence Problem

Three ingredients:

<div class="derived-box" style="font-family: monospace; white-space: pre-line;">
<b>SHARED IDENTITY</b>     Multiple paths to "the same" data
        +
<b>MULTIPLE SPACES</b>     Data exists in more than one location
        +
<b>TIME FLOWS</b>          Mutation can occur
        =
<b>COHERENCE PROBLEM</b>   Which STATE is true? How to sync?
</div>

Remove any one:

<table class="derived-table">
<tr><th>Remove</th><th>Strategy</th><th>System Equivalent</th></tr>
<tr><td><span data-hover="remove-identity">Shared Identity</span></td><td>Value semantics, deep copy</td><td>Independent caches</td></tr>
<tr><td><span data-hover="remove-space">Multiple Spaces</span></td><td>Single source of truth</td><td>No replication</td></tr>
<tr><td><span data-hover="remove-time">Time flows</span></td><td>Immutability</td><td>Immutable source</td></tr>
</table>

### Operations

<table class="derived-table">
<tr><th>Operation</th><th>Language Level</th><th>System Equivalent</th></tr>
<tr><td><span data-hover="op-read">Read</span></td><td>Observe value</td><td>Cache hit / DB read</td></tr>
<tr><td><span data-hover="op-write">Write</span></td><td>Mutate value</td><td>Write to primary</td></tr>
<tr><td><span data-hover="op-copy">Copy</span></td><td>Create independent duplicate</td><td>Fork / snapshot</td></tr>
<tr><td><span data-hover="op-move">Move</span></td><td>Transfer ownership, invalidate source</td><td>Migration</td></tr>
<tr><td><span data-hover="op-alias">Alias</span></td><td>Second reference to same location</td><td>Multiple replicas</td></tr>
<tr><td><span data-hover="op-sync">Sync</span></td><td>Reconcile divergent copies</td><td>Replication protocol</td></tr>
</table>

### Sync Strategies

<table class="derived-table">
<tr><th>Strategy</th><th>System Level</th><th>Language Level</th></tr>
<tr><td><span data-hover="sync-forbid">Forbid the problem</span></td><td>Single region</td><td>Ownership (move semantics)</td></tr>
<tr><td><span data-hover="sync-freeze">Freeze time</span></td><td>Immutable source</td><td>Immutable bindings</td></tr>
<tr><td><span data-hover="sync-serialize">Serialize access</span></td><td>Distributed lock</td><td>Mutex, RwLock</td></tr>
<tr><td><span data-hover="sync-hardware">Hardware arbitration</span></td><td>—</td><td>Atomics, CAS</td></tr>
<tr><td><span data-hover="sync-compile">Compile-time proof</span></td><td>—</td><td>Borrow checker</td></tr>
<tr><td><span data-hover="sync-cow">Copy-on-write</span></td><td>CoW filesystem</td><td>CoW data structures</td></tr>
<tr><td><span data-hover="sync-message">Message passing</span></td><td>Event sourcing</td><td>Channels</td></tr>
<tr><td><span data-hover="sync-optimistic">Optimistic</span></td><td>MVCC</td><td>STM, persistent structures</td></tr>
<tr><td><span data-hover="sync-trust">Trust the user</span></td><td>—</td><td><code>unsafe</code>, raw pointers</td></tr>
</table>

### Language Choices

<table class="derived-table">
<tr><th>Language</th><th>IDENTITY</th><th>TIME</th><th>Coherence</th></tr>
<tr><td><span data-hover="lang-haskell">Haskell</span></td><td>Shared freely</td><td>Frozen</td><td>No mutation → no problem</td></tr>
<tr><td><span data-hover="lang-erlang">Erlang</span></td><td>Process isolation</td><td>Frozen + messages</td><td>Copy between processes</td></tr>
<tr><td><span data-hover="lang-clojure">Clojure</span></td><td>Shared freely</td><td>Frozen</td><td>Persistent structures</td></tr>
<tr><td><span data-hover="lang-rust">Rust</span></td><td>Ownership + borrowing</td><td>Controlled</td><td>Compile-time proof</td></tr>
<tr><td><span data-hover="lang-go">Go</span></td><td>Shared</td><td>Free</td><td>Channels or locks</td></tr>
<tr><td><span data-hover="lang-java">Java</span></td><td>Shared</td><td>Free</td><td>Locks / volatile</td></tr>
<tr><td><span data-hover="lang-c">C/C++</span></td><td>Unrestricted</td><td>Free</td><td>Programmer responsibility</td></tr>
<tr><td><span data-hover="lang-js">JavaScript</span></td><td>Shared</td><td>Free</td><td>Single-threaded</td></tr>
<tr><td><span data-hover="lang-python">Python</span></td><td>Shared</td><td>Free</td><td>GIL</td></tr>
</table>

### Language Constructs

<table class="derived-table">
<tr><th>Construct</th><th>TIME</th><th>SPACE</th><th>IDENTITY</th><th>Coherence</th></tr>
<tr><td><span data-hover="construct-register">Register</span></td><td>Runtime</td><td>Register</td><td>Unique</td><td>N/A</td></tr>
<tr><td><span data-hover="construct-stack">Stack variable</span></td><td>Runtime</td><td>Stack</td><td>Scoped</td><td>Lexical scope</td></tr>
<tr><td><span data-hover="construct-heap">Heap allocation</span></td><td>Runtime</td><td>Heap</td><td>Reference(s)</td><td>Manual / GC / ownership</td></tr>
<tr><td><span data-hover="construct-const">Compile-time const</span></td><td>Compile</td><td>Inlined</td><td>N/A</td><td>None needed</td></tr>
<tr><td><span data-hover="construct-static">Static / global</span></td><td>Program lifetime</td><td>Data segment</td><td>Global</td><td>Atomic / lock / immutable</td></tr>
<tr><td><span data-hover="construct-tls">Thread-local</span></td><td>Runtime</td><td>Per-thread</td><td>Thread-scoped</td><td>No sharing</td></tr>
<tr><td><span data-hover="construct-immutable">Immutable value</span></td><td>Frozen</td><td>Any</td><td>Shared freely</td><td>Frozen → valid</td></tr>
<tr><td><span data-hover="construct-locked">Mutable + locked</span></td><td>Serialized</td><td>Heap</td><td>Shared</td><td>Mutex</td></tr>
<tr><td><span data-hover="construct-atomic">Atomic</span></td><td>Hardware</td><td>RAM</td><td>Shared</td><td>Hardware coherence</td></tr>
<tr><td><span data-hover="construct-channel">Channel message</span></td><td>Runtime</td><td>Copied</td><td>Transferred</td><td>No shared state</td></tr>
</table>

### Examples

<ul class="derived-list">
<li><span data-hover="example-immutable"><b>Immutability enables safe sharing</b></span> — frozen TIME allows IDENTITY to span SPACEs</li>
<li><span data-hover="example-rust-concurrency"><b>Rust's fearless concurrency</b></span> — compile-time proof of IDENTITY rules</li>
<li><span data-hover="example-locks-slow"><b>Locks are slow</b></span> — serialize TIME globally, threads wait</li>
<li><span data-hover="example-lockfree"><b>Lock-free is hard</b></span> — hardware IDENTITY arbitration is subtle</li>
<li><span data-hover="example-gc-races"><b>GC doesn't prevent data races</b></span> — GC manages SPACE, not IDENTITY+TIME</li>
<li><span data-hover="example-js-single"><b>JavaScript is single-threaded</b></span> — serialize TIME globally</li>
<li><span data-hover="example-python-gil"><b>Python's GIL</b></span> — serialize TIME at interpreter level</li>
<li><span data-hover="example-value-types"><b>Value types are easier</b></span> — copy creates new IDENTITY</li>
<li><span data-hover="example-pointers"><b>Pointers are dangerous</b></span> — unrestricted IDENTITY + free TIME</li>
<li><span data-hover="example-const-differs"><b>const differs across languages</b></span> — different TIME/IDENTITY choices</li>
</ul>

### Tradeoff

<div class="tradeoff-spectrum">
<pre>
FLEXIBILITY ◄─────────────────────────► SAFETY

  Unrestricted aliasing         Restricted IDENTITY
  Free mutation                 Controlled TIME
  Manual management             Compiler/runtime enforced
         │                              │
         ▼                              ▼
  Maximum power                 Maximum guarantees
  C, unsafe Rust                Haskell, Rust safe, Erlang
</pre>
</div>

<div class="derived-grid">
<div class="derived-card">
<div class="derived-card-title">1. Which axis to constrain?</div>
<div class="derived-card-item">• <b>TIME</b> → immutability</div>
<div class="derived-card-item">• <b>SPACE</b> → single location</div>
<div class="derived-card-item">• <b>IDENTITY</b> → ownership, value semantics</div>
</div>
<div class="derived-card">
<div class="derived-card-title">2. Who enforces it?</div>
<div class="derived-card-item">• Programmer</div>
<div class="derived-card-item">• Compiler</div>
<div class="derived-card-item">• Runtime</div>
<div class="derived-card-item">• Hardware</div>
</div>
<div class="derived-card">
<div class="derived-card-title">3. When to check?</div>
<div class="derived-card-item">• Compile-time</div>
<div class="derived-card-item">• Runtime</div>
<div class="derived-card-item">• Never</div>
</div>
</div>

### Equivalences

<div class="derived-box" style="font-family: monospace;">
CDN edge cache    ≅  CPU cache line   ≅  borrowed reference<br>
DB replica        ≅  thread-local     ≅  cloned value<br>
TTL invalidation  ≅  cache coherence  ≅  borrow checker<br>
distributed lock  ≅  mutex            ≅  serialized TIME<br>
immutable source  ≅  const            ≅  frozen TIME
</div>

<script src="{{ '/assets/js/lang-derived.js' | relative_url }}"></script>

---

## Abstraction Layers

Every layer in computing—hardware or software—can be understood as an **abstractor**: hiding complexity below while exposing a simpler interface above.

**Click** a concept to see its dependency thread. **Hover** to see connections.

<div id="engineers-bar">
  <div class="engineers-header">
    <span class="engineers-toggle">▶</span>
    <span>Engineering Profiles</span>
  </div>
  <div class="engineers-content"></div>
</div>
<div id="concept-grid"></div>

<script src="https://d3js.org/d3.v7.min.js"></script>
<script src="{{ '/assets/js/abstractor-viz.js' | relative_url }}"></script>

---

## Layer Details

Expand each layer to see what it **abstracts** (hides), **exposes** (provides), **leaks** (breaks through), and **escapes** (workarounds).

<div id="layer-cards"></div>

<script src="{{ '/assets/js/abstractor-cards.js' | relative_url }}"></script>
