---
layout: page
title: Portfolio
icon: fas fa-folder-tree
order: 5
---

# Learning Journey Portfolio

A chronological map of projects, what I learned, and how they connected to my career.

---

## Career Thread

```
Project Learning                    → Career Application
─────────────────────────────────────────────────────────────
Animation/Locomotion               → AI Sandbox, Guerrilla: "motion planning for humanoids"
Physics/Constraints                → Havok: "Core Physics Engine R&D"
Navigation/Pathfinding             → AI Sandbox, Guerrilla: "automated navigable space generation"
MLCP/Linear Programming            → Havok: "linear programming in physics solvers"
Numerical Analysis                 → Guided Knowledge: "nonlinear optimization for IMU"
All foundations                    → AV Simulation (Audi/Argo/FERNRIDE/Irreducible)
```

---

## Study Notes (TheNotes)

Comprehensive mathematical notes that supported the projects below:

### Physics & Mechanics
- [Rigid Body Dynamics Treatment](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf)
- [Lagrangian Framework](https://github.com/jadnohra/TheNotes/blob/master/2015/lagrfrmw.pdf)
- [Hestenes' Geometric Algebra for Physics](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Hestenes%27%20%27Reforming%20the%20Mathematical%20Language%20of%20Physics%27.pdf)

### Linear Algebra & Numerical
- [Linear Complementarity Problem](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20Linear%20Complementarity%20Problem.pdf)
- [GJK Algorithm](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20GJK%20Algorithm..pdf)
- [Eigenvalue Problems](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Parlett's%20The%20Symmetric%20Eigenvalue%20Problem.pdf)
- [Lagrange Multiplier Proofs](https://github.com/jadnohra/TheNotes/blob/master/2016/proof_lagrmul.pdf)

### Geometry & Rotation
- [Rotation Mathematics](https://github.com/jadnohra/TheNotes/blob/master/2015/rotation.pdf)
- [Covariance](https://github.com/jadnohra/TheNotes/blob/master/2015/covar.pdf)
- [Differential Forms](https://github.com/jadnohra/TheNotes/blob/master/2016/dform.pdf)
- [Manifolds](https://github.com/jadnohra/TheNotes/blob/master/2016/mfold_short.pdf)

### Logic & Computation
- [First-Order Logic](https://github.com/jadnohra/TheNotes/blob/master/2018/Free%20Occurrences%20in%20FOL.pdf)
- [Programming Logic](https://github.com/jadnohra/TheNotes/blob/master/2017/plql.pdf)

**Full collection:** [TheNotes Repository](https://github.com/jadnohra/TheNotes)

---

## Digital Library (tag_export)

Curated collection of academic resources that informed this work:

- Mathematics & Logic
- Physics (Classical, Quantum, Mechanics)
- Numerical Analysis & Optimization
- Machine Learning & AI
- Computer Science & Algorithms

**Library Index:** [tag_export Repository](https://github.com/jadnohra/tag_export)

---

## Projects

- **2015 - MLCP 2.0 (Julia Implementation)**
  - Tech Stack: Julia, GLPK
  - Algorithms: Custom LP solver, LU decomposition, MLCP solving with benchmarks
  - Learned: Julia for numerical computing, solver performance optimization
  - Related Notes: [Lagrange Multiplier Proofs](https://github.com/jadnohra/TheNotes/blob/master/2016/proof_lagrmul.pdf), [Eigenvalue Problems](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Parlett's%20The%20Symmetric%20Eigenvalue%20Problem.pdf)
  - Applied: → Havok solver optimization → Guided Knowledge: "nonlinear optimization for IMU sensors"
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Lab2015/mlcp2.0)

- **2015 - asinus_salutem (Math Expression Parser)**
  - Tech Stack: Python, SymPy
  - Algorithms: Tokenization, expression parsing, symbolic computation, AST building
  - Learned: Compiler techniques for math expressions, symbolic math integration
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Lab2015/asinus_salutem)

- **2014 - Labs2014 Projects**
  - dynamics - Physics simulation research
  - formal1 - Formal methods exploration
  - glfw - Graphics library bindings
  - rseq - Sequence processing
  - Tech Stack: C++, GLFW
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs2014)

- **2013-2014 - Bigfoot Skeletal Animation System**
  - {% include embed/youtube.html id='XMj0xhCzx4U' %}
  - Tech Stack: C++, OpenGL, GLM, strtk tokenizer, Bigeye UI
  - Algorithms: BVH parsing, quaternion conversion, rigid body dynamics, joint constraints, Lagrange multipliers, footstep planning, stride geometry, support polygon, skeleton topology analysis, semantic inference, motion retargeting, pose transfer
  - Learned: Motion capture pipelines, physics-based animation, skate-free locomotion
  - Related Notes: [Rigid Body Dynamics](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf), [Lagrangian Framework](https://github.com/jadnohra/TheNotes/blob/master/2015/lagrfrmw.pdf), [Rotation Mathematics](https://github.com/jadnohra/TheNotes/blob/master/2015/rotation.pdf), [Covariance](https://github.com/jadnohra/TheNotes/blob/master/2015/covar.pdf)
  - Applied: → [AI Sandbox](https://web.archive.org/web/20190719084901/http://aigamedev.com/): "Motion Graphs, Locomotion Planning" → Guerrilla: "motion planning for humanoids" → Havok animation R&D
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigfoot)

- **2013-2014 - Bigeye UI Framework**
  - Tech Stack: C++, OpenGL, MagickWand, Win32, DroidSans font
  - Algorithms: Retained-mode rendering, scene graph traversal, render tree building, widget event handling
  - Learned: UI architecture patterns, immediate vs retained mode tradeoffs
  - Applied: → Rapid prototyping tools throughout career
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigeye)

- **2013-2014 - Puppeteer (BVH Viewer)**
  - Tech Stack: C++, OpenGL, GLM
  - Algorithms: BVH loading, skeletal rendering, animation playback
  - Learned: Motion capture visualization, skeleton rendering
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Puppeteer)

- **2013-2014 - BlenderToBVH Export Tool**
  - Tech Stack: Python, Blender API
  - Algorithms: Armature traversal, coordinate system conversion, BVH format export
  - Learned: Blender scripting, animation pipeline tools
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/BlenderToBVH)

- **2013-2014 - Physics Research (lamby/MLCP)**
  - Tech Stack: C++, FLANN (nearest neighbors)
  - Algorithms: Mixed Linear Complementarity Problem (MLCP), constraint solving, Lagrange multipliers
  - Learned: How physics engines solve constraints, contact resolution
  - Related Notes: [Linear Complementarity Problem](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20Linear%20Complementarity%20Problem.pdf), [Supplementary LCP Notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Supplementary%20Notes%20on%20the%20Linear%20Complemetarity%20Problem.pdf), [GJK Algorithm](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20GJK%20Algorithm..pdf)
  - Applied: → Havok: "linear programming in physics solvers"
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Physics/lamby)

- **2013-2014 - Floating Point Analysis**
  - Tech Stack: Python, C++
  - Algorithms: IEEE 754 bit manipulation, epsilon calculation, denormalized number handling
  - Learned: Numerical precision limits, floating point representation
  - Applied: → Havok: "Low-level optimization, numerical analysis"
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/fp)

- **2013-2014 - Numerical Libraries**
  - FLANN - Fast Library for Approximate Nearest Neighbors
  - randomc - Random number generation
  - mprog - Mathematical programming solvers
  - nics - Numerical computing utilities
  - Tech Stack: C++, CMake
  - Algorithms: KD-trees, approximate nearest neighbor search, random distributions, LP solvers
  - Learned: Spatial queries, numerical computing foundations
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Numerical)

- **2012 - Graphics Framework (Quake3 BSP)**
  - Tech Stack: C++, OpenGL, Win32
  - Algorithms: BSP tree traversal, PVS (Potentially Visible Set) culling, Lightmap rendering, Texture streaming
  - Learned: Level streaming architecture, occlusion culling, game engine internals
  - Applied: [TODO - user to fill]
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Graphics/Framework1)

- **2008-2009 - TicTacToe AI Testbed**
  - Tech Stack: C++, Visual Studio 2008
  - Algorithms: Minimax, Alpha-Beta pruning, Reinforcement Learning (Value Iteration), Random play
  - Learned: Game tree search, RL convergence to optimal policy, comparing AI paradigms
  - Related Notes: [FOL notes](https://github.com/jadnohra/TheNotes/blob/master/2018/Free%20Occurrences%20in%20FOL.pdf)
  - Applied: → [AI Sandbox](https://web.archive.org/web/20190719084901/http://aigamedev.com/): "Reinforcement Learning" contribution → Foundation for game AI roles
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI/TicTacToe)

- **2008-2009 - RecastMOD Navigation System**
  - Tech Stack: C++, OpenGL, Recast & Detour v1.4
  - Algorithms: Voxelization, Navigation mesh generation, A* pathfinding, Tiled mesh streaming
  - Learned: Spatial decomposition, automatic walkability analysis, pathfinding at scale
  - Related Notes: [Collision Detection](https://github.com/jadnohra/TheNotes/blob/master/2016/notes_coll_det.pdf)
  - Applied: → [AI Sandbox](https://web.archive.org/web/20190719084901/http://aigamedev.com/): "A*/Hierarchical A*, Terrain Clustering" → Guerrilla: "automated navigable space generation"
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI/RecastMOD)

- **2009-2010 - AI Sandbox (AiGameDev.com)**
  - Main programmer for the AI Sandbox platform
  - Tech Stack: C++
  - Work: HPA* (Hierarchical Path-Finding A*), locomotion system, reinforcement learning
  - Co-authored: "The Secrets of Parallel Pathfinding on Modern Computer Hardware" (2010)
  - Archive: [AiGameDev Articles](https://web.archive.org/web/20190719084901/http://aigamedev.com/articles/?tx_ttnews%5Bcat%5D=17&cHash=93c15cd059)
  - Note: Bought and studied the entire AI Game Programming Wisdom series (4 volumes). The series later went out of print - sold the collection for $500.

- **2008-2009 - Obstacle Avoidance**
  - {% include embed/youtube.html id='RZHIchPK1TE' %}
  - Tech Stack: C++, 2D visualization
  - Algorithms: Velocity obstacles, collision prediction, steering behaviors
  - Learned: Real-time avoidance in dynamic environments
  - Applied: → AI Sandbox → Guerrilla AI work
  - Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI)

---

## Other Projects

- **defdefqed**
  - Exploration through definitions - semantic graph crawler for mathematical resources and FOL theorem prover.
  - Tech Stack: Python
  - Algorithms: Semantic graph traversal, First-Order Logic proving
  - Video: [FOL Prover Demo](https://www.youtube.com/watch?v=766N2wSXbbc)
  - Source: [GitHub](https://github.com/jadnohra/defdefqed)

- **old_tech_experience**
  - Historical technical documentation and CV materials.
  - Source: [GitHub](https://github.com/jadnohra/old_tech_experience)

---

## Utility Projects

- **pyhotkey** - Keyboard hotkey library (Python)
- **RaySpam** - Ray tracing/raycasting experiments
- **gaussy** - Gaussian distribution tools
- **LowLevel** - CPU utilities (alignment, CPUID)
- **Math** - Specialized math algorithms

Source: [GitHub](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs)
