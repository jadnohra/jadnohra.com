---
layout: page
title: Abstractor
icon: fas fa-layer-group
order: 3
---

<script src="https://d3js.org/d3.v7.min.js"></script>
<script src="https://unpkg.com/d3-sankey@0.12.3/dist/d3-sankey.min.js"></script>

<style>
.viz-section {
  margin: 2rem 0;
  padding: 1.5rem;
  background: linear-gradient(135deg, #1e293b 0%, #0f172a 100%);
  border-radius: 12px;
}
.viz-section h3 {
  color: #e2e8f0;
  margin-bottom: 1rem;
  font-size: 1.1rem;
}
.viz-container {
  min-height: 400px;
  position: relative;
}
.viz-container svg {
  width: 100%;
  height: 100%;
}
</style>

Every layer in computing—hardware or software—can be understood as an **abstractor**: a tuple `(abstracts, exposes, leaks, escapes)` that hides complexity below while exposing a simpler interface above.

This visualization explores **20 layers** from Physics to Python, tracing how concepts propagate through the stack.

---

## 1. Thread Tracer

Click any concept to trace its dependencies down (what enables it) and up (what depends on it).

<div class="viz-section">
  <div id="viz-thread-tracer" class="viz-container"></div>
</div>

---

## 2. Expanding Tower

Click a layer to expand and see its abstracts, exposes, leaks, and escapes.

<div class="viz-section">
  <div id="viz-expanding-tower" class="viz-container"></div>
</div>

---

## 3. Sankey Flow

See how concepts flow from physics up through the software stack.

<div class="viz-section">
  <div id="viz-sankey" class="viz-container"></div>
</div>

---

## 4. Radial View

Physics at center, higher abstractions in outer rings.

<div class="viz-section">
  <div id="viz-radial" class="viz-container"></div>
</div>

---

## 5. Thread Explorer

Click any concept to trace its thread. Hierarchical colors with gradient connections.

<div class="viz-section">
  <div id="viz-tower2" class="viz-container"></div>
</div>

<script src="{{ '/assets/js/abstractor-viz.js' | relative_url }}"></script>
