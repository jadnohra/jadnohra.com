# Portfolio Enhancement - Argo AI Archives Investigation

## Status: COMPLETE (Investigation Phase)

---

## Files Explored

### ArgoArchive Directory (`/Users/jad_irred/Downloads/ArgoArchive/`)
Media files (videos, screenshots, PDFs):
- `multilayer-short-4k.mov` - Multilayer scenario generation demo
- `scenario-gen-multilayer.mov` - Scenario generation visualization
- `scenariogen_variation_fast_preview.mov` - Scenario variation preview
- `pacing_planner_proto_1.mov`, `pacing_planner_proto2.mov` - Pacing planner prototypes
- `ttc_extractor_2021-10-11.mov` - TTC (Time-To-Collision) extractor
- `optimizer_viz - Jupyter Notebook.html` - Optimizer visualization
- `first-screenshot-layered-scenario-gen.png` - Layered scenario generation
- `scenario_gen_smv-322.png` - Scenario gen SMV screenshot

### argoai Directory - Files Read
- [x] `vtt-team/README.md` - VTT team overview
- [x] `smv/modellib/README.md` - ModelLib overview
- [x] `knowledge/argo/sim/concepts/layered_sim/layered_sim.md` - Layered simulation architecture
- [x] `knowledge/argo/sim/resim.md` - Resimulation system (MTR, overlays, batch processing)
- [x] `smv/modellib/product/av_compute_model/about.md` - Compute model for timing/latency
- [x] `smv/modellib/product/av_sensing_model/about.md` - Sensing/tracking model
- [x] `smv/modellib/product/av_vehicle_dynamics_model/about.md` - Vehicle dynamics (under construction)
- [x] `smv/modellib/product/driver_model/about.md` - Driver behavior model (comprehensive)
- [x] `smv/modellib/product/trajectory_model/about.md` - Trajectory modeling
- [x] `smv/modellib/product/situation_model/about.md` - Situation/scenario modeling
- [x] `smv/modellib/product/tool_validity/ul4600-requirements.md` - UL4600 safety requirements
- [x] `smv/modellib/technical/compute_model/compute-model.md` - Compute model technical details
- [x] `knowledge/robotics/trajectory-generation.md` - Trajectory generation literature review
- [x] `knowledge/argo/motion-control/2021.04.17-av-path-refinement.md` - Path refinement (major doc)
- [x] `knowledge/argo/motion-control/2021.11.12-mv-vehicle-models.md` - Vehicle models
- [x] `knowledge/argo/vtt/vtt-tool-ecosystem.md` - VTT tool ecosystem (major doc)

---

## Key Technical Findings

### 1. VTT (Virtual Test Tooling) Ecosystem
**Role:** Engineering Manager overseeing comprehensive virtual testing infrastructure

**VTT Tool Categories:**
- **Test Management:** Test/scenario/log catalogues, scenario finders, static/dynamic analyzers
- **Test Authoring:** Scenario editor, L2S (Log-to-Scenario), fuzzers, augmenters
- **Test Execution:** Virtual, mixed reality, robotic modes
- **Test Assessment:** Big data analytics, Looker dashboards

**Testing Modalities:**
1. **Virtual** - Full simulation, scale advantage
2. **Mixed Reality** - Real AV with virtual injected objects
3. **Robotic** - Physical rovers directed by scenarios

### 2. SMV ModelLib - Simulation Model Validation
**Purpose:** Enable simulation-based virtual testing
**Philosophy:** "A model is good if it enables tests that use it to produce faithful evaluations of the system under test"
**Process:** Modeling → Tuning → Validation → Documentation of limitations

**Models Developed:**

#### AV Compute Model (ACM)
- Models real-time distributed system timing on embedded hardware
- Addresses clock synchronization, communication delays, execution time
- 8 dimensions: Execution Time, Branching, Messaging, Triggering, Transmission Time, Queuing, Clock Sync, Load
- Critical for ensuring virtual tests don't pass when real AV would fail due to latency
- Connects to SOTIF analysis and FMEA (Failure Mode and Effects Analysis)

