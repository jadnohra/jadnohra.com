---
layout: page
title: Portfolio
icon: fas fa-folder-tree
order: 5
---

<style>
.algo { color: #2563eb; }
.algo a { color: #2563eb; text-decoration: underline; }
.study { color: #059669; }
.study a { color: #059669; text-decoration: underline; }
.ref { color: #7c3aed; }
.ref a { color: #7c3aed; text-decoration: underline; }
.career { color: #dc2626; }
.career a { color: #dc2626; text-decoration: underline; }
strong a { text-decoration: underline; }
</style>

# Learning Journey

Technical tree showing projects, algorithms, study, and career connections.

## Projects

- **May 2019 - [TheNotes](https://github.com/jadnohra/TheNotes)** — Study notes 2013-2018 published to GitHub
  - Collection of mathematical and technical study notes accumulated over 5+ years, covering physics foundations, linear algebra, analysis, and formal logic — a personal reference and learning archive.
  - Covers physics, linear algebra, geometry, logic across 90+ documents.
  - Notes: <span class="study">[LinAlg Done Right](https://github.com/jadnohra/TheNotes/blob/master/2019/memo_linear-algebra-done-right.pdf), [2022 robotics transforms](https://github.com/jadnohra/TheNotes/blob/master/2022/mapping_robotics_transform_terminology_to_abstract_linear_algebra.pdf)</span>
  <details>
  <summary>Tree</summary>

  ```
  ├── 2011-2012
  │   ├── attribution.pdf
  │   ├── quotes1-0-100.pdf
  │   └── theplan.pdf
  ├── 2013-2014
  │   ├── Notes on the GJK Algorithm..pdf
  │   ├── Notes on the Linear Complementarity Problem.pdf
  │   ├── Notes on Euler's Writings.pdf
  │   └── ... (30 files)
  ├── 2015
  │   ├── Notes on Potter's Set Theory and its Philosophy.pdf
  │   ├── rotation.pdf
  │   ├── lagrfrmw.pdf
  │   └── ... (26 files)
  ├── 2016
  │   ├── dform.pdf
  │   ├── notes_coll_det.pdf
  │   ├── ml_survey.pdf
  │   └── ... (19 files)
  ├── 2017
  │   ├── treat_rbd_1.pdf
  │   ├── plql.pdf
  │   └── ... (11 files)
  ├── 2018
  │   ├── Free Occurrences in FOL.pdf
  │   └── thought_base.pdf
  ├── 2019
  │   └── memo_linear-algebra-done-right.pdf
  └── 2022
      └── mapping_robotics_transform_terminology_to_abstract_linear_algebra.pdf
  ```
  </details>

- **Sep 2017 - [tag_export](https://github.com/jadnohra/tag_export)** — Digital library index
  - A curated, tagged bibliography of technical books and papers — built to quickly locate authoritative references during study sessions and project research.
  - Curated references for physics, math, CS — used for quick lookup during study.
  - <span class="ref">[tag_export](https://github.com/jadnohra/tag_export)</span>

- **Apr 2017 - [defdefqed](https://github.com/jadnohra/defdefqed)** — FOL prover, semantic graph crawler
  - A first-order logic theorem prover and semantic graph crawler — built to formalize mathematical exploration and automatically extract relationships from mathematical texts.
  - <span class="algo">Tech: Python. Algos: [First-Order Logic](https://en.wikipedia.org/wiki/First-order_logic) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2018/Free%20Occurrences%20in%20FOL.pdf)), resolution, unification.</span>
  - Notes: <span class="study">[FOL](https://github.com/jadnohra/TheNotes/blob/master/2018/Free%20Occurrences%20in%20FOL.pdf), [thought_base](https://github.com/jadnohra/TheNotes/blob/master/2018/thought_base.pdf), [Barwise Liar](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Barwise's%20%27The%20Liar%27.pdf), [Zakon](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Elias%20Zakon's%20Basic%20Concepts%20of%20Mathematics.pdf)</span>
  - Motivated by desire to formalize mathematical exploration. [Video demo](https://www.youtube.com/watch?v=766N2wSXbbc).

- **2017 - Study: Programming Logic**
  - Deep dive into the theoretical foundations of programming languages — type theory, lambda calculus, and their connections to logic.
  - <span class="algo">Algos: [Lambda calculus](https://en.wikipedia.org/wiki/Lambda_calculus), [Type theory](https://en.wikipedia.org/wiki/Type_theory).</span>
  - Notes: <span class="study">[plql](https://github.com/jadnohra/TheNotes/blob/master/2017/plql.pdf), [plql_short](https://github.com/jadnohra/TheNotes/blob/master/2016/plql_short.pdf), [plql_pl_flavors](https://github.com/jadnohra/TheNotes/blob/master/2016/plql_pl_flavors.pdf), [compute](https://github.com/jadnohra/TheNotes/blob/master/2017/compute.pdf)</span>
  - <span class="ref">[tag_export](https://github.com/jadnohra/tag_export): Type Theory, Lambda Calculus</span>

- **2017 - Study: Rigid Body Dynamics**
  - Formal treatment of rigid body dynamics after years of hands-on work at Havok — connecting practical simulation experience to mathematical foundations in mechanics and constraint solving.
  - <span class="algo">Algos: [Rigid body dynamics](https://en.wikipedia.org/wiki/Rigid_body_dynamics) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf)), [Lagrangian mechanics](https://en.wikipedia.org/wiki/Lagrangian_mechanics) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2015/lagrfrmw.pdf)).</span>
  - Notes: <span class="study">[treat_rbd_1](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf), [treat_rn](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rn.pdf), [solve](https://github.com/jadnohra/TheNotes/blob/master/2017/solve.pdf), [ineq](https://github.com/jadnohra/TheNotes/blob/master/2017/ineq.pdf), [speed_proof](https://github.com/jadnohra/TheNotes/blob/master/2017/speed_proof.pdf)</span>
  - <span class="ref">[tag_export](https://github.com/jadnohra/tag_export): Mechanics, RBD (★★ Penalty Method, Multibody, Geometric Mechanics)</span>
  - Formalized understanding after years at <span class="career">[#5 Havok](#career)</span>.

- **2016 - Study: Analysis & Geometry**
  - Rigorous study of differential geometry and real analysis — the mathematical framework underlying physics simulation, manifolds, and continuous optimization.
  - <span class="algo">Algos: [Differential forms](https://en.wikipedia.org/wiki/Differential_form), [Manifolds](https://en.wikipedia.org/wiki/Manifold), [Measure theory](https://en.wikipedia.org/wiki/Measure_(mathematics)).</span>
  - Notes: <span class="study">[Differential Forms](https://github.com/jadnohra/TheNotes/blob/master/2016/dform.pdf), [road_dform](https://github.com/jadnohra/TheNotes/blob/master/2017/road_dform.pdf), [Manifolds](https://github.com/jadnohra/TheNotes/blob/master/2016/mfold_short.pdf), [Lagrange proof](https://github.com/jadnohra/TheNotes/blob/master/2016/proof_lagrmul.pdf), [Analysis](https://github.com/jadnohra/TheNotes/blob/master/2016/treat_analy_1.pdf), [MVT/IVT/Taylor](https://github.com/jadnohra/TheNotes/blob/master/2016/hand_mvt_ivt_taylor_onot.pdf), [Measure](https://github.com/jadnohra/TheNotes/blob/master/2016/meas_short.pdf), [Continuity](https://github.com/jadnohra/TheNotes/blob/master/2016/cont_short.pdf), [pointillistic](https://github.com/jadnohra/TheNotes/blob/master/2016/pointillistic.pdf)</span>
  - <span class="ref">[tag_export](https://github.com/jadnohra/tag_export): DiffGeom, Calculus of Variations, Real Analysis, NSA</span>
  - Applied at <span class="career">[#5 Havok](#career)</span> for solver formalization.

- **2015 - Study: Foundations & Logic**
  - Study of mathematical foundations — set theory, logic, and the philosophical underpinnings of mathematics itself.
  - <span class="algo">Algos: [Set theory](https://en.wikipedia.org/wiki/Set_theory), [Model theory](https://en.wikipedia.org/wiki/Model_theory), [Proof theory](https://en.wikipedia.org/wiki/Proof_theory).</span>
  - Notes: <span class="study">[Potter Set Theory](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Potter's%20Set%20Theory%20and%20its%20Philosophy.pdf), [Potter Reason's Kin](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Potter's%20%27Reason's%20Nearest%20Kin%27.pdf), [Gödel 1](https://github.com/jadnohra/TheNotes/blob/master/2015/Study%20of%20%27There's%20Something%20about%20Goedel%27.pdf), [Gödel 2](https://github.com/jadnohra/TheNotes/blob/master/2015/Study%20of%20%27There's%20Something%20about%20Goedel%27%202.pdf), [Semantics](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Semantics.pdf), [Hestenes GA](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Hestenes%27%20%27Reforming%20the%20Mathematical%20Language%20of%20Physics%27.pdf)</span>
  - <span class="ref">[tag_export](https://github.com/jadnohra/tag_export): Set Theory, Model Theory, Proof Theory</span>

- **Sep 2015 - [MLCP 2.0](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Lab2015/mlcp2.0)** — LP solver in Julia
  - Implementation of linear programming and LU decomposition — exploring numerical methods for constraint solving that underpin physics engines.
  - <span class="algo">Tech: Julia, GLPK. Algos: [Linear programming](https://en.wikipedia.org/wiki/Linear_programming) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20Linear%20Complementarity%20Problem.pdf)), [LU decomposition](https://en.wikipedia.org/wiki/LU_decomposition).</span>
  - <span class="algo">Key files: [lp_glpk.jl](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Lab2015/mlcp2.0/lp_glpk.jl), [lu.jl](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Lab2015/mlcp2.0/lu.jl).</span>
  - Notes: <span class="study">[Eigenvalues](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Parlett's%20The%20Symmetric%20Eigenvalue%20Problem.pdf), [eigenvalues2](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/notes%20on%20eigenvalues.pdf)</span>
  - Built on lamby research; applied at <span class="career">[#5 Havok](#career)</span> and <span class="career">[#10 Guided Knowledge](#career)</span>.
  <details>
  <summary>Tree</summary>

  ```
  ├── lp_glpk.jl
  ├── lp_jad.jl
  ├── lp.jl
  ├── lu_julia.jl
  ├── lu_std.jl
  ├── lu.jl
  ├── mlcp.jl
  ├── shared_funcs.jl
  └── shared_types.jl
  ```
  </details>

- **Jul 2015 - [asinus_salutem](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Lab2015/asinus_salutem)** — Math expression parser
  - Interactive math expression parser with symbolic computation — a CLI tool for quick mathematical exploration and verification.
  - <span class="algo">Tech: Python, SymPy. Algos: Expression parsing, AST construction.</span>
  - <span class="algo">Key files: [as.py](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Lab2015/asinus_salutem/as.py).</span>
  - Built for mathematical exploration tooling.
  <details>
  <summary>Tree</summary>

  ```
  ├── as.py
  ├── helper_arg.py
  ├── helper_math1.py
  ├── helper_term.py
  └── scripts/
      ├── script1.py
      └── test_*.txt
  ```
  </details>

- **2013-2014 - Deep Mathematical Study** at <span class="career">[#5 Havok](#career)</span>
  - Intensive self-study period covering mathematical physics, linear algebra, and numerical methods — driven by the need to deeply understand the algorithms powering Havok's physics engine.
  - <span class="algo">Algos: [LCP](https://en.wikipedia.org/wiki/Linear_complementarity_problem) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20Linear%20Complementarity%20Problem.pdf)), [GJK](https://en.wikipedia.org/wiki/Gilbert%E2%80%93Johnson%E2%80%93Keerthi_distance_algorithm) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20GJK%20Algorithm..pdf)), [Eigenvalue decomposition](https://en.wikipedia.org/wiki/Eigendecomposition_of_a_matrix).</span>
  - Notes: Physics: <span class="study">[LCP](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20Linear%20Complementarity%20Problem.pdf), [LCP supp](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Supplementary%20Notes%20on%20the%20Linear%20Complemetarity%20Problem.pdf), [GJK](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20the%20GJK%20Algorithm..pdf), [Jammer's Force](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Jammer's%20%27Force%27.pdf), [Stevin](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Note%20on%20Stevin's%20inclined%20plane%20equilibrium.pdf), [Whittaker Aether](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20%27Theories%20of%20the%20Aether%20and%20Electricity%2C%20Whittaker%27.pdf)</span>
  - Notes: Linear Algebra: <span class="study">[Eigenvalues](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Parlett's%20The%20Symmetric%20Eigenvalue%20Problem.pdf), [Hefferon LinAlg](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Hefferon's%20%27Linear%20Algebra%27.pdf)</span>
  - Notes: Foundations: <span class="study">[Euler](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Euler's%20Writings.pdf), [Euclid](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Euclid's%20Elements.pdf), [Morris Kline](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Morris%20Kline's%20%27Mathematical%20Thought%27.pdf), [Number Theory](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Notes%20on%20Number%20Theory.pdf), [Infinity](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/On%20the%20Necessity%20of%20Infinity.pdf), [Convergence](https://github.com/jadnohra/TheNotes/blob/master/2013-2014%20/Convergence%20proofs%20in%20the%20literature.pdf)</span>
  - <span class="algo">Key files: [lamby](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Physics/lamby), [gjk0.py](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Physics/fc/gjk/gjk0.py)</span>
  - Applied linear programming in Havok physics solvers.

- **May 2011 - [Floating Point Analysis](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/fp)** — IEEE 754 deep dive
  - Low-level exploration of IEEE 754 floating-point representation — bit manipulation, epsilon calculation, and denormal handling essential for numerical stability in physics code.
  - <span class="algo">Tech: Python, C++. Algos: [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) bit manipulation, epsilon calculation, denormals.</span>
  - <span class="algo">Key files: [bitfp.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/fp/bitfp.cpp), [bitfp.py](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/fp/bitfp.py).</span>
  - Notes: <span class="study">[asymp_not](https://github.com/jadnohra/TheNotes/blob/master/2016/asymp_not.pdf)</span>
  - Applied at <span class="career">[#5 Havok](#career)</span> for low-level numerical optimization.
  <details>
  <summary>Tree</summary>

  ```
  ├── bitfp.cpp
  ├── bitfp.py
  └── line_err.py
  ```
  </details>

- **Nov 2010 - [RecastMOD](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI/RecastMOD)** — Navigation mesh system
  - Modified version of Recast & Detour — the industry-standard navmesh library used in AAA games for AI pathfinding through voxelization and mesh generation.
  - <span class="algo">Tech: C++, OpenGL, Recast & Detour. Algos: [Voxelization](https://en.wikipedia.org/wiki/Voxel), [Navigation mesh](https://en.wikipedia.org/wiki/Navigation_mesh), [A*](https://en.wikipedia.org/wiki/A*_search_algorithm).</span>
  - <span class="algo">Key files: [Recast.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/AI/RecastMOD/Recast/Source/Recast.cpp), [RecastRasterization.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/AI/RecastMOD/Recast/Source/RecastRasterization.cpp), [DetourStatNavMesh.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/AI/RecastMOD/Detour/Source/DetourStatNavMesh.cpp).</span>
  - Notes: <span class="study">[Collision Detection](https://github.com/jadnohra/TheNotes/blob/master/2016/notes_coll_det.pdf), [algcrumb](https://github.com/jadnohra/TheNotes/blob/master/2016/algcrumb.pdf)</span>
  - Built on AI Sandbox pathfinding; applied at <span class="career">[#6 Guerrilla](#career)</span> for automated navigable space.

- **Aug 2010 - [Bigfoot Animation](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigfoot)** — Skeletal animation with physics
  - Skeleton retargeting and motion planning system — exploring character animation pipelines, physics-based locomotion, and motion capture processing.
  - <span class="algo">Tech: C++, OpenGL, GLM, Bigeye UI. Algos: [BVH](https://en.wikipedia.org/wiki/Biovision_Hierarchy), [Skeletal animation](https://en.wikipedia.org/wiki/Skeletal_animation), [Motion retargeting](https://en.wikipedia.org/wiki/Motion_capture#Retargeting), [Quaternion](https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation) ([notes](https://github.com/jadnohra/TheNotes/blob/master/2015/rotation.pdf)).</span>
  - <span class="algo">Key files: [LoaderBVH.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigfoot/src/BF/LoaderBVH.cpp), [Retarget.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigfoot/src/Retarget.cpp), [Locomo.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigfoot/src/BF/Locomo.cpp), [Planner.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigfoot/src/Planner.cpp).</span>
  - Notes: <span class="study">[RBD](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf), [Lagrangian](https://github.com/jadnohra/TheNotes/blob/master/2015/lagrfrmw.pdf), [Rotation](https://github.com/jadnohra/TheNotes/blob/master/2015/rotation.pdf), [Covariance](https://github.com/jadnohra/TheNotes/blob/master/2015/covar.pdf), [Hestenes GA](https://github.com/jadnohra/TheNotes/blob/master/2015/Notes%20on%20Hestenes%27%20%27Reforming%20the%20Mathematical%20Language%20of%20Physics%27.pdf)</span>
  - Built on Puppeteer/Bigeye; applied at <span class="career">[#7](#career)→[#6](#career)→[#5](#career)</span>. [Video](https://www.youtube.com/watch?v=XMj0xhCzx4U).
  <details>
  <summary>Tree</summary>

  ```
  ├── media/
  │   └── *.bvh
  └── src/
      ├── BF/
      ├── Main.cpp
      ├── Planner.cpp
      ├── Retarget.cpp
      └── Viewer.cpp
  ```
  </details>

- **Aug 2010 - [BlenderToBVH](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/BlenderToBVH)** — Animation export tool
  - Blender addon to export armature animations to BVH format — enabling custom animation pipelines and motion capture integration.
  - <span class="algo">Tech: Python, Blender API. Algos: Armature traversal, coordinate system conversion.</span>
  - <span class="algo">Key files: [bvh_export.py](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/BlenderToBVH/bvh_export.py).</span>
  - Used in Bigfoot animation pipeline.
  <details>
  <summary>Tree</summary>

  ```
  ├── bvh_export.py
  ├── *.blend (test armatures)
  └── *.bvh (test exports)
  ```
  </details>

- **May 2010 - [Bigeye UI](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigeye)** — UI framework
  - Lightweight retained-mode UI framework with scene graph rendering — built for rapid prototyping of graphics and simulation tools.
  - <span class="algo">Tech: C++, OpenGL, MagickWand, Win32. Algos: Retained-mode rendering, [Scene graph](https://en.wikipedia.org/wiki/Scene_graph).</span>
  - <span class="algo">Key files: [Bigeye.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigeye/src/BE/Bigeye.cpp), [RenderNodes.h](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigeye/src/BE/RenderNodes.h).</span>
  - Used for rapid prototyping throughout career.
  <details>
  <summary>Tree</summary>

  ```
  ├── lib/
  │   └── media/
  └── src/
      └── BE/
  ```
  </details>

- **Apr 2010 - [Puppeteer](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Puppeteer)** — BVH viewer
  - BVH file loader and skeletal animation viewer — foundation for character animation research and motion capture visualization.
  - <span class="algo">Tech: C++, OpenGL, GLM. Algos: [BVH](https://en.wikipedia.org/wiki/Biovision_Hierarchy) parsing, skeletal rendering.</span>
  - <span class="algo">Key files: [LoaderBVH.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Puppeteer/src/LoaderBVH.cpp), [Skeleton.h](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Puppeteer/src/Skeleton.h).</span>
  - Foundation for Bigfoot animation system.
  <details>
  <summary>Tree</summary>

  ```
  └── src/
      ├── LoaderBVH.cpp
      ├── LoaderBVH.h
      ├── Skeleton.cpp
      ├── Skeleton.h
      ├── SkeletonRenderer.cpp
      ├── Camera.cpp
      └── main.cpp
  ```
  </details>

- **2008-2010 - [AI Sandbox](https://web.archive.org/web/20190719084901/http://aigamedev.com/articles/?tx_ttnews%5Bcat%5D=17&cHash=93c15cd059)** at <span class="career">[#7 AiGameDev](#career)</span>
  - Open-source game AI development framework — main programmer for a platform used by the AI game development community for research and education.
  - <span class="algo">Tech: C++. Algos: [HPA*](https://en.wikipedia.org/wiki/Hierarchical_pathfinding), locomotion system, [Reinforcement learning](https://en.wikipedia.org/wiki/Reinforcement_learning).</span>
  - Notes: <span class="study">AI Game Programming Wisdom (4 vols, sold $500), [ml_survey](https://github.com/jadnohra/TheNotes/blob/master/2016/ml_survey.pdf)</span>
  - Main programmer; co-authored "Parallel Pathfinding on Modern Hardware". Applied at <span class="career">[#6](#career), [#5](#career)</span>.

- **Apr 2009 - [Obstacle Avoidance](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI)** — Steering behaviors
  - Implementation of velocity obstacles and steering behaviors — local collision avoidance for crowds and NPC navigation.
  - <span class="algo">Tech: C++, 2D visualization. Algos: [Velocity obstacles](https://en.wikipedia.org/wiki/Velocity_obstacle), collision prediction, steering.</span>
  - Applied at <span class="career">[#7](#career), [#6](#career)</span>. [Video](https://www.youtube.com/watch?v=RZHIchPK1TE).

- **Apr 2009 - [TicTacToe AI](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI/TicTacToe)** — AI algorithms testbed
  - Classic game AI algorithms implemented as a learning exercise — minimax, alpha-beta pruning, and reinforcement learning on a simple domain.
  - <span class="algo">Tech: C++, Visual Studio. Algos: [Minimax](https://en.wikipedia.org/wiki/Minimax), [Alpha-Beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning), [Value iteration](https://en.wikipedia.org/wiki/Markov_decision_process#Value_iteration) (RL).</span>
  - Notes: <span class="study">[FOL](https://github.com/jadnohra/TheNotes/blob/master/2018/Free%20Occurrences%20in%20FOL.pdf), [plql](https://github.com/jadnohra/TheNotes/blob/master/2017/plql.pdf)</span>
  - RL contribution to <span class="career">[#7 AI Sandbox](#career)</span>.
  <details>
  <summary>Tree</summary>

  ```
  └── src/
      ├── main.cpp
      ├── TicTacToeGame.cpp
      ├── TicTacToeAIPlayerAlphaBetaMinimax.cpp
      ├── TicTacToeAIPlayerRLValueIter.cpp
      └── TicTacToeAIPlayerHeuristic.cpp
  ```
  </details>

- **Apr 2009 - [Graphics Framework](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Graphics/Framework1)** — Quake3 BSP renderer
  - BSP-based renderer loading Quake 3 maps — exploring spatial partitioning, PVS culling, and real-time rendering techniques from classic game engines.
  - <span class="algo">Tech: C++, OpenGL, Win32. Algos: [BSP](https://en.wikipedia.org/wiki/Binary_space_partitioning) traversal, [PVS](https://en.wikipedia.org/wiki/Potentially_visible_set) culling, lightmaps.</span>
  - Built on thrashit3d experience from university.
  <details>
  <summary>Tree</summary>

  ```
  ├── GfxFramework/
  │   └── Source/
  ├── Quake3App/
  │   └── Source/
  └── Quake3Lib/
      ├── Q3ShaderParser/
      └── Source/
  ```
  </details>

- **2006-2008 - [World-Of-Football](https://github.com/jadnohra/World-Of-Football)** at <span class="career">[#9 Kimera](#career)</span>
  - Complete indie game engine for a football (soccer) game — custom rendering, physics, animation, and neural network-based goalkeeper AI.
  - <span class="algo">Tech: C++, DirectX. Algos: Custom physics, [Neural networks](https://en.wikipedia.org/wiki/Artificial_neural_network) (goalkeeper AI), spatial partitioning.</span>
  - Foundation for <span class="career">[#8 Sixteen Tons](#career)</span> and <span class="career">[#7 AiGameDev](#career)</span>.
  <details>
  <summary>Tree</summary>

  ```
  ├── dep/
  │   ├── DXUT_9_0_c/
  │   ├── squirrel_2_1_1_sqplus/
  │   └── tinyxml/
  ├── Doc/
  ├── Media/
  │   └── (screenshots, videos)
  ├── WitchEngine3/
  │   └── src/
  ├── WOF2/
  │   ├── moduleResources/
  │   └── src/
  └── WOF_Debugger/
      └── src/
  ```
  </details>

- **1998 - University Projects (AUB)** — Early graphics and systems programming
  - First explorations in 3D graphics and distributed computing during computer engineering degree at American University of Beirut — foundations for all subsequent graphics and systems work.
  - <span class="algo">Tech: C++, Win32, software rendering, distributed computing.</span>
  - **[GraphEdit](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/graphedit)** — 3D equation grapher with distributed computing for rendering mathematical surfaces across networked machines
  - **[thrashit3d](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/thrashit3d)** — Software 3D renderer implementing rasterization, z-buffering, and texture mapping from scratch
  - **[xu1](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/xu1)** — 3D racing game with collision detection and track rendering
  - **[watereffect](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/watereffect)** — 2D water ripple effect simulation using wave propagation
  - **[2dshooter](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/2dshooter)** — 2D arcade shooter game with sprite rendering
  - **[RayTracer](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/RayTracer)** — Classic [ray tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)) implementation with reflections and shadows
  - Foundation for <span class="career">[#9 Kimera](#career)</span> and all subsequent graphics work.
  <details>
  <summary>Tree</summary>

  ```
  ├── 2dshooter/
  ├── graphedit/
  ├── RayTracer/
  ├── thrashit3d/
  ├── watereffect/
  └── xu1/
  ```
  </details>

---

## Career

1. **Irreducible** (2024-) Engineering Director, ZK/hardware
2. **FERNRIDE** (2023-24) Principal Engineer, autonomous trucks
3. **Argo AI** (2021-22) Engineering Manager, AV simulation
4. **Audi AID** (2018-20) Simulation Tech Lead
5. **Havok/Microsoft** (2011-18) Lead Physics R&D
6. **Guerrilla Games** (2009-11) Senior AI, Killzone 3
7. **AiGameDev** (2008-10) Main programmer, AI Sandbox
8. **Sixteen Tons** (2008) Nintendo DS engine
9. **Kimera Studios** (2006-08) Indie engine
10. **Guided Knowledge** (2015) IMU optimization
