// Circular Quote Wheel - Topic → Subtopic → Random Quote
(function() {
  const width = 500;
  const height = 500;
  const outerRadius = 220;
  const innerRadius = 120;
  const centerRadius = 80;

  // Color scheme for main topics
  const topicColors = {
    "Jad's System": "#14b8a6",  // teal - personal tag system
    "Foundations": "#dc2626",
    "Analysis": "#2563eb",
    "Algebra": "#059669",
    "Geometry": "#7c3aed",
    "Physics": "#f59e0b",
    "Philosophy": "#ec4899"
  };

  let quotesData = null;
  let selectedTopic = null;
  let selectedSubtopic = null;

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
    const topics = buildTopicStructure();

    // Create SVG
    const svg = d3.select(container)
      .append("svg")
      .attr("viewBox", [-width/2, -height/2, width, height])
      .attr("width", width)
      .attr("height", height)
      .style("max-width", "100%")
      .style("height", "auto")
      .style("font", "12px sans-serif");

    // Create groups
    const outerWheel = svg.append("g").attr("class", "outer-wheel");
    const innerWheel = svg.append("g").attr("class", "inner-wheel").style("opacity", 0);
    const centerCircle = svg.append("g").attr("class", "center");

    // Draw outer wheel (main topics)
    drawOuterWheel(outerWheel, topics);

    // Draw center
    drawCenter(centerCircle);

    // Quote display area
    const quoteDisplay = d3.select(container)
      .append("div")
      .attr("class", "quote-display")
      .style("margin-top", "20px")
      .style("padding", "20px")
      .style("min-height", "150px")
      .style("background", "rgba(0,0,0,0.03)")
      .style("border-radius", "10px")
      .style("text-align", "center");

    quoteDisplay.append("p")
      .attr("class", "quote-text")
      .style("font-style", "italic")
      .style("font-size", "1.1em")
      .style("line-height", "1.6")
      .text("Select a topic from the wheel...");

    quoteDisplay.append("p")
      .attr("class", "quote-source")
      .style("margin-top", "10px")
      .style("color", "#666")
      .style("font-size", "0.9em");

    quoteDisplay.append("button")
      .attr("class", "next-quote-btn")
      .style("margin-top", "15px")
      .style("padding", "8px 20px")
      .style("border", "none")
      .style("border-radius", "5px")
      .style("background", "#2563eb")
      .style("color", "white")
      .style("cursor", "pointer")
      .style("display", "none")
      .text("Next Quote")
      .on("click", showRandomQuote);

    // Store references
    container._svg = svg;
    container._outerWheel = outerWheel;
    container._innerWheel = innerWheel;
    container._topics = topics;
    container._quoteDisplay = quoteDisplay;
  }

  function buildTopicStructure() {
    const topics = {};

    for (const quote of quotesData.quotes) {
      for (const topicPath of quote.topics) {
        const [main, sub] = topicPath.split('/');
        if (!topics[main]) topics[main] = {};
        if (!topics[main][sub]) topics[main][sub] = [];
        topics[main][sub].push(quote);
      }
    }

    return topics;
  }

  function drawOuterWheel(g, topics) {
    const mainTopics = Object.keys(topics);
    const arcAngle = (2 * Math.PI) / mainTopics.length;

    const arc = d3.arc()
      .innerRadius(innerRadius)
      .outerRadius(outerRadius);

    const segments = g.selectAll("g.segment")
      .data(mainTopics)
      .join("g")
      .attr("class", "segment")
      .style("cursor", "pointer");

    segments.append("path")
      .attr("d", (d, i) => arc({
        startAngle: i * arcAngle - Math.PI/2,
        endAngle: (i + 1) * arcAngle - Math.PI/2
      }))
      .attr("fill", d => topicColors[d] || "#999")
      .attr("stroke", "white")
      .attr("stroke-width", 2)
      .style("opacity", 0.8)
      .on("mouseover", function() {
        d3.select(this).style("opacity", 1);
      })
      .on("mouseout", function() {
        d3.select(this).style("opacity", 0.8);
      });

    // Labels
    segments.append("text")
      .attr("transform", (d, i) => {
        const angle = (i + 0.5) * arcAngle - Math.PI/2;
        const r = (innerRadius + outerRadius) / 2;
        const x = r * Math.cos(angle);
        const y = r * Math.sin(angle);
        const rotation = (angle * 180 / Math.PI) + (angle > Math.PI/2 && angle < 3*Math.PI/2 ? 180 : 0);
        return `translate(${x},${y}) rotate(${rotation})`;
      })
      .attr("text-anchor", "middle")
      .attr("dy", "0.35em")
      .attr("fill", "white")
      .attr("font-weight", "bold")
      .attr("font-size", "11px")
      .text(d => d)
      .style("pointer-events", "none");

    segments.on("click", function(event, d) {
      selectTopic(d, topics);
    });
  }

  function drawCenter(g) {
    g.append("circle")
      .attr("r", centerRadius)
      .attr("fill", "#1f2937")
      .attr("stroke", "#374151")
      .attr("stroke-width", 2);

    g.append("text")
      .attr("text-anchor", "middle")
      .attr("dy", "-0.5em")
      .attr("fill", "white")
      .attr("font-size", "14px")
      .attr("font-weight", "bold")
      .text("Math");

    g.append("text")
      .attr("text-anchor", "middle")
      .attr("dy", "1em")
      .attr("fill", "#9ca3af")
      .attr("font-size", "11px")
      .text("Quotes");

    // Back button (hidden initially)
    g.append("text")
      .attr("class", "back-btn")
      .attr("text-anchor", "middle")
      .attr("dy", "2.5em")
      .attr("fill", "#60a5fa")
      .attr("font-size", "10px")
      .style("cursor", "pointer")
      .style("display", "none")
      .text("← Back")
      .on("click", goBack);
  }

  function selectTopic(topic, topics) {
    selectedTopic = topic;
    selectedSubtopic = null;

    const container = document.getElementById('quotes-wheel');
    const innerWheel = container._svg.select(".inner-wheel");
    const outerWheel = container._svg.select(".outer-wheel");
    const center = container._svg.select(".center");

    // Fade outer wheel
    outerWheel.transition().duration(300).style("opacity", 0.3);

    // Show back button
    center.select(".back-btn").style("display", "block");

    // Draw subtopic wheel
    const subtopics = Object.keys(topics[topic]);
    const arcAngle = (2 * Math.PI) / subtopics.length;

    innerWheel.selectAll("*").remove();

    const arc = d3.arc()
      .innerRadius(centerRadius + 10)
      .outerRadius(innerRadius - 10);

    const segments = innerWheel.selectAll("g.sub-segment")
      .data(subtopics)
      .join("g")
      .attr("class", "sub-segment")
      .style("cursor", "pointer");

    segments.append("path")
      .attr("d", (d, i) => arc({
        startAngle: i * arcAngle - Math.PI/2,
        endAngle: (i + 1) * arcAngle - Math.PI/2
      }))
      .attr("fill", topicColors[topic] || "#999")
      .attr("stroke", "white")
      .attr("stroke-width", 1)
      .style("opacity", 0.6)
      .on("mouseover", function() {
        d3.select(this).style("opacity", 0.9);
      })
      .on("mouseout", function() {
        d3.select(this).style("opacity", 0.6);
      });

    segments.append("text")
      .attr("transform", (d, i) => {
        const angle = (i + 0.5) * arcAngle - Math.PI/2;
        const r = (centerRadius + innerRadius) / 2;
        const x = r * Math.cos(angle);
        const y = r * Math.sin(angle);
        return `translate(${x},${y})`;
      })
      .attr("text-anchor", "middle")
      .attr("dy", "0.35em")
      .attr("fill", "white")
      .attr("font-size", "9px")
      .text(d => d.length > 12 ? d.substring(0, 10) + "..." : d)
      .style("pointer-events", "none");

    segments.on("click", function(event, d) {
      selectSubtopic(topic, d, topics);
    });

    innerWheel.transition().duration(300).style("opacity", 1);
  }

  function selectSubtopic(topic, subtopic, topics) {
    selectedSubtopic = subtopic;
    const container = document.getElementById('quotes-wheel');

    // Show next quote button
    container._quoteDisplay.select(".next-quote-btn").style("display", "inline-block");

    showRandomQuote();
  }

  function showRandomQuote() {
    if (!selectedTopic || !selectedSubtopic) return;

    const container = document.getElementById('quotes-wheel');
    const topics = container._topics;
    const quotes = topics[selectedTopic][selectedSubtopic];

    if (!quotes || quotes.length === 0) return;

    const quote = quotes[Math.floor(Math.random() * quotes.length)];

    container._quoteDisplay.select(".quote-text")
      .style("opacity", 0)
      .transition()
      .duration(300)
      .style("opacity", 1)
      .text(`"${quote.text}"`);

    container._quoteDisplay.select(".quote-source")
      .text(quote.source ? `— ${quote.source}` : "");
  }

  function goBack() {
    selectedTopic = null;
    selectedSubtopic = null;

    const container = document.getElementById('quotes-wheel');
    const innerWheel = container._svg.select(".inner-wheel");
    const outerWheel = container._svg.select(".outer-wheel");
    const center = container._svg.select(".center");

    innerWheel.transition().duration(300).style("opacity", 0);
    outerWheel.transition().duration(300).style("opacity", 1);
    center.select(".back-btn").style("display", "none");

    container._quoteDisplay.select(".quote-text").text("Select a topic from the wheel...");
    container._quoteDisplay.select(".quote-source").text("");
    container._quoteDisplay.select(".next-quote-btn").style("display", "none");
  }

  // Initialize when DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
