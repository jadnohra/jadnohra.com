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
    initExpandingTower2();
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
  // VIZ 3: SELECTIVE THREAD SANKEY
  // ============================================================
  function initSankey() {
    const container = document.getElementById('viz-sankey');
    if (!container) return;

    container.innerHTML = '';

    // Build dependency maps
    const dependsOn = {};  // conceptId -> [sourceConceptIds]
    const dependents = {}; // conceptId -> [dependentConceptIds]

    data.layers.forEach(layer => {
      layer.exposes.forEach(e => {
        dependsOn[e.id] = [];
        dependents[e.id] = [];
      });
    });

    // Build relationships from abstracts
    data.layers.forEach(layer => {
      layer.abstracts.forEach(a => {
        if (a.from) {
          a.from.forEach(fromId => {
            // Find which exposes in this layer relate to this abstract
            layer.exposes.forEach(exp => {
              if (!dependsOn[exp.id]) dependsOn[exp.id] = [];
              if (!dependsOn[exp.id].includes(fromId)) {
                dependsOn[exp.id].push(fromId);
              }
              if (!dependents[fromId]) dependents[fromId] = [];
              if (!dependents[fromId].includes(exp.id)) {
                dependents[fromId].push(exp.id);
              }
            });
          });
        }
      });
    });

    // Get unique levels and sort layers
    const levels = [...new Set(data.layers.map(l => l.level))].sort((a, b) => a - b);
    const layersByLevel = {};
    levels.forEach(l => layersByLevel[l] = []);
    data.layers.forEach(layer => layersByLevel[layer.level].push(layer));

    // Flatten to get column order (pick representative layers per level)
    const columns = [];
    levels.forEach(level => {
      layersByLevel[level].forEach(layer => {
        columns.push(layer);
      });
    });

    // Create HTML layout
    const wrapper = document.createElement('div');
    wrapper.style.cssText = 'position: relative; width: 100%; overflow-x: auto;';

    // SVG for connections (positioned absolutely)
    const svgNS = 'http://www.w3.org/2000/svg';
    const svg = document.createElementNS(svgNS, 'svg');
    svg.style.cssText = 'position: absolute; top: 0; left: 0; width: 100%; height: 100%; pointer-events: none; z-index: 1;';
    svg.id = 'sankey-connections';

    // Columns container
    const columnsDiv = document.createElement('div');
    columnsDiv.style.cssText = 'display: flex; gap: 8px; padding: 10px; min-width: max-content; position: relative; z-index: 2;';

    const conceptElements = {}; // conceptId -> DOM element
    const conceptPositions = {}; // conceptId -> {x, y}

    // Create columns
    columns.forEach((layer, colIdx) => {
      const col = document.createElement('div');
      col.style.cssText = `
        min-width: 120px;
        max-width: 150px;
        background: ${domainColors[layer.domain]}15;
        border-radius: 8px;
        padding: 8px;
        flex-shrink: 0;
      `;

      // Header
      const header = document.createElement('div');
      header.style.cssText = `
        font-size: 11px;
        font-weight: 600;
        color: ${domainColors[layer.domain]};
        margin-bottom: 8px;
        padding-bottom: 4px;
        border-bottom: 1px solid ${domainColors[layer.domain]}44;
        white-space: nowrap;
        overflow: hidden;
        text-overflow: ellipsis;
      `;
      header.textContent = layer.name;
      header.title = layer.name;
      col.appendChild(header);

      // Exposes
      layer.exposes.forEach(exp => {
        const item = document.createElement('div');
        item.className = 'sankey-item';
        item.dataset.conceptId = exp.id;
        item.dataset.layerId = layer.id;
        item.style.cssText = `
          font-size: 10px;
          color: #94a3b8;
          padding: 4px 6px;
          margin: 2px 0;
          background: rgba(255,255,255,0.05);
          border-radius: 4px;
          cursor: pointer;
          transition: all 0.2s;
          white-space: nowrap;
          overflow: hidden;
          text-overflow: ellipsis;
        `;
        item.textContent = exp.name;
        item.title = exp.desc || exp.name;

        conceptElements[exp.id] = item;
        col.appendChild(item);
      });

      columnsDiv.appendChild(col);
    });

    // Clear button
    const clearBtn = document.createElement('button');
    clearBtn.textContent = 'Clear';
    clearBtn.style.cssText = `
      position: absolute;
      top: 10px;
      right: 10px;
      padding: 4px 12px;
      font-size: 11px;
      background: #334155;
      color: #e2e8f0;
      border: none;
      border-radius: 4px;
      cursor: pointer;
      z-index: 10;
    `;

    wrapper.appendChild(svg);
    wrapper.appendChild(columnsDiv);
    wrapper.appendChild(clearBtn);
    container.appendChild(wrapper);

    // After layout, get positions
    setTimeout(() => {
      Object.entries(conceptElements).forEach(([id, el]) => {
        const rect = el.getBoundingClientRect();
        const containerRect = wrapper.getBoundingClientRect();
        conceptPositions[id] = {
          x: rect.left - containerRect.left + rect.width / 2,
          y: rect.top - containerRect.top + rect.height / 2,
          left: rect.left - containerRect.left,
          right: rect.right - containerRect.left,
          el
        };
      });

      // Update SVG size
      const totalHeight = columnsDiv.scrollHeight;
      const totalWidth = columnsDiv.scrollWidth;
      svg.setAttribute('viewBox', `0 0 ${totalWidth} ${totalHeight}`);
      svg.style.width = totalWidth + 'px';
      svg.style.height = totalHeight + 'px';
    }, 100);

    // Trace thread function
    function traceThread(conceptId) {
      const inThread = new Set([conceptId]);
      const connections = [];

      // Trace dependencies (going down/left)
      function traceDown(id, visited = new Set()) {
        if (visited.has(id)) return;
        visited.add(id);
        const deps = dependsOn[id] || [];
        deps.forEach(depId => {
          if (conceptPositions[depId]) {
            inThread.add(depId);
            connections.push({ from: depId, to: id, direction: 'up' });
            traceDown(depId, visited);
          }
        });
      }

      // Trace dependents (going up/right)
      function traceUp(id, visited = new Set()) {
        if (visited.has(id)) return;
        visited.add(id);
        const deps = dependents[id] || [];
        deps.forEach(depId => {
          if (conceptPositions[depId]) {
            inThread.add(depId);
            connections.push({ from: id, to: depId, direction: 'up' });
            traceUp(depId, visited);
          }
        });
      }

      traceDown(conceptId);
      traceUp(conceptId);

      return { inThread, connections };
    }

    // Draw connections
    function drawConnections(connections, selectedId) {
      svg.innerHTML = '';

      // Add animation style
      const style = document.createElementNS(svgNS, 'style');
      style.textContent = `
        @keyframes flowRight {
          from { stroke-dashoffset: 20; }
          to { stroke-dashoffset: 0; }
        }
        .flow-line {
          animation: flowRight 0.5s linear infinite;
        }
      `;
      svg.appendChild(style);

      connections.forEach(({ from, to }) => {
        const fromPos = conceptPositions[from];
        const toPos = conceptPositions[to];
        if (!fromPos || !toPos) return;

        const path = document.createElementNS(svgNS, 'path');
        const x1 = fromPos.right;
        const y1 = fromPos.y;
        const x2 = toPos.left;
        const y2 = toPos.y;
        const midX = (x1 + x2) / 2;

        path.setAttribute('d', `M${x1},${y1} C${midX},${y1} ${midX},${y2} ${x2},${y2}`);
        path.setAttribute('fill', 'none');
        path.setAttribute('stroke', '#f97316');
        path.setAttribute('stroke-width', '2');
        path.setAttribute('stroke-dasharray', '5,5');
        path.classList.add('flow-line');
        path.style.opacity = '0.7';

        svg.appendChild(path);
      });
    }

    // Click handler
    function handleClick(e) {
      const item = e.target.closest('.sankey-item');
      if (!item) return;

      const conceptId = item.dataset.conceptId;
      const { inThread, connections } = traceThread(conceptId);

      // Update styles
      Object.entries(conceptElements).forEach(([id, el]) => {
        if (id === conceptId) {
          el.style.background = '#f9731644';
          el.style.color = '#fff';
          el.style.fontWeight = '600';
        } else if (inThread.has(id)) {
          el.style.background = 'rgba(255,255,255,0.1)';
          el.style.color = '#e2e8f0';
          el.style.fontWeight = 'normal';
        } else {
          el.style.background = 'rgba(255,255,255,0.02)';
          el.style.color = '#475569';
          el.style.fontWeight = 'normal';
        }
      });

      drawConnections(connections, conceptId);
    }

    // Clear handler
    function handleClear() {
      svg.innerHTML = '';
      Object.values(conceptElements).forEach(el => {
        el.style.background = 'rgba(255,255,255,0.05)';
        el.style.color = '#94a3b8';
        el.style.fontWeight = 'normal';
      });
    }

    columnsDiv.addEventListener('click', handleClick);
    clearBtn.addEventListener('click', handleClear);
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

  // ============================================================
  // VIZ 5: EXPANDING TOWER 2 - THREAD EXPLORER
  // ============================================================
  function initExpandingTower2() {
    const container = document.getElementById('viz-tower2');
    if (!container) return;

    container.innerHTML = '';

    // Domain hues for hierarchical coloring
    const domainHues = {
      physical: 25,
      digital: 210,
      architecture: 270,
      toolchain: 50,
      system: 0,
      runtime: 175,
      language: 145
    };

    // Build item colors
    const itemColors = {};
    const layerColors = {};
    data.layers.forEach(layer => {
      const baseHue = domainHues[layer.domain] || 0;
      const sat = layer.domain === 'toolchain' ? 20 : 70;
      layerColors[layer.id] = `hsl(${baseHue}, ${sat}%, 45%)`;

      layer.exposes.forEach((exp, i) => {
        const hueOffset = (i - layer.exposes.length / 2) * 4;
        itemColors[exp.id] = `hsl(${baseHue + hueOffset}, ${sat}%, 55%)`;
      });
      layer.abstracts.forEach((a, i) => {
        const hueOffset = (i - layer.abstracts.length / 2) * 4;
        itemColors[a.id] = `hsl(${baseHue + hueOffset}, ${sat - 10}%, 50%)`;
      });
    });

    // Build dependency maps
    const dependsOn = {};
    const dependents = {};

    data.layers.forEach(layer => {
      layer.exposes.forEach(e => {
        dependsOn[e.id] = [];
        dependents[e.id] = [];
      });
    });

    data.layers.forEach(layer => {
      layer.abstracts.forEach(a => {
        if (a.from) {
          a.from.forEach(fromId => {
            layer.exposes.forEach(exp => {
              if (!dependsOn[exp.id]) dependsOn[exp.id] = [];
              if (!dependsOn[exp.id].includes(fromId)) {
                dependsOn[exp.id].push(fromId);
              }
              if (!dependents[fromId]) dependents[fromId] = [];
              if (!dependents[fromId].includes(exp.id)) {
                dependents[fromId].push(exp.id);
              }
            });
          });
        }
      });
    });

    // Sort layers by level
    const sortedLayers = [...data.layers].sort((a, b) => a.level - b.level);

    // Create wrapper with SVG overlay
    const wrapper = document.createElement('div');
    wrapper.style.cssText = 'position: relative; width: 100%;';

    const svgNS = 'http://www.w3.org/2000/svg';
    const svg = document.createElementNS(svgNS, 'svg');
    svg.style.cssText = 'position: absolute; top: 0; left: 0; width: 100%; height: 100%; pointer-events: none; z-index: 10;';
    svg.id = 'tower2-connections';

    // Defs for gradients
    const defs = document.createElementNS(svgNS, 'defs');
    svg.appendChild(defs);

    const tower = document.createElement('div');
    tower.className = 'tower2';
    tower.style.cssText = 'display: flex; flex-direction: column; gap: 2px;';

    const layerElements = {};
    const itemElements = {};
    const itemPositions = {};

    sortedLayers.forEach(layer => {
      const layerEl = document.createElement('div');
      layerEl.className = 'tower2-layer';
      layerEl.dataset.layerId = layer.id;
      layerEl.style.cssText = `
        background: ${layerColors[layer.id]}15;
        border-left: 3px solid ${layerColors[layer.id]};
        border-radius: 4px;
        overflow: hidden;
        transition: all 0.3s ease;
      `;

      // Header
      const header = document.createElement('div');
      header.className = 'tower2-header';
      header.style.cssText = `
        padding: 6px 10px;
        display: flex;
        justify-content: space-between;
        align-items: center;
        background: ${layerColors[layer.id]}25;
        cursor: pointer;
      `;
      header.innerHTML = `
        <span style="color: ${layerColors[layer.id]}; font-weight: 600; font-size: 12px;">${layer.name}</span>
        <span style="color: ${layerColors[layer.id]}88; font-size: 10px;">L${layer.level}</span>
      `;

      // Content with exposes
      const content = document.createElement('div');
      content.className = 'tower2-content';
      content.style.cssText = `
        max-height: 0;
        overflow: hidden;
        transition: max-height 0.3s ease;
        display: flex;
        flex-wrap: wrap;
        gap: 4px;
        padding: 0 8px;
      `;

      layer.exposes.forEach(exp => {
        const item = document.createElement('div');
        item.className = 'tower2-item';
        item.dataset.conceptId = exp.id;
        item.dataset.layerId = layer.id;
        item.style.cssText = `
          font-size: 10px;
          color: #e2e8f0;
          padding: 3px 8px;
          background: ${itemColors[exp.id]}33;
          border: 1px solid ${itemColors[exp.id]}66;
          border-radius: 12px;
          cursor: pointer;
          transition: all 0.2s;
          white-space: nowrap;
        `;
        item.textContent = exp.name;
        item.title = exp.desc || exp.name;
        itemElements[exp.id] = item;
        content.appendChild(item);
      });

      layerEl.appendChild(header);
      layerEl.appendChild(content);
      tower.appendChild(layerEl);
      layerElements[layer.id] = { el: layerEl, header, content };

      // Toggle on header click
      header.onclick = (e) => {
        e.stopPropagation();
        const isOpen = content.style.maxHeight !== '0px' && content.style.maxHeight !== '';
        content.style.maxHeight = isOpen ? '0' : (content.scrollHeight + 16) + 'px';
        content.style.padding = isOpen ? '0 8px' : '8px';
      };
    });

    // Clear button
    const clearBtn = document.createElement('button');
    clearBtn.textContent = 'Clear';
    clearBtn.style.cssText = `
      position: absolute;
      top: 8px;
      right: 8px;
      padding: 4px 12px;
      font-size: 11px;
      background: #334155;
      color: #e2e8f0;
      border: none;
      border-radius: 4px;
      cursor: pointer;
      z-index: 20;
    `;

    wrapper.appendChild(svg);
    wrapper.appendChild(tower);
    wrapper.appendChild(clearBtn);
    container.appendChild(wrapper);

    // Get positions after render
    function updatePositions() {
      const wrapperRect = wrapper.getBoundingClientRect();
      Object.entries(itemElements).forEach(([id, el]) => {
        const rect = el.getBoundingClientRect();
        itemPositions[id] = {
          x: rect.left - wrapperRect.left + rect.width / 2,
          y: rect.top - wrapperRect.top + rect.height / 2,
          left: rect.left - wrapperRect.left,
          right: rect.right - wrapperRect.left,
          top: rect.top - wrapperRect.top,
          bottom: rect.bottom - wrapperRect.top
        };
      });
      svg.setAttribute('viewBox', `0 0 ${wrapper.scrollWidth} ${wrapper.scrollHeight}`);
      svg.style.width = wrapper.scrollWidth + 'px';
      svg.style.height = wrapper.scrollHeight + 'px';
    }

    // Trace thread
    function traceThread(conceptId) {
      const inThread = new Set([conceptId]);
      const connections = [];
      const activeLayers = new Set();

      function traceDown(id, visited = new Set()) {
        if (visited.has(id)) return;
        visited.add(id);
        const deps = dependsOn[id] || [];
        deps.forEach(depId => {
          if (itemElements[depId]) {
            inThread.add(depId);
            const sourceLayer = exposeToLayer[depId];
            const targetLayer = exposeToLayer[id];
            if (sourceLayer) activeLayers.add(sourceLayer);
            if (targetLayer) activeLayers.add(targetLayer);
            connections.push({ from: depId, to: id });
            traceDown(depId, visited);
          }
        });
      }

      function traceUp(id, visited = new Set()) {
        if (visited.has(id)) return;
        visited.add(id);
        const deps = dependents[id] || [];
        deps.forEach(depId => {
          if (itemElements[depId]) {
            inThread.add(depId);
            const sourceLayer = exposeToLayer[id];
            const targetLayer = exposeToLayer[depId];
            if (sourceLayer) activeLayers.add(sourceLayer);
            if (targetLayer) activeLayers.add(targetLayer);
            connections.push({ from: id, to: depId });
            traceUp(depId, visited);
          }
        });
      }

      const startLayer = exposeToLayer[conceptId];
      if (startLayer) activeLayers.add(startLayer);
      traceDown(conceptId);
      traceUp(conceptId);

      return { inThread, connections, activeLayers };
    }

    // Draw gradient connections
    function drawConnections(connections, selectedId) {
      // Clear old
      while (defs.firstChild) defs.removeChild(defs.firstChild);
      svg.querySelectorAll('path').forEach(p => p.remove());

      connections.forEach(({ from, to }, i) => {
        const fromPos = itemPositions[from];
        const toPos = itemPositions[to];
        if (!fromPos || !toPos) return;

        const fromColor = itemColors[from] || '#888';
        const toColor = itemColors[to] || '#888';

        // Create gradient
        const gradId = `grad-${i}`;
        const grad = document.createElementNS(svgNS, 'linearGradient');
        grad.id = gradId;
        grad.setAttribute('x1', '0%');
        grad.setAttribute('y1', '0%');
        grad.setAttribute('x2', '100%');
        grad.setAttribute('y2', '0%');

        const stop1 = document.createElementNS(svgNS, 'stop');
        stop1.setAttribute('offset', '0%');
        stop1.setAttribute('stop-color', fromColor);
        const stop2 = document.createElementNS(svgNS, 'stop');
        stop2.setAttribute('offset', '100%');
        stop2.setAttribute('stop-color', toColor);
        grad.appendChild(stop1);
        grad.appendChild(stop2);
        defs.appendChild(grad);

        // Draw bezier
        const path = document.createElementNS(svgNS, 'path');
        const x1 = fromPos.right;
        const y1 = fromPos.y;
        const x2 = toPos.left;
        const y2 = toPos.y;

        // If same vertical position, curve around
        const yDiff = Math.abs(y2 - y1);
        const xMid = (x1 + x2) / 2;

        if (yDiff < 10) {
          // Horizontal - arc up or down
          const arcY = y1 - 30;
          path.setAttribute('d', `M${x1},${y1} Q${xMid},${arcY} ${x2},${y2}`);
        } else {
          path.setAttribute('d', `M${x1},${y1} C${x1 + 40},${y1} ${x2 - 40},${y2} ${x2},${y2}`);
        }

        path.setAttribute('fill', 'none');
        path.setAttribute('stroke', `url(#${gradId})`);
        path.setAttribute('stroke-width', '2');
        path.setAttribute('stroke-opacity', '0.8');
        svg.appendChild(path);
      });
    }

    // Click handler
    function handleClick(e) {
      const item = e.target.closest('.tower2-item');
      if (!item) return;

      const conceptId = item.dataset.conceptId;
      const { inThread, connections, activeLayers } = traceThread(conceptId);

      // Update layer visibility
      Object.entries(layerElements).forEach(([layerId, { el, header, content }]) => {
        if (activeLayers.has(layerId)) {
          el.style.opacity = '1';
          content.style.maxHeight = (content.scrollHeight + 16) + 'px';
          content.style.padding = '8px';
        } else {
          el.style.opacity = '0.25';
          content.style.maxHeight = '0';
          content.style.padding = '0 8px';
        }
      });

      // Update item styles
      Object.entries(itemElements).forEach(([id, el]) => {
        if (id === conceptId) {
          el.style.background = itemColors[id];
          el.style.color = '#fff';
          el.style.fontWeight = '600';
          el.style.transform = 'scale(1.1)';
          el.style.boxShadow = `0 0 10px ${itemColors[id]}`;
        } else if (inThread.has(id)) {
          el.style.background = `${itemColors[id]}55`;
          el.style.color = '#e2e8f0';
          el.style.fontWeight = 'normal';
          el.style.transform = 'scale(1)';
          el.style.boxShadow = 'none';
        } else {
          el.style.background = 'rgba(100,100,100,0.1)';
          el.style.color = '#475569';
          el.style.fontWeight = 'normal';
          el.style.transform = 'scale(1)';
          el.style.boxShadow = 'none';
        }
      });

      // Draw after positions update
      setTimeout(() => {
        updatePositions();
        drawConnections(connections, conceptId);
      }, 350);
    }

    // Clear handler
    function handleClear() {
      while (defs.firstChild) defs.removeChild(defs.firstChild);
      svg.querySelectorAll('path').forEach(p => p.remove());

      Object.entries(layerElements).forEach(([layerId, { el, content }]) => {
        el.style.opacity = '1';
        content.style.maxHeight = '0';
        content.style.padding = '0 8px';
      });

      Object.entries(itemElements).forEach(([id, el]) => {
        el.style.background = `${itemColors[id]}33`;
        el.style.color = '#e2e8f0';
        el.style.fontWeight = 'normal';
        el.style.transform = 'scale(1)';
        el.style.boxShadow = 'none';
      });
    }

    tower.addEventListener('click', handleClick);
    clearBtn.addEventListener('click', handleClear);

    // Initial position calc
    setTimeout(updatePositions, 100);
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
