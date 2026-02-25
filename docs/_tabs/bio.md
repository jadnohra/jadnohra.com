---
layout: post
title: Bio
icon: fas fa-folder-tree
order: 1
toc: true
---

<style>
.bio-explorer *{margin:0;padding:0;box-sizing:border-box}
.bio-explorer{color:#1e293b;font-family:system-ui,-apple-system,sans-serif;max-width:780px;margin:0 auto}
.bio-explorer button{font-family:inherit;display:inline-flex !important;width:auto !important;text-align:left !important;border:none !important;background:none !important}
.bio-explorer h1{font-size:20px;font-weight:700}
.bio-explorer .sub{color:#6b7280;font-size:11px;margin:2px 0 0}
.bio-explorer .header{display:flex;align-items:center;justify-content:space-between;margin-bottom:12px;flex-wrap:wrap;gap:8px}
.bio-explorer .header-right{display:flex;align-items:center;gap:12px}
.bio-explorer .search-wrap{position:relative}
.bio-explorer .search-wrap svg{position:absolute;left:8px;top:7px;color:#6b7280}
.bio-explorer .search-wrap input{background:#f8fafc;border:1px solid #e2e8f0;border-radius:6px;padding:5px 10px 5px 28px;color:#1e293b;font-size:12px;width:140px;outline:none}
.bio-explorer .be-row{display:flex !important;gap:6px;margin-bottom:10px;flex-wrap:wrap;align-items:center}
.bio-explorer .be-pill{border-radius:6px !important;padding:4px 10px !important;cursor:pointer;font-size:12px !important;transition:all 0.12s;display:inline-flex !important;align-items:center;gap:5px;white-space:nowrap;border:1px solid #e2e8f0 !important;background:#f8fafc !important;color:#64748b !important;width:auto !important}
.bio-explorer .be-pill.active{background:var(--ac,#f97316)22 !important;border-color:var(--ac,#f97316)66 !important;color:var(--ac,#f97316) !important}
.bio-explorer .be-pill .cnt{font-size:9px;opacity:0.5}
.bio-explorer .be-pill .co-period{font-size:10px;opacity:0.6}
.bio-explorer .be-pill .co-name{font-weight:600}
.bio-explorer .toggle-btn{background:none;border:1px solid #e2e8f0;border-radius:6px;padding:4px 10px;font-size:11px;cursor:pointer;display:flex;align-items:center;gap:5px;transition:all 0.15s}
.bio-explorer .toggle-btn.on{color:#f97316;border-color:rgba(249,115,22,0.3)}
.bio-explorer .toggle-btn.off{color:#6b7280}
.bio-explorer .clear-btn{background:none;border:none;color:#f97316;font-size:11px;cursor:pointer;padding:4px 6px;display:flex;align-items:center;gap:3px}
.bio-explorer .cat-header{background:none;border:none;color:#6b7280;font-size:11px;cursor:pointer;padding:0;display:flex;align-items:center;gap:4px;font-weight:600;text-transform:uppercase;letter-spacing:0.5px;margin-bottom:5px}
.bio-explorer .cat-header .desc{font-weight:400;font-size:10px;opacity:0.6;text-transform:none}
.bio-explorer .cat-header .ac-count{color:#f97316;font-size:10px;font-weight:400}
.bio-explorer .cat-tags{display:flex;gap:4px;flex-wrap:wrap;margin-bottom:8px}
.bio-explorer .filter-row{display:flex;justify-content:space-between;align-items:center;margin-bottom:8px;margin-top:12px}
.bio-explorer .filter-row .count{font-size:11px;color:#6b7280}
.bio-explorer .active-tags{display:flex;gap:3px;flex-wrap:wrap}
.bio-explorer .active-tag{font-size:10px;padding:2px 6px;border-radius:4px;display:flex;align-items:center;gap:3px;cursor:pointer}
.bio-explorer .cards{display:flex;flex-direction:column;gap:4px}
.bio-explorer .card{border-radius:8px;padding:8px 12px;transition:all 0.12s;border:1px solid #e2e8f0;background:#fafbfc}
.bio-explorer .card.open{background:#fff7ed;border-color:rgba(249,115,22,0.3)}
.bio-explorer .card-head{cursor:pointer;display:flex;align-items:center;gap:8px}
.bio-explorer .card-co{font-size:10px;font-weight:700;min-width:26px}
.bio-explorer .card-id{color:#6b7280;font-size:10px;font-weight:600;min-width:40px;font-family:monospace}
.bio-explorer .card-title{font-size:13px;font-weight:600;flex:1}
.bio-explorer .card-period{font-size:10px;color:#94a3b8;min-width:50px;text-align:right}
.bio-explorer .card-icons{display:flex;gap:2px}
.bio-explorer .card-icons svg{width:12px;height:12px}
.bio-explorer .card-body{margin-top:8px;padding-top:8px;border-top:1px solid #e2e8f0}
.bio-explorer .card-body p{font-size:13px;color:#475569;line-height:1.55;margin:0 0 8px}
.bio-explorer .card-body .tech-line{font-size:12px;color:#64748b;font-family:'SF Mono',SFMono-Regular,Consolas,monospace;background:#f1f5f9;padding:4px 8px;border-radius:4px;margin:0 0 8px;line-height:1.5}
.bio-explorer .card-body .no-full{font-size:11px;color:#94a3b8;font-style:italic;margin:0 0 8px}
.bio-explorer .card-links{display:flex;gap:4px;flex-wrap:wrap;margin:0 0 6px}
.bio-explorer .card-link{font-size:10px;padding:2px 8px;border-radius:4px;background:#eff6ff;color:#3b82f6;text-decoration:none;display:inline-flex;align-items:center;gap:3px;border:1px solid #bfdbfe}
.bio-explorer .card-link:hover{background:#dbeafe}
.bio-explorer .card-rel{font-size:10px;color:#f97316;margin:0 0 6px;display:flex;align-items:center;gap:4px;flex-wrap:wrap}
.bio-explorer .card-rel span{background:rgba(249,115,22,0.1);padding:1px 6px;border-radius:3px}
.bio-explorer .tag-row{display:flex;gap:3px;flex-wrap:wrap;margin-bottom:3px;align-items:center}
.bio-explorer .tag-row .cat-label{font-size:9px;color:#94a3b8;min-width:55px;text-transform:uppercase}
.bio-explorer .mini-tag{display:inline-flex;align-items:center;gap:3px;font-size:10px;padding:1px 6px;border-radius:3px;background:#f1f5f9;color:#64748b;cursor:pointer}
.bio-explorer .mini-tag.active{color:var(--ac);background:var(--ac-bg)}
.bio-explorer-popover{position:fixed;background:#fff;border:1px solid #f97316;border-radius:8px;padding:12px 16px;z-index:9999;pointer-events:none;box-shadow:0 4px 24px rgba(0,0,0,0.12);opacity:0;transition:opacity 0.15s;max-height:60vh;overflow-y:auto}
.bio-explorer-popover.visible{opacity:1}
.bio-explorer-popover .pop-head{display:flex;gap:8px;align-items:center;margin-bottom:8px}
.bio-explorer-popover .pop-co{font-size:10px;font-weight:700}
.bio-explorer-popover .pop-id{color:#f97316;font-weight:700;font-size:14px}
.bio-explorer-popover .pop-title{color:#1e293b;font-size:13px;font-weight:600}
.bio-explorer-popover .pop-text{color:#475569;font-size:12px;line-height:1.6}
.bio-explorer-popover .pop-tech{font-size:11px;color:#64748b;font-family:'SF Mono',SFMono-Regular,Consolas,monospace;background:#f1f5f9;padding:3px 6px;border-radius:3px;margin-top:6px;line-height:1.5}
.bio-explorer-popover .pop-tags{display:flex;gap:4px;flex-wrap:wrap;margin-top:8px}
.bio-explorer-popover .pop-tag{display:inline-flex;align-items:center;gap:3px;font-size:10px;padding:1px 6px;border-radius:3px}
.bio-explorer .arc{margin-top:24px;border-top:1px solid #e2e8f0;padding-top:16px}
.bio-explorer .arc-header{background:none;border:none;color:#f97316;font-size:14px;font-weight:700;cursor:pointer;padding:0;display:flex;align-items:center;gap:6px}
.bio-explorer .arc-header .a-sub{font-size:11px;font-weight:400;color:#6b7280}
.bio-explorer .a-list{margin-top:12px;display:flex;flex-direction:column;gap:6px}
.bio-explorer .a-card{background:#fafbfc;border:1px solid #e2e8f0;border-radius:6px;padding:10px 14px;position:relative}
.bio-explorer .a-card::before{content:'';position:absolute;left:0;top:0;bottom:0;width:3px;border-radius:6px 0 0 6px;background:var(--ac,#f97316)}
.bio-explorer .a-card .a-head{display:flex;align-items:baseline;gap:8px;margin-bottom:4px}
.bio-explorer .a-card .a-num{color:var(--ac,#f97316);font-size:16px;font-weight:400;min-width:20px}
.bio-explorer .a-card .a-name{color:var(--ac,#f97316);font-weight:700;font-size:14px}
.bio-explorer .a-card .a-period{font-size:11px;color:#94a3b8;margin-left:auto}
.bio-explorer .a-card .a-short{color:#475569;font-size:13px;margin:0 0 6px;padding-left:28px}
.bio-explorer .a-chain{padding-left:28px;display:flex;flex-direction:column;gap:4px}
.bio-explorer .chain-step{display:flex;align-items:baseline;gap:6px;font-size:12px}
.bio-explorer .chain-arrow{color:var(--ac);font-weight:700;font-size:14px;flex-shrink:0}
.bio-explorer .chain-step .a-story-link{font-size:10px;padding:1px 6px;border-radius:3px;background:rgba(249,115,22,0.12);color:#f97316;cursor:pointer;flex-shrink:0}
.bio-explorer .chain-note{color:#64748b;font-size:12px;line-height:1.4}
.bio-explorer .empty{color:#94a3b8;font-size:13px;text-align:center;padding:20px}
.bio-explorer svg.i{width:12px;height:12px;fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0}
.bio-explorer svg.i14{width:14px;height:14px;fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0}
.bio-explorer svg.i16{width:16px;height:16px;fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0}
.bio-explorer svg.i9{width:9px;height:9px;fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0}
.bio-explorer svg.i11{width:11px;height:11px;fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0}
.bio-explorer .card-title svg.i11{stroke:#eab308;fill:#eab308}
.bio-explorer .gallery{display:flex;gap:8px;overflow-x:auto;padding:4px 0 12px;margin-bottom:8px;scrollbar-width:thin}
.bio-explorer .gallery::-webkit-scrollbar{height:4px}
.bio-explorer .gallery::-webkit-scrollbar-thumb{background:#e2e8f0;border-radius:2px}
.bio-explorer .gal-card{flex-shrink:0;width:180px;border-radius:8px;overflow:hidden;border:1px solid #e2e8f0;background:#fafbfc;cursor:pointer;transition:all 0.15s}
.bio-explorer .gal-card:hover{border-color:rgba(249,115,22,0.4);box-shadow:0 2px 12px rgba(0,0,0,0.08)}
.bio-explorer .gal-card img{width:100%;height:101px;object-fit:cover;display:block;background:#f1f5f9}
.bio-explorer .gal-card .gal-site{width:100%;height:101px;display:flex;align-items:center;justify-content:center;background:linear-gradient(135deg,#f8fafc,#f1f5f9);font-size:16px;font-weight:700;color:#64748b;letter-spacing:0.5px}
.bio-explorer .gal-card .gal-info{padding:5px 8px}
.bio-explorer .gal-card .gal-id{font-size:9px;color:#94a3b8;font-family:monospace}
.bio-explorer .gal-card .gal-name{font-size:11px;font-weight:600;color:#1e293b;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.bio-explorer .inline-thumb{margin:0 0 8px;display:flex;gap:6px;flex-wrap:wrap}
.bio-explorer .inline-thumb a{display:block;border-radius:6px;overflow:hidden;border:1px solid #e2e8f0;transition:all 0.15s}
.bio-explorer .inline-thumb a:hover{border-color:rgba(249,115,22,0.4)}
.bio-explorer .inline-thumb img{height:80px;width:auto;display:block;background:#f1f5f9}
.bio-explorer .gal-thumbs{display:flex;gap:6px;flex-wrap:wrap;align-items:center;padding:6px 0 2px 34px}
.bio-explorer .gal-thumb{display:block;border-radius:4px;overflow:hidden;border:1px solid #e2e8f0;transition:all 0.15s;cursor:pointer;position:relative}
.bio-explorer .gal-thumb:hover{border-color:rgba(249,115,22,0.5);box-shadow:0 2px 12px rgba(0,0,0,0.1)}
.bio-explorer .gal-thumbs img{height:48px;width:auto;display:block;background:#f1f5f9}
.bio-explorer .gal-play{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);color:#fff;background:rgba(0,0,0,0.5);border-radius:50%;width:20px;height:20px;display:flex;align-items:center;justify-content:center}
.bio-lightbox{position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,0.8);z-index:99999;display:flex;align-items:center;justify-content:center;cursor:pointer;opacity:0;pointer-events:none;transition:opacity 0.2s}
.bio-lightbox.visible{opacity:1;pointer-events:auto}
.bio-lightbox img{max-width:90vw;max-height:90vh;border-radius:8px;box-shadow:0 8px 40px rgba(0,0,0,0.4);object-fit:contain}
.bio-lightbox iframe{width:80vw;height:45vw;max-width:960px;max-height:540px;border-radius:8px;box-shadow:0 8px 40px rgba(0,0,0,0.4)}
/* SVG icons inside popover */
.bio-explorer-popover svg.i9{width:9px;height:9px;fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round;flex-shrink:0}
</style>

<div id="bio-app" class="bio-explorer"></div>

<script src="{{ '/assets/js/bio-explorer.js' | relative_url }}"></script>

---

## Knowledge Graph

Interactive visualization of connections between career, projects, algorithms, and theory. Hover over nodes to highlight relationships.

<script src="https://d3js.org/d3.v7.min.js"></script>

<div id="portfolio-viz" style="display:flex;justify-content:center;margin:1rem 0"></div>

<style>
#portfolio-viz svg { font: 11px sans-serif !important; }
#portfolio-viz svg text { font-size: 12px !important; }
</style>

<script src="{{ '/assets/js/portfolio-viz.js' | relative_url }}"></script>

---

## Books

Favorite technical books studied throughout career.

1. **[Statistics for Experimenters](https://www.wiley.com/en-us/Statistics+for+Experimenters%3A+Design%2C+Innovation%2C+and+Discovery%2C+2nd+Edition-p-9780471718130)** (Box, Hunter, Hunter) — Design of experiments, hypothesis testing, confidence intervals
2. **[Probabilistic Safety Assessment in the Chemical and Nuclear Industries](https://books.google.de/books/about/Probabilistic_safety_assessment_in_the_c.html?id=CAUuWcTmNrQC)** (Fullwood) — Safety analysis for chemical and nuclear industries
3. **AI Game Programming Wisdom** (4 volumes) — Game AI algorithms and techniques
4. **[Linear Algebra](https://hefferon.net/linearalgebra/)** (Hefferon) — Free textbook, worked through completely in [77-neurons](https://github.com/jadnohra/77-neurons-project-perelman) with [detailed notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Hefferon's%20%27Linear%20Algebra%27.pdf)
5. **[Book of Proof](https://www.people.vcu.edu/~rhammack/BookOfProof/)** (Hammack) — Introduction to mathematical proof techniques
6. **[Linear Algebra Done Right](https://linear.axler.net/)** (Axler) — Abstract approach to linear algebra, [memo notes](https://github.com/jadnohra/TheNotes/blob/master/2019/memo_linear-algebra-done-right.pdf)
7. **[Mathematical Thought from Ancient to Modern Times](https://global.oup.com/academic/product/mathematical-thought-from-ancient-to-modern-times-9780195061352)** (Morris Kline) — History of mathematics, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Morris%20Kline's%20%27Mathematical%20Thought%27.pdf)
8. **[Euclid's Elements](https://mathcs.clarku.edu/~djoyce/elements/elements.html)** (Heath translation) — Classical foundations of geometry, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Euclid's%20Elements.pdf)
9. **[The Calculus Lifesaver](https://press.princeton.edu/books/paperback/9780691130880/the-calculus-lifesaver)** (Banner) — Worked through in [77-neurons](https://github.com/jadnohra/77-neurons-project-perelman) weeks 1-20
10. **[The Symmetric Eigenvalue Problem](https://epubs.siam.org/doi/book/10.1137/1.9781611971163)** (Parlett) — Numerical linear algebra, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Parlett's%20The%20Symmetric%20Eigenvalue%20Problem.pdf)
11. **[Set Theory and its Philosophy](https://global.oup.com/academic/product/set-theory-and-its-philosophy-9780199270415)** (Potter) — Foundations of mathematics, [notes](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Potter's%20Set%20Theory%20and%20its%20Philosophy.pdf)
12. **[Reason's Nearest Kin](https://global.oup.com/academic/product/reasons-nearest-kin-9780199252619)** (Potter) — Philosophy of mathematics, [notes](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Potter's%20%27Reason's%20Nearest%20Kin%27.pdf)
13. **[The Liar: An Essay on Truth and Circularity](https://global.oup.com/academic/product/the-liar-9780195059441)** (Barwise & Etchemendy) — Logic and paradoxes, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Barwise's%20%27The%20Liar%27.pdf)
14. **[Basic Concepts of Mathematics](https://www.trillia.com/zakon1.html)** (Zakon) — Foundations, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Elias%20Zakon's%20Basic%20Concepts%20of%20Mathematics.pdf)
15. **[Concepts of Force](https://www.hup.harvard.edu/books/9780674017108)** (Jammer) — History and philosophy of physics, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Jammer's%20%27Force%27.pdf)
16. **[A History of the Theories of Aether and Electricity](https://archive.org/details/historyoftheorie00whitrich)** (Whittaker) — Classical physics history, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20%27Theories%20of%20the%20Aether%20and%20Electricity%2C%20Whittaker%27.pdf)
17. **[There's Something about Gödel](https://www.wiley.com/en-us/There's+Something+about+G%C3%B6del%3A+The+Complete+Guide+to+the+Incompleteness+Theorem-p-9781405197663)** (Berto) — Incompleteness theorem, [notes 1](https://github.com/jadnohra/TheNotes/blob/master/2015/Study%20of%20%27There's%20Something%20about%20Goedel%27.pdf), [notes 2](https://github.com/jadnohra/TheNotes/blob/master/2015/Study%20of%20%27There's%20Something%20about%20Goedel%27%202.pdf)
18. **[Mathematical Logic](https://global.oup.com/academic/product/mathematical-logic-9780198571001)** (Chiswell & Hodges) — First-order logic, [handwritten notes](https://github.com/jadnohra/TheNotes/blob/master/2017/math_logic_chiswell_1.jpeg)
19. **[Reforming the Mathematical Language of Physics](http://geocalc.clas.asu.edu/pdf/OersijGAPhysics.pdf)** (Hestenes) — Geometric algebra for physics, [notes](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Hestenes%27%20%27Reforming%20the%20Mathematical%20Language%20of%20Physics%27.pdf)
20. **Euler's Writings** — Primary sources, [notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Euler's%20Writings.pdf)

---

## Bio Extended

[Extended bio](/bio-ext/)
