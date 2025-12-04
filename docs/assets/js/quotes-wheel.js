// Math Quotes - Fortune Cookie
(function() {
  const colors = {
    "Special Tags": "#14b8a6",
    "Foundations": "#ef4444",
    "Analysis": "#3b82f6",
    "Algebra": "#22c55e",
    "Geometry": "#a855f7",
    "Physics": "#f59e0b",
    "Philosophy": "#ec4899"
  };

  const icons = {
    "Special Tags": "◈",
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
      if (!resp.ok) resp = await fetch('/assets/data/quotes.json');
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
          padding: 2rem 2rem 2.5rem 2rem;
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
          <div id="quote-gene" style="
            display: flex;
            justify-content: center;
            gap: 0.5rem;
            margin-bottom: 1.5rem;
            min-height: 28px;
            font-size: 20px;
          "></div>
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
        <div style="
          margin-top: 1rem;
          color: #475569;
          font-size: 11px;
          font-family: -apple-system, BlinkMacSystemFont, sans-serif;
        ">${data.quotes.length} quotes · click anywhere</div>
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

  const allIcons = Object.values(icons);

  function drawQuote(animate = false) {
    const q = data.quotes[Math.floor(Math.random() * data.quotes.length)];

    const geneEl = document.getElementById('quote-gene');
    const textEl = document.getElementById('quote-text');
    const sourceEl = document.getElementById('quote-source');
    const topicsEl = document.getElementById('quote-topics');

    // Get unique main topics for this quote's gene
    const quoteTopics = [...new Set(q.topics.map(t => t.split('/')[0]))];
    if (q.original_tags && q.original_tags.length > 0) {
      quoteTopics.push("Special Tags");
    }
    const finalGene = [...new Set(quoteTopics)].map(t => ({
      icon: icons[t] || '◇',
      color: colors[t] || '#64748b'
    }));

    // Fade out content
    textEl.style.opacity = '0';
    sourceEl.style.opacity = '0';
    topicsEl.style.opacity = '0';

    // Animate gene icons (computing effect)
    if (animate) {
      let shuffleCount = 0;
      const shuffleInterval = setInterval(() => {
        geneEl.innerHTML = '';
        const randomCount = Math.floor(Math.random() * 3) + 2;
        for (let i = 0; i < randomCount; i++) {
          const randIcon = allIcons[Math.floor(Math.random() * allIcons.length)];
          const span = document.createElement('span');
          span.style.cssText = `opacity: 0.5; transition: all 0.1s;`;
          span.textContent = randIcon;
          geneEl.appendChild(span);
        }
        shuffleCount++;
        if (shuffleCount >= 8) {
          clearInterval(shuffleInterval);
          // Settle on final gene
          showFinalGene();
        }
      }, 80);
    } else {
      showFinalGene();
    }

    function showFinalGene() {
      geneEl.innerHTML = '';
      finalGene.forEach((g, i) => {
        const span = document.createElement('span');
        span.style.cssText = `
          color: ${g.color};
          opacity: 0;
          transform: scale(0.5);
          transition: all 0.3s ease;
          display: inline-block;
        `;
        span.textContent = g.icon;
        geneEl.appendChild(span);
        setTimeout(() => {
          span.style.opacity = '1';
          span.style.transform = 'scale(1)';
        }, 50 + i * 100);
      });
    }

    setTimeout(() => {
      // Quote text
      textEl.innerHTML = `"${q.text}"`;

      // Source
      sourceEl.innerHTML = q.source ? `— ${q.source}` : '';

      // Build topic pills (labels at bottom)
      topicsEl.innerHTML = '';
      const allTopicsList = [...q.topics];
      if (q.original_tags && q.original_tags.length > 0) {
        for (const tag of q.original_tags) {
          allTopicsList.push(`Special Tags/${tag}`);
        }
      }

      allTopicsList.forEach((topic, i) => {
        const [main, sub] = topic.split('/');
        const pillColor = colors[main] || '#64748b';
        const isSpecial = main === "Special Tags";
        const label = isSpecial ? sub : `${main}${sub ? ' › ' + sub : ''}`;

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

        setTimeout(() => {
          pill.style.opacity = '1';
          pill.style.transform = 'translateY(0)';
        }, 50 + i * 60);
      });

      // Fade in text and topics container
      textEl.style.transition = 'opacity 0.4s';
      sourceEl.style.transition = 'opacity 0.4s';
      topicsEl.style.transition = 'opacity 0.2s';
      textEl.style.opacity = '1';
      sourceEl.style.opacity = '1';
      topicsEl.style.opacity = '1';
    }, animate ? 700 : 200);
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
