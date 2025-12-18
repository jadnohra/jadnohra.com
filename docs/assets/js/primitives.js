// System Primitives Visualization
(function() {
  let data = null;

  async function init() {
    const container = document.getElementById('system-primitives');
    if (!container) return;

    try {
      let resp = await fetch('./assets/data/primitives.json');
      if (!resp.ok) resp = await fetch('/assets/data/primitives.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:#ef4444;">Failed to load primitives data.</p>';
      return;
    }

    render(container);
  }

  function render(container) {
    container.innerHTML = '';

    // Intro text
    const intro = document.createElement('div');
    intro.className = 'primitives-intro';
    intro.innerHTML = 'These constraints are rooted in physics and fundamental CS. They apply at every layer. Abstractions can defer them, transform them, or trade one for another — but never eliminate them.';
    container.appendChild(intro);

    // Render each category
    data.categories.forEach(category => {
      const card = document.createElement('div');
      card.className = 'category-card';
      card.style.setProperty('--category-color', category.color);

      // Category header
      const header = document.createElement('div');
      header.className = 'category-header';
      header.textContent = category.name;
      card.appendChild(header);

      // Primitives
      category.primitives.forEach(primitive => {
        const item = document.createElement('div');
        item.className = 'primitive-item';

        // Name (clickable if has explanation)
        const hasExplanation = primitive.explanation && primitive.explanation.trim() !== '';
        const name = document.createElement('div');
        name.className = 'primitive-name';

        if (hasExplanation) {
          name.innerHTML = `<span class="expand-icon">▶</span>${escapeHtml(primitive.name)}`;
          name.addEventListener('click', () => {
            item.classList.toggle('expanded');
          });
        } else {
          name.innerHTML = escapeHtml(primitive.name);
          name.style.cursor = 'default';
        }
        item.appendChild(name);

        // Implications (always visible)
        const implications = document.createElement('div');
        implications.className = 'primitive-implications';
        primitive.implications.forEach(imp => {
          const impEl = document.createElement('div');
          impEl.className = 'primitive-implication';
          impEl.textContent = imp;
          implications.appendChild(impEl);
        });
        item.appendChild(implications);

        // Explanation (hidden by default)
        if (hasExplanation) {
          const explanation = document.createElement('div');
          explanation.className = 'primitive-explanation';
          explanation.textContent = primitive.explanation;
          item.appendChild(explanation);
        }

        card.appendChild(item);
      });

      container.appendChild(card);
    });
  }

  function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
