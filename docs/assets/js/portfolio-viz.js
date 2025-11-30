// Portfolio Hierarchical Edge Bundling Visualization
// Based on Mike Bostock's D3 example: https://observablehq.com/@d3/hierarchical-edge-bundling

(function() {
  const width = 800;
  const radius = width / 2;
  const innerRadius = radius - 120;

  // Color scheme matching portfolio CSS
  const colors = {
    career: "#dc2626",      // red
    project: "#2563eb",     // blue
    algo: "#059669",        // green
    theory: "#7c3aed"       // purple
  };

  // Create the visualization
  async function createViz() {
    const container = document.getElementById('portfolio-viz');
    if (!container) return;

    // Load data
    // Get baseurl from page if available
    const baseUrl = document.querySelector('script[src*="portfolio-viz"]')?.src.replace(/\/assets\/js\/portfolio-viz\.js.*/, '') || '';
    const response = await fetch(baseUrl + '/assets/data/portfolio-graph.json');
    const data = await response.json();

    // Process hierarchy
    const root = d3.hierarchy(data)
      .sum(d => d.children ? 0 : 1);

    // Create cluster layout
    const cluster = d3.cluster()
      .size([2 * Math.PI, innerRadius]);

    cluster(root);

    // Get all leaves
    const leaves = root.leaves();

    // Create name->node map
    const nodeByName = new Map(leaves.map(d => [d.data.name, d]));

    // Process imports into links
    const links = [];
    for (const leaf of leaves) {
      if (leaf.data.imports) {
        for (const imp of leaf.data.imports) {
          const target = nodeByName.get(imp);
          if (target) {
            links.push({source: leaf, target: target});
          }
        }
      }
    }

    // Create SVG
    const svg = d3.select(container)
      .append("svg")
      .attr("viewBox", [-width / 2, -width / 2, width, width])
      .attr("width", width)
      .attr("height", width)
      .attr("style", "max-width: 100%; height: auto; font: 10px sans-serif;");

    // Add links
    const line = d3.lineRadial()
      .curve(d3.curveBundle.beta(0.85))
      .radius(d => d.y)
      .angle(d => d.x);

    const link = svg.append("g")
      .attr("fill", "none")
      .attr("stroke-opacity", 0.6)
      .selectAll("path")
      .data(links)
      .join("path")
      .attr("stroke", d => {
        const sourceType = d.source.data.name.split('.')[0];
        return colors[sourceType] || "#999";
      })
      .attr("stroke-width", 1.5)
      .attr("d", d => {
        const path = d.source.path(d.target);
        return line(path);
      });

    // Add nodes
    const node = svg.append("g")
      .selectAll("g")
      .data(leaves)
      .join("g")
      .attr("transform", d => `rotate(${d.x * 180 / Math.PI - 90}) translate(${d.y},0)`)
      .append("text")
      .attr("dy", "0.31em")
      .attr("x", d => d.x < Math.PI ? 6 : -6)
      .attr("text-anchor", d => d.x < Math.PI ? "start" : "end")
      .attr("transform", d => d.x >= Math.PI ? "rotate(180)" : null)
      .attr("fill", d => {
        const type = d.data.name.split('.')[0];
        return colors[type] || "#333";
      })
      .attr("font-weight", "500")
      .text(d => d.data.label || d.data.name.split('.')[1])
      .call(text => text.append("title").text(d => d.data.name));

    // Add interactivity
    node.on("mouseover", function(event, d) {
      // Highlight connected links
      link.attr("stroke-opacity", l => {
        if (l.source === d || l.target === d) return 1;
        return 0.1;
      });
      link.attr("stroke-width", l => {
        if (l.source === d) return 2.5; // outgoing
        if (l.target === d) return 2.5; // incoming
        return 1;
      });
      link.attr("stroke", l => {
        if (l.source === d) return "#ef4444"; // red for outgoing
        if (l.target === d) return "#22c55e"; // green for incoming
        const sourceType = l.source.data.name.split('.')[0];
        return colors[sourceType] || "#999";
      });

      // Highlight connected nodes
      const connected = new Set();
      for (const l of links) {
        if (l.source === d) connected.add(l.target);
        if (l.target === d) connected.add(l.source);
      }
      connected.add(d);

      node.attr("fill-opacity", n => connected.has(n) ? 1 : 0.2);
      node.attr("font-weight", n => connected.has(n) ? "bold" : "normal");
    });

    node.on("mouseout", function() {
      link.attr("stroke-opacity", 0.6);
      link.attr("stroke-width", 1.5);
      link.attr("stroke", d => {
        const sourceType = d.source.data.name.split('.')[0];
        return colors[sourceType] || "#999";
      });
      node.attr("fill-opacity", 1);
      node.attr("font-weight", "500");
    });

    // Add legend
    const legend = svg.append("g")
      .attr("transform", `translate(${-width/2 + 20}, ${-width/2 + 20})`);

    const legendData = [
      {label: "Career", color: colors.career},
      {label: "Project", color: colors.project},
      {label: "Algorithm", color: colors.algo},
      {label: "Theory", color: colors.theory}
    ];

    legend.selectAll("g")
      .data(legendData)
      .join("g")
      .attr("transform", (d, i) => `translate(0, ${i * 20})`)
      .call(g => {
        g.append("circle")
          .attr("r", 5)
          .attr("fill", d => d.color);
        g.append("text")
          .attr("x", 12)
          .attr("y", 4)
          .attr("font-size", "12px")
          .attr("fill", "#666")
          .text(d => d.label);
      });
  }

  // Initialize when DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', createViz);
  } else {
    createViz();
  }
})();
