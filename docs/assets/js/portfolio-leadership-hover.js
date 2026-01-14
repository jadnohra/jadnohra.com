// Portfolio Leadership Hover - Extract story content from page
(function() {
  let popover = null;
  let storyCache = {};

  function init() {
    // Create popover
    popover = document.createElement('div');
    popover.className = 'story-popover';
    document.body.appendChild(popover);

    // Build cache of all stories from the page
    buildStoryCache();

    // Find all internal links and add hover behavior
    setupHovers();
  }

  function buildStoryCache() {
    // Find all h4 headings (story titles)
    const headings = document.querySelectorAll('h4');

    headings.forEach(h4 => {
      const id = h4.id;
      if (!id) return;

      // Normalize ID for lookup (lowercase, remove -star-* suffixes for matching)
      const normalizedId = id.toLowerCase();

      // Extract content following the heading until next heading
      let content = [];
      let sourceLink = null;
      let sibling = h4.nextElementSibling;

      while (sibling && !['H1', 'H2', 'H3', 'H4'].includes(sibling.tagName)) {
        if (sibling.tagName === 'P') {
          const text = sibling.innerHTML;
          content.push(text);

          // Check for Source link (rendered HTML: <strong>Source:</strong> <a href="#...">)
          const sourceMatch = text.match(/<strong>Source:<\/strong>.*?<a[^>]*href="#([^"]+)"[^>]*>/i);
          if (sourceMatch) {
            sourceLink = sourceMatch[1];
          }
        }
        sibling = sibling.nextElementSibling;
      }

      // Store in cache
      storyCache[normalizedId] = {
        title: h4.textContent,
        content: content,
        sourceLink: sourceLink
      };
    });
  }

  function getStoryContent(targetId) {
    // Normalize the target ID
    const normalizedTarget = targetId.toLowerCase().replace(/^#/, '');

    // Try exact match first
    if (storyCache[normalizedTarget]) {
      return storyCache[normalizedTarget];
    }

    // Try partial match (some links have -star-* suffixes)
    for (const key of Object.keys(storyCache)) {
      if (key.startsWith(normalizedTarget) || normalizedTarget.startsWith(key)) {
        return storyCache[key];
      }
    }

    return null;
  }

  function getFullStoryContent(sourceId) {
    const normalizedId = sourceId.toLowerCase();

    // Look for the full story (usually has -full suffix)
    for (const key of Object.keys(storyCache)) {
      if (key === normalizedId || key.includes(normalizedId)) {
        return storyCache[key];
      }
    }

    return null;
  }

  function positionPopover(mouseX, mouseY) {
    const offset = 16;
    const margin = 12;

    let left = mouseX + offset;
    let top = mouseY;

    // Get popover dimensions after content is set
    const popWidth = popover.offsetWidth || 600;
    const popHeight = popover.offsetHeight || 200;

    if (left + popWidth > window.innerWidth - margin) {
      left = mouseX - popWidth - offset;
    }
    if (left < margin) left = margin;

    if (top + popHeight > window.innerHeight - margin) {
      top = window.innerHeight - popHeight - margin;
    }
    if (top < margin) top = margin;

    popover.style.left = left + 'px';
    popover.style.top = top + 'px';
  }

  function setupHovers() {
    // Find all internal links that point to story sections
    const links = document.querySelectorAll('a[href^="#"]');

    links.forEach(link => {
      const href = link.getAttribute('href');
      if (!href || href === '#') return;

      const targetId = href.substring(1);
      const story = getStoryContent(targetId);

      if (!story || story.content.length === 0) return;

      link.addEventListener('mouseenter', (e) => {
        let html = '<div class="story-popover-title">' + story.title + '</div>';
        html += '<div class="story-popover-star">';

        story.content.forEach(p => {
          // Remove just the Source line, keep rest of content (STAR lines may be in same paragraph)
          const cleaned = p.replace(/<strong>Source:<\/strong>.*?(?=<strong>|<\/p>|$)/gi, '').trim();
          if (cleaned) {
            html += '<p>' + cleaned + '</p>';
          }
        });

        html += '</div>';

        // If there's a source link to full story, include it
        if (story.sourceLink) {
          const fullStory = getFullStoryContent(story.sourceLink);
          if (fullStory && fullStory.content.length > 0) {
            html += '<div class="story-popover-full">';
            html += '<div class="story-popover-full-title">Full:</div>';
            fullStory.content.forEach(p => {
              // Remove just the Source line for full stories too
              const cleaned = p.replace(/<strong>Source:<\/strong>.*?(?=<strong>|<\/p>|$)/gi, '').trim();
              if (cleaned) {
                html += '<p>' + cleaned + '</p>';
              }
            });
            html += '</div>';
          }
        }

        popover.innerHTML = html;
        positionPopover(e.clientX, e.clientY);
        popover.classList.add('visible');
      });

      link.addEventListener('mousemove', (e) => {
        positionPopover(e.clientX, e.clientY);
      });

      link.addEventListener('mouseleave', () => {
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
