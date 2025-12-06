---
layout: page
title: Portfolio
icon: fas fa-folder-tree
order: 1
---

<style>
/* All links should be blue and underlined by default */
a {
  color: #2563eb;
  text-decoration: underline;
  text-underline-offset: 2px;
}
a:hover {
  color: #1d4ed8;
}
/* Category spans with colored links */
.algo, .algo a { color: #2563eb; text-decoration: underline; }
.study, .study a { color: #059669; text-decoration: underline; }
.ref, .ref a { color: #7c3aed; text-decoration: underline; }
.career, .career a { color: #dc2626; text-decoration: underline; }
strong a { text-decoration: underline; }
#portfolio-viz {
  display: flex;
  justify-content: center;
  margin: 2rem 0;
}
#portfolio-viz svg {
  background: transparent;
}
</style>

<script src="https://d3js.org/d3.v7.min.js"></script>

# Learning Journey

Technical tree showing projects, algorithms, study, and career connections.

## Knowledge Graph

Interactive visualization of connections between career, projects, algorithms, and theory. Hover over nodes to highlight relationships.

<div id="portfolio-viz"></div>

<script src="{{ '/assets/js/portfolio-viz.js' | relative_url }}"></script>

## Projects

- **2016 → 2025**: Work in progress

- **2021-2022 - Statistical Test Design for AV Safety Validation** — Fleet-level safety statistics
  - Statistical framework for proving autonomous vehicle safety at deployment scale — designing hypothesis tests for fleet-level safety validation, applying Design of Experiments (DoE) to test strategy, and quantifying confidence bounds for rare-event failure rates.
  - **Fleet-Level Safety Statistics:** [Binomial confidence intervals](https://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval) ([Wilson score method](https://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval#Wilson_score_interval)) for p≈0. Fleet encounter probability formula: p_encounter = 1 - (1 - rarity_p)^fleet_size. Safe fleet size estimation from test data with significance level α=0.05. Lower confidence bound calculations for conservative safety claims.
  - **Hypothesis Testing Framework:** Null hypothesis: True failure probability ≥ threshold. Test design: observing zero failures in N test trials. Statistical power analysis for deployment decisions. Calculated failure reduction multipliers (e.g., 1/256 reduction needed for 100-vehicle fleet at 1 failure per day tolerance).
  - **Design of Experiments (DoE) for AV Testing:** Recognized [stratified sampling](https://en.wikipedia.org/wiki/Stratified_sampling) vs [simple random sampling](https://en.wikipedia.org/wiki/Simple_random_sample) distinction for ODD coverage. Critiqued "vanilla binomial confidence intervals" as inapplicable to full ODD (cannot treat as zero-dimensional reliability test). [Factorial design](https://en.wikipedia.org/wiki/Factorial_experiment) applied to test space. [Plackett-Burman designs](https://en.wikipedia.org/wiki/Plackett%E2%80%93Burman_design) for efficient screening. Analysis of Monte Carlo limitations.
  - **Statistical Test Strategy:** Coverage dimensions as strata in stratified sampling. Equivalence classes as expert-opinion factorial design. DVP (Design Verification Plan) from DoE perspective. Integration of white-box analysis with statistical validation. Documents: "AV Testing From a Design of Experiments Perspective", "DVP Coverage from a DoE Perspective".
  - **Model Validation & Uncertainty Quantification:** [Gaussian process](https://en.wikipedia.org/wiki/Gaussian_process) model discrepancy quantification. Parameter uncertainty vs model inadequacy. Sequential calibration approaches. Bayesian [uncertainty quantification](https://en.wikipedia.org/wiki/Uncertainty_quantification) methods.
  - <span class="algo">Tech: Python, SciPy, Statsmodels. Algos: [Binomial confidence intervals](https://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval), [Wilson score](https://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval#Wilson_score_interval), [Hypothesis testing](https://en.wikipedia.org/wiki/Statistical_hypothesis_testing), [Design of Experiments](https://en.wikipedia.org/wiki/Design_of_experiments), [Stratified sampling](https://en.wikipedia.org/wiki/Stratified_sampling), [Factorial design](https://en.wikipedia.org/wiki/Factorial_experiment), [Plackett-Burman](https://en.wikipedia.org/wiki/Plackett%E2%80%93Burman_design), [Gaussian processes](https://en.wikipedia.org/wiki/Gaussian_process), [Uncertainty quantification](https://en.wikipedia.org/wiki/Uncertainty_quantification).</span>
  - Books studied: [Statistics for Experimenters](https://www.wiley.com/en-us/Statistics+for+Experimenters%3A+Design%2C+Innovation%2C+and+Discovery%2C+2nd+Edition-p-9780471718130) (Box, Hunter, Hunter), [Probabilistic Safety Assessment in the Chemical and Nuclear Industries](https://books.google.de/books/about/Probabilistic_safety_assessment_in_the_c.html?id=CAUuWcTmNrQC) (Fullwood).
  - Integration with SOTIF, UL4600, safety case documentation.
  - Patent: [US20230030474A1 - Scenario-based safety validation](https://patents.google.com/patent/US20230030474A1/en).
  - Applied at <span class="career">[#3 Argo AI](#career)</span>.

- **2021-2022 - VTT Simulation Platform** — AV simulation framework
  - Multi-layer simulation framework for autonomous vehicle testing — enabling systematic validation of detection, prediction, tracking, and motion planning through physics-based modeling and deterministic execution.
  - **Layered Simulation Architecture:** Multi-layer Environment State (ES) with Transformation Functions (TF). Layer 0 ground truth, subsequent layers apply noise/filtering. Enables isolated testing of detection, prediction, tracking, motion planning.
  - **AV Compute Model:** 8-dimension timing model — Execution Time (distribution sampling), Branching, Messaging, Triggering, Transmission Time, Queuing (FIFO/LIFO), Clock Synchronization, System Load. Time series modeling for latency. SOTIF/FMEA connection.
  - **Driver Model:** Built on [Wiedemann Model](https://en.wikipedia.org/wiki/Microscopic_traffic_flow_model) (1974), [Intelligent Driver Model](https://en.wikipedia.org/wiki/Intelligent_driver_model) (1999), Enhanced IDM. "Specialist Driver" concept for edge case testing. [Agent-based modeling](https://en.wikipedia.org/wiki/Agent-based_model) validation methodology. [TTC](https://en.wikipedia.org/wiki/Time_to_collision) metrics.
  - **Vehicle Dynamics Modeling:** [Bicycle model](https://en.wikipedia.org/wiki/Bicycle_and_motorcycle_dynamics), tire cornering stiffness, yaw inertia, road load coefficients (coast-down testing). Steering actuator time constants, rack conversion.
  - **Path Refinement Analysis:** [QP optimization](https://en.wikipedia.org/wiki/Quadratic_programming) with horizon points (trajectory discretization). Linearized dynamics constraints. [CVXGEN](https://cvxgen.com/) solver with iterative refinement. Soft/hard envelope constraints (convexification). [Savitzky-Golay filtering](https://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter) for steering analysis. [Reachability analysis](https://en.wikipedia.org/wiki/Reachability) for solver convergence.
  - **Resimulation System:** Transform Graph DAG, MTR orchestration, overlay management, deterministic discrete-event scheduling at scale.
  - <span class="algo">Tech: C++, Python. Algos: [Discrete-event simulation](https://en.wikipedia.org/wiki/Discrete-event_simulation), [MPC](https://en.wikipedia.org/wiki/Model_predictive_control), [QP](https://en.wikipedia.org/wiki/Quadratic_programming), [ABM](https://en.wikipedia.org/wiki/Agent-based_model), [IDM](https://en.wikipedia.org/wiki/Intelligent_driver_model), [Bicycle model](https://en.wikipedia.org/wiki/Bicycle_and_motorcycle_dynamics), [Time series](https://en.wikipedia.org/wiki/Time_series), [Convex optimization](https://en.wikipedia.org/wiki/Convex_optimization).</span>
  - UL4600 safety certification. SOTIF analysis. FMEA integration.
  - Applied at <span class="career">[#3 Argo AI](#career)</span>, <span class="career">[#2 FERNRIDE](#career)</span>.

- **2018-2020 - [daisy](https://github.com/jadnohra/daisy)** — Traffic simulator
  - Traffic flow simulation for autonomous vehicle testing scenarios — built during AV work to model realistic traffic patterns and edge cases.
  - <span class="algo">Tech: Python. Algos: Agent-based simulation, traffic flow modeling.</span>
  - Applied at <span class="career">[#4 Audi AID](#career)</span>.

- **2018-2020 - [PaCoS](https://github.com/jadnohra/PaCoS)** — Concurrency determinism simulator
  - Deterministic concurrency simulator for debugging distributed systems — ensuring reproducible behavior in parallel AV software stacks.
  - <span class="algo">Tech: Python. Algos: Discrete event simulation, deterministic scheduling.</span>
  - Applied at <span class="career">[#4 Audi AID](#career)</span>.

- **May 2019 - [TheNotes](https://github.com/jadnohra/TheNotes)** — Study notes 2013-2018 published to GitHub
  - Collection of mathematical and technical study notes accumulated over 5+ years, covering physics foundations, linear algebra, analysis, and formal logic — a personal reference and learning archive.
  - Covers physics, linear algebra, geometry, logic across 90+ documents.
  - Notes: <span class="study">[LinAlg Done Right](https://github.com/jadnohra/TheNotes/blob/master/2019/memo_linear-algebra-done-right.pdf), [2022 robotics transforms](https://github.com/jadnohra/TheNotes/blob/master/2022/mapping_robotics_transform_terminology_to_abstract_linear_algebra.pdf)</span>
  <details>
  <summary>Source Tree</summary>
  <pre>
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
  </pre>
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

- **~2016 - HKDS High-Fidelity Solver** at <span class="career">[#5 Havok](#career)</span>
  - R&D for high-fidelity constraint solver targeting robotics and machinery simulation — going beyond game physics to industrial precision.
  - <span class="algo">Tech: C++. Algos: [MLCP](https://en.wikipedia.org/wiki/Linear_complementarity_problem), substepping, co-simulation.</span>
  - Key documents: HKDS overview, constraint detail, substepping algorithm.
  - Notes: <span class="study">[Rotations and Basics (TheMathematicsOfHavoksSolver.pdf)](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf), [treat_rbd_1](https://github.com/jadnohra/TheNotes/blob/master/2017/treat_rbd_1.pdf)</span>
  - Prototype videos: Titan arm, excavator digger.

- **~2016 - TOI Argument** at <span class="career">[#5 Havok](#career)</span>
  - Formal proof that the Time-Of-Impact tunneling problem has no complete solution — proving an open problem unsolvable, redirecting team focus from finding a solution to managing trade-offs.
  - <span class="algo">Algos: [Continuous collision detection](https://en.wikipedia.org/wiki/Collision_detection#A_posteriori_(discrete)_versus_a_priori_(continuous)), TOI analysis.</span>
  - Effect: stopped searching for impossible solution, focused on artifact trade-off management.

- **~2015 - Network Physics R&D** at <span class="career">[#5 Havok](#career)</span>
  - Research and prototyping for networked multiplayer physics — synchronization, prediction, and rollback for distributed rigid body simulation.
  - <span class="algo">Tech: C++. Algos: Client-server physics, state synchronization, sector partitioning.</span>
  - Deliverables: Research report with proposed architectures, working prototype.

- **~2015 - [hinges_py](https://github.com/jadnohra/hinges_py)** — RBD simulator
  - Python rigid body dynamics simulator for research and prototyping — exploring constraint formulations and numerical methods outside the main codebase.
  - <span class="algo">Tech: Python. Algos: [Rigid body dynamics](https://en.wikipedia.org/wiki/Rigid_body_dynamics), constraint solving, forward Euler.</span>
  - Used for HKDS research and arxiv publication validation.

- **~2015 - [trace](https://github.com/jadnohra/trace)** — Real-time data plotting
  - Real-time data visualization tool for physics debugging — plotting solver convergence, constraint forces, and numerical residuals.
  - <span class="algo">Tech: Python. Algos: Streaming data visualization.</span>
  - Applied at <span class="career">[#5 Havok](#career)</span> for dynamics debugging.

- **2015 - [arxiv: Uniqueness of Minima](https://arxiv.org/pdf/1605.08221.pdf)** at <span class="career">[#10 Guided Knowledge](#career)</span>
  - Published mathematical paper proving uniqueness conditions for certain least squares problems — theoretical foundations for IMU sensor fusion optimization.
  - <span class="algo">Algos: [Least squares](https://en.wikipedia.org/wiki/Least_squares), optimization theory, uniqueness proofs.</span>
  - Notes: <span class="study">[Project GK notes](https://gitlab.com/jadnohra/project_gk/blob/master/simp-hing-analyt/simp-hing-analyt.pdf)</span>

- **~2014 - Physics-2014 Contact Solver** at <span class="career">[#5 Havok](#career)</span>
  - Complete rewrite of Havok's contact constraint solver — shipped in all games since ~2014, optimized for branch prediction and cache efficiency.
  - <span class="algo">Tech: C++, SIMD. Algos: [Gauss-Seidel](https://en.wikipedia.org/wiki/Gauss%E2%80%93Seidel_method), sequential impulse, constraint solving.</span>
  - Key achievements: No bugs ever reported, tunable defensiveness, found slow-accumulation bugs in other code.
  - Critical component in all Havok-powered games for a decade.

- **~2013-2015 - Geometry Numerical Issues** at <span class="career">[#5 Havok](#career)</span>
  - Deep investigation of floating-point numerical issues in collision geometry — ray-triangle, ray-sphere intersection with robust epsilon handling.
  - <span class="algo">Tech: C++. Algos: [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754), interval arithmetic, [ULP](https://en.wikipedia.org/wiki/Unit_in_the_last_place) analysis.</span>
  - Built interval math library (2x slower than normal arithmetic) for guaranteed bounds.
  - Notes: <span class="study">[Residuals](https://github.com/jadnohra/TheNotes/blob/master/2016/asymp_not.pdf)</span>
  - Led to deep interest in numerical analysis and studying.

- **~2012 - Nintendo WiiU Math Library** at <span class="career">[#5 Havok](#career)</span>
  - Low-level math library optimization for Nintendo WiiU's PowerPC-based CPU — working around buggy compiler optimizations and limited SIMD.
  - <span class="algo">Tech: C++, PowerPC assembly. Algos: [SIMD](https://en.wikipedia.org/wiki/SIMD) vectorization, instruction scheduling.</span>
  - Instruction-level ARM (handicapped two-float SIMD), workarounds for compiler bugs.

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
  <summary>Source Tree</summary>
  <pre>
├── _dcd_off.jl
├── _dcd_on.jl
├── _load_common.jl
├── _load_lp_1.jl
├── _load_lu_1.jl
├── arg.jl
├── bench/
│   └── impcol_a.mtx.gz
├── conv.jl
├── load_lp_dcd.jl
├── load_lp.jl
├── load_lu.jl
├── load.jl
├── lp_bench.jl
├── lp_glpk.jl
├── lp_jad.jl
├── lp.jl
├── lu_bench.jl
├── lu_julia.jl
├── lu_std.jl
├── lu.jl
├── mlcp.jl
├── shared_funcs.jl
└── shared_types.jl
  </pre>
  </details>

- **Jul 2015 - [asinus_salutem](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Lab2015/asinus_salutem)** — Math expression parser
  - Interactive math expression parser with symbolic computation — a CLI tool for quick mathematical exploration and verification.
  - <span class="algo">Tech: Python, SymPy. Algos: Expression parsing, AST construction.</span>
  - <span class="algo">Key files: [as.py](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Lab2015/asinus_salutem/as.py).</span>
  - Built for mathematical exploration tooling.
  <details>
  <summary>Source Tree</summary>
  <pre>
├── as.py
├── helper_arg.py
├── helper_math1.py
├── helper_term.py
└── scripts/
    ├── gk_notes_p45.txt
    ├── script1.py
    ├── test_basics.txt
    ├── test_comp.txt
    ├── test_df.txt
    ├── test_mtest.txt
    ├── test_parser.txt
    ├── test_scope.txt
    └── test_ztest.txt
  </pre>
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
  <summary>Source Tree</summary>
  <pre>
├── bitfp.cpp
├── bitfp.py
└── line_err.py
  </pre>
  </details>

- **2009-2011 - Killzone 3 AI Systems** at <span class="career">[#6 Guerrilla Games](#career)</span>
  - Environment-aware AI physical behavior for Killzone 3 — jetpack navigation, owl patrol AI, environment interaction.
  - <span class="algo">Tech: C++, PS3/SPU. Algos: Behavior trees, environment queries, physical animation.</span>
  - [Video: Jetpack and Owl AI](https://www.youtube.com/watch?v=_mXYAPUIAqg).
  - Also: SPU + PS3 multithreading/DMA tricky bug chasing.

- **2009-2011 - Killzone 3 Vehicle AI** at <span class="career">[#6 Guerrilla Games](#career)</span>
  - Vehicle pathfinding and motion planning for AI-controlled vehicles in Killzone 3.
  - <span class="algo">Tech: C++. Algos: Motion planning, vehicle dynamics, path following.</span>
  - [Video: Vehicle AI](https://www.youtube.com/watch?v=QjmRA2Obu9I).
  - Prototyping and planner development.

- **2009-2011 - Automatic Region Generation** at <span class="career">[#6 Guerrilla Games](#career)</span>
  - Automated navmesh region generation using skeletonization — improving on Recast's "ugly and cryptic" code with better corner case handling.
  - <span class="algo">Tech: C++. Algos: [Skeletonization](https://en.wikipedia.org/wiki/Topological_skeleton), [Medial axis transform](https://en.wikipedia.org/wiki/Medial_axis), region decomposition.</span>
  - Improvements on corner cases that other companies presented as challenges during GDC.

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
  <summary>Source Tree</summary>
  <pre>
└── src/
    ├── BF/
    │   ├── 3rdparty/
    │   ├── BFMath.h
    │   ├── Camera.cpp
    │   ├── Camera.h
    │   ├── ColorUtil.cpp
    │   ├── ColorUtil.h
    │   ├── DrawUtil.h
    │   ├── GridRenderer.cpp
    │   ├── GridRenderer.h
    │   ├── LoaderBVH.cpp
    │   ├── LoaderBVH.h
    │   ├── Locomo.cpp
    │   ├── Locomo.h
    │   ├── LocomoRender.h
    │   ├── Random.cpp
    │   ├── Random.h
    │   ├── Rendering.h
    │   ├── Skeleton.cpp
    │   ├── Skeleton.h
    │   ├── SkeletonAnalyzer.cpp
    │   ├── SkeletonAnalyzer.h
    │   ├── SkeletonPhysicsModel.cpp
    │   ├── SkeletonPhysicsModel.h
    │   ├── SkeletonRenderer.cpp
    │   └── SkeletonRenderer.h
    ├── Main.cpp
    ├── Planner.cpp
    ├── Retarget.cpp
    └── Viewer.cpp
  </pre>
  </details>

- **Aug 2010 - [BlenderToBVH](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/BlenderToBVH)** — Animation export tool
  - Blender addon to export armature animations to BVH format — enabling custom animation pipelines and motion capture integration.
  - <span class="algo">Tech: Python, Blender API. Algos: Armature traversal, coordinate system conversion.</span>
  - <span class="algo">Key files: [bvh_export.py](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/BlenderToBVH/bvh_export.py).</span>
  - Used in Bigfoot animation pipeline.
  <details>
  <summary>Source Tree</summary>
  <pre>
├── bvh_export.py
├── BVHFriendlyArmature.blend
├── OneBone_NoRots.blend
├── OneBone_RotX90.blend
├── OneBone_RotY90.blend
├── OneBone_RotZ90.blend
├── OneRoot_NoRots_1Child_NoRots.blend
├── OneRoot_NoRots_1Child_Rot1.blend
├── OneRoot_Rot1_1Child_Rot1.blend
├── OneRoot_Rot1_2Child_Rot1.blend
├── Proto1.blend
├── Test3Bones.blend
├── proto1_export.bvh
├── Test1.bvh ... Test10.bvh
  </pre>
  </details>

- **May 2010 - [Bigeye UI](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigeye)** — UI framework
  - Lightweight retained-mode UI framework with scene graph rendering — built for rapid prototyping of graphics and simulation tools.
  - <span class="algo">Tech: C++, OpenGL, MagickWand, Win32. Algos: Retained-mode rendering, [Scene graph](https://en.wikipedia.org/wiki/Scene_graph).</span>
  - <span class="algo">Key files: [Bigeye.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigeye/src/BE/Bigeye.cpp), [RenderNodes.h](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Bigeye/src/BE/RenderNodes.h).</span>
  - Used for rapid prototyping throughout career.
  <details>
  <summary>Source Tree</summary>
  <pre>
└── src/BE/
    ├── ARB_Multisample.cpp
    ├── ARB_Multisample.h
    ├── Assert.h
    ├── BEMath.h
    ├── Bigeye.cpp
    ├── Bigeye.h
    ├── Demo.cpp
    ├── LibInclude.h
    ├── MagickWand.cpp
    ├── MagickWand.h
    ├── MainWindow.cpp
    ├── MainWindow.h
    ├── OGL.cpp
    ├── OGL.h
    ├── Platform.h
    ├── Renderer.cpp
    ├── Renderer.h
    ├── RenderNodes.cpp
    ├── RenderNodes.h
    ├── RenderTreeBuilder.cpp
    ├── RenderTreeBuilder.h
    ├── WidgetEvent.h
    ├── Widgets.cpp
    └── Widgets.h
  </pre>
  </details>

- **Apr 2010 - [Puppeteer](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Puppeteer)** — BVH viewer
  - BVH file loader and skeletal animation viewer — foundation for character animation research and motion capture visualization.
  - <span class="algo">Tech: C++, OpenGL, GLM. Algos: [BVH](https://en.wikipedia.org/wiki/Biovision_Hierarchy) parsing, skeletal rendering.</span>
  - <span class="algo">Key files: [LoaderBVH.cpp](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Puppeteer/src/LoaderBVH.cpp), [Skeleton.h](https://github.com/jadnohra/jad-pre-2015-dabblings/blob/master/Labs/Puppeteer/src/Skeleton.h).</span>
  - Foundation for Bigfoot animation system.
  <details>
  <summary>Source Tree</summary>
  <pre>
└── src/
    ├── app.h
    ├── ARB_Multisample.cpp
    ├── ARB_Multisample.h
    ├── Camera.cpp
    ├── Camera.h
    ├── dSFMT/
    ├── freeglut/
    ├── glm/
    ├── LoaderBVH.cpp
    ├── LoaderBVH.h
    ├── main.cpp
    ├── mesh.h
    ├── NeHeGL.cpp
    ├── NeHeGL.h
    ├── Random.cpp
    ├── Random.h
    ├── Rendering.h
    ├── Skeleton.cpp
    ├── Skeleton.h
    ├── SkeletonRenderer.cpp
    ├── SkeletonRenderer.h
    ├── strtk/
    └── wavefrontObjLoader/
  </pre>
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
  <summary>Source Tree</summary>
  <pre>
├── Readme.txt
├── TicTacToe.sln
├── TicTacToe.vcproj
└── src/
    ├── Array.hpp
    ├── main.cpp
    ├── Ptr.hpp
    ├── Random.hpp
    ├── SuperHeader.hpp
    ├── TicTacToeAIPlayerAlphaBetaMinimax.cpp
    ├── TicTacToeAIPlayerAlphaBetaMinimax.hpp
    ├── TicTacToeAIPlayerConsoleInput.cpp
    ├── TicTacToeAIPlayerConsoleInput.hpp
    ├── TicTacToeAIPlayerHeuristic.cpp
    ├── TicTacToeAIPlayerHeuristic.hpp
    ├── TicTacToeAIPlayerRandom.hpp
    ├── TicTacToeAIPlayerRLValueIter.cpp
    ├── TicTacToeAIPlayerRLValueIter.h
    ├── TicTacToeGame.cpp
    └── TicTacToeGame.hpp
  </pre>
  </details>

- **Apr 2009 - [Graphics Framework](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Graphics/Framework1)** — Quake3 BSP renderer
  - BSP-based renderer loading Quake 3 maps — exploring spatial partitioning, PVS culling, and real-time rendering techniques from classic game engines.
  - <span class="algo">Tech: C++, OpenGL, Win32. Algos: [BSP](https://en.wikipedia.org/wiki/Binary_space_partitioning) traversal, [PVS](https://en.wikipedia.org/wiki/Potentially_visible_set) culling, lightmaps.</span>
  - Built on thrashit3d experience from university.
  <details>
  <summary>Source Tree</summary>
  <pre>
├── GfxFramework/
│   └── Source/
│       ├── BasicRenderer.cs
│       ├── BezierPatch.cs
│       ├── EffectContext.cs
│       ├── Renderer.cs
│       ├── RenderResourceManager.cs
│       ├── Scene.cs
│       ├── TriangleListType.cs
│       └── UniformGridTesselator.cs
├── Quake3App/
│   └── Source/
│       ├── Game1.cs
│       └── ManualCamera.cs
└── Quake3Lib/
    ├── Q3ShaderParser/
    └── Source/
  </pre>
  </details>

- **2006-2008 - [World-Of-Football](https://github.com/jadnohra/World-Of-Football)** at <span class="career">[#9 Kimera](#career)</span>
  - Complete indie game engine for a football (soccer) game — custom rendering, physics, animation, and neural network-based goalkeeper AI.
  - <span class="algo">Tech: C++, DirectX. Algos: Custom physics, [Neural networks](https://en.wikipedia.org/wiki/Artificial_neural_network) (goalkeeper AI), spatial partitioning.</span>
  - Foundation for <span class="career">[#8 Sixteen Tons](#career)</span> and <span class="career">[#7 AiGameDev](#career)</span>.
  <details>
  <summary>Source Tree</summary>
  <pre>
├── dep/
│   ├── DXUT_9_0_c/
│   ├── squirrel_2_1_1_sqplus/
│   └── tinyxml/
├── Doc/
├── Media/ (screenshots, videos)
├── WitchEngine3/src/WE3/
│   ├── audio/
│   ├── coll/
│   ├── coordSys/
│   ├── geometry/
│   ├── input/
│   ├── math/
│   ├── mesh/
│   ├── phys/
│   ├── render/
│   ├── scene/
│   ├── script/
│   ├── skeleton/
│   ├── spatialManager/
│   └── ui/
├── WOF2/src/WOF/
│   ├── match/
│   │   ├── Clock.cpp
│   │   ├── Match.cpp
│   │   ├── Team.cpp
│   │   ├── Player.cpp
│   │   ├── collision/
│   │   ├── entity/
│   │   ├── scene/
│   │   ├── script/
│   │   └── spatialQuery/
│   └── script/
└── WOF_Debugger/
  </pre>
  </details>

- **1998 - University Projects (AUB)** — Early graphics and systems programming
  - First explorations in 3D graphics and distributed computing during computer engineering degree at American University of Beirut — foundations for all subsequent graphics and systems work.
  - **[thrashit3d](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/thrashit3d)** — Software 3D renderer with full pipeline: projection, occlusion, <span class="algo">[rasterization](https://en.wikipedia.org/wiki/Rasterisation)</span> (flat/<span class="algo">[Gouraud](https://en.wikipedia.org/wiki/Gouraud_shading)</span>/wireframe), <span class="algo">[z-buffering](https://en.wikipedia.org/wiki/Z-buffering)</span>, viewport clipping
  - **[RayTracer](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/RayTracer)** — <span class="algo">[Ray tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics))</span> with <span class="algo">[BVH](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)</span> acceleration, <span class="algo">[Blinn-Phong](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model)</span> shading, hard shadows, <span class="algo">[supersampling](https://en.wikipedia.org/wiki/Supersampling)</span>, parallel rendering
  - **[xu1](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/xu1)** — 3D racing game engine with <span class="algo">[ASE](https://en.wikipedia.org/wiki/ASE_(file_format))</span> model loading, <span class="algo">[octree](https://en.wikipedia.org/wiki/Octree)</span> spatial partitioning, static geometry builder
  - **[watereffect](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/watereffect)** — Java <span class="algo">[height map](https://en.wikipedia.org/wiki/Heightmap)</span> wave simulation with circular/spherical wave propagation, real-time image distortion
  - **[2dshooter](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/2dshooter)** — Java game engine (fivemagics) with 2D <span class="algo">[collision detection](https://en.wikipedia.org/wiki/Collision_detection)</span>, physics engine, GUI system, resource loading, XML scene description
  - **[GraphEdit](https://github.com/jadnohra/jadnohra.com/tree/main/repos/ancient-1998/graphedit)** — 3D equation grapher with <span class="algo">[symbolic expression](https://en.wikipedia.org/wiki/Computer_algebra)</span> evaluation and distributed rendering
  - Foundation for <span class="career">[#9 Kimera](#career)</span> and all subsequent graphics work.
  <details>
  <summary>Source Tree</summary>
  <pre>
├── 2dshooter/fivemagics/ (Java game engine)
│   ├── applet/GenericApplet.class
│   ├── collision/
│   │   ├── ICollisionHandler.class
│   │   └── d2/{circle,rect}/CollisionArea.class, CollisionEngine.class
│   ├── dataformats/AwtImage.class, XMLFile.class, XMLTag.class
│   ├── engine/
│   │   ├── IEngine.class
│   │   ├── d2/physics/PhEngine.class, PhObject.class
│   │   └── gui/GUIEngine.class, WMenu.class, WTextConsole.class
│   ├── evt/Event.class, TimedEventManager.class
│   ├── geo/d2/D2FloatPoint.class, D2Line.class
│   ├── gfx/
│   │   ├── PixelData.class, IBlittable.class, IGfxImage.class
│   │   ├── d2/GfxEngine.class, ImageAnimation.class
│   │   └── filter/hmap/HeightMap.class, CircularWave.class
│   ├── res/ResEngine.class, ResLoaderThread.class
│   └── projects/thrashem/Applet.class, PlayerShip.class, EnemyShip.class
├── graphedit/ (screenshots, index.html)
├── RayTracer/
│   ├── data/bunny.obj, cornell_box.obj/.mtl
│   ├── screenshots/ (render results)
│   └── src/
│       ├── main.cpp
│       ├── 3rdparty/
│       │   ├── awilliam/
│       │   ├── LofstedtAkenineMoller05/
│       │   ├── PixelToaster1.4/
│       │   ├── plyLoader/
│       │   └── wavefrontObjLoader/
│       ├── rayTracer/
│       │   ├── BoundingVolumeHierarchy.cpp/.hpp
│       │   ├── Camera.cpp/.hpp
│       │   ├── Material.cpp/.hpp
│       │   ├── Math.cpp/.hpp
│       │   ├── ParallelProcessor_Simple.cpp/.hpp
│       │   ├── PostProcess.cpp/.hpp, GaussianBlur.cpp/.hpp
│       │   ├── Primitive.cpp/.hpp
│       │   ├── Primitive_{Sphere,Plane,Triangle,Quad,Cone}.hpp
│       │   ├── Ray.hpp, RayTracer.hpp
│       │   ├── Renderer.cpp/.hpp
│       │   ├── RenderImage.cpp/.hpp, RenderTarget.cpp/.hpp
│       │   ├── Scene.cpp/.hpp
│       │   └── SpatialAccelerator.cpp/.hpp
│       └── WE3/ (engine utilities)
├── thrashit3d/
│   ├── engine/
│   │   ├── ThrashIt3D.cpp, ThrashIt3D.h
│   │   ├── TIStageOcclusion.cpp/.h
│   │   ├── TIStageProjection.cpp/.h
│   │   ├── TIStageRasterization.cpp/.h
│   │   ├── TIStageRasterizationFlat.cpp/.h
│   │   ├── TIStageRasterizationGouraud.cpp/.h
│   │   ├── TIStageRasterizationWireframe.cpp/.h
│   │   ├── TIStageTransformation.cpp/.h
│   │   ├── TIViewport.cpp/.h
│   │   └── TIZBuffer.cpp/.h
│   └── geom/
│       ├── TIAngle.cpp/.h
│       ├── TIBasicVertex.cpp/.h
│       ├── TIGeometry.cpp/.h
│       ├── TITransform.cpp/.h
│       ├── TITriangle.cpp/.h
│       └── TIVertex.cpp/.h
├── watereffect/ (23 Java classes)
│   ├── Applet1.class, GenericApplet.class
│   ├── CircularWave.class, CircularWave2.class, SphereWave.class
│   ├── HeightMapImage.class, IHeightMapGenerator.class
│   ├── IScene.class, Scene1.class
│   └── Drop1Event.class, TimedDrop1Event.class
└── xu1/src/ThrashIt/
    ├── FileFormats/
    │   ├── ASE/ASEFile.cpp/.h, ASEGeomObject.cpp/.h
    │   └── TIS/TISFileStructs.cpp/.h
    ├── General/Collection/
    │   ├── CArray.cpp/.h, CHashTable.cpp/.h
    │   ├── Tree.cpp/.h, FixedTree.cpp/.h
    │   └── String.cpp/.h, Fraction.cpp/.h
    └── MabenEngine/TIStaticGeometry/
        ├── PreBuild/TIStaticBuilder.cpp
        └── RealTime/TIStaticGeometryRealTime.cpp/.h
  </pre>
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
