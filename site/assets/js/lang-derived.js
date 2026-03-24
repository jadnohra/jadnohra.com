// Language-Level Derived Data Hover Visualization
(function() {
  let data = null;
  let popover = null;

  async function init() {
    // Check if any hover elements exist on the page
    if (!document.querySelector('[data-hover]')) return;

    try {
      let resp = await fetch('./assets/data/lang-derived.json');
      if (!resp.ok) resp = await fetch('/assets/data/lang-derived.json');
      data = await resp.json();
    } catch (e) {
      console.error('Failed to load lang-derived data:', e);
      return;
    }

    setupHovers();
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

    const popHeight = popover.offsetHeight || 150;
    if (top + popHeight > window.innerHeight - margin) {
      top = window.innerHeight - popHeight - margin;
    }
    if (top < margin) top = margin;

    popover.style.left = left + 'px';
    popover.style.top = top + 'px';
  }

  function setupHovers() {
    // Create popover
    popover = document.createElement('div');
    popover.className = 'lang-derived-popover';
    document.body.appendChild(popover);

    // Find all elements with data-hover attribute
    const hoverElements = document.querySelectorAll('[data-hover]');

    hoverElements.forEach(el => {
      const hoverId = el.getAttribute('data-hover');
      const hoverData = data[hoverId];

      if (!hoverData) {
        console.warn('No hover data for:', hoverId);
        return;
      }

      // Add hoverable styling
      el.classList.add('hoverable');

      el.addEventListener('mouseenter', (e) => {
        let html = '<div class="lang-derived-popover-title">' + hoverData.title + '</div>';
        if (hoverData.section) {
          html += '<div class="lang-derived-popover-section">' + hoverData.section + '</div>';
        }
        html += '<div class="lang-derived-popover-content">' + hoverData.content + '</div>';
        popover.innerHTML = html;
        positionPopover(e.clientX, e.clientY);
        popover.classList.add('visible');
      });

      el.addEventListener('mousemove', (e) => {
        positionPopover(e.clientX, e.clientY);
      });

      el.addEventListener('mouseleave', () => {
        popover.classList.remove('visible');
      });
    });
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
