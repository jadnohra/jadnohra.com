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

  function positionPopover(mouseX, mouseY) {
    const offset = 16;
    const popWidth = 400;
    const margin = 12;

    // Position to the right of mouse
    let left = mouseX + offset;
    let top = mouseY;

    // If overflows right, show on left of mouse
    if (left + popWidth > window.innerWidth - margin) {
      left = mouseX - popWidth - offset;
    }
    if (left < margin) left = margin;

    // Keep in vertical bounds
    const popHeight = popover.offsetHeight || 100;
    if (top + popHeight > window.innerHeight - margin) {
      top = window.innerHeight - popHeight - margin;
    }
    if (top < margin) top = margin;

    popover.style.left = left + 'px';
    popover.style.top = top + 'px';
  }

  function render(container) {
    container.innerHTML = '';

    // Create popover
    popover = document.createElement('div');
    popover.className = 'primitive-popover';
    document.body.appendChild(popover);

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
          item.addEventListener('mouseenter', (e) => {
            popover.innerHTML = primitive.explanation;
            popover.style.setProperty('--popover-color', category.color);
            popover.style.transform = '';
            positionPopover(e.clientX, e.clientY);
            popover.classList.add('visible');
          });

          item.addEventListener('mousemove', (e) => {
            positionPopover(e.clientX, e.clientY);
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
