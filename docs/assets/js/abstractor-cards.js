// Abstractor - Layer Cards Visualization
(function() {
  const domainColors = {
    physical: "#f97316",
    digital: "#3b82f6",
    architecture: "#8b5cf6",
    toolchain: "#6b7280",
    system: "#ef4444",
    runtime: "#14b8a6",
    language: "#22c55e"
  };

  let data = null;

  async function init() {
    const container = document.getElementById('layer-cards');
    if (!container) return;

    try {
      let resp = await fetch('./assets/data/abstractor.json');
      if (!resp.ok) resp = await fetch('/assets/data/abstractor.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:#ef4444;">Failed to load data.</p>';
      return;
    }

    renderCards(container);
  }

  function renderCards(container) {
    container.innerHTML = '';

    const sortedLayers = [...data.layers].sort((a, b) => a.level - b.level);

    sortedLayers.forEach(layer => {
      const color = domainColors[layer.domain] || '#64748b';
      const card = document.createElement('div');
      card.className = 'layer-card';
      card.style.setProperty('--domain-color', color);

      // Stats counts
      const abstractCount = layer.abstracts?.length || 0;
      const exposeCount = layer.exposes?.length || 0;
      const leakCount = layer.leaks?.length || 0;
      const escapeCount = layer.escapes?.length || 0;

      // Header
      const header = document.createElement('div');
      header.className = 'card-header';
      header.innerHTML = `
        <div class="card-title">
          <span class="card-name">${layer.name}</span>
          <span class="card-level">L${layer.level}</span>
        </div>
        <div class="card-stats">
          ${abstractCount > 0 ? `<span class="card-stat"><span class="card-stat-dot" style="background:#f97316"></span>${abstractCount}</span>` : ''}
          ${exposeCount > 0 ? `<span class="card-stat"><span class="card-stat-dot" style="background:#22c55e"></span>${exposeCount}</span>` : ''}
          ${leakCount > 0 ? `<span class="card-stat"><span class="card-stat-dot" style="background:#ef4444"></span>${leakCount}</span>` : ''}
          ${escapeCount > 0 ? `<span class="card-stat"><span class="card-stat-dot" style="background:#a855f7"></span>${escapeCount}</span>` : ''}
          <span class="card-toggle">▼</span>
        </div>
      `;

      // Body
      const body = document.createElement('div');
      body.className = 'card-body';

      // Abstracts section
      if (abstractCount > 0) {
        body.appendChild(createSection('abstracts', 'Abstracts (Hidden)', layer.abstracts, true));
      }

      // Exposes section
      if (exposeCount > 0) {
        body.appendChild(createSection('exposes', 'Exposes (Provided)', layer.exposes));
      }

      // Leaks section
      if (leakCount > 0) {
        body.appendChild(createSection('leaks', 'Leaks (Breaks Through)', layer.leaks));
      }

      // Escapes section
      if (escapeCount > 0) {
        body.appendChild(createSection('escapes', 'Escapes (Workarounds)', layer.escapes));
      }

      card.appendChild(header);
      card.appendChild(body);

      // Toggle expand/collapse
      header.addEventListener('click', () => {
        card.classList.toggle('expanded');
      });

      container.appendChild(card);
    });
  }

  function createSection(type, title, items, showFrom = false) {
    const section = document.createElement('div');
    section.className = `card-section section-${type}`;

    const titleEl = document.createElement('div');
    titleEl.className = 'card-section-title';
    titleEl.textContent = title;
    section.appendChild(titleEl);

    const itemsContainer = document.createElement('div');
    itemsContainer.className = 'card-items';

    items.forEach(item => {
      const itemEl = document.createElement('span');
      itemEl.className = `card-item item-${type.slice(0, -1)}`;

      let content = item.name;
      if (showFrom && item.from && item.from.length > 0) {
        content += `<span class="card-from">← ${item.from.join(', ')}</span>`;
      }
      itemEl.innerHTML = content;

      if (item.desc) {
        itemEl.title = item.desc;
      }

      itemsContainer.appendChild(itemEl);
    });

    section.appendChild(itemsContainer);
    return section;
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
