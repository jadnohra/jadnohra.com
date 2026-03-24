// shell.js — injects tabs, breadcrumb, and statusbar into every page
// Usage: set data-page="pagename" on <body>, include this script

(function () {
  var tabs = [
    { name: 'index', href: '/' },
    { name: 'tech', href: '/tech.html' },
    { name: 'writing', href: '/writing/' },
    { name: 'learn-rust', href: '/learn-rust/' },
    { name: 'quotes', href: '/quotes.html' },
    { name: 'music', href: '/music/' },
    { name: 'cv', href: '/cv/' }
  ];

  var page = document.body.getAttribute('data-page') || '';
  var path = window.location.pathname;

  // Determine which tab is active
  function isActive(tab) {
    if (tab.name === page) return true;
    if (tab.name === 'index' && (path === '/' || path === '/index.html')) return true;
    if (tab.name === 'writing' && path.indexOf('/writing') === 0) return true;
    return false;
  }

  // Build tabs
  var tabsHtml = '<div class="tabs">';
  for (var i = 0; i < tabs.length; i++) {
    var t = tabs[i];
    var cls = isActive(t) ? ' active' : '';
    tabsHtml += '<a class="tab' + cls + '" href="' + t.href + '">' + t.name + '</a>';
  }
  tabsHtml += '</div>';

  // Build breadcrumb from URL path
  var parts = path.replace(/\/index\.html$/, '/').replace(/\.html$/, '').split('/').filter(Boolean);
  var crumbHtml = '<div class="breadcrumb">jadnohra.com';
  for (var j = 0; j < parts.length; j++) {
    crumbHtml += '<span class="sep">/</span>' + parts[j];
  }
  crumbHtml += '</div>';

  // Build statusbar
  var statusPath = parts.length > 0 ? parts.join(' / ') : 'index';
  var statusHtml = '<div class="statusbar"><span>jadnohra.com / ' + statusPath + '</span><span>UTF-8</span></div>';

  // Inject at start of body (before content)
  var shell = document.createElement('div');
  shell.innerHTML = tabsHtml + crumbHtml;
  document.body.insertBefore(shell, document.body.firstChild);

  // Inject statusbar at end
  document.body.insertAdjacentHTML('beforeend', statusHtml);

  // TOC generator — activate with data-toc="true" on <body>
  if (document.body.getAttribute('data-toc') === 'true') {
    var content = document.querySelector('.content');
    if (content) {
      var headings = content.querySelectorAll('h2, h3');
      if (headings.length > 2) {
        // Add IDs to headings
        var tocHtml = '<div class="toc"><p class="toc-title">CONTENTS</p>';
        for (var k = 0; k < headings.length; k++) {
          var h = headings[k];
          var id = h.textContent.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, '');
          h.id = id;
          var indent = h.tagName === 'H3' ? 'toc-h3' : 'toc-h2';
          tocHtml += '<a class="toc-item ' + indent + '" href="#' + id + '">' + h.textContent + '</a>';
        }
        tocHtml += '</div>';
        content.insertAdjacentHTML('afterbegin', tocHtml);
      }
    }
  }
})();
