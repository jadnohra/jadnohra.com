// Abstractor - Concept Grid Visualization
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
  let exposeToLayer = {};
  let layerById = {};
  let dependsOn = {};
  let dependents = {};

  // Engineer mode state
  let engineerMode = false;
  let selectedProfiles = new Set();
  let combineMode = 'union'; // 'union' or 'intersection'

  async function init() {
    const container = document.getElementById('concept-grid');
    if (!container) return;

    try {
      let resp = await fetch('./assets/data/abstractor.json');
      if (!resp.ok) resp = await fetch('/jadnohra.com/assets/data/abstractor.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:#ef4444;">Failed to load data.</p>';
      return;
    }

    buildLookups();
    setupEngineersBar();
    renderGrid(container);
  }

  function buildLookups() {
    // Build layer and expose lookups
    data.layers.forEach(layer => {
      layerById[layer.id] = layer;
      layer.exposes.forEach(e => {
        exposeToLayer[e.id] = layer.id;
        dependsOn[e.id] = [];
        dependents[e.id] = [];
      });
    });

    // Build dependency maps from abstracts
    data.layers.forEach(layer => {
      layer.abstracts.forEach(a => {
        if (a.from) {
          a.from.forEach(fromId => {
            layer.exposes.forEach(exp => {
              if (!dependsOn[exp.id].includes(fromId)) {
                dependsOn[exp.id].push(fromId);
              }
              if (dependents[fromId] && !dependents[fromId].includes(exp.id)) {
                dependents[fromId].push(exp.id);
              }
            });
          });
        }
      });
    });
  }

  function setupEngineersBar() {
    const bar = document.getElementById('engineers-bar');
    if (!bar || !data.profiles) return;

    const header = bar.querySelector('.engineers-header');
    const content = bar.querySelector('.engineers-content');
    const toggle = bar.querySelector('.engineers-toggle');

    // Build content
    // Mode toggle button
    const modeBtn = document.createElement('button');
    modeBtn.className = 'mode-toggle union';
    modeBtn.textContent = '∪';
    modeBtn.title = 'Union mode: add weights (saturate to 1)';
    modeBtn.addEventListener('click', (e) => {
      e.stopPropagation();
      if (combineMode === 'union') {
        combineMode = 'intersection';
        modeBtn.textContent = '∩';
        modeBtn.title = 'Intersection mode: multiply weights';
        modeBtn.className = 'mode-toggle intersection';
      } else {
        combineMode = 'union';
        modeBtn.textContent = '∪';
        modeBtn.title = 'Union mode: add weights (saturate to 1)';
        modeBtn.className = 'mode-toggle union';
      }
      updateEngineerHighlights();
    });
    content.appendChild(modeBtn);

    // Profile buttons
    data.profiles.forEach(profile => {
      const btn = document.createElement('button');
      btn.className = 'profile-btn';
      btn.dataset.profileId = profile.id;
      btn.innerHTML = `<span>${profile.name}</span>`;
      btn.title = profile.desc;
      btn.addEventListener('click', (e) => {
        e.stopPropagation();
        if (selectedProfiles.has(profile.id)) {
          selectedProfiles.delete(profile.id);
          btn.classList.remove('selected');
        } else {
          selectedProfiles.add(profile.id);
          btn.classList.add('selected');
        }
        updateEngineerHighlights();
      });
      content.appendChild(btn);
    });

    // Toggle expand/collapse
    header.addEventListener('click', () => {
      const wasExpanded = bar.classList.contains('expanded');
      bar.classList.toggle('expanded');
      toggle.textContent = bar.classList.contains('expanded') ? '▼' : '▶';

      // Mode switch
      engineerMode = !wasExpanded;

      if (!engineerMode) {
        // Exiting engineer mode - clear highlights
        clearEngineerHighlights();
      } else {
        // Entering engineer mode - apply highlights if profiles selected
        updateEngineerHighlights();
      }
    });
  }

  let conceptElements = {};

  function renderGrid(container) {
    container.innerHTML = '';

    // SVG for connections
    const svgNS = 'http://www.w3.org/2000/svg';
    const svg = document.createElementNS(svgNS, 'svg');
    svg.id = 'connection-lines';

    // Grid container
    const gridDiv = document.createElement('div');
    gridDiv.className = 'grid-container';

    // Track highlighted layer groups
    const layerGroups = {};

    conceptElements = {};
    const layerHeaders = {};
    const sortedLayers = [...data.layers].sort((a, b) => a.level - b.level);

    // Create layer groups
    sortedLayers.forEach(layer => {
      const group = document.createElement('div');
      group.className = 'layer-group';
      group.dataset.layerId = layer.id;
      layerGroups[layer.id] = group;

      const header = document.createElement('div');
      header.className = 'layer-header';
      header.style.color = domainColors[layer.domain];
      header.textContent = layer.name;
      header.dataset.layerId = layer.id;
      layerHeaders[layer.id] = header;
      group.appendChild(header);

      // Add exposes as concepts
      layer.exposes.forEach(exp => {
        const concept = document.createElement('div');
        concept.className = 'concept';
        concept.dataset.id = exp.id;
        concept.dataset.layer = layer.id;
        concept.textContent = exp.name;
        concept.title = exp.desc || '';
        conceptElements[exp.id] = concept;
        group.appendChild(concept);
      });

      gridDiv.appendChild(group);
    });

    container.appendChild(svg);
    container.appendChild(gridDiv);

    // State
    let selectedId = null;

    // Get positions for connection drawing
    function getPos(id) {
      const el = conceptElements[id];
      if (!el) return null;
      const rect = el.getBoundingClientRect();
      const containerRect = container.getBoundingClientRect();
      return {
        x: rect.left - containerRect.left + rect.width / 2,
        y: rect.top - containerRect.top + rect.height / 2,
        right: rect.right - containerRect.left,
        left: rect.left - containerRect.left
      };
    }

    // Trace full thread
    function traceThread(id) {
      const inThread = new Set([id]);

      function down(cid, visited = new Set()) {
        if (visited.has(cid)) return;
        visited.add(cid);
        (dependsOn[cid] || []).forEach(dep => {
          if (conceptElements[dep]) {
            inThread.add(dep);
            down(dep, visited);
          }
        });
      }

      function up(cid, visited = new Set()) {
        if (visited.has(cid)) return;
        visited.add(cid);
        (dependents[cid] || []).forEach(dep => {
          if (conceptElements[dep]) {
            inThread.add(dep);
            up(dep, visited);
          }
        });
      }

      down(id);
      up(id);
      return inThread;
    }

    // Get position for layer header
    function getHeaderPos(layerId) {
      const el = layerHeaders[layerId];
      if (!el) return null;
      const rect = el.getBoundingClientRect();
      const containerRect = container.getBoundingClientRect();
      return {
        x: rect.left - containerRect.left + rect.width / 2,
        y: rect.top - containerRect.top + rect.height / 2,
        right: rect.right - containerRect.left,
        left: rect.left - containerRect.left
      };
    }

    // Clear layer highlights
    function clearLayerHighlights() {
      Object.values(layerGroups).forEach(group => {
        group.classList.remove('highlighted');
        group.style.color = '';
      });
    }

    // Draw connections on hover - collapse to layer if >50% connected
    function drawConnections(id) {
      svg.innerHTML = '';
      clearLayerHighlights();
      const fromPos = getPos(id);
      if (!fromPos) return;

      const sourceLayer = exposeToLayer[id];
      const color = sourceLayer ? domainColors[layerById[sourceLayer].domain] : '#f97316';

      // Group dependencies by layer
      const depsByLayer = {};
      (dependsOn[id] || []).forEach(depId => {
        const layerId = exposeToLayer[depId];
        if (layerId) {
          if (!depsByLayer[layerId]) depsByLayer[layerId] = [];
          depsByLayer[layerId].push(depId);
        }
      });

      // Draw dependencies - collapse if >50% of layer
      Object.entries(depsByLayer).forEach(([layerId, deps]) => {
        const layer = layerById[layerId];
        const threshold = Math.ceil(layer.exposes.length * 0.5);
        const layerColor = domainColors[layer.domain] || '#64748b';
        if (deps.length >= threshold && layer.exposes.length > 2) {
          // Draw to layer header instead and highlight the layer group
          const headerPos = getHeaderPos(layerId);
          if (headerPos) {
            drawLine(headerPos, fromPos, color);
            layerGroups[layerId].classList.add('highlighted');
            layerGroups[layerId].style.color = layerColor;
          }
        } else {
          // Draw to individual concepts
          deps.forEach(depId => {
            const toPos = getPos(depId);
            if (toPos) drawLine(toPos, fromPos, color);
          });
        }
      });

      // Group dependents by layer
      const dentsByLayer = {};
      (dependents[id] || []).forEach(depId => {
        const layerId = exposeToLayer[depId];
        if (layerId) {
          if (!dentsByLayer[layerId]) dentsByLayer[layerId] = [];
          dentsByLayer[layerId].push(depId);
        }
      });

      // Draw dependents - collapse if >50% of layer
      Object.entries(dentsByLayer).forEach(([layerId, deps]) => {
        const layer = layerById[layerId];
        const threshold = Math.ceil(layer.exposes.length * 0.5);
        const layerColor = domainColors[layer.domain] || '#64748b';
        if (deps.length >= threshold && layer.exposes.length > 2) {
          // Draw to layer header instead and highlight the layer group
          const headerPos = getHeaderPos(layerId);
          if (headerPos) {
            drawLine(fromPos, headerPos, color);
            layerGroups[layerId].classList.add('highlighted');
            layerGroups[layerId].style.color = layerColor;
          }
        } else {
          // Draw to individual concepts
          deps.forEach(depId => {
            const toPos = getPos(depId);
            if (toPos) drawLine(fromPos, toPos, color);
          });
        }
      });
    }

    function drawLine(from, to, color) {
      const path = document.createElementNS(svgNS, 'path');
      // Use center points for more flexible curves
      const x1 = from.x;
      const y1 = from.y;
      const x2 = to.x;
      const y2 = to.y;

      const dx = x2 - x1;
      const dy = y2 - y1;
      const dist = Math.sqrt(dx * dx + dy * dy);
      const offset = Math.min(50, Math.max(20, dist * 0.3));

      // Adaptive control points based on direction
      let cx1, cy1, cx2, cy2;
      if (Math.abs(dx) > Math.abs(dy)) {
        // More horizontal - curve horizontally
        cx1 = x1 + (dx > 0 ? offset : -offset);
        cy1 = y1;
        cx2 = x2 + (dx > 0 ? -offset : offset);
        cy2 = y2;
      } else {
        // More vertical - curve vertically
        cx1 = x1;
        cy1 = y1 + (dy > 0 ? offset : -offset);
        cx2 = x2;
        cy2 = y2 + (dy > 0 ? -offset : offset);
      }

      path.setAttribute('d', `M${x1},${y1} C${cx1},${cy1} ${cx2},${cy2} ${x2},${y2}`);
      path.setAttribute('fill', 'none');
      path.setAttribute('stroke', color);
      path.setAttribute('stroke-width', '1.5');
      path.setAttribute('stroke-opacity', '0.6');
      svg.appendChild(path);
    }

    // Update visual state (normal mode)
    function updateState(inThread) {
      Object.entries(conceptElements).forEach(([id, el]) => {
        el.classList.remove('selected', 'active', 'inactive', 'engineer-highlight');
        el.style.color = '';
        el.style.opacity = '';
        if (selectedId === id) {
          el.classList.add('selected');
        } else if (inThread && inThread.has(id)) {
          el.classList.add('active');
          el.style.color = domainColors[layerById[exposeToLayer[id]]?.domain] || '#e2e8f0';
        } else if (inThread) {
          el.classList.add('inactive');
        }
      });
    }

    // Click handler - toggle selection
    gridDiv.addEventListener('click', (e) => {
      if (engineerMode) return; // Disable in engineer mode

      const concept = e.target.closest('.concept');
      if (!concept) {
        // Clicked outside - clear
        selectedId = null;
        updateState(null);
        svg.innerHTML = '';
        clearLayerHighlights();
        return;
      }

      const id = concept.dataset.id;
      if (selectedId === id) {
        // Toggle off if clicking same concept
        selectedId = null;
        updateState(null);
        svg.innerHTML = '';
        clearLayerHighlights();
      } else {
        // Select new concept
        selectedId = id;
        const inThread = traceThread(id);
        updateState(inThread);
      }
    });

    // Hover handler
    gridDiv.addEventListener('mouseover', (e) => {
      if (engineerMode) return; // Disable in engineer mode

      const concept = e.target.closest('.concept');
      if (concept) {
        drawConnections(concept.dataset.id);
      }
    });

    gridDiv.addEventListener('mouseout', (e) => {
      if (engineerMode) return; // Disable in engineer mode

      const concept = e.target.closest('.concept');
      if (concept) {
        svg.innerHTML = '';
        clearLayerHighlights();
      }
    });

    // Update SVG size
    setTimeout(() => {
      svg.setAttribute('viewBox', `0 0 ${container.scrollWidth} ${container.scrollHeight}`);
      svg.style.width = container.scrollWidth + 'px';
      svg.style.height = container.scrollHeight + 'px';
    }, 100);
  }

  // Calculate combined weight for a concept based on selected profiles
  function calculateWeight(conceptId) {
    if (selectedProfiles.size === 0) return 0;

    const weights = [];
    selectedProfiles.forEach(profileId => {
      const profile = data.profiles.find(p => p.id === profileId);
      if (profile && profile.touchpoints[conceptId] !== undefined) {
        weights.push(profile.touchpoints[conceptId]);
      } else {
        weights.push(0);
      }
    });

    if (weights.length === 0) return 0;

    if (combineMode === 'union') {
      // Add weights, saturate to 1
      return Math.min(1, weights.reduce((a, b) => a + b, 0));
    } else {
      // Multiply weights
      return weights.reduce((a, b) => a * b, 1);
    }
  }

  // Update concept highlighting in engineer mode
  function updateEngineerHighlights() {
    if (!engineerMode) return;

    Object.entries(conceptElements).forEach(([id, el]) => {
      el.classList.remove('selected', 'active', 'inactive');
      el.classList.add('engineer-highlight');

      const weight = calculateWeight(id);

      if (weight > 0) {
        // Highlight with weight-based opacity/brightness - use grayscale
        // Apply sqrt (gamma 0.5) to expand differences in lower weights
        // This makes intersection mode results more distinguishable
        const gamma = Math.sqrt(weight); // 0.64 -> 0.8, 0.25 -> 0.5
        const brightness = 0.25 + (gamma * 0.75); // Range from 0.25 to 1.0
        el.style.color = '#e2e8f0'; // Light gray/white
        el.style.opacity = brightness;
      } else {
        // Not touched by any selected profile
        el.style.color = '#64748b';
        el.style.opacity = '0.15';
      }
    });
  }

  // Clear engineer mode highlights
  function clearEngineerHighlights() {
    Object.entries(conceptElements).forEach(([id, el]) => {
      el.classList.remove('engineer-highlight');
      el.style.color = '';
      el.style.opacity = '';
    });
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
