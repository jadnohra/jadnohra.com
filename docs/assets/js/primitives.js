// System Primitives Visualization
(function() {
  let data = null;
  let popover = null;

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

    // Create popover
    popover = document.createElement('div');
    popover.className = 'primitive-popover';
    document.body.appendChild(popover);

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

        const hasExplanation = primitive.explanation && primitive.explanation.trim() !== '';

        // Name
        const name = document.createElement('div');
        name.className = 'primitive-name';
        name.textContent = primitive.name;
        if (hasExplanation) {
          item.classList.add('has-explanation');
        }
        item.appendChild(name);

        // Implications
        const implications = document.createElement('div');
        implications.className = 'primitive-implications';
        primitive.implications.forEach(imp => {
          const impEl = document.createElement('div');
          impEl.className = 'primitive-implication';
          impEl.textContent = imp;
          implications.appendChild(impEl);
        });
        item.appendChild(implications);

        // Hover for popover
        if (hasExplanation) {
          item.addEventListener('mouseenter', () => {
            popover.textContent = primitive.explanation;
            popover.style.setProperty('--popover-color', category.color);

            const rect = item.getBoundingClientRect();
            const popWidth = Math.min(500, window.innerWidth - 24);

            // Position above
            let left = rect.left;
            if (left + popWidth > window.innerWidth - 12) {
              left = window.innerWidth - popWidth - 12;
            }
            if (left < 12) left = 12;

            popover.style.left = left + 'px';
            popover.style.width = popWidth + 'px';
            popover.style.top = (rect.top - 8) + 'px';
            popover.style.transform = 'translateY(-100%)';
            popover.classList.add('visible');
          });

          item.addEventListener('mouseleave', () => {
            popover.classList.remove('visible');
          });
        }

        card.appendChild(item);
      });

      container.appendChild(card);
    });
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