#### AV Sensing Model
- Surrogate for tracker at SUT boundary
- Tracks extracted from virtual world entities
- Current: Perfect tracks (goal: model imperfections for coverage)

#### Driver Model (Most Comprehensive)
- Background: Wiedemann Model (1974), Intelligent Driver Model (1999), Enhanced IDM (2009)
- **Key innovation:** "Specialist Driver" - receives specific instructions for critical situations
- Challenge: AV testing requires unsafe manoeuvres that AV itself cannot execute
- Features matrix: Sense (environment, agents, prediction), Plan (trajectory, instructions, speed), Act (interpolation, dynamics)
- Explicit discussion of model validation challenges (Agent-Based Modeling)
- References game AI techniques: behavior trees, hierarchical task planners

#### Trajectory Model
- Path vs Trajectory distinction (spatial vs. spatial+time)
- Current: Interpolation-based following (no physics)
- Goals: Trajectory sculpting tool, trajectory assessment tool
- Supports physicality assessment and comfort metrics

#### Situation Model
- Simception scenario generation integration
- Event detectors, filters, populators
- Scenario composition and variation

### 3. Layered Simulation Architecture
**Core Concept:** Multiple Environment State (ES) layers for testing different AV sub-components

- **Layer 0:** Ground truth - perfect information from scenario specs
- **Detection Layer:** Noisified ground truth (position noise, shape distribution)
- **Subsequent Layers:** Created by Transformation Functions (TF)
- **Benefit:** Test detection, prediction, tracking, motion planning separately

### 4. Resimulation System (Resim)
**Purpose:** Run AV software stack with recorded sensor data

**Architecture:**
- **Transform Graph** - DAG of overlay producers
- **MTR (Multi-Task Resim Director)** - Deterministic discrete-event and time-stepped orchestration
- **Overlays** - Partial logs containing newly produced messages
- **Batch Processing** - Jobs Platform integration

**Key Teams:** VTT-RTO (Runtime Task Orchestration), Logapps, Resim Integration, Jobs Platform

### 5. Path Refinement System (Motion Control)
**Stack:** Speed Limiter → Longitudinal Refiner → Lateral Refiner

#### Longitudinal Refiner
- Quadratic Programming optimization
- Model Predictive Control flavor with bicycle model
- **5 Modes:** Velocity, Coast, Distance (Track Behind, Track Ahead, Pass, Slow Speed Tracking, Cut In)
- Soft/hard envelopes for collision avoidance
- CVXGEN solver with 25 iteration limit
- Input certification for solver convergence

#### Lateral Refiner
- 40 horizon points (20 short + 20 long)
- Purpose: Smooth re-engagement with planner path
- Veer maneuvers, safety, comfort optimization
- Challenges: Solver convergence, greedy clamping

**Technical Challenges Addressed:**
- Solver numerical issues (iterative refinement)
- Dynamic input handling
- ROLI (Region of Likely Intersection) stops
- Steering wheel acceleration limits (3500 deg/s², 150 deg/s rate)

### 6. Trajectory Generation Literature
Comprehensive literature review covering:
- Time-optimal trajectory planning (178+ citations papers)
- Spline-based approaches
- Model Predictive Control methods
- Human comfort optimization
- Real-time algorithms
- Behavioral interaction modeling

### 7. UL4600 Safety Requirements
Standard for Safety, Evaluation of Autonomous Products
Key requirements for simulation tools:
- Hazards/limitations of simulations must be identified
- Simplifications, assumptions, abstractions documented
- Tool functionality issues: physics accuracy, time management, result reporting
- Hazard mitigation through real-world comparison

---

## Portfolio Items Identified

### 1. VTT Simulation Platform (Argo AI, 2021-2022)
**Role:** Engineering Manager, Simulation
**Algorithms/Topics:**
- Layered simulation architecture
- Discrete-event simulation scheduling
- Resimulation with deterministic replay
- Transform graph batch processing
- Virtual/Mixed Reality/Robotic testing modes

