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
.content h2 { margin-top: 1.5rem; margin-bottom: 0.5rem; }
.content h3 { margin-top: 1rem; margin-bottom: 0.3rem; }
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
</style>

<div id="system-primitives"></div>

<script src="{{ '/assets/js/primitives.js' | relative_url }}"></script>

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
