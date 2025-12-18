---
layout: post
title: Compilers
icon: fas fa-cogs
order: 6
toc: true
---

<style>
/* Compact layout */
.content p { margin-bottom: 0.5rem; }
.content h2 { margin-top: 1.5rem; margin-bottom: 0.5rem; }
.content h3 { margin-top: 1rem; margin-bottom: 0.3rem; }
.content ul, .content ol { margin-bottom: 0.5rem; }
.content li { margin-bottom: 0.2rem; }
.content blockquote { margin: 0.5rem 0; }
.content hr { margin: 1rem 0; }
</style>

## Compiler Stages

Compiler stages exist because different parts of the problem belong to different complexity classes. Each class has a minimal machine that handles it. Mixing them forces a more powerful machine everywhere.

```
Complexity class           Minimal machine              Stage
──────────────────────────────────────────────────────────────────
Character patterns         finite automaton (DFA)       lexer
Nesting / structure        pushdown automaton (stack)   parser
Names, scopes, types       symbol table + tree walk     semantic analysis
Control flow properties    graph + fixed-point          dataflow analysis
Value flow                 SSA + graph algorithms       optimization
Resource constraints       graph coloring (NP-hard)     register allocation
Ordering constraints       topological sort / ILP       instruction scheduling
```

---

### The Principle

> Each stage uses the minimal machine for its complexity class. Stage boundaries are complexity class boundaries.

Mixing complexity classes means:
- Using a more powerful machine than necessary (wasteful)
- Or building something outside clean models (hard to verify)

Separating means:
- Each stage is well-understood (automata theory, CFGs, dataflow lattices)
- Each stage can be verified against its theory
- Complexity is additive, not multiplicative

---

### The Stages

```
Source Text
  → Lexer
      complexity: regular (character patterns)
      machine: DFA
      produces: token stream

  → Parser
      complexity: context-free (nesting, structure)
      machine: pushdown automaton
      produces: AST

  → Semantic Analysis
      complexity: context-sensitive (names, types)
      machine: symbol table + tree walk
      produces: annotated AST
      passes:
        → name resolution (build symbol table, resolve identifiers)
        → type checking (infer/check types, insert coercions)

  → IR Generation
      complexity: translation (tree → graph)
      produces: IR (CFG, SSA)

  → Optimization
      complexity: dataflow (value flow, control flow)
      machine: lattice + fixed-point iteration
      produces: optimized IR
      passes:
        → dead code elimination
        → constant propagation
        → common subexpression elimination
        → inlining
        → loop optimizations

  → Code Generation
      complexity: resource constraints (NP-hard)
      machine: graph coloring, scheduling heuristics
      produces: machine code / bytecode
      passes:
        → instruction selection (IR ops → machine ops)
        → register allocation (virtual → physical)
        → instruction scheduling (order for pipeline)

Bytecode / Machine Code
```

---

### Why Separate

**Lexer from parser**

Character patterns are regular — DFA handles them in O(n), no memory beyond state. Nesting is context-free — needs a stack. Mixed: grammar is context-free over characters, huge and slow. Separated: lexer is trivial DFA, parser assumes tokens.

**Parser from semantic analysis**

Nesting is context-free — parsable without knowing what identifiers mean. Type checking is context-sensitive — `a + b` depends on types of `a` and `b`. Mixed: grammar must encode type rules, no longer context-free. Separated: parser handles structure, semantic analysis handles meaning.

**Semantic analysis in passes**

Type of `x + y` depends on types of `x` and `y`. Types depend on declarations. Declarations require name resolution. So: resolve names first, then types are available. Phase ordering is a dependency graph.

**Dataflow from parsing**

"Is this variable live?" requires traversing control flow paths — a graph problem, fixed-point iteration. AST is a tree, wrong shape. Need CFG (graph) and lattice-based analysis. Different structure, different algorithms.

**Register allocation late**

Graph coloring is NP-hard. Optimization first reduces problem size (fewer variables, simpler code). Optimization is also easier with unlimited virtual registers — no spill code, no constraints. Do the hard pass once, at the end, on a smaller problem.

---

### IR as Pivot Point

```
     Solidity                    EVM bytecode
     Vyper        →    IR    →   eWASM
     Fe                          future targets
```

N languages × M targets = N×M compilers without IR, N+M with IR. IR is the abstraction that compresses the combinatorics. Same reason Unix has "everything is a file" — standard interface reduces combinations.

---

### SSA for Optimization