### 2. Simulation Model Library (ModelLib)
**Models Led:**
- **AV Compute Model** - Real-time distributed system timing
- **Driver Model** - Agent-based behavior modeling with ABM validation
- **Trajectory Model** - Path/trajectory assessment and sculpting
- **Sensing Model** - Perception surrogate with noise modeling
- **Situation Model** - Scenario composition and variation
**Algos:** Discrete-event simulation, Intelligent Driver Model, Behavior Trees

### 3. Path Refinement Knowledge Work
**Technical Analysis:**
- QP-based Model Predictive Control
- Bicycle model dynamics
- Solver convergence analysis
- Steering acceleration constraints
**Algos:** Quadratic Programming, MPC, CVXGEN solver

### 4. Scenario Generation System
**Evidence:** Videos showing multilayer generation and variations
**Topics:** Event detection, scenario augmentation, fuzzing

### 5. Pacing Planner Prototype
**Evidence:** `pacing_planner_proto_1.mov`, `pacing_planner_proto2.mov`

### 6. TTC Extractor Tool
**Evidence:** `ttc_extractor_2021-10-11.mov`
Time-To-Collision extraction/analysis

### 7. Optimizer Visualization
**Evidence:** `optimizer_viz - Jupyter Notebook.html`

### 8. UL4600 Tool Validation Work
**Evidence:** `tool_validity/ul4600-requirements.md`
Safety certification requirements for simulation tools

---

## Recommended Portfolio Entries

### Primary Entry: VTT Simulation Platform
**Title:** "AV Simulation Platform - Virtual Test Tooling"
**Date:** 2021-2022
**Role:** Engineering Manager, Simulation
**Description:**
Led development of comprehensive simulation platform for autonomous vehicle testing. Designed layered simulation architecture enabling separate testing of perception, prediction, and planning subsystems. Built ModelLib - a suite of validated simulation models including compute timing, driver behavior, and trajectory assessment. Implemented resimulation system with deterministic replay and batch processing at scale.

**Key Technical Areas:**
- Layered Environment State architecture with Transformation Functions
- Discrete-event simulation with deterministic scheduling
- Model Predictive Control for path refinement analysis
- Agent-based driver modeling with ABM validation methodology
- UL4600 safety certification requirements for simulation tools

**Algos:** `Discrete-Event Simulation`, `MPC`, `Quadratic Programming`, `Agent-Based Modeling`, `Intelligent Driver Model`
**Theory:** `Constraint Solving`, `Optimization`, `Safety Certification`
**Career:** `career.argo`

### Secondary Entry: Resimulation System
**Title:** "Resimulation Infrastructure"
**Date:** 2021-2022
**Description:**
Cross-team infrastructure for replaying AV software with recorded sensor data. Transform Graph DAG architecture, MTR orchestration, overlay management, batch processing at scale.

---

## Graph Nodes to Add

### New Algorithm Nodes
- `algo.discreteeventsim` - Discrete Event Simulation
- `algo.mpc` - Model Predictive Control
- `algo.abm` - Agent-Based Modeling
- `algo.idm` - Intelligent Driver Model
- `algo.qp` - Quadratic Programming

### New Project Node
```json
{
  "name": "project.vtt",
  "label": "VTT Platform",
  "imports": ["career.argo", "algo.discreteeventsim", "algo.mpc", "algo.abm", "algo.qp", "theory.constraintsolving"]
}
```

---

## Notes for Implementation

1. **Videos available** for demos - can be used on portfolio page
2. **Technical depth** is substantial - can reference specific algorithms
3. **Cross-cutting concerns:** Safety (UL4600), Scale (batch processing), Validation (ABM methodology)
4. **Unique aspects:**
   - Layered simulation architecture is novel
   - "Specialist Driver" concept for testing
   - Multi-modal testing (virtual/mixed reality/robotic)
