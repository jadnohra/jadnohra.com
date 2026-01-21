# Books Research - Resume Context

## Task
Find notable books from TheNotes and 77-neurons repos, then decide where/how to add them to portfolio.

---

## Consolidated Findings (Both Repos)

### 77-neurons Project (2011-2012) - Structured Weekly Study

**Core curriculum books worked through chapter by chapter:**
1. **The Calculus Lifesaver** (Adrian Banner) — weeks 1-20
2. **Book of Proof** (Richard Hammack) — weeks 21-32
3. **Linear Algebra** (Jim Hefferon) — weeks 33-55

---

### TheNotes (2013-2019) - Books with Dedicated Note Files

**Mathematics (2013-2014):**
- **Hefferon's 'Linear Algebra'** — continued/revisited from 77-neurons
- **Elias Zakon's 'Basic Concepts of Mathematics'**
- **Euclid's Elements** — with separate figures PDF
- **Morris Kline's 'Mathematical Thought from Ancient to Modern Times'**
- **Euler's Writings**
- **Parlett's 'The Symmetric Eigenvalue Problem'**

**Logic & Foundations (2013-2017):**
- **Barwise's 'The Liar'** (2013-2014) — logic/paradoxes
- **Potter's 'Set Theory and its Philosophy'** (2015)
- **Potter's 'Reason's Nearest Kin'** (2015) — philosophy of mathematics
- **'There's Something about Gödel'** (2015) — studied twice (2 PDFs)
- **Chiswell's 'Mathematical Logic'** (2017) — handwritten notes

**Physics & History of Science (2013-2015):**
- **Jammer's 'Concepts of Force'** (2013-2014) — history/philosophy of physics
- **Whittaker's 'Theories of the Aether and Electricity'** (2013-2014)
- **Stevin's Inclined Plane** (2013-2014) — historical mechanics
- **Hestenes' 'Reforming the Mathematical Language of Physics'** (2015)

**CS/Algorithms (2013-2014):**
- **GJK Algorithm** — collision detection
- **Linear Complementarity Problem** — with supplementary notes

**Later (2019):**
- **Linear Algebra Done Right** (Axler) — memo notes

---

## Existing Books Section (portfolio.md line 759+)

```markdown
## Books

Favorite technical books studied throughout career.

1. **[Statistics for Experimenters](https://www.wiley.com/en-us/Statistics+for+Experimenters%3A+Design%2C+Innovation%2C+and+Discovery%2C+2nd+Edition-p-9780471718130)** (Box, Hunter, Hunter) — Design of experiments, hypothesis testing, confidence intervals
2. **[Probabilistic Safety Assessment in the Chemical and Nuclear Industries](https://books.google.de/books/about/Probabilistic_safety_assessment_in_the_c.html?id=CAUuWcTmNrQC)** (Fullwood) — Safety analysis for chemical and nuclear industries
3. **AI Game Programming Wisdom** (4 volumes) — Game AI algorithms and techniques
```

---

## Decision Needed

Options:
1. Expand the existing Books section with more entries
2. Create Study entries that reference books (like existing "2015 - Study: Foundations & Logic")
3. Both - key books in Books section, detailed study in Study entries

---

## Next Steps

1. ~~Run 77-neurons agent to find topology/Perelman books~~ DONE
2. ~~Decide on approach~~ DONE (Option 3: Both)
3. ~~Implement additions to portfolio.md~~ DONE

## Changes Made

**New timeline entry added (line 362):**
- 2011-2012 - Study: Mathematical Foundations — 77-neurons Project Perelman
- References: Calculus Lifesaver, Book of Proof, Hefferon's Linear Algebra

**Books section expanded (lines 769-788) — now 20 entries:**
- Statistics for Experimenters (Box et al.)
- Probabilistic Safety Assessment (Fullwood)
- AI Game Programming Wisdom (4 vols)
- Linear Algebra (Hefferon) — 77-neurons + TheNotes
- Book of Proof (Hammack)
- Linear Algebra Done Right (Axler)
- Mathematical Thought (Morris Kline)
- Euclid's Elements (Heath)
- The Calculus Lifesaver (Banner)
- The Symmetric Eigenvalue Problem (Parlett)
- Set Theory and its Philosophy (Potter)
- Reason's Nearest Kin (Potter)
- The Liar (Barwise & Etchemendy)
- Basic Concepts of Mathematics (Zakon)
- Concepts of Force (Jammer)
- Theories of Aether and Electricity (Whittaker)
- There's Something about Gödel (Berto)
- Mathematical Logic (Chiswell & Hodges)
- Reforming the Mathematical Language of Physics (Hestenes)
- Euler's Writings

