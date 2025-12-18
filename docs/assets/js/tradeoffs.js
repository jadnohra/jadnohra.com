// Tradeoff Cards Visualization
(function() {
  let data = null;
  let popover = null;

  async function init() {
    const container = document.getElementById('tradeoff-cards');
    if (!container) return;

    try {
      let resp = await fetch('./assets/data/tradeoffs.json');
      if (!resp.ok) resp = await fetch('/assets/data/tradeoffs.json');
      data = await resp.json();
    } catch (e) {
      container.innerHTML = '<p style="color:#ef4444;">Failed to load tradeoffs data.</p>';
      return;
    }

    render(container);
  }

  function positionPopover(mouseX, mouseY) {
    const offset = 16;
    const popWidth = 480;
    const margin = 12;

    let left = mouseX + offset;
    let top = mouseY;

    if (left + popWidth > window.innerWidth - margin) {
      left = mouseX - popWidth - offset;
    }
    if (left < margin) left = margin;

    const popHeight = popover.offsetHeight || 200;
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
    popover.className = 'tradeoff-popover';
    document.body.appendChild(popover);

    // Render cards in a grid
    const grid = document.createElement('div');
    grid.className = 'tradeoff-grid';

    data.cards.forEach(card => {
      const item = document.createElement('div');
      item.className = 'tradeoff-item';

      const name = document.createElement('div');
      name.className = 'tradeoff-name';
      name.textContent = card.name;
      item.appendChild(name);

      // Count of primitives
      const count = document.createElement('span');
      count.className = 'tradeoff-count';
      count.textContent = card.tradeoffs.length;
      item.appendChild(count);

      // Hover for popover
      item.addEventListener('mouseenter', (e) => {
        let html = '<div class="tradeoff-popover-title">' + card.name + '</div>';
        card.tradeoffs.forEach(t => {
          html += '<div class="tradeoff-popover-item">';
          html += '<b>' + t.primitive + ':</b> ' + t.explanation;
          html += '</div>';
        });
        popover.innerHTML = html;
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

      grid.appendChild(item);
    });

    container.appendChild(grid);
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
