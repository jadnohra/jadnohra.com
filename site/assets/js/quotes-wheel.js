// Math Quotes - Fortune Cookie
(function() {
  const colors = {
    "Special Tags": "#111",
    "Foundations": "#111",
    "Analysis": "#111",
    "Algebra": "#111",
    "Geometry": "#111",
    "Physics": "#111",
    "Philosophy": "#111"
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
      let resp = await fetch('/assets/data/quotes.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:#555;">Failed to load quotes.</p>';
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
          background: #fff;
          border: 1px solid #ddd;
          padding: 2rem 2rem 2.5rem 2rem;
          min-height: 200px;
          display: flex;
          flex-direction: column;
          justify-content: center;
          cursor: pointer;
          transition: border-color 0.2s ease;
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
            color: #111;
            font-size: 14px;
            line-height: 1.8;
            font-family: inherit;
            font-style: italic;
            margin-bottom: 1.5rem;
          "></div>
          <div id="quote-source" style="
            color: #555;
            font-size: 12px;
            font-family: inherit;
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
          color: #999;
          font-size: 11px;
          font-family: inherit;
        ">${data.quotes.length} quotes · click anywhere</div>
      </div>
    `;

    const card = document.getElementById('quote-display');

    card.addEventListener('mouseover', () => {
      card.style.borderColor = '#111';
    });
    card.addEventListener('mouseout', () => {
      card.style.borderColor = '#ddd';
    });
    card.addEventListener('click', () => {
      drawQuote(true);
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
          padding: 2px 8px;
          background: none;
          color: #555;
          border: 1px solid #ddd;
          font-size: 10px;
          font-family: inherit;
          font-weight: 400;
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
