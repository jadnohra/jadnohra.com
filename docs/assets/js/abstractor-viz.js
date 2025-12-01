// Abstractor Visualizations
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

  const categoryColors = {
    abstracts: "#f59e0b",
    exposes: "#22c55e",
    leaks: "#ef4444",
    escapes: "#8b5cf6"
  };

  let data = null;

  async function init() {
    try {
      let resp = await fetch('./assets/data/abstractor.json');
      if (!resp.ok) resp = await fetch('/jadnohra.com/assets/data/abstractor.json');
      data = await resp.json();
    } catch (e) {
      console.error('Failed to load abstractor data:', e);
      return;
    }

    // Build lookup maps
    buildLookups();

    // Initialize all visualizations
    initThreadTracer();
    initExpandingTower();
    initSankey();
    initRadial();
  }

  // Lookup maps
  let exposeToLayer = {};
  let conceptById = {};
  let layerById = {};

  function buildLookups() {
    data.layers.forEach(layer => {
      layerById[layer.id] = layer;
      layer.exposes.forEach(e => {
        exposeToLayer[e.id] = layer.id;
        conceptById[e.id] = { ...e, type: 'exposes', layerId: layer.id };
      });
      layer.abstracts.forEach(a => {
        conceptById[a.id] = { ...a, type: 'abstracts', layerId: layer.id };
      });
      layer.leaks.forEach(l => {
        conceptById[l.id] = { ...l, type: 'leaks', layerId: layer.id };
      });
      layer.escapes.forEach(e => {
        conceptById[e.id] = { ...e, type: 'escapes', layerId: layer.id };
      });
    });
  }

  // ============================================================
  // VIZ 1: THREAD TRACER
  // ============================================================
  function initThreadTracer() {
    const container = document.getElementById('viz-thread-tracer');
    if (!container) return;

    const width = container.clientWidth || 800;
    const height = 500;
    const layerHeight = 40;
    const padding = { top: 20, right: 300, bottom: 20, left: 20 };

    const svg = d3.select(container)
      .append('svg')
      .attr('width', width)
      .attr('height', height)
      .attr('viewBox', `0 0 ${width} ${height}`);

    // Sort layers by level
    const sortedLayers = [...data.layers].sort((a, b) => a.level - b.level);

    // Draw layers
    const layerGroup = svg.append('g').attr('class', 'layers');
    const layerBoxWidth = 250;

    sortedLayers.forEach((layer, i) => {
      const y = padding.top + i * layerHeight;
      const g = layerGroup.append('g')
        .attr('class', 'layer')
        .attr('data-layer', layer.id);

      g.append('rect')
        .attr('x', padding.left)
        .attr('y', y)
        .attr('width', layerBoxWidth)
        .attr('height', layerHeight - 4)
        .attr('rx', 4)
        .attr('fill', domainColors[layer.domain] || '#666')
        .attr('opacity', 0.8)
        .style('cursor', 'pointer');

      g.append('text')
        .attr('x', padding.left + 10)
        .attr('y', y + layerHeight / 2)
        .attr('fill', '#fff')
        .attr('font-size', '12px')
        .attr('dominant-baseline', 'middle')
        .text(layer.name)
        .style('pointer-events', 'none');

      // Level indicator
      g.append('text')
        .attr('x', padding.left + layerBoxWidth - 10)
        .attr('y', y + layerHeight / 2)
        .attr('fill', 'rgba(255,255,255,0.6)')
        .attr('font-size', '10px')
        .attr('text-anchor', 'end')
        .attr('dominant-baseline', 'middle')
        .text(`L${layer.level}`)
        .style('pointer-events', 'none');
    });

    // Info panel
    const infoPanel = svg.append('g')
      .attr('class', 'info-panel')
      .attr('transform', `translate(${padding.left + layerBoxWidth + 30}, ${padding.top})`);

    infoPanel.append('text')
      .attr('class', 'info-title')
      .attr('fill', '#e2e8f0')
      .attr('font-size', '14px')
      .attr('font-weight', 'bold')
      .text('Click a layer to explore');

    infoPanel.append('text')
      .attr('class', 'info-detail')
      .attr('y', 25)
      .attr('fill', '#94a3b8')
      .attr('font-size', '11px');

    // Connection lines group
    const linesGroup = svg.append('g').attr('class', 'connections');

    // Click handler
    layerGroup.selectAll('.layer').on('click', function(event, d) {
      const layerId = this.getAttribute('data-layer');
      const layer = layerById[layerId];
      showLayerConnections(layer, sortedLayers, linesGroup, layerGroup, infoPanel, padding, layerHeight, layerBoxWidth);
    });
  }

  function showLayerConnections(layer, sortedLayers, linesGroup, layerGroup, infoPanel, padding, layerHeight, layerBoxWidth) {
    // Clear previous
    linesGroup.selectAll('*').remove();
    layerGroup.selectAll('rect').attr('opacity', 0.3);

    // Highlight selected layer
    layerGroup.select(`[data-layer="${layer.id}"] rect`).attr('opacity', 1);

    // Find dependencies (what this layer's abstracts depend on)
    const dependsOn = new Set();
    layer.abstracts.forEach(a => {
      if (a.from) {
        a.from.forEach(fromId => {
          const sourceLayerId = exposeToLayer[fromId];
          if (sourceLayerId) dependsOn.add(sourceLayerId);
        });
      }
    });

    // Find dependents (what depends on this layer's exposes)
    const dependents = new Set();
    data.layers.forEach(otherLayer => {
      otherLayer.abstracts.forEach(a => {
        if (a.from) {
          a.from.forEach(fromId => {
            if (layer.exposes.some(e => e.id === fromId)) {
              dependents.add(otherLayer.id);
            }
          });
        }
      });
    });

    // Draw connections
    const layerIndex = {};
    sortedLayers.forEach((l, i) => layerIndex[l.id] = i);
    const selectedY = padding.top + layerIndex[layer.id] * layerHeight + layerHeight / 2;

    // Down connections (dependencies) - orange
    dependsOn.forEach(depId => {
      layerGroup.select(`[data-layer="${depId}"] rect`).attr('opacity', 0.8);
      const depY = padding.top + layerIndex[depId] * layerHeight + layerHeight / 2;

      linesGroup.append('path')
        .attr('d', `M${padding.left + layerBoxWidth + 5},${selectedY}
                    Q${padding.left + layerBoxWidth + 40},${(selectedY + depY) / 2}
                    ${padding.left + layerBoxWidth + 5},${depY}`)
        .attr('fill', 'none')
        .attr('stroke', '#f97316')
        .attr('stroke-width', 2)
        .attr('opacity', 0)
        .transition()
        .duration(300)
        .attr('opacity', 0.8);
    });

    // Up connections (dependents) - blue
    dependents.forEach(depId => {
      layerGroup.select(`[data-layer="${depId}"] rect`).attr('opacity', 0.8);
      const depY = padding.top + layerIndex[depId] * layerHeight + layerHeight / 2;

      linesGroup.append('path')
        .attr('d', `M${padding.left - 5},${selectedY}
                    Q${padding.left - 40},${(selectedY + depY) / 2}
                    ${padding.left - 5},${depY}`)
        .attr('fill', 'none')
        .attr('stroke', '#3b82f6')
        .attr('stroke-width', 2)
        .attr('opacity', 0)
        .transition()
        .duration(300)
        .attr('opacity', 0.8);
    });

    // Update info panel
    infoPanel.select('.info-title').text(layer.name);
    infoPanel.select('.info-detail')
      .text(`Abstracts: ${layer.abstracts.length} | Exposes: ${layer.exposes.length} | Leaks: ${layer.leaks.length} | Escapes: ${layer.escapes.length}`);
  }

  // ============================================================
  // VIZ 2: EXPANDING TOWER
  // ============================================================
  function initExpandingTower() {
    const container = document.getElementById('viz-expanding-tower');
    if (!container) return;

    const width = container.clientWidth || 800;

    // Sort layers by level
    const sortedLayers = [...data.layers].sort((a, b) => a.level - b.level);

    // Create HTML-based tower (easier for expand/collapse)
    container.innerHTML = '';
    const tower = document.createElement('div');
    tower.className = 'tower';
    tower.style.cssText = 'display: flex; flex-direction: column; gap: 4px;';

    sortedLayers.forEach(layer => {
      const layerEl = document.createElement('div');
      layerEl.className = 'tower-layer';
      layerEl.style.cssText = `
        background: ${domainColors[layer.domain]}22;
        border-left: 4px solid ${domainColors[layer.domain]};
        border-radius: 4px;
        overflow: hidden;
        transition: all 0.3s ease;
      `;

      // Header
      const header = document.createElement('div');
      header.style.cssText = `
        padding: 8px 12px;
        cursor: pointer;
        display: flex;
        justify-content: space-between;
        align-items: center;
        background: ${domainColors[layer.domain]}33;
      `;
      header.innerHTML = `
        <span style="color: #e2e8f0; font-weight: 500;">${layer.name}</span>
        <span style="color: ${domainColors[layer.domain]}; font-size: 11px;">L${layer.level} ▼</span>
      `;

      // Content
      const content = document.createElement('div');
      content.className = 'tower-content';
      content.style.cssText = `
        max-height: 0;
        overflow: hidden;
        transition: max-height 0.3s ease;
        padding: 0 12px;
      `;

      const grid = document.createElement('div');
      grid.style.cssText = 'display: grid; grid-template-columns: repeat(4, 1fr); gap: 8px; padding: 12px 0;';

      ['abstracts', 'exposes', 'leaks', 'escapes'].forEach(cat => {
        const col = document.createElement('div');
        col.innerHTML = `<div style="color: ${categoryColors[cat]}; font-size: 10px; text-transform: uppercase; margin-bottom: 6px;">${cat}</div>`;

        layer[cat].forEach(item => {
          const pill = document.createElement('div');
          pill.style.cssText = `
            font-size: 10px;
            color: #94a3b8;
            padding: 2px 6px;
            background: rgba(255,255,255,0.05);
            border-radius: 3px;
            margin-bottom: 3px;
            cursor: pointer;
            transition: all 0.2s;
          `;
          pill.textContent = item.name;
          pill.title = item.desc || '';
          pill.onmouseenter = () => pill.style.background = `${categoryColors[cat]}33`;
          pill.onmouseleave = () => pill.style.background = 'rgba(255,255,255,0.05)';
          col.appendChild(pill);
        });

        if (layer[cat].length === 0) {
          col.innerHTML += '<div style="font-size: 10px; color: #475569; font-style: italic;">none</div>';
        }
        grid.appendChild(col);
      });

      content.appendChild(grid);
      layerEl.appendChild(header);
      layerEl.appendChild(content);
      tower.appendChild(layerEl);

      // Toggle handler
      header.onclick = () => {
        const isOpen = content.style.maxHeight !== '0px' && content.style.maxHeight !== '';
        content.style.maxHeight = isOpen ? '0' : content.scrollHeight + 'px';
        content.style.padding = isOpen ? '0 12px' : '0 12px';
      };
    });

    container.appendChild(tower);
  }

  // ============================================================
  // VIZ 3: SANKEY FLOW
  // ============================================================
  function initSankey() {
    const container = document.getElementById('viz-sankey');
    if (!container) return;

    const width = container.clientWidth || 800;
    const height = 500;

    const svg = d3.select(container)
      .append('svg')
      .attr('width', width)
      .attr('height', height);

    // Build nodes and links for sankey
    const nodes = [];
    const nodeIndex = {};
    const links = [];

    // Add all concepts as nodes
    data.layers.forEach(layer => {
      layer.exposes.forEach(e => {
        const id = `${layer.id}:${e.id}`;
        nodeIndex[id] = nodes.length;
        nodes.push({
          id,
          name: e.name,
          layer: layer.id,
          level: layer.level,
          domain: layer.domain
        });
      });
    });

    // Add links from abstracts to their sources
    data.layers.forEach(layer => {
      layer.abstracts.forEach(a => {
        if (a.from) {
          // Find what this layer exposes that relates to this abstract
          const targetExposes = layer.exposes.filter(e =>
            e.name.toLowerCase().includes(a.name.split(' ')[0].toLowerCase()) ||
            a.name.toLowerCase().includes(e.name.split(' ')[0].toLowerCase())
          );

          a.from.forEach(fromId => {
            const sourceLayerId = exposeToLayer[fromId];
            if (sourceLayerId) {
              const sourceId = `${sourceLayerId}:${fromId}`;
              // Connect to first relevant expose or just the layer's first expose
              const targetExpose = targetExposes[0] || layer.exposes[0];
              if (targetExpose && nodeIndex[sourceId] !== undefined) {
                const targetId = `${layer.id}:${targetExpose.id}`;
                if (nodeIndex[targetId] !== undefined) {
                  links.push({
                    source: nodeIndex[sourceId],
                    target: nodeIndex[targetId],
                    value: 1
                  });
                }
              }
            }
          });
        }
      });
    });

    if (links.length === 0) {
      svg.append('text')
        .attr('x', width / 2)
        .attr('y', height / 2)
        .attr('text-anchor', 'middle')
        .attr('fill', '#94a3b8')
        .text('Sankey: Building dependency flow...');
      return;
    }

    // Create sankey layout
    const sankey = d3.sankey()
      .nodeWidth(15)
      .nodePadding(10)
      .extent([[20, 20], [width - 20, height - 20]]);

    const { nodes: sankeyNodes, links: sankeyLinks } = sankey({
      nodes: nodes.map(d => Object.assign({}, d)),
      links: links.map(d => Object.assign({}, d))
    });

    // Draw links
    svg.append('g')
      .selectAll('path')
      .data(sankeyLinks)
      .join('path')
      .attr('d', d3.sankeyLinkHorizontal())
      .attr('fill', 'none')
      .attr('stroke', d => domainColors[d.source.domain] || '#666')
      .attr('stroke-width', d => Math.max(1, d.width))
      .attr('stroke-opacity', 0.3)
      .on('mouseenter', function() { d3.select(this).attr('stroke-opacity', 0.7); })
      .on('mouseleave', function() { d3.select(this).attr('stroke-opacity', 0.3); });

    // Draw nodes
    svg.append('g')
      .selectAll('rect')
      .data(sankeyNodes)
      .join('rect')
      .attr('x', d => d.x0)
      .attr('y', d => d.y0)
      .attr('width', d => d.x1 - d.x0)
      .attr('height', d => d.y1 - d.y0)
      .attr('fill', d => domainColors[d.domain] || '#666')
      .append('title')
      .text(d => d.name);

    // Labels for larger nodes
    svg.append('g')
      .selectAll('text')
      .data(sankeyNodes.filter(d => (d.y1 - d.y0) > 15))
      .join('text')
      .attr('x', d => d.x0 < width / 2 ? d.x1 + 6 : d.x0 - 6)
      .attr('y', d => (d.y0 + d.y1) / 2)
      .attr('text-anchor', d => d.x0 < width / 2 ? 'start' : 'end')
      .attr('dominant-baseline', 'middle')
      .attr('fill', '#94a3b8')
      .attr('font-size', '9px')
      .text(d => d.name.length > 20 ? d.name.slice(0, 18) + '...' : d.name);
  }

  // ============================================================
  // VIZ 4: RADIAL VIEW
  // ============================================================
  function initRadial() {
    const container = document.getElementById('viz-radial');
    if (!container) return;

    const width = container.clientWidth || 800;
    const height = 500;
    const centerX = width / 2;
    const centerY = height / 2;
    const maxRadius = Math.min(width, height) / 2 - 40;

    const svg = d3.select(container)
      .append('svg')
      .attr('width', width)
      .attr('height', height);

    const g = svg.append('g')
      .attr('transform', `translate(${centerX}, ${centerY})`);

    // Get unique levels
    const levels = [...new Set(data.layers.map(l => l.level))].sort((a, b) => a - b);
    const levelRadius = maxRadius / (levels.length + 1);

    // Draw level rings
    levels.forEach((level, i) => {
      const r = (i + 1) * levelRadius;
      g.append('circle')
        .attr('r', r)
        .attr('fill', 'none')
        .attr('stroke', '#334155')
        .attr('stroke-width', 1)
        .attr('stroke-dasharray', '2,4');
    });

    // Position layers on rings
    const layersByLevel = {};
    levels.forEach(l => layersByLevel[l] = []);
    data.layers.forEach(layer => layersByLevel[layer.level].push(layer));

    const layerPositions = {};

    levels.forEach((level, levelIdx) => {
      const layersAtLevel = layersByLevel[level];
      const r = (levelIdx + 1) * levelRadius;
      const angleStep = (2 * Math.PI) / Math.max(layersAtLevel.length, 1);

      layersAtLevel.forEach((layer, i) => {
        const angle = i * angleStep - Math.PI / 2;
        const x = r * Math.cos(angle);
        const y = r * Math.sin(angle);
        layerPositions[layer.id] = { x, y, angle, r };

        // Draw layer node
        const nodeG = g.append('g')
          .attr('transform', `translate(${x}, ${y})`)
          .style('cursor', 'pointer');

        nodeG.append('circle')
          .attr('r', 20)
          .attr('fill', domainColors[layer.domain] || '#666')
          .attr('stroke', '#1e293b')
          .attr('stroke-width', 2);

        // Abbreviated name
        const abbrev = layer.name.split(/[\s\/()]+/).map(w => w[0]).join('').slice(0, 3);
        nodeG.append('text')
          .attr('text-anchor', 'middle')
          .attr('dominant-baseline', 'middle')
          .attr('fill', '#fff')
          .attr('font-size', '9px')
          .attr('font-weight', 'bold')
          .text(abbrev);

        // Full name on hover
        nodeG.append('title').text(`${layer.name} (L${layer.level})`);

        // Hover effect
        nodeG.on('mouseenter', function() {
          d3.select(this).select('circle').attr('r', 25);
          showRadialConnections(layer, g, layerPositions);
        });
        nodeG.on('mouseleave', function() {
          d3.select(this).select('circle').attr('r', 20);
          g.selectAll('.radial-connection').remove();
        });
      });
    });

    // Center label
    g.append('text')
      .attr('text-anchor', 'middle')
      .attr('fill', '#64748b')
      .attr('font-size', '10px')
      .text('Physics');

    // Legend
    const legend = svg.append('g')
      .attr('transform', `translate(20, 20)`);

    Object.entries(domainColors).forEach(([domain, color], i) => {
      legend.append('rect')
        .attr('x', 0)
        .attr('y', i * 18)
        .attr('width', 12)
        .attr('height', 12)
        .attr('fill', color)
        .attr('rx', 2);
      legend.append('text')
        .attr('x', 18)
        .attr('y', i * 18 + 10)
        .attr('fill', '#94a3b8')
        .attr('font-size', '10px')
        .text(domain);
    });
  }

  function showRadialConnections(layer, g, layerPositions) {
    g.selectAll('.radial-connection').remove();

    const pos = layerPositions[layer.id];
    if (!pos) return;

    // Find dependencies
    const deps = new Set();
    layer.abstracts.forEach(a => {
      if (a.from) {
        a.from.forEach(fromId => {
          const sourceLayerId = exposeToLayer[fromId];
          if (sourceLayerId && layerPositions[sourceLayerId]) {
            deps.add(sourceLayerId);
          }
        });
      }
    });

    // Draw connections
    deps.forEach(depId => {
      const depPos = layerPositions[depId];
      if (!depPos) return;

      g.append('path')
        .attr('class', 'radial-connection')
        .attr('d', `M${pos.x},${pos.y} Q0,0 ${depPos.x},${depPos.y}`)
        .attr('fill', 'none')
        .attr('stroke', '#f97316')
        .attr('stroke-width', 2)
        .attr('stroke-opacity', 0.6);
    });
  }

  // Initialize on DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
