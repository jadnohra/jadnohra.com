// Math Quotes - Fortune Cookie
(function() {
  const colors = {
    "Jad's System": "#14b8a6",
    "Foundations": "#ef4444",
    "Analysis": "#3b82f6",
    "Algebra": "#22c55e",
    "Geometry": "#a855f7",
    "Physics": "#f59e0b",
    "Philosophy": "#ec4899"
  };

  let data = null;
  let container = null;

  async function init() {
    container = document.getElementById('quotes-wheel');
    if (!container) return;

    try {
      let resp = await fetch('./assets/data/quotes.json');
      if (!resp.ok) resp = await fetch('/jadnohra.com/assets/data/quotes.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:red;">Failed to load quotes.</p>';
      return;
    }

    // Build UI
    container.innerHTML = `
      <div id="fortune-cookie" style="
        max-width: 700px;
        margin: 0 auto;
        text-align: center;
      ">
        <div id="quote-display" style="
          background: linear-gradient(135deg, #1e293b 0%, #0f172a 100%);
          border-radius: 16px;
          padding: 2.5rem 2rem;
          min-height: 200px;
          display: flex;
          flex-direction: column;
          justify-content: center;
          margin-bottom: 1.5rem;
        ">
          <div id="quote-text" style="
            color: #e2e8f0;
            font-size: 17px;
            line-height: 1.8;
            font-family: Georgia, 'Times New Roman', serif;
            font-style: italic;
            margin-bottom: 1.5rem;
          "></div>
          <div id="quote-source" style="
            color: #94a3b8;
            font-size: 13px;
            font-family: -apple-system, BlinkMacSystemFont, sans-serif;
          "></div>
          <div id="quote-topics" style="
            margin-top: 1.5rem;
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            gap: 0.5rem;
          "></div>
        </div>
        <button id="draw-btn" style="
          background: linear-gradient(135deg, #3b82f6 0%, #2563eb 100%);
          color: white;
          border: none;
          padding: 1rem 2.5rem;
          font-size: 16px;
          font-weight: 600;
          border-radius: 12px;
          cursor: pointer;
          transition: transform 0.15s, box-shadow 0.15s;
          box-shadow: 0 4px 14px rgba(59, 130, 246, 0.4);
        ">I Love Science</button>
        <div style="
          margin-top: 1rem;
          color: #64748b;
          font-size: 12px;
        ">${data.quotes.length} quotes from 7 years of mathematical study</div>
      </div>
    `;

    const btn = document.getElementById('draw-btn');
    btn.addEventListener('mouseover', () => {
      btn.style.transform = 'translateY(-2px)';
      btn.style.boxShadow = '0 6px 20px rgba(59, 130, 246, 0.5)';
    });
    btn.addEventListener('mouseout', () => {
      btn.style.transform = 'translateY(0)';
      btn.style.boxShadow = '0 4px 14px rgba(59, 130, 246, 0.4)';
    });
    btn.addEventListener('click', drawQuote);

    // Draw initial quote
    drawQuote();
  }

  function drawQuote() {
    const q = data.quotes[Math.floor(Math.random() * data.quotes.length)];

    const textEl = document.getElementById('quote-text');
    const sourceEl = document.getElementById('quote-source');
    const topicsEl = document.getElementById('quote-topics');

    // Fade out
    textEl.style.opacity = '0';
    sourceEl.style.opacity = '0';
    topicsEl.style.opacity = '0';

    setTimeout(() => {
      // Quote text - full, no truncation
      textEl.innerHTML = `"${q.text}"`;

      // Source
      if (q.source) {
        sourceEl.innerHTML = `— ${q.source}`;
      } else {
        sourceEl.innerHTML = '';
      }

      // Topics as colored pills
      let topicsHtml = '';
      for (const topic of q.topics) {
        const [main, sub] = topic.split('/');
        const color = colors[main] || '#64748b';
        topicsHtml += `<span style="
          display: inline-block;
          padding: 0.25rem 0.75rem;
          background: ${color}22;
          color: ${color};
          border-radius: 20px;
          font-size: 11px;
          font-family: -apple-system, BlinkMacSystemFont, sans-serif;
          font-weight: 500;
        ">${main}${sub ? ' › ' + sub : ''}</span>`;
      }

      // Add Jad's tags if any
      if (q.original_tags && q.original_tags.length > 0) {
        for (const tag of q.original_tags) {
          const color = colors["Jad's System"];
          topicsHtml += `<span style="
            display: inline-block;
            padding: 0.25rem 0.75rem;
            background: ${color}22;
            color: ${color};
            border-radius: 20px;
            font-size: 11px;
            font-family: -apple-system, BlinkMacSystemFont, sans-serif;
            font-weight: 500;
          ">Jad: ${tag}</span>`;
        }
      }

      topicsEl.innerHTML = topicsHtml;

      // Fade in
      textEl.style.transition = 'opacity 0.4s';
      sourceEl.style.transition = 'opacity 0.4s';
      topicsEl.style.transition = 'opacity 0.4s';
      textEl.style.opacity = '1';
      sourceEl.style.opacity = '1';
      topicsEl.style.opacity = '1';
    }, 200);
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
