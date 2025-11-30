// Math Quotes - Constellation Map
(function() {
  const width = 700;
  const height = 550;

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
  let svg, starsGroup, nodesGroup;

  async function init() {
    const container = document.getElementById('quotes-wheel');
    if (!container) return;

    try {
      let resp = await fetch('./assets/data/quotes.json');
      if (!resp.ok) resp = await fetch('/jadnohra.com/assets/data/quotes.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:red;">Failed to load quotes.</p>';
      return;
    }

    topics = {};
    for (const q of data.quotes) {
      for (const path of q.topics) {
        const [t, s] = path.split('/');
        if (!topics[t]) topics[t] = {};
        if (!topics[t][s]) topics[t][s] = [];
        topics[t][s].push(q);
      }
    }

    svg = d3.select(container)
      .append('svg')
      .attr('width', width)
      .attr('height', height)
      .style('background', bg)
      .style('border-radius', '12px')
      .style('display', 'block')
      .style('margin', '0 auto');

    starsGroup = svg.append('g').attr('class', 'stars');
    createBackgroundStars();

    nodesGroup = svg.append('g').attr('class', 'nodes');

    drawTopics();
  }

  function createBackgroundStars() {
    for (let i = 0; i < 120; i++) {
      const x = Math.random() * width;
      const y = Math.random() * height;
      const r = Math.random() * 1.2 + 0.3;

      starsGroup.append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', r)
        .attr('fill', starColor)
        .attr('opacity', Math.random() * 0.4 + 0.1)
        .call(twinkle);
    }
  }

  function twinkle(star) {
    star.transition()
      .duration(1500 + Math.random() * 2000)
      .attr('opacity', Math.random() * 0.2 + 0.05)
      .transition()
      .duration(1500 + Math.random() * 2000)
      .attr('opacity', Math.random() * 0.4 + 0.2)
      .on('end', function() { twinkle(d3.select(this)); });
  }

  function drawTopics() {
    nodesGroup.selectAll('*').transition().duration(200).style('opacity', 0).remove();

    setTimeout(() => {
      nodesGroup.selectAll('*').remove();

      const topicNames = Object.keys(topics);
      const cx = width / 2;
      const cy = height / 2;
      const radius = 170;

      // Center text
      const centerG = nodesGroup.append('g').style('opacity', 0);

      centerG.append('text')
        .attr('x', cx)
        .attr('y', cy - 15)
        .attr('text-anchor', 'middle')
        .attr('fill', '#e2e8f0')
        .attr('font-size', '18px')
        .attr('font-weight', '300')
        .text('Math Quotes');

      centerG.append('text')
        .attr('x', cx)
        .attr('y', cy + 10)
        .attr('text-anchor', 'middle')
        .attr('fill', '#64748b')
        .attr('font-size', '12px')
        .text(`${data.quotes.length} quotes`);

      centerG.append('text')
        .attr('x', cx)
        .attr('y', cy + 28)
        .attr('text-anchor', 'middle')
        .attr('fill', '#475569')
        .attr('font-size', '10px')
        .text('click a star');

      centerG.transition().duration(400).style('opacity', 1);

      // Topics
      topicNames.forEach((name, i) => {
        const angle = (i / topicNames.length) * 2 * Math.PI - Math.PI / 2;
        const x = cx + radius * Math.cos(angle);
        const y = cy + radius * Math.sin(angle);
        const color = topicColors[name] || '#fff';
        const quoteCount = Object.values(topics[name]).flat().length;

        const g = nodesGroup.append('g')
          .style('cursor', 'pointer')
          .style('opacity', 0)
          .on('click', () => selectTopic(name));

        // Glow
        g.append('circle')
          .attr('cx', x)
          .attr('cy', y)
          .attr('r', 0)
          .attr('fill', color)
          .attr('opacity', 0.25)
          .transition()
          .delay(i * 50)
          .duration(500)
          .attr('r', 28);

        // Star
        const star = g.append('circle')
          .attr('cx', x)
          .attr('cy', y)
          .attr('r', 0)
          .attr('fill', color);

        star.transition()
          .delay(i * 50)
          .duration(400)
          .attr('r', 9);

        star.on('mouseover', function() {
            d3.select(this).transition().duration(150).attr('r', 14);
          })
          .on('mouseout', function() {
            d3.select(this).transition().duration(150).attr('r', 9);
          });

        // Label
        g.append('text')
          .attr('x', x)
          .attr('y', y + 24)
          .attr('text-anchor', 'middle')
          .attr('fill', '#e2e8f0')
          .attr('font-size', '11px')
          .attr('font-weight', '500')
          .style('opacity', 0)
          .text(name)
          .transition()
          .delay(i * 50 + 200)
          .duration(300)
          .style('opacity', 1);

        g.append('text')
          .attr('x', x)
          .attr('y', y + 37)
          .attr('text-anchor', 'middle')
          .attr('fill', '#64748b')
          .attr('font-size', '9px')
          .text(`${quoteCount}`);

        g.transition().delay(i * 50).duration(300).style('opacity', 1);
      });
    }, 250);
  }

  function selectTopic(topicName) {
    nodesGroup.selectAll('*').transition().duration(200).style('opacity', 0);

    setTimeout(() => {
      nodesGroup.selectAll('*').remove();

      const color = topicColors[topicName] || '#fff';
      const subtopics = Object.keys(topics[topicName]);
      const cx = width / 2;
      const cy = height / 2;

      // Topic at center
      const centerG = nodesGroup.append('g').style('opacity', 0);

      centerG.append('circle')
        .attr('cx', cx)
        .attr('cy', cy)
        .attr('r', 40)
        .attr('fill', color)
        .attr('opacity', 0.2);

      centerG.append('circle')
        .attr('cx', cx)
        .attr('cy', cy)
        .attr('r', 14)
        .attr('fill', color);

      centerG.append('text')
        .attr('x', cx)
        .attr('y', cy + 32)
        .attr('text-anchor', 'middle')
        .attr('fill', color)
        .attr('font-size', '14px')
        .attr('font-weight', 'bold')
        .text(topicName);

      centerG.transition().duration(300).style('opacity', 1);

      // Subtopics
      const radius = Math.min(150, 180 - subtopics.length * 5);

      subtopics.forEach((sub, i) => {
        const angle = (i / subtopics.length) * 2 * Math.PI - Math.PI / 2;
        const x = cx + radius * Math.cos(angle);
        const y = cy + radius * Math.sin(angle);
        const quotes = topics[topicName][sub];

        // Line
        nodesGroup.append('line')
          .attr('x1', cx)
          .attr('y1', cy)
          .attr('x2', cx)
          .attr('y2', cy)
          .attr('stroke', color)
          .attr('stroke-opacity', 0.15)
          .attr('stroke-width', 1)
          .transition()
          .delay(i * 30 + 100)
          .duration(400)
          .attr('x2', x)
          .attr('y2', y);

        const g = nodesGroup.append('g')
          .style('cursor', 'pointer')
          .style('opacity', 0)
          .on('click', () => showQuote(topicName, sub));

        g.append('circle')
          .attr('cx', x)
          .attr('cy', y)
          .attr('r', 12)
          .attr('fill', color)
          .attr('opacity', 0.12);

        const star = g.append('circle')
          .attr('cx', x)
          .attr('cy', y)
          .attr('r', 5)
          .attr('fill', starColor);

        star.on('mouseover', function() {
            d3.select(this).transition().duration(100).attr('r', 8).attr('fill', color);
          })
          .on('mouseout', function() {
            d3.select(this).transition().duration(100).attr('r', 5).attr('fill', starColor);
          });

        let label = sub || 'General';
        if (label.length > 14) label = label.substring(0, 12) + '..';

        g.append('text')
          .attr('x', x)
          .attr('y', y + 18)
          .attr('text-anchor', 'middle')
          .attr('fill', '#cbd5e1')
          .attr('font-size', '9px')
          .text(label);

        g.append('text')
          .attr('x', x)
          .attr('y', y + 28)
          .attr('text-anchor', 'middle')
          .attr('fill', '#64748b')
          .attr('font-size', '8px')
          .text(quotes.length);

        g.transition().delay(i * 30 + 150).duration(300).style('opacity', 1);
      });

      // Back hint
      nodesGroup.append('text')
        .attr('x', cx)
        .attr('y', height - 25)
        .attr('text-anchor', 'middle')
        .attr('fill', '#475569')
        .attr('font-size', '10px')
        .style('opacity', 0)
        .text('click star for quote · click anywhere else to go back')
        .transition()
        .delay(400)
        .duration(300)
        .style('opacity', 1);

      // Click background to go back
      svg.on('click.back', function(event) {
        if (event.target.tagName === 'svg' || event.target.closest('.stars')) {
          svg.on('click.back', null);
          drawTopics();
        }
      });
    }, 250);
  }

  function showQuote(topicName, subtopicName) {
    const quotes = topics[topicName][subtopicName];
    if (!quotes || quotes.length === 0) return;

    const q = quotes[Math.floor(Math.random() * quotes.length)];
    const color = topicColors[topicName] || '#fff';
    const cx = width / 2;
    const cy = height / 2;

    nodesGroup.selectAll('*').transition().duration(200).style('opacity', 0);

    setTimeout(() => {
      nodesGroup.selectAll('*').remove();
      svg.on('click.back', null);

      // Quote display
      const quoteG = nodesGroup.append('g').style('opacity', 0);

      // Header
      quoteG.append('text')
        .attr('x', cx)
        .attr('y', 40)
        .attr('text-anchor', 'middle')
        .attr('fill', color)
        .attr('font-size', '12px')
        .attr('font-weight', '500')
        .text(`${topicName} › ${subtopicName || 'General'}`);

      // Quote text - wrap it
      let text = q.text;
      const maxChars = 55;
      const words = text.split(' ');
      const lines = [];
      let line = '';

      for (const w of words) {
        if ((line + ' ' + w).length > maxChars) {
          lines.push(line);
          line = w;
        } else {
          line = line ? line + ' ' + w : w;
        }
      }
      if (line) lines.push(line);

      const maxLines = 12;
      const showLines = lines.slice(0, maxLines);
      if (lines.length > maxLines) showLines[maxLines - 1] += '...';

      const startY = cy - (showLines.length * 11);

      quoteG.append('text')
        .attr('x', cx - 180)
        .attr('y', startY - 15)
        .attr('fill', '#64748b')
        .attr('font-size', '24px')
        .attr('font-family', 'Georgia, serif')
        .text('"');

      showLines.forEach((ln, i) => {
        quoteG.append('text')
          .attr('x', cx)
          .attr('y', startY + i * 22)
          .attr('text-anchor', 'middle')
          .attr('fill', '#e2e8f0')
          .attr('font-size', '14px')
          .attr('font-family', 'Georgia, serif')
          .attr('font-style', 'italic')
          .text(ln);
      });

      // Source
      if (q.source) {
        let src = q.source;
        if (src.length > 60) src = src.substring(0, 57) + '...';
        quoteG.append('text')
          .attr('x', cx)
          .attr('y', startY + showLines.length * 22 + 20)
          .attr('text-anchor', 'middle')
          .attr('fill', '#64748b')
          .attr('font-size', '11px')
          .text('— ' + src);
      }

      // Buttons
      const btnY = height - 60;

      // Next button
      const nextBtn = quoteG.append('g')
        .style('cursor', 'pointer')
        .on('click', (e) => { e.stopPropagation(); showQuote(topicName, subtopicName); });

      nextBtn.append('rect')
        .attr('x', cx - 50)
        .attr('y', btnY - 12)
        .attr('width', 100)
        .attr('height', 28)
        .attr('rx', 6)
        .attr('fill', color);

      nextBtn.append('text')
        .attr('x', cx)
        .attr('y', btnY + 5)
        .attr('text-anchor', 'middle')
        .attr('fill', 'white')
        .attr('font-size', '12px')
        .text('Next Quote →');

      // Count
      quoteG.append('text')
        .attr('x', cx)
        .attr('y', btnY + 30)
        .attr('text-anchor', 'middle')
        .attr('fill', '#475569')
        .attr('font-size', '10px')
        .text(`${quotes.length} quotes · click anywhere to return`);

      quoteG.transition().duration(400).style('opacity', 1);

      // Click anywhere to go back to start
      svg.on('click.back', function(event) {
        if (!event.target.closest('g[style*="cursor: pointer"]')) {
          svg.on('click.back', null);
          drawTopics();
        }
      });
    }, 250);
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
