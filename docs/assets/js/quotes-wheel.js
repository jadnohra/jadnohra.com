// Math Quotes - Constellation Map
(function() {
  const width = 700;
  const height = 500;

  // Colors
  const bg = '#0f172a';
  const starColor = '#fef3c7';
  const topicColors = {
    "Jad's System": "#14b8a6",
    "Foundations": "#ef4444",
    "Analysis": "#3b82f6",
    "Algebra": "#22c55e",
    "Geometry": "#a855f7",
    "Physics": "#f59e0b",
    "Philosophy": "#ec4899"
  };

  let data = null;
  let topics = null;
  let svg, starsGroup, nodesGroup, quoteCard;
  let selectedTopic = null;
  let subtopicNodes = [];

  async function init() {
    const container = document.getElementById('quotes-wheel');
    if (!container) return;

    // Load data
    try {
      let resp = await fetch('./assets/data/quotes.json');
      if (!resp.ok) resp = await fetch('/jadnohra.com/assets/data/quotes.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:red;">Failed to load quotes.</p>';
      return;
    }

    // Build topics
    topics = {};
    for (const q of data.quotes) {
      for (const path of q.topics) {
        const [t, s] = path.split('/');
        if (!topics[t]) topics[t] = {};
        if (!topics[t][s]) topics[t][s] = [];
        topics[t][s].push(q);
      }
    }

    // Create SVG
    svg = d3.select(container)
      .append('svg')
      .attr('width', width)
      .attr('height', height)
      .style('background', bg)
      .style('border-radius', '12px')
      .style('display', 'block')
      .style('margin', '0 auto');

    // Background stars
    starsGroup = svg.append('g').attr('class', 'stars');
    createBackgroundStars();

    // Nodes group
    nodesGroup = svg.append('g').attr('class', 'nodes');

    // Quote card below SVG
    quoteCard = d3.select(container)
      .append('div')
      .attr('id', 'quote-card')
      .style('max-width', width + 'px')
      .style('margin', '1rem auto')
      .style('padding', '1.5rem')
      .style('background', '#1e293b')
      .style('border-radius', '12px')
      .style('color', 'white')
      .style('display', 'none')
      .style('font-family', 'Georgia, serif');

    // Draw initial constellation
    drawTopics();
  }

  function createBackgroundStars() {
    // Random tiny stars
    for (let i = 0; i < 150; i++) {
      const x = Math.random() * width;
      const y = Math.random() * height;
      const r = Math.random() * 1.2 + 0.3;
      const opacity = Math.random() * 0.5 + 0.2;

      starsGroup.append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', r)
        .attr('fill', starColor)
        .attr('opacity', opacity)
        .call(twinkle);
    }
  }

  function twinkle(star) {
    star.transition()
      .duration(1000 + Math.random() * 2000)
      .attr('opacity', Math.random() * 0.3 + 0.1)
      .transition()
      .duration(1000 + Math.random() * 2000)
      .attr('opacity', Math.random() * 0.5 + 0.3)
      .on('end', function() { twinkle(d3.select(this)); });
  }

  function drawTopics() {
    nodesGroup.selectAll('*').remove();
    selectedTopic = null;
    subtopicNodes = [];
    quoteCard.style('display', 'none');

    const topicNames = Object.keys(topics);
    const cx = width / 2;
    const cy = height / 2;
    const radius = 160;

    topicNames.forEach((name, i) => {
      const angle = (i / topicNames.length) * 2 * Math.PI - Math.PI / 2;
      const x = cx + radius * Math.cos(angle);
      const y = cy + radius * Math.sin(angle);
      const color = topicColors[name] || '#fff';
      const subtopicCount = Object.keys(topics[name]).length;
      const quoteCount = Object.values(topics[name]).flat().length;

      const g = nodesGroup.append('g')
        .attr('class', 'topic-node')
        .style('cursor', 'pointer')
        .on('click', () => selectTopic(name));

      // Glow
      g.append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', 25)
        .attr('fill', color)
        .attr('opacity', 0.2)
        .attr('filter', 'blur(8px)');

      // Star
      g.append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', 8)
        .attr('fill', color)
        .on('mouseover', function() {
          d3.select(this).transition().duration(150).attr('r', 12);
        })
        .on('mouseout', function() {
          d3.select(this).transition().duration(150).attr('r', 8);
        });

      // Label
      g.append('text')
        .attr('x', x)
        .attr('y', y + 22)
        .attr('text-anchor', 'middle')
        .attr('fill', '#e2e8f0')
        .attr('font-size', '11px')
        .attr('font-weight', '500')
        .text(name);

      // Count
      g.append('text')
        .attr('x', x)
        .attr('y', y + 35)
        .attr('text-anchor', 'middle')
        .attr('fill', '#64748b')
        .attr('font-size', '9px')
        .text(`${quoteCount} quotes`);
    });

    // Center text
    nodesGroup.append('text')
      .attr('x', cx)
      .attr('y', cy - 10)
      .attr('text-anchor', 'middle')
      .attr('fill', '#94a3b8')
      .attr('font-size', '14px')
      .text('Math Quotes');

    nodesGroup.append('text')
      .attr('x', cx)
      .attr('y', cy + 10)
      .attr('text-anchor', 'middle')
      .attr('fill', '#64748b')
      .attr('font-size', '11px')
      .text(`${data.quotes.length} quotes · Click a topic`);
  }

  function selectTopic(topicName) {
    nodesGroup.selectAll('*').remove();
    selectedTopic = topicName;
    quoteCard.style('display', 'none');

    const color = topicColors[topicName] || '#fff';
    const subtopics = Object.keys(topics[topicName]);
    const cx = width / 2;
    const cy = height / 2;

    // Back button
    const backBtn = nodesGroup.append('g')
      .style('cursor', 'pointer')
      .on('click', drawTopics);

    backBtn.append('circle')
      .attr('cx', 40)
      .attr('cy', 40)
      .attr('r', 18)
      .attr('fill', '#334155');

    backBtn.append('text')
      .attr('x', 40)
      .attr('y', 44)
      .attr('text-anchor', 'middle')
      .attr('fill', '#94a3b8')
      .attr('font-size', '16px')
      .text('←');

    // Topic at center
    nodesGroup.append('circle')
      .attr('cx', cx)
      .attr('cy', cy)
      .attr('r', 35)
      .attr('fill', color)
      .attr('opacity', 0.15);

    nodesGroup.append('circle')
      .attr('cx', cx)
      .attr('cy', cy)
      .attr('r', 12)
      .attr('fill', color);

    nodesGroup.append('text')
      .attr('x', cx)
      .attr('y', cy + 28)
      .attr('text-anchor', 'middle')
      .attr('fill', color)
      .attr('font-size', '13px')
      .attr('font-weight', 'bold')
      .text(topicName);

    // Subtopics around it
    const radius = 140;
    subtopicNodes = [];

    subtopics.forEach((sub, i) => {
      const angle = (i / subtopics.length) * 2 * Math.PI - Math.PI / 2;
      const x = cx + radius * Math.cos(angle);
      const y = cy + radius * Math.sin(angle);
      const quotes = topics[topicName][sub];

      subtopicNodes.push({ name: sub, x, y, quotes });

      // Connection line
      nodesGroup.append('line')
        .attr('x1', cx)
        .attr('y1', cy)
        .attr('x2', x)
        .attr('y2', y)
        .attr('stroke', color)
        .attr('stroke-opacity', 0.2)
        .attr('stroke-width', 1);

      const g = nodesGroup.append('g')
        .style('cursor', 'pointer')
        .on('click', () => showQuote(topicName, sub));

      // Glow
      g.append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', 15)
        .attr('fill', color)
        .attr('opacity', 0.15);

      // Star
      g.append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', 5)
        .attr('fill', starColor)
        .on('mouseover', function() {
          d3.select(this).transition().duration(150).attr('r', 8).attr('fill', color);
        })
        .on('mouseout', function() {
          d3.select(this).transition().duration(150).attr('r', 5).attr('fill', starColor);
        });

      // Label
      let label = sub;
      if (label && label.length > 15) label = label.substring(0, 13) + '..';

      g.append('text')
        .attr('x', x)
        .attr('y', y + 18)
        .attr('text-anchor', 'middle')
        .attr('fill', '#cbd5e1')
        .attr('font-size', '10px')
        .text(label || 'General');

      // Count
      g.append('text')
        .attr('x', x)
        .attr('y', y + 29)
        .attr('text-anchor', 'middle')
        .attr('fill', '#64748b')
        .attr('font-size', '8px')
        .text(`${quotes.length}`);
    });
  }

  function showQuote(topicName, subtopicName) {
    const quotes = topics[topicName][subtopicName];
    if (!quotes || quotes.length === 0) return;

    const q = quotes[Math.floor(Math.random() * quotes.length)];
    const color = topicColors[topicName] || '#fff';

    quoteCard.style('display', 'block')
      .html(`
        <div style="color: ${color}; font-size: 12px; margin-bottom: 0.75rem; font-family: sans-serif;">
          ${topicName} › ${subtopicName || 'General'}
        </div>
        <div style="font-size: 15px; line-height: 1.7; font-style: italic; margin-bottom: 1rem;">
          "${q.text}"
        </div>
        ${q.source ? `<div style="color: #94a3b8; font-size: 12px; font-family: sans-serif;">— ${q.source}</div>` : ''}
        <div style="margin-top: 1.25rem; display: flex; gap: 0.75rem; align-items: center;">
          <button id="quote-back" style="padding: 0.5rem 1rem; background: #334155; color: #94a3b8; border: none; border-radius: 6px; cursor: pointer; font-size: 12px;">← Back</button>
          <button id="quote-next" style="padding: 0.5rem 1rem; background: ${color}; color: white; border: none; border-radius: 6px; cursor: pointer; font-size: 12px;">Next Quote →</button>
          <span style="color: #64748b; font-size: 11px; margin-left: auto;">${quotes.length} quotes</span>
        </div>
      `);

    document.getElementById('quote-back').onclick = () => selectTopic(topicName);
    document.getElementById('quote-next').onclick = () => showQuote(topicName, subtopicName);
  }

  // Start
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
