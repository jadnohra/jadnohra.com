---
layout: page
title: Abstractor
icon: fas fa-layer-group
order: 3
---

<style>
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
</style>

Every layer in computing—hardware or software—can be understood as an **abstractor**: hiding complexity below while exposing a simpler interface above.

**Click** a concept to see its dependency thread. **Hover** to see connections.

<div id="concept-grid"></div>

<script src="https://d3js.org/d3.v7.min.js"></script>
<script src="{{ '/assets/js/abstractor-viz.js' | relative_url }}"></script>

---

## Layer Details

Expand each layer to see what it **abstracts** (hides), **exposes** (provides), **leaks** (breaks through), and **escapes** (workarounds).

<div id="layer-cards"></div>

<script src="{{ '/assets/js/abstractor-cards.js' | relative_url }}"></script>