```
x = 1
x = 2
y = x  // which x?

// SSA form:
x1 = 1
x2 = 2
y = x2  // obvious
```

In SSA, each variable has one definition. "Where does this value come from?" — follow the name. Without SSA, track all possible definitions at each point. SSA is a representation that makes dataflow cheap.

---

### Information Accumulation

Each stage adds information not present before:

```
Source text       → characters
Token stream      → + lexical categories
AST               → + structure
Annotated AST     → + types, resolved names
CFG / SSA         → + explicit control flow, single assignment
Optimized IR      → + optimization decisions
Machine code      → + physical resource assignments
```

Later stages assume earlier work is done. Parser assumes tokens are valid. Type checker assumes names are resolved. Optimizer assumes types are correct.

---

### Representation Fitness

Each representation makes certain operations cheap:

```
String            → editing, display
Token stream      → lexical queries (is this a keyword?)
AST               → structural transforms (refactoring, macros)
CFG               → control flow analysis (reachability, dominance)
SSA               → dataflow optimization (single def = easy propagation)
Linear IR         → instruction scheduling
Machine code      → execution
```

Wrong representation = fighting the structure. AST is wrong for dataflow. CFG is wrong for pretty-printing.

---

### The Derived Data Connection

A compiler is a chain of derived data:

| Derived | Source | Transform | Sync (incremental) |
|---------|--------|-----------|-------------------|
| Tokens | source text | lexing | re-lex changed region |
| AST | tokens | parsing | re-parse changed file |
| Symbol table | AST | name resolution | invalidate dependents |
| Typed AST | AST + symbol table | type checking | re-check changed + dependents |
| IR | typed AST | lowering | re-lower changed functions |
| Optimized IR | IR | optimization | re-optimize changed |
| Bytecode | optimized IR | codegen | re-codegen changed |

Incremental compilation is cache invalidation. When source changes, which derived data is stale? Query-based compilers (rustc, new TypeScript) make this explicit: each derived fact is a cached query with tracked dependencies.

---

### Solidity / EVM Specifics

EVM has unusual constraints that shape the compiler:

**Stack machine target**

EVM is stack-based, not register-based. No random access to locals — must manage stack explicitly. Affects:
- IR design (stack-oriented IR helps)
- Codegen (stack scheduling, not register allocation)
- Optimization (stack depth is a cost)

**Gas is the cost model**

Not cycles, not memory — gas. Different cost structure:
- Storage: 20,000 gas SSTORE (cold), 100 SLOAD (warm)
- Memory: quadratic growth
- Compute: cheap

Optimizer targets gas, not speed. Storage access patterns dominate.

**Contract size limit**

24KB bytecode max. Inlining can blow the limit. Dead code elimination is critical. Code size is first-class optimization target.

**Immutability after deploy**

No patches. Bugs are permanent. Raises stakes for:
- Semantic analysis (catch errors early)
- Optimization correctness (can't fix later)
- Verification (formal methods more valuable)

**ABI as interface**

ABI is the serialization format for calls. Compiler must:
- Generate ABI from source
- Generate encoding/decoding code
- Selector computation (function signature hashing)

ABI is interface-as-derived-data — projection of contract interface.

---

### Design Insights

**Multi-level IR**

Solc has Yul as IR — high enough to express Solidity constructs, low enough to optimize. Some compilers have multiple IRs:
- High-level IR (close to source, source-level optimization)
- Low-level IR (close to target, machine-level optimization)

Tradeoff: more IRs = more passes = more maintenance, but each IR is simpler.

**Optimization pass ordering**

Pass A might enable pass B (inlining exposes constant propagation). Pass B might enable pass A (constant propagation simplifies branches for inlining). Fixed-point: run until no changes. Slow in practice — use heuristic ordering.

**Yul optimizer approach**

Solc's Yul optimizer uses small, composable passes:

```
ExpressionSimplifier
CommonSubexpressionEliminator
DeadCodeEliminator
UnusedPruner
...
```

Each pass is simple. Complexity emerges from composition. Easier to verify, easier to debug.

---

### Connection to System Primitives

| System Design | Compiler |
|---------------|----------|
| Memory hierarchy | Representation fitness (AST, CFG, SSA) |
| Derived data | Each IR is derived from previous |
| Sync obligation | Incremental compilation |
| Time vs space | Precompute analysis vs recompute |
| Immutability | SSA, persistent data structures |
| Separation of concerns | Stage per complexity class |
| Staged computation | Lexer → parser → semantic → IR → codegen |
