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

    // Create shared popover element
    popover = document.createElement('div');
    popover.className = 'primitive-popover';
    popover.innerHTML = '<div class="primitive-popover-title"></div><div class="primitive-popover-text"></div>';
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

        // Hover events for popover
        if (hasExplanation) {
          item.classList.add('has-explanation');

          item.addEventListener('mouseenter', (e) => {
            showPopover(primitive.name, primitive.explanation, category.color, item);
          });

          item.addEventListener('mouseleave', () => {
            hidePopover();
          });
        }

        card.appendChild(item);
      });

      container.appendChild(card);
    });
  }

  function showPopover(title, text, color, targetEl) {
    const titleEl = popover.querySelector('.primitive-popover-title');
    const textEl = popover.querySelector('.primitive-popover-text');

    titleEl.textContent = title;
    textEl.textContent = text;
    popover.style.setProperty('--popover-color', color);

    // Position popover
    const rect = targetEl.getBoundingClientRect();
    const popoverWidth = 420;
    const margin = 12;

    // Horizontal: try to align left with item, but keep in viewport
    let left = rect.left;
    if (left + popoverWidth > window.innerWidth - margin) {
      left = window.innerWidth - popoverWidth - margin;
    }
    if (left < margin) left = margin;

    // Vertical: prefer above, but show below if not enough space
    popover.style.visibility = 'hidden';
    popover.style.display = 'block';
    const popoverHeight = popover.offsetHeight;
    popover.style.display = '';
    popover.style.visibility = '';

    let top;
    if (rect.top - popoverHeight - margin > 0) {
      // Show above
      top = rect.top - popoverHeight - margin;
    } else {
      // Show below
      top = rect.bottom + margin;
    }

    popover.style.left = left + 'px';
    popover.style.top = top + 'px';
    popover.classList.add('visible');
  }

  function hidePopover() {
    popover.classList.remove('visible');
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
