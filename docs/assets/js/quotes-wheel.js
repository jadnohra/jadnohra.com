// Math Quotes Wheel - Simple, Robust Implementation
(function() {
  const width = 550;
  const height = 550;
  const outerRadius = 220;
  const innerRadius = 80;

  // Colors for topics
  const colors = {
    "Jad's System": "#14b8a6",
    "Foundations": "#dc2626",
    "Analysis": "#2563eb",
    "Algebra": "#059669",
    "Geometry": "#7c3aed",
    "Physics": "#f59e0b",
    "Philosophy": "#ec4899"
  };

  // State
  let data = null;
  let topics = null;
  let level = 0; // 0=topics, 1=subtopics, 2=quote
  let currentTopic = null;
  let currentSubtopic = null;
  let svg, wheel, center;

  // Initialize
  async function init() {
    const container = document.getElementById('quotes-wheel');
    if (!container) return;

    // Try to load data
    try {
      // Try relative path first (works with Jekyll baseurl)
      let resp = await fetch('./assets/data/quotes.json');
      if (!resp.ok) {
        // Fallback: try from root with baseurl
        const base = document.querySelector('base')?.href || '';
        resp = await fetch(base + 'assets/data/quotes.json');
      }
      if (!resp.ok) {
        // Final fallback: absolute path
        resp = await fetch('/jadnohra.com/assets/data/quotes.json');
      }
      data = await resp.json();
    } catch (e) {
      console.error('Failed to load quotes:', e);
      container.innerHTML = '<p style="color:red;">Failed to load quotes. Check console.</p>';
      return;
    }

    // Build topic tree: { topicName: { subtopicName: [quotes] } }
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
      .attr('viewBox', `${-width/2} ${-height/2} ${width} ${height}`)
      .style('max-width', '100%')
      .style('height', 'auto')
      .style('display', 'block')
      .style('margin', '0 auto');

    // Outer ring for aesthetics
    svg.append('circle')
      .attr('r', outerRadius + 8)
      .attr('fill', 'none')
      .attr('stroke', '#d1d5db')
      .attr('stroke-width', 2);

    // Wheel group
    wheel = svg.append('g').attr('class', 'wheel');

    // Center group
    center = svg.append('g').attr('class', 'center');

    // Draw initial view
    drawTopics();
  }

  // Draw main topics
  function drawTopics() {
    level = 0;
    currentTopic = null;
    currentSubtopic = null;

    const items = Object.keys(topics);
    drawSegments(items, null);
    drawCenterLabel('Math\nQuotes', `${data.quotes.length} quotes`, null);
  }

  // Draw subtopics for a topic
  function drawSubtopics(topicName) {
    level = 1;
    currentTopic = topicName;
    currentSubtopic = null;

    const subs = Object.keys(topics[topicName]);
    drawSegments(subs, colors[topicName]);

    const count = subs.reduce((n, s) => n + topics[topicName][s].length, 0);
    drawCenterLabel(topicName, `${count} quotes`, goBack);
  }

  // Draw wheel segments
  function drawSegments(items, fixedColor) {
    wheel.selectAll('*').remove();

    const n = items.length;
    const angle = (2 * Math.PI) / n;
    const arc = d3.arc().innerRadius(innerRadius + 15).outerRadius(outerRadius);

    items.forEach((item, i) => {
      const startAngle = i * angle - Math.PI / 2;
      const endAngle = (i + 1) * angle - Math.PI / 2;
      const midAngle = (startAngle + endAngle) / 2;
      const color = fixedColor || colors[item] || '#6b7280';

      // Segment group
      const g = wheel.append('g')
        .style('cursor', 'pointer')
        .on('click', () => handleSegmentClick(item));

      // Arc path
      g.append('path')
        .attr('d', arc({ startAngle: startAngle + 0.02, endAngle: endAngle - 0.02 }))
        .attr('fill', color)
        .attr('stroke', 'white')
        .attr('stroke-width', 2)
        .on('mouseover', function() {
          d3.select(this).attr('fill', d3.color(color).brighter(0.3));
        })
        .on('mouseout', function() {
          d3.select(this).attr('fill', color);
        });

      // Label
      const labelR = (innerRadius + outerRadius) / 2 + 5;
      const labelX = labelR * Math.cos(midAngle);
      const labelY = labelR * Math.sin(midAngle);
      let rotation = midAngle * 180 / Math.PI;
      if (rotation > 90 || rotation < -90) rotation += 180;

      let displayText = item;
      if (displayText.length > 12) displayText = displayText.substring(0, 10) + '..';

      g.append('text')
        .attr('x', labelX)
        .attr('y', labelY)
        .attr('transform', `rotate(${rotation}, ${labelX}, ${labelY})`)
        .attr('text-anchor', 'middle')
        .attr('dominant-baseline', 'middle')
        .attr('fill', 'white')
        .attr('font-size', n > 8 ? '10px' : '12px')
        .attr('font-weight', 'bold')
        .style('pointer-events', 'none')
        .text(displayText);
    });
  }

  // Draw center label
  function drawCenterLabel(title, subtitle, backFn) {
    center.selectAll('*').remove();

    // Center circle
    center.append('circle')
      .attr('r', innerRadius)
      .attr('fill', '#1f2937');

    // Title (can have newlines)
    const lines = title.split('\n');
    lines.forEach((line, i) => {
      center.append('text')
        .attr('x', 0)
        .attr('y', (i - (lines.length - 1) / 2) * 20 - 5)
        .attr('text-anchor', 'middle')
        .attr('dominant-baseline', 'middle')
        .attr('fill', 'white')
        .attr('font-size', '15px')
        .attr('font-weight', 'bold')
        .text(line);
    });

    // Subtitle
    if (subtitle) {
      center.append('text')
        .attr('x', 0)
        .attr('y', lines.length * 12 + 8)
        .attr('text-anchor', 'middle')
        .attr('fill', '#9ca3af')
        .attr('font-size', '10px')
        .text(subtitle);
    }

    // Back button
    if (backFn) {
      const btn = center.append('g')
        .attr('transform', `translate(0, ${innerRadius - 20})`)
        .style('cursor', 'pointer')
        .on('click', backFn);

      btn.append('circle')
        .attr('r', 14)
        .attr('fill', '#374151');

      btn.append('text')
        .attr('text-anchor', 'middle')
        .attr('dominant-baseline', 'middle')
        .attr('fill', '#60a5fa')
        .attr('font-size', '16px')
        .text('←');
    }
  }

  // Handle segment click
  function handleSegmentClick(item) {
    if (level === 0) {
      // Show subtopics
      drawSubtopics(item);
    } else if (level === 1) {
      // Show quote
      currentSubtopic = item;
      showQuote();
    }
  }

  // Go back one level
  function goBack() {
    if (level === 1) {
      drawTopics();
    } else if (level === 2) {
      drawSubtopics(currentTopic);
    }
  }

  // Show a random quote
  function showQuote() {
    level = 2;
    const quotes = topics[currentTopic][currentSubtopic];
    if (!quotes || quotes.length === 0) return;

    const q = quotes[Math.floor(Math.random() * quotes.length)];

    // Fade wheel
    wheel.transition().duration(200).style('opacity', 0.15);

    // Clear center
    center.selectAll('*').remove();

    // Expanded background
    center.append('circle')
      .attr('r', outerRadius - 10)
      .attr('fill', '#1f2937')
      .attr('fill-opacity', 0.97);

    // Header
    center.append('text')
      .attr('x', 0)
      .attr('y', -outerRadius + 40)
      .attr('text-anchor', 'middle')
      .attr('fill', colors[currentTopic])
      .attr('font-size', '13px')
      .attr('font-weight', 'bold')
      .text(`${currentTopic} › ${currentSubtopic}`);

    // Quote text - wrap it
    let text = q.text;
    if (text.length > 350) text = text.substring(0, 350) + '...';

    const words = text.split(' ');
    const lines = [];
    let line = '';
    const maxChars = 38;

    for (const w of words) {
      if ((line + ' ' + w).length > maxChars) {
        lines.push(line);
        line = w;
      } else {
        line = line ? line + ' ' + w : w;
      }
    }
    if (line) lines.push(line);

    const maxLines = 9;
    const showLines = lines.slice(0, maxLines);
    if (lines.length > maxLines) showLines[maxLines - 1] += '...';

    const startY = -showLines.length * 10;
    showLines.forEach((ln, i) => {
      center.append('text')
        .attr('x', 0)
        .attr('y', startY + i * 20)
        .attr('text-anchor', 'middle')
        .attr('fill', 'white')
        .attr('font-size', '12px')
        .attr('font-style', 'italic')
        .text(ln);
    });

    // Source
    if (q.source) {
      let src = q.source;
      if (src.length > 45) src = src.substring(0, 42) + '...';
      center.append('text')
        .attr('x', 0)
        .attr('y', startY + showLines.length * 20 + 15)
        .attr('text-anchor', 'middle')
        .attr('fill', '#9ca3af')
        .attr('font-size', '10px')
        .text('— ' + src);
    }

    // Buttons
    const btnY = outerRadius - 45;

    // Back button
    const backBtn = center.append('g')
      .attr('transform', `translate(-45, ${btnY})`)
      .style('cursor', 'pointer')
      .on('click', goBack);

    backBtn.append('rect')
      .attr('x', -30).attr('y', -12)
      .attr('width', 60).attr('height', 24)
      .attr('rx', 4)
      .attr('fill', '#374151');

    backBtn.append('text')
      .attr('text-anchor', 'middle')
      .attr('dominant-baseline', 'middle')
      .attr('fill', '#60a5fa')
      .attr('font-size', '11px')
      .text('← Back');

    // Next button
    const nextBtn = center.append('g')
      .attr('transform', `translate(45, ${btnY})`)
      .style('cursor', 'pointer')
      .on('click', showQuote);

    nextBtn.append('rect')
      .attr('x', -30).attr('y', -12)
      .attr('width', 60).attr('height', 24)
      .attr('rx', 4)
      .attr('fill', colors[currentTopic]);

    nextBtn.append('text')
      .attr('text-anchor', 'middle')
      .attr('dominant-baseline', 'middle')
      .attr('fill', 'white')
      .attr('font-size', '11px')
      .text('Next →');

    // Count
    center.append('text')
      .attr('x', 0)
      .attr('y', btnY + 25)
      .attr('text-anchor', 'middle')
      .attr('fill', '#6b7280')
      .attr('font-size', '9px')
      .text(`${quotes.length} quotes in this category`);
  }

  // Start
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