---

## Proposal: Add Books to Interactive Wheel

The portfolio-graph.json already has a `books` category (amber color) with 3 entries. Propose adding the new books with connections to theories, algorithms, and projects.

### New Book Nodes to Add

```json
{"name": "books.hefferon", "label": "Linear Algebra (Hefferon)", "imports": ["theory.linalg"]},
{"name": "books.bookofproof", "label": "Book of Proof", "imports": ["theory.prooftheory", "theory.settheory"]},
{"name": "books.axler", "label": "Linear Algebra Done Right", "imports": ["theory.linalg"]},
{"name": "books.morriskline", "label": "Mathematical Thought", "imports": ["theory.realanalysis", "theory.settheory"]},
{"name": "books.euclid", "label": "Euclid's Elements", "imports": ["theory.prooftheory"]},
{"name": "books.banner", "label": "Calculus Lifesaver", "imports": ["theory.realanalysis"]},
{"name": "books.parlett", "label": "Symmetric Eigenvalue Problem", "imports": ["theory.linalg", "algo.eigenvalue"]},
{"name": "books.potterset", "label": "Potter Set Theory", "imports": ["theory.settheory"]},
{"name": "books.potterreason", "label": "Potter Reason's Nearest Kin", "imports": ["theory.settheory", "theory.fol"]},
{"name": "books.barwise", "label": "The Liar (Barwise)", "imports": ["theory.fol", "theory.settheory"]},
{"name": "books.zakon", "label": "Zakon Basic Concepts", "imports": ["theory.settheory", "theory.realanalysis"]},
{"name": "books.jammer", "label": "Concepts of Force", "imports": ["theory.rbd", "theory.lagrangian"]},
{"name": "books.whittaker", "label": "Aether & Electricity", "imports": ["theory.rbd", "theory.lagrangian"]},
{"name": "books.berto", "label": "Something about Gödel", "imports": ["theory.fol", "theory.prooftheory"]},
{"name": "books.chiswell", "label": "Mathematical Logic", "imports": ["theory.fol", "theory.modeltheory"]},
{"name": "books.hestenes", "label": "Hestenes GA for Physics", "imports": ["theory.rbd", "theory.linalg"]},
{"name": "books.euler", "label": "Euler's Writings", "imports": ["theory.realanalysis", "theory.numbertheory"]}
```

### Projects to Update (add book imports)

1. **project.thenotes** — add: `books.hefferon`, `books.jammer`, `books.whittaker`, `books.euclid`, `books.morriskline`, `books.euler`, `books.parlett`, `books.potterset`, `books.potterreason`, `books.barwise`, `books.berto`, `books.chiswell`, `books.hestenes`, `books.zakon`

2. **Add new project node for 77-neurons:**
```json
{"name": "project.77neurons", "label": "77-neurons Project", "imports": ["books.banner", "books.bookofproof", "books.hefferon", "theory.linalg", "theory.prooftheory", "theory.realanalysis"]}
```

3. **project.defdefqed** — add: `books.potterset`, `books.barwise`, `books.chiswell`, `books.berto`

4. **project.mlcp** — add: `books.hefferon`, `books.parlett`

5. **project.bigfoot** — add: `books.jammer` (concepts of force relevant to mechanics)

6. **algo.gjk** — add: `books.hefferon` (linear algebra foundations)

7. **algo.eigenvalue** — add: `books.parlett`, `books.axler`

### Summary

- 17 new book nodes
- 1 new project node (77-neurons)
- ~6 existing projects/algos to update with book imports
- Books become connected to theories they cover, and projects that used them

### IMPLEMENTED ✓

All changes applied to `portfolio-graph.json`:
- Added 17 book nodes (lines 166-182)
- Added project.77neurons (line 28)
- Updated project.thenotes with 14 book imports (line 29)
- Updated project.defdefqed with 4 book imports (line 30)
- Updated project.mlcp with 2 book imports (line 34)
- Updated project.bigfoot with books.jammer (line 47)
- Updated algo.gjk with books.hefferon (line 90)
- Updated algo.eigenvalue with books.parlett, books.axler (line 95)
- Fixed project ordering: zkaccel and fernridesafety now at top (lines 22-23)
