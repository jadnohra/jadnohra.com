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

  const icons = {
    "Jad's System": "◈",
    "Foundations": "⊢",
    "Analysis": "∞",
    "Algebra": "λ",
    "Geometry": "△",
    "Physics": "⚛",
    "Philosophy": "φ"
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
          cursor: pointer;
          transition: transform 0.2s ease, box-shadow 0.2s ease;
          box-shadow: 0 4px 20px rgba(0, 0, 0, 0.3);
          position: relative;
          overflow: hidden;
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
          <div id="transition-icon" style="
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 48px;
            opacity: 0;
            pointer-events: none;
            transition: all 0.3s ease;
          "></div>
        </div>
        <div style="
          margin-top: 1rem;
          color: #475569;
          font-size: 11px;
          font-family: -apple-system, BlinkMacSystemFont, sans-serif;
        ">${data.quotes.length} quotes · click anywhere</div>
        </div>
      </div>
    `;

    const card = document.getElementById('quote-display');

    card.addEventListener('mouseover', () => {
      card.style.transform = 'translateY(-4px)';
      card.style.boxShadow = '0 12px 40px rgba(59, 130, 246, 0.3), 0 0 60px rgba(59, 130, 246, 0.1)';
    });
    card.addEventListener('mouseout', () => {
      card.style.transform = 'translateY(0)';
      card.style.boxShadow = '0 4px 20px rgba(0, 0, 0, 0.3)';
    });
    card.addEventListener('click', () => {
      card.style.transform = 'scale(0.98)';
      setTimeout(() => {
        card.style.transform = 'translateY(0)';
        drawQuote(true);
      }, 100);
    });

    // Draw initial quote
    drawQuote();
  }

  function drawQuote(animate = false) {
    const q = data.quotes[Math.floor(Math.random() * data.quotes.length)];

    const textEl = document.getElementById('quote-text');
    const sourceEl = document.getElementById('quote-source');
    const topicsEl = document.getElementById('quote-topics');
    const iconEl = document.getElementById('transition-icon');

    // Get main topic for icon
    const mainTopic = q.topics.length > 0 ? q.topics[0].split('/')[0] : 'Philosophy';
    const icon = icons[mainTopic] || '✨';
    const color = colors[mainTopic] || '#64748b';

    // Fade out content
    textEl.style.opacity = '0';
    sourceEl.style.opacity = '0';
    topicsEl.style.opacity = '0';

    // Show transition icon if animating
    if (animate) {
      iconEl.innerHTML = icon;
      iconEl.style.color = color;
      iconEl.style.opacity = '1';
      iconEl.style.transform = 'translate(-50%, -50%) scale(1.2)';
      setTimeout(() => {
        iconEl.style.opacity = '0';
        iconEl.style.transform = 'translate(-50%, -50%) scale(0.8)';
      }, 250);
    }

    setTimeout(() => {
      // Quote text - full, no truncation
      textEl.innerHTML = `"${q.text}"`;

      // Source
      if (q.source) {
        sourceEl.innerHTML = `— ${q.source}`;
      } else {
        sourceEl.innerHTML = '';
      }

      // Build topic pills with stagger animation
      topicsEl.innerHTML = '';
      const allTopics = [...q.topics];
      if (q.original_tags && q.original_tags.length > 0) {
        for (const tag of q.original_tags) {
          allTopics.push(`Jad's System/${tag}`);
        }
      }

      allTopics.forEach((topic, i) => {
        const [main, sub] = topic.split('/');
        const pillColor = colors[main] || '#64748b';
        const pillIcon = icons[main] || '';
        const isJad = main === "Jad's System";
        const label = isJad ? `${pillIcon} ${sub}` : `${pillIcon} ${main}${sub ? ' › ' + sub : ''}`;

        const pill = document.createElement('span');
        pill.style.cssText = `
          display: inline-block;
          padding: 0.25rem 0.75rem;
          background: ${pillColor}22;
          color: ${pillColor};
          border-radius: 20px;
          font-size: 11px;
          font-family: -apple-system, BlinkMacSystemFont, sans-serif;
          font-weight: 500;
          opacity: 0;
          transform: translateY(10px);
          transition: all 0.3s ease;
        `;
        pill.textContent = label;
        topicsEl.appendChild(pill);

        // Stagger animation
        setTimeout(() => {
          pill.style.opacity = '1';
          pill.style.transform = 'translateY(0)';
        }, 50 + i * 60);
      });

      // Fade in text
      textEl.style.transition = 'opacity 0.4s, transform 0.4s';
      sourceEl.style.transition = 'opacity 0.4s, transform 0.4s';
      textEl.style.opacity = '1';
      sourceEl.style.opacity = '1';
    }, animate ? 300 : 200);
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
