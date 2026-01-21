# Proposal: Add "Context:" paragraphs from README

Keep portfolio as-is, add one bullet point per project with verbatim README content.

---

## ~2012 - Nintendo WiiU Math Library

**Add:**
- Context: Instruction-level ARM (handicapped two-float SIMD). Buggy compiler in terms of performance optimization.

---

## ~2014 - Physics-2014 Contact Solver

**Add:**
- Context: Mental models of CPUs at race with CPU improvements — benchmarking ignoring real data becomes dangerous. Research ([Agner's manuals](https://www.agner.org/optimize/#manuals)) → branch prediction as main actor in this context. Proof using counter example. New (intrusive) optimization: coupling geometry processing to solver using data rearrangement. For comparison: [Bullet solver](https://github.com/bulletphysics/bullet3/blob/cdd56e46411527772711da5357c856a90ad9ea67/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.cpp). Havok's focus: performance. No bugs ever reported. Defensive code (tunable defensiveness — math library only has INFTY and NAN warnings). Found bugs in other pieces of code, including hard to find bugs due to slow accumulation of wrong calculation. Tunable per-platform using a very carefully configured set of macros (>10 options → code path combinations). Also: deterministic scaling of cores (Intel many-core CPU R&D).

---

## ~2015 - Network Physics R&D

**Add:**
- Context: [Research report](https://github.com/jadnohra/projects-pre-2015/blob/master/netw/csarch1.pdf) and proposed designs. Prototype including low-level framework.

---

## ~2013-2015 - Geometry Numerical Issues

**Add:**
- Context: [Jira epsilon analysis](https://github.com/jadnohra/projects-pre-2015/blob/master/geom_num/JiraEpsilon.pdf). Ray-sphere: improvement by intuitive solution. Ray-triangle: improvement by analytical solution (books like *Floating-Point Computation* by Strebenz). Telescoping debugger. Led to interest about numerical analysis. [Sampler prototype (RRT)](https://github.com/jadnohra/projects-pre-2015/tree/master/Labs/LabsMore/Numerical/nics). Interval math library with pessimistic approximate but fast bounds (2x slower than normal arithmetic). Also first steps into numerical analysis for dynamics: [Residuals](https://github.com/jadnohra/projects-pre-2015/blob/master/hvk_other/respart1.pdf).

---

## ~2016 - TOI Argument

**Add:**
- Context: Open problem ever since the 'TOI' engine was abandoned. Multiple attempts that replaced artifacts with other artifacts. Proved that the problem is not solvable: [TOI argument](https://github.com/jadnohra/projects-pre-2015/blob/master/hvk_other/toiarg1.pdf). Effect: stop searching for the solution, focus on which artifacts to trade and when.

---

## ~2016 - HKDS High-Fidelity Solver

**Add:**
- Context: [HKDS overview](https://github.com/jadnohra/projects-pre-2015/blob/master/hkds/ds_overview1.pdf). [More detail](https://github.com/jadnohra/projects-pre-2015/blob/master/hkds/ds_detail.pdf). Videos: [Titan arm](https://github.com/jadnohra/projects-pre-2015/blob/master/hkds/HifiSolverVid/arm.mp4), [Digger](https://github.com/jadnohra/projects-pre-2015/blob/master/hkds/HifiSolverVid/digger.mp4). Prerequisites: [Rotations and Basics](https://github.com/jadnohra/projects-pre-2015/blob/master/hvk_other/TheMathematicsOfHavoksSolver.pdf), work on stabilizing constraints, notebook notes on 'universal constraint', [substepping algorithm](https://github.com/jadnohra/projects-pre-2015/blob/master/hvk_other/substep1.pdf) (a stumbling block for others at multiple occasions). Prototyping using Julia: [MLCP solver](https://github.com/jadnohra/projects-pre-2015/blob/master/Labs/Lab2015/mlcp2.0/lp_jad.jl). Dynamics debugging tool: [trace](https://github.com/jadnohra/trace).

---

## 2009-2011 - Automatic Region Generation

**Add:**
- Context: Porting of Recast 'ugly and cryptic' code. Improvements on corner cases that other companies presented as challenges during GDC. Research skeletonization algorithms. Choose and implement. Modify to fit application (stray edges).

---

## 2009-2011 - Killzone 3 Vehicle AI

**Add:**
- Context: Prototyping ([VehicleTest](https://github.com/jadnohra/projects-pre-2015/tree/master/Labs/LabsMore/BlackRiceLabs/VehicleTest)).

---

## 2009-2011 - Killzone 3 AI Systems

**Add:**
- Context: SPU + PS3 multithreading/DMA tricky bug chasing.

---

## 2006-2008 - World-Of-Football

**Add:**
- Context: [YouTube videos](https://www.youtube.com/watch?v=lDNY4-mupf0&list=PL5ED86276C6DB1347).

---

## Aug 2010 - Bigfoot Animation

**Add:**
- Context: Mocap loader, skeleton semantics, footplant detection. [Bigfoot video](https://www.youtube.com/watch?v=SEntMI6s6RQ). Example: smart pointers 10 years ago.

---

## May 2010 - Bigeye UI

*Already covered in Bigfoot context*

---

## Job queue (not in portfolio yet - could add?)

- Context: 1/100000 repro → found in an hour.

---

## Notes on approach (from README section 5)

Could add to a "Study methodology" or similar section:
- Physical notebooks (show GK notes)
- Large single-topic latex documents (show PL/QL printed book)
- Combination of physical notes, latex summaries, and gitlab project management
- The biggest challenge is still memorization
- Work as a 'processing machine' — open notes, switch context, process for results
- The older the topic, the longer the lead time (hours to weeks)
