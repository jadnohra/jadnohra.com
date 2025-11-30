// Circular Quote Wheel - Single morphing wheel
// Level 1: Topics → Level 2: Subtopics → Quote display
(function() {
  const width = 600;
  const height = 600;
  const radius = 250;
  const innerRadius = 100;

  // Color scheme for main topics
  const topicColors = {
    "Jad's System": "#14b8a6",
    "Foundations": "#dc2626",
    "Analysis": "#2563eb",
    "Algebra": "#059669",
    "Geometry": "#7c3aed",
    "Physics": "#f59e0b",
    "Philosophy": "#ec4899"
  };

  let quotesData = null;
  let topics = null;
  let currentLevel = 0;  // 0 = main topics, 1 = subtopics, 2 = quote
  let selectedTopic = null;
  let selectedSubtopic = null;
  let svg, wheelGroup, centerGroup;

  async function init() {
    const container = document.getElementById('quotes-wheel');
    if (!container) return;

    // Load quotes data
    const baseUrl = document.querySelector('script[src*="quotes-wheel"]')?.src.replace(/\/assets\/js\/quotes-wheel\.js.*/, '') || '';
    try {
      const response = await fetch(baseUrl + '/assets/data/quotes.json');
      quotesData = await response.json();
    } catch (e) {
      container.innerHTML = '<p>Failed to load quotes data.</p>';
      return;
    }

    // Build topic structure
    topics = buildTopicStructure();

    // Create SVG
    svg = d3.select(container)
      .append("svg")
      .attr("viewBox", [-width/2, -height/2, width, height])
      .attr("width", width)
      .attr("height", height)
      .style("max-width", "100%")
      .style("height", "auto");

    // Background circle for aesthetics
    svg.append("circle")
      .attr("r", radius + 10)
      .attr("fill", "none")
      .attr("stroke", "#e5e7eb")
      .attr("stroke-width", 2);

    // Wheel group (segments)
    wheelGroup = svg.append("g").attr("class", "wheel");

    // Center group
    centerGroup = svg.append("g").attr("class", "center");

    // Draw initial state
    drawWheel(Object.keys(topics), null);
    drawCenter("Math\nQuotes", "Select a topic", null);
  }

  function buildTopicStructure() {
    const t = {};
    for (const quote of quotesData.quotes) {
      for (const topicPath of quote.topics) {
        const [main, sub] = topicPath.split('/');
        if (!t[main]) t[main] = {};
        if (!t[main][sub]) t[main][sub] = [];
        t[main][sub].push(quote);
      }
    }
    return t;
  }

  function drawWheel(items, parentColor) {
    const n = items.length;
    const arcAngle = (2 * Math.PI) / n;

    const arc = d3.arc()
      .innerRadius(innerRadius + 20)
      .outerRadius(radius);

    // Remove old segments with animation
    wheelGroup.selectAll("g.segment")
      .transition()
      .duration(300)
      .style("opacity", 0)
      .remove();

    // Add new segments
    const segments = wheelGroup.selectAll("g.segment-new")
      .data(items)
      .join("g")
      .attr("class", "segment")
      .style("opacity", 0)
      .style("cursor", "pointer");

    segments.append("path")
      .attr("d", (d, i) => arc({
        startAngle: i * arcAngle - Math.PI/2 + 0.02,
        endAngle: (i + 1) * arcAngle - Math.PI/2 - 0.02
      }))
      .attr("fill", (d) => {
        if (parentColor) return parentColor;
        return topicColors[d] || "#6b7280";
      })
      .attr("stroke", "white")
      .attr("stroke-width", 3)
      .style("filter", "drop-shadow(0 2px 4px rgba(0,0,0,0.1))")
      .on("mouseover", function(event, d) {
        d3.select(this)
          .transition()
          .duration(150)
          .attr("transform", function() {
            const i = items.indexOf(d);
            const angle = (i + 0.5) * arcAngle - Math.PI/2;
            return `translate(${5 * Math.cos(angle)}, ${5 * Math.sin(angle)})`;
          });
      })
      .on("mouseout", function() {
        d3.select(this)
          .transition()
          .duration(150)
          .attr("transform", "translate(0, 0)");
      });

    // Labels - curved along the arc
    segments.append("text")
      .attr("transform", (d, i) => {
        const angle = (i + 0.5) * arcAngle - Math.PI/2;
        const r = (innerRadius + radius) / 2 + 10;
        const x = r * Math.cos(angle);
        const y = r * Math.sin(angle);
        let rotation = angle * 180 / Math.PI;
        if (rotation > 90 || rotation < -90) rotation += 180;
        return `translate(${x},${y}) rotate(${rotation})`;
      })
      .attr("text-anchor", "middle")
      .attr("dy", "0.35em")
      .attr("fill", "white")
      .attr("font-weight", "bold")
      .attr("font-size", n > 6 ? "11px" : "13px")
      .style("text-shadow", "0 1px 2px rgba(0,0,0,0.3)")
      .style("pointer-events", "none")
      .text(d => {
        // Shorten long names
        if (d.length > 14) return d.substring(0, 12) + "...";
        return d;
      });

    // Click handler
    segments.on("click", function(event, d) {
      handleClick(d);
    });

    // Fade in
    segments.transition()
      .duration(400)
      .style("opacity", 1);
  }

  function drawCenter(title, subtitle, backAction) {
    centerGroup.selectAll("*").remove();

    // Center circle
    centerGroup.append("circle")
      .attr("r", innerRadius)
      .attr("fill", "#1f2937")
      .style("filter", "drop-shadow(0 4px 6px rgba(0,0,0,0.2))");

    // Title
    const lines = title.split('\n');
    lines.forEach((line, i) => {
      centerGroup.append("text")
        .attr("text-anchor", "middle")
        .attr("y", (i - (lines.length - 1) / 2) * 20)
        .attr("fill", "white")
        .attr("font-size", "16px")
        .attr("font-weight", "bold")
        .text(line);
    });

    // Subtitle
    if (subtitle) {
      centerGroup.append("text")
        .attr("text-anchor", "middle")
        .attr("y", lines.length * 15 + 10)
        .attr("fill", "#9ca3af")
        .attr("font-size", "11px")
        .text(subtitle);
    }

    // Back button
    if (backAction) {
      const backBtn = centerGroup.append("g")
        .attr("class", "back-btn")
        .attr("transform", `translate(0, ${innerRadius - 25})`)
        .style("cursor", "pointer")
        .on("click", backAction);

      backBtn.append("circle")
        .attr("r", 18)
        .attr("fill", "#374151");

      backBtn.append("text")
        .attr("text-anchor", "middle")
        .attr("dy", "0.35em")
        .attr("fill", "#60a5fa")
        .attr("font-size", "18px")
        .text("←");
    }
  }

  function handleClick(item) {
    if (currentLevel === 0) {
      // Clicked main topic → show subtopics
      selectedTopic = item;
      currentLevel = 1;
      const subtopics = Object.keys(topics[item]);
      drawWheel(subtopics, topicColors[item]);
      drawCenter(item, `${subtopics.length} subtopics`, goBack);
    } else if (currentLevel === 1) {
      // Clicked subtopic → show quote
      selectedSubtopic = item;
      currentLevel = 2;
      showQuote();
    }
  }

  function showQuote() {
    const quotes = topics[selectedTopic][selectedSubtopic];
    if (!quotes || quotes.length === 0) return;

    const quote = quotes[Math.floor(Math.random() * quotes.length)];

    // Fade out wheel
    wheelGroup.transition().duration(300).style("opacity", 0.2);

    // Show quote in expanded center
    centerGroup.selectAll("*").remove();

    // Larger background
    centerGroup.append("circle")
      .attr("r", radius - 20)
      .attr("fill", "#1f2937")
      .style("opacity", 0)
      .transition()
      .duration(400)
      .style("opacity", 0.95);

    // Topic/subtopic header
    centerGroup.append("text")
      .attr("text-anchor", "middle")
      .attr("y", -radius + 60)
      .attr("fill", topicColors[selectedTopic])
      .attr("font-size", "14px")
      .attr("font-weight", "bold")
      .style("opacity", 0)
      .transition()
      .delay(200)
      .duration(300)
      .style("opacity", 1)
      .text(`${selectedTopic} › ${selectedSubtopic}`);

    // Quote text (wrapped)
    const quoteText = quote.text.length > 400
      ? quote.text.substring(0, 400) + "..."
      : quote.text;

    const words = quoteText.split(' ');
    const lines = [];
    let currentLine = '';
    const maxWidth = 35; // chars per line

    for (const word of words) {
      if ((currentLine + ' ' + word).length > maxWidth) {
        lines.push(currentLine);
        currentLine = word;
      } else {
        currentLine = currentLine ? currentLine + ' ' + word : word;
      }
    }
    if (currentLine) lines.push(currentLine);

    // Limit lines
    const displayLines = lines.slice(0, 10);
    if (lines.length > 10) displayLines[9] += '...';

    const textGroup = centerGroup.append("g")
      .attr("transform", `translate(0, ${-displayLines.length * 8})`);

    displayLines.forEach((line, i) => {
      textGroup.append("text")
        .attr("text-anchor", "middle")
        .attr("y", i * 22)
        .attr("fill", "white")
        .attr("font-size", "13px")
        .attr("font-style", "italic")
        .style("opacity", 0)
        .transition()
        .delay(300 + i * 30)
        .duration(300)
        .style("opacity", 1)
        .text(line);
    });

    // Source
    if (quote.source) {
      centerGroup.append("text")
        .attr("text-anchor", "middle")
        .attr("y", displayLines.length * 11 + 30)
        .attr("fill", "#9ca3af")
        .attr("font-size", "11px")
        .style("opacity", 0)
        .transition()
        .delay(500)
        .duration(300)
        .style("opacity", 1)
        .text(`— ${quote.source.substring(0, 50)}${quote.source.length > 50 ? '...' : ''}`);
    }

    // Buttons
    const btnY = radius - 50;

    // Back button
    const backBtn = centerGroup.append("g")
      .attr("transform", `translate(-50, ${btnY})`)
      .style("cursor", "pointer")
      .style("opacity", 0)
      .on("click", goBackFromQuote);

    backBtn.append("rect")
      .attr("x", -35)
      .attr("y", -15)
      .attr("width", 70)
      .attr("height", 30)
      .attr("rx", 5)
      .attr("fill", "#374151");

    backBtn.append("text")
      .attr("text-anchor", "middle")
      .attr("dy", "0.35em")
      .attr("fill", "#60a5fa")
      .attr("font-size", "12px")
      .text("← Back");

    backBtn.transition().delay(600).duration(300).style("opacity", 1);

    // Next button
    const nextBtn = centerGroup.append("g")
      .attr("transform", `translate(50, ${btnY})`)
      .style("cursor", "pointer")
      .style("opacity", 0)
      .on("click", showQuote);

    nextBtn.append("rect")
      .attr("x", -35)
      .attr("y", -15)
      .attr("width", 70)
      .attr("height", 30)
      .attr("rx", 5)
      .attr("fill", topicColors[selectedTopic]);

    nextBtn.append("text")
      .attr("text-anchor", "middle")
      .attr("dy", "0.35em")
      .attr("fill", "white")
      .attr("font-size", "12px")
      .text("Next →");

    nextBtn.transition().delay(600).duration(300).style("opacity", 1);

    // Quote count
    centerGroup.append("text")
      .attr("text-anchor", "middle")
      .attr("y", btnY + 35)
      .attr("fill", "#6b7280")
      .attr("font-size", "10px")
      .style("opacity", 0)
      .transition()
      .delay(700)
      .duration(300)
      .style("opacity", 1)
      .text(`${quotes.length} quotes in this category`);
  }

  function goBack() {
    if (currentLevel === 1) {
      currentLevel = 0;
      selectedTopic = null;
      drawWheel(Object.keys(topics), null);
      drawCenter("Math\nQuotes", "Select a topic", null);
    }
  }

  function goBackFromQuote() {
    currentLevel = 1;
    wheelGroup.transition().duration(300).style("opacity", 1);
    const subtopics = Object.keys(topics[selectedTopic]);
    drawWheel(subtopics, topicColors[selectedTopic]);
    drawCenter(selectedTopic, `${subtopics.length} subtopics`, goBack);
  }

  // Initialize when DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
