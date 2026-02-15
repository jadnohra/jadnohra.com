---
layout: post
title: Rust
icon: fab fa-rust
order: 7
toc: true
---

<style>
/* Compact layout */
.content p { margin-bottom: 0.5rem; }
.content h2 { margin-top: 1.5rem; margin-bottom: 0.5rem; display: block !important; visibility: visible !important; }
.content h3 { margin-top: 1rem; margin-bottom: 0.3rem; display: block !important; visibility: visible !important; }
.content ul, .content ol { margin-bottom: 0.5rem; }
.content li { margin-bottom: 0.2rem; }
.content blockquote { margin: 0.5rem 0; }
.content hr { margin: 1rem 0; }

/* All links should be blue and underlined by default */
a {
  color: #2563eb;
  text-decoration: underline;
  text-underline-offset: 2px;
}
a:hover {
  color: #1d4ed8;
}

/* Derived Data styles (shared with abstractor) */
.derived-box {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 6px;
  padding: 12px 16px;
  margin: 1rem 0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 13px;
  color: #e2e8f0;
  border-left: 3px solid #f97316;
}
.derived-box b {
  color: #f97316;
}
.derived-table {
  width: 100%;
  border-collapse: collapse;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  margin: 1rem 0;
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  overflow: hidden;
}
.derived-table th {
  background: rgba(255,255,255,0.05);
  color: #f1f5f9;
  font-weight: 600;
  padding: 10px 12px;
  text-align: left;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.derived-table td {
  padding: 8px 12px;
  color: #cbd5e1;
  border-bottom: 1px solid rgba(255,255,255,0.03);
}
.derived-table tr:last-child td {
  border-bottom: none;
}
.derived-table tr:hover td {
  background: rgba(255,255,255,0.02);
}
.derived-table code {
  background: rgba(249, 115, 22, 0.2);
  color: #f97316;
  padding: 2px 6px;
  border-radius: 3px;
  font-size: 11px;
}

/* Triangle diagram */
.triangle-diagram {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  overflow-x: auto;
}
.triangle-diagram pre {
  margin: 0;
  color: #f97316;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  line-height: 1.4;
}

/* Code blocks */
.rust-code {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  overflow-x: auto;
  border-left: 3px solid #f97316;
}
.rust-code pre {
  margin: 0;
  color: #e2e8f0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  line-height: 1.5;
}
.rust-code .comment {
  color: #64748b;
}
.rust-code .keyword {
  color: #f97316;
}

/* Equivalences box */
.equivalences-box {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  color: #cbd5e1;
  line-height: 1.8;
}

/* Pipeline diagram */
.pipeline-diagram {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  overflow-x: auto;
}
.pipeline-diagram pre {
  margin: 0;
  color: #e2e8f0;
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 11px;
  line-height: 1.4;
}
</style>

<div style="background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%); border-radius: 8px; padding: 1.2rem 1.5rem; margin-bottom: 2rem; text-align: center;">
  <span style="color: #e2e8f0; font-size: 1.1rem;">For the course version → </span>
  <a href="/learn-rust/" style="color: #f97316; font-size: 1.1rem; font-weight: 600; text-decoration: underline;">Learn Rust</a>
</div>

## Derived Data Choices

Rust's memory model is a set of choices on the [SPACE / TIME / IDENTITY triangle](/abstractor/).

<div class="derived-box">
Core strategy: <b>prove coherence at compile time</b>.
</div>

---

## The Two Projections

Each axis exists in two forms:

<table class="derived-table">
<tr><th>Axis</th><th>Compile-time</th><th>Runtime</th></tr>
<tr><td>SPACE</td><td>Types, layouts</td><td>Bytes in memory</td></tr>
<tr><td>TIME</td><td>CFG regions</td><td>Execution sequence</td></tr>
<tr><td>IDENTITY</td><td>Names</td><td>Addresses</td></tr>
</table>

The borrow checker works in compile-time projection. It tracks paths like `x` and `x.field` (not runtime addresses), analyzes control flow regions (not actual execution), and reasons about types (not bytes). It proves coherence in this abstract model.

### Zero-Cost Abstraction

Rust claims "zero-cost abstractions" — safety checks that don't add runtime overhead.

For the borrow checker, this means:
- Analysis happens at compile time
- Names and regions are erased before execution
- At runtime: just addresses, just execution, no tracking

The cost is compile time and programmer effort. The runtime pays nothing.

---

## Vocabulary

Rust's terminology conflates projections. Before we use these terms, here's what they mean in the framework.

### The Key Translations

| Rust says | Suggests | Actually means |
|-----------|----------|----------------|
| `&T` | "reference" | shared IDENTITY, frozen TIME |
| `&mut T` | "mutable reference" | exclusive IDENTITY, TIME flows |
| "lifetime" | runtime duration | compile-time TIME region |
| "borrow" | lending | creating IDENTITY relationship |
| "move" | relocating data | transferring IDENTITY |
| "borrow checker" | rule checker | coherence prover |

### The &mut Misconception

`&mut T` is not about mutation. It's about **exclusive IDENTITY**.

The exclusivity is what allows mutation safely. If you're the only one with access, you can change things without conflict.

`&T` is not about immutability. It's about **shared IDENTITY**. Sharing requires frozen TIME to stay coherent.

### Reading Rust with the Framework

| When you see | Think |
|--------------|-------|
| `&x` | shared IDENTITY to x's SPACE |
| `&mut x` | exclusive IDENTITY to x's SPACE |
| `'a` | compile-time TIME region |
| `let y = x` | IDENTITY transfer (if not Copy) |
| `x.clone()` | SPACE duplication |
| `drop(x)` | SPACE freed at TIME boundary |

---

## The Coherence Problem

Two paths to the same SPACE. One writes, one reads. The reader sees partial state. This is incoherence.

<div class="derived-box">
Sharing freezes. Mutation isolates.
</div>

Either:
- **Shared IDENTITY** → many can access → TIME must freeze (no mutation)
- **Exclusive IDENTITY** → one can access → TIME can flow (mutation ok)

### How Rust Enforces This

The borrow checker analyzes your code at compile time:

1. **Tracks names** — which variables alias which SPACE
2. **Builds a graph** — possible execution paths (the CFG)
3. **Computes regions** — when each reference is live
4. **Checks for conflicts** — shared + mutation overlapping?

<div class="rust-code">
<pre>let mut x = 5;
let r = &x;        <span class="comment">// shared IDENTITY starts</span>
println!("{}", r); <span class="comment">// shared IDENTITY used, then ends</span>
let m = &mut x;    <span class="comment">// exclusive starts — OK, no overlap</span></pre>
</div>

The borrow checker sees: shared region ends before exclusive begins. No conflict.

<div class="rust-code">
<pre>let mut x = 5;
let r = &x;        <span class="comment">// shared IDENTITY starts</span>
let m = &mut x;    <span class="comment">// exclusive starts — ERROR</span>
println!("{}", r); <span class="comment">// shared still live here</span></pre>
</div>

The borrow checker sees: shared and exclusive regions overlap. Conflict. Rejected.

### SPACE Granularity

The borrow checker tracks IDENTITY at a certain granularity:

| Path type | Can prove disjoint? | Why |
|-----------|--------------------|----|
| `p.x` vs `p.y` | Yes | Struct fields known at compile time |
| `v[0]` vs `v[1]` | No | Indices are runtime values |

<div class="rust-code">
<pre>let mut p = Point { x: 1, y: 2 };
let rx = &mut p.x;
let ry = &mut p.y;  <span class="comment">// OK: different fields</span></pre>
</div>

<div class="rust-code">
<pre>let mut v = vec![1, 2, 3];
let r = &mut v[0];
let s = &mut v[1];  <span class="comment">// ERROR: both "borrow v"</span></pre>
</div>

The checker sees both as "borrow of v" — it can't prove `v[0] ≠ v[1]`. More on this in [Borrow Checker Internals](#borrow-checker-internals).

### Zero Cost

All analysis happens at compile time. Names are erased. At runtime: just addresses and execution. No tracking, no checks, no overhead.

---

## Bindings

<table class="derived-table">
<tr><th>Construct</th><th>SPACE</th><th>TIME</th><th>IDENTITY</th></tr>
<tr><td><code>const</code></td><td>Inlined</td><td>Compile</td><td>Compile only</td></tr>
<tr><td><code>static</code></td><td>Data segment</td><td>Compile init</td><td>Both</td></tr>
<tr><td><code>static mut</code></td><td>Data segment</td><td>Runtime</td><td>Both (unsafe)</td></tr>
<tr><td><code>let</code></td><td>Stack</td><td>Runtime</td><td>Both</td></tr>
<tr><td><code>let mut</code></td><td>Stack</td><td>Runtime</td><td>Both</td></tr>
</table>

**const** — Purely compile-time. No runtime IDENTITY exists. Each use is inlined. Taking `&CONST` may yield different addresses—there is no single location.

<div class="rust-code">
<pre>const X: u32 = 5;
let a = &X;
let b = &X;
<span class="comment">// a and b may have different addresses</span></pre>
</div>

**static** — Bridges projections. Initialized at compile-time, lives in data segment at runtime. Has a stable address.

**static mut** — Runtime mutation of global IDENTITY. The borrow checker can't prove safety across the whole program. Every access requires `unsafe`.

**let / let mut** — Runtime evaluation, stack SPACE. `mut` allows rebinding the name, not mutation through it.

---

## References

References are how you create IDENTITY to existing SPACE.

<table class="derived-table">
<tr><th>Reference</th><th>IDENTITY</th><th>TIME</th><th>Rule</th></tr>
<tr><td><code>&T</code></td><td>Shared</td><td>Frozen</td><td>Many allowed</td></tr>
<tr><td><code>&mut T</code></td><td>Exclusive</td><td>Flows</td><td>One allowed</td></tr>
</table>

This is the coherence rule applied:

**&T — Shared IDENTITY:**

<div class="rust-code">
<pre>let x = vec![1, 2, 3];
let r1 = &x;
let r2 = &x;  <span class="comment">// OK: multiple shared</span></pre>
</div>

Many paths to same SPACE. TIME frozen—no mutation through any path. No conflict possible.

**&mut T — Exclusive IDENTITY:**

<div class="rust-code">
<pre>let mut x = vec![1, 2, 3];
let r = &mut x;
r.push(4);  <span class="comment">// OK: exclusive access</span></pre>
</div>

One path to SPACE. TIME flows—mutation allowed. No conflict possible because no one else can see.

The borrow checker tracks: which names have IDENTITY to which SPACE, during which TIME regions. It proves no shared+exclusive overlap.

---

## IDENTITY at Runtime

At runtime, names are gone. Only addresses remain.

**RAM can reference itself.**

RAM locations have addresses. Addresses are numbers. Numbers are data. Data can be stored in RAM. Therefore: RAM can store addresses of its own locations.

```
┌─────────┬───────────────┐
│  100    │   5       ← x │ ←──┐
├─────────┼───────────────┤    │
│  108    │  100      ← r │ ───┘
└─────────┴───────────────┘
```

This enables references, sharing, and complex structures.

**Registers have no address.**

Registers hold bytes but lack addresses. Taking a reference forces the value to stack.

<div class="rust-code">
<pre>let x = 5;      <span class="comment">// might live in register</span>
let r = &x;     <span class="comment">// x now has address (stack)</span></pre>
</div>

**Cost of sharing:**

| Cost | Cause |
|------|-------|
| 8 bytes | Address storage |
| Indirection | Follow address to reach data |
| Forces RAM | Registers have no address |

For small types, copying is cheaper than referencing. This is why `Copy` exists.

---

## Stack and Heap

**Stack SPACE is automatic.** Variables live on the stack. When they go out of scope, their SPACE is reclaimed. The compiler knows the size at compile time.

**Heap SPACE is dynamic.** For SPACE whose size isn't known at compile time, or SPACE that outlives the current scope.

<div class="rust-code">
<pre>let b = Box::new(String::from("hello"));</pre>
</div>

`Box::new(x)` allocates SPACE on the heap, puts x there, and returns owning coordinates.

| Rust | Ownership | Cleanup |
|------|-----------|---------|
| `&y` | Borrowing | Not responsible |
| `&mut y` | Borrowing | Not responsible |
| `Box::new(x)` | Owning | Frees heap when dropped |

No garbage collector. No manual `free()`. The compiler inserts cleanup at scope end.

---

## Ownership and Move

<div class="rust-code">
<pre>let v = vec![1, 2, 3];
let w = v;  <span class="comment">// IDENTITY transfers to w</span>
<span class="comment">// v is now invalid</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>What happens</th></tr>
<tr><td>IDENTITY</td><td>Transferred — old name invalid</td></tr>
<tr><td>SPACE</td><td>Same or new (compiler decides)</td></tr>
<tr><td>Coherence</td><td>Only one owner → no aliasing</td></tr>
</table>

The borrow checker tracks these transfers. After `let w = v`, using `v` is an error—its IDENTITY is gone.

Move semantics eliminate shared IDENTITY by default. No coherence problem if only one owner exists.

---

## Copy vs Clone

Duplicating SPACE happens two ways:

**Copy** — Copy the bytes. Implicit on assignment.

**Clone** — Call `.clone()`. Always explicit.

**When copying bytes works:**

For `i32`, SPACE is self-contained. Copying bytes produces an independent value.

```
┌─────────┐    ┌─────────┐
│    5    │    │    5    │
└─────────┘    └─────────┘
```

**When copying bytes fails:**

For `String`, SPACE contains coordinates to heap SPACE:

```
┌─────────────────┐
│ addr: 0x1234    │     Heap:
│ len: 5          │     ┌───────────┐
│ capacity: 5     │     │ h e l l o │
└─────────────────┘     └───────────┘
```

Copying bytes would create two owners of the same heap SPACE—two IDENTITYs, coherence problem. Rust requires explicit choice: `.clone()` for deep copy, `Rc::clone()` for shared ownership, or move.

<table class="derived-table">
<tr><th>Type</th><th>Copy?</th><th>Clone?</th><th>Why</th></tr>
<tr><td><code>i32</code></td><td>Yes</td><td>Yes</td><td>Self-contained</td></tr>
<tr><td><code>&T</code></td><td>Yes</td><td>Yes</td><td>Coordinates, doesn't own</td></tr>
<tr><td><code>String</code></td><td>No</td><td>Yes</td><td>Owns heap—must choose strategy</td></tr>
<tr><td><code>Vec&lt;T&gt;</code></td><td>No</td><td>Yes</td><td>Owns heap</td></tr>
</table>

---

## Shared Ownership

`Box<T>` is unique IDENTITY to SPACE. One owner, clear responsibility.

What if you need shared IDENTITY to the same SPACE? Who frees it?

`Rc<T>` / `Arc<T>`: count the IDENTITYs. When count hits zero, SPACE ends.

<table class="derived-table">
<tr><th>Type</th><th>IDENTITY</th><th>SPACE lifetime</th><th>Thread</th></tr>
<tr><td><code>Box&lt;T&gt;</code></td><td>Unique</td><td>Owner ends it</td><td>Any</td></tr>
<tr><td><code>Rc&lt;T&gt;</code></td><td>Shared, counted</td><td>Last IDENTITY ends it</td><td>Single</td></tr>
<tr><td><code>Arc&lt;T&gt;</code></td><td>Shared, atomic counted</td><td>Last IDENTITY ends it</td><td>Multi</td></tr>
</table>

This solves SPACE × IDENTITY. But contents are immutable—TIME is frozen.

To combine shared ownership with mutation, you need interior mutability.

---

## Interior Mutability

The borrow checker is **sound but incomplete**:
- If it accepts → definitely safe
- If it rejects → might still be safe

Interior mutability handles the "might still be safe" cases by moving verification to runtime. Same rule, different when checked.

<table class="derived-table">
<tr><th>Type</th><th>Checked when</th><th>Cost</th><th>Thread</th></tr>
<tr><td><code>&T</code> / <code>&mut T</code></td><td>Compile</td><td>Zero</td><td>Any</td></tr>
<tr><td><code>Cell&lt;T&gt;</code></td><td>Runtime</td><td>Copy in/out</td><td>Single</td></tr>
<tr><td><code>RefCell&lt;T&gt;</code></td><td>Runtime</td><td>Borrow count</td><td>Single</td></tr>
<tr><td><code>Mutex&lt;T&gt;</code></td><td>Runtime</td><td>Lock</td><td>Multi</td></tr>
<tr><td><code>RwLock&lt;T&gt;</code></td><td>Runtime</td><td>Lock</td><td>Multi</td></tr>
<tr><td><code>Atomic*</code></td><td>Runtime</td><td>Hardware</td><td>Multi</td></tr>
<tr><td><code>UnsafeCell&lt;T&gt;</code></td><td>Never</td><td>Zero</td><td>Any</td></tr>
</table>

**Cell** — Copy values in/out. No references into contents.

<div class="rust-code">
<pre>let x = Cell::new(5);
x.set(10);       <span class="comment">// mutate through shared ref</span>
let v = x.get(); <span class="comment">// returns copy</span></pre>
</div>

**RefCell** — Runtime borrow checker. Panics on violation.

<div class="rust-code">
<pre>let x = RefCell::new(vec![1, 2, 3]);
let r = x.borrow();      <span class="comment">// shared</span>
let s = x.borrow();      <span class="comment">// OK</span>
<span class="comment">// x.borrow_mut();       // PANIC: already borrowed</span></pre>
</div>

**Mutex** — Serialize TIME. Only one accessor at a time.

<div class="rust-code">
<pre>let x = Mutex::new(vec![1, 2, 3]);
let mut guard = x.lock().unwrap();
guard.push(4);
<span class="comment">// other threads block until guard drops</span></pre>
</div>

**When to use:**

| Situation | Use |
|-----------|-----|
| Simple flag, single thread | `Cell<T>` |
| Complex data, single thread | `RefCell<T>` |
| Shared across threads | `Mutex<T>` |
| Read-heavy, multi-thread | `RwLock<T>` |
| Counter, multi-thread | `AtomicU64` |
| Borrow checker accepts code | None needed |

---

## Thread Safety

<table class="derived-table">
<tr><th>Trait</th><th>Meaning</th></tr>
<tr><td><code>Send</code></td><td>IDENTITY can transfer across thread boundary</td></tr>
<tr><td><code>Sync</code></td><td>Shared IDENTITY (<code>&T</code>) safe across threads</td></tr>
</table>

<div class="rust-code">
<pre><span class="comment">// Rc&lt;T&gt; is !Send and !Sync — single-thread only</span>
<span class="comment">// Arc&lt;T&gt; is Send and Sync — multi-thread safe</span>
<span class="comment">// Mutex&lt;T&gt;: T is Send → Mutex&lt;T&gt; is Send + Sync</span></pre>
</div>

The borrow checker proves these at compile time. If your code compiles, thread safety is proven.

---

## Summary

<table class="derived-table">
<tr><th>Problem</th><th>Rust's Solution</th></tr>
<tr><td>When to evaluate?</td><td><code>const</code> (compile) vs <code>let</code> (runtime)</td></tr>
<tr><td>Where to store?</td><td>Inlined / stack / heap / static</td></tr>
<tr><td>Aliasing + mutation?</td><td>Borrow checker: shared OR exclusive</td></tr>
<tr><td>Global state?</td><td><code>static</code> with atomics or interior mutability</td></tr>
<tr><td>Cross-thread sharing?</td><td><code>Send</code> / <code>Sync</code> traits</td></tr>
<tr><td>Escaping the rules?</td><td><code>unsafe</code> — you prove coherence</td></tr>
</table>

<div class="derived-box">
Rust's bet: prove coherence in compile-time projection. Pay with complexity. Gain zero-cost guarantees at runtime.
</div>

### Equivalences

<div class="equivalences-box">
const         ≅  inlined value      ≅  no runtime IDENTITY<br>
static        ≅  single replica     ≅  bridges projections<br>
&T            ≅  read replica       ≅  shared IDENTITY, frozen TIME<br>
&mut T        ≅  exclusive lock     ≅  unique IDENTITY, TIME flows<br>
Mutex&lt;T&gt;      ≅  distributed lock   ≅  serialize TIME<br>
move          ≅  transfer           ≅  IDENTITY changes hands<br>
Clone         ≅  snapshot           ≅  new IDENTITY, new SPACE<br>
unsafe        ≅  "trust me"         ≅  you prove coherence
</div>

---

## Borrow Checker Internals

You've seen what the borrow checker enforces. Here's how it works.

### The Pipeline

The borrow checker operates entirely in compile-time projection:

<div class="pipeline-diagram">
<pre>
Source Code
     │
     ▼
┌───────────────┐
│  Lower to MIR │ ← IDENTITY ops explicit
└───────────────┘
     │
     ▼
┌───────────────┐
│   Build CFG   │ ← branching TIME
└───────────────┘
     │
     ▼
┌───────────────┐
│   Liveness    │ ← "will this IDENTITY be used?"
└───────────────┘
     │
     ▼
┌───────────────┐
│    Region     │ ← compute minimal TIME spans
│   Inference   │
└───────────────┘
     │
     ▼
┌───────────────┐
│   Conflict    │ ← SPACE × TIME × IDENTITY check
│   Detection   │
└───────────────┘
     │
  ▼       ▼
 OK     ERROR
</pre>
</div>

### Phase 1: MIR

MIR makes IDENTITY operations explicit:

<div class="rust-code">
<pre>_2 = &(*_1)     <span class="comment">// borrow: create IDENTITY</span>
_3 = use(_2)   <span class="comment">// use: access through IDENTITY</span>
drop(_2)       <span class="comment">// end: IDENTITY released</span></pre>
</div>

### Phase 2: CFG

The function becomes a directed graph. Each node is a program point, edges are possible transitions.

TIME in a program is not linear. The CFG captures all possible orderings.

### Phase 3: Liveness

Liveness asks: "From this point, on some future path, will this IDENTITY be used?"

Computed via backward dataflow. The IDENTITY must be valid at all points where it's live.

### Phase 4: Region Inference

A region is a set of CFG points. Regions form a lattice under subset ordering.

The solver finds the minimal region satisfying all constraints. Fixed-point iteration: expand regions until stable.

### Phase 5: Conflict Detection

With regions computed, check for conflicts:

<div class="rust-code">
<pre>ERROR if:
  ∃ borrows B1, B2 of same SPACE where:
    - regions overlap
    - AND one is exclusive
    - AND B1 ≠ B2</pre>
</div>

### SPACE Granularity Deep Dive

The borrow checker tracks paths:
- `x` — the whole binding
- `x.field` — a struct field (statically known)
- `x[i]` — an array element (runtime index)

**Struct fields**: statically known, can prove disjoint.

**Array indices**: runtime values, can't prove `i ≠ j` in general.

**Workarounds:**

<div class="rust-code">
<pre><span class="comment">// split_at_mut divides into disjoint slices</span>
let (left, right) = v.split_at_mut(1);
let r = &mut left[0];
let s = &mut right[0];  <span class="comment">// OK: different slices</span></pre>
</div>

### Decidability

The borrow checker uses decidable approximation:

| Perfect question | Approximation |
|-----------------|---------------|
| "Will this branch run?" | "Assume all branches might run" |
| "Same address?" | "Same base → might alias" |
| "Will this be used?" | "Might be used on some path" |

Sound but incomplete:
- OK → definitely safe
- ERROR → might be safe (false positives exist)

This is why interior mutability exists: to handle false positives.

### Rejected Valid Programs

**Can't prove branch won't run:**

<div class="rust-code">
<pre>let mut x = 5;
let r = &mut x;
if false {
    println!("{}", r);  <span class="comment">// never runs</span>
}
let s = &x;  <span class="comment">// ERROR: r might be used</span></pre>
</div>

**Can't distinguish indices:**

<div class="rust-code">
<pre>let mut v = vec![1, 2, 3];
let r = &mut v[0];
let s = &mut v[1];  <span class="comment">// ERROR: two borrows of v</span></pre>
</div>

### Curry-Howard

Types are propositions. Programs are proofs.

`&'a T` is a proof of: "There exists SPACE containing T, and IDENTITY to it is valid for TIME span 'a."

A successful borrow check means: "All IDENTITY/TIME claims are consistent."

---

## Memory Ordering

Atomics make operations indivisible. But when does a write in one TIME line become visible in another?

**Hardware defers coherence:**

- **Store buffers:** Writes queue locally before reaching RAM.
- **Caches:** Each core copies values locally. Writes don't instantly propagate.
- **Reordering:** CPU executes out of order for performance.

All three: optimizations assuming single TIME line. Multiple TIME lines expose the deferred sync.

### The Orderings

**Relaxed** — Atomic access only. No visibility guarantees.

**Acquire/Release** — Coherence at sync points.

Release: everything before this point visible before this write.

Acquire: everything after this sees writes from before a Release.

```
TIME line A:                TIME line B:

write x = 1
write y = 2
Release(flag = true)  ───>  Acquire(flag)
                            read y  // sees 2
                            read x  // sees 1
```

**SeqCst** — Total order. All TIME lines observe same sequence. Expensive.

| Ordering | Guarantee | Use |
|----------|-----------|-----|
| Relaxed | Operation is atomic | Counters |
| Release | Writes before visible to Acquire | Producer |
| Acquire | Reads after see writes before Release | Consumer |
| AcqRel | Both | Read-modify-write |
| SeqCst | Global total order | When in doubt |

---

## Syntax

Rust overloads `&` and `mut` based on position.

### Position Rules

| Position | `&mut x` means |
|----------|----------------|
| Expression (right of `=`) | Produce reference to x |
| Pattern (left of `=`) | Expect reference, bind target |
| Type (right of `:`) | Type "reference to T" |

### Examples

**Expression:**
<div class="rust-code">
<pre>let p = &mut x;  <span class="comment">// produce exclusive IDENTITY</span></pre>
</div>

**Pattern:**
<div class="rust-code">
<pre>let &mut target = some_ref;  <span class="comment">// follow IDENTITY, bind target</span></pre>
</div>

### Primitives

| Primitive | What |
|-----------|------|
| `&` | shared IDENTITY |
| `&mut` | exclusive IDENTITY |
| `mut` | rebindable binding |

`&mut` is one token. There is no `mut&`.

**Rebind vs mutate:**

<div class="rust-code">
<pre>let mut x = &y;
x = &z;         <span class="comment">// rebind: x now points elsewhere</span>

let x = &mut y;
*x = 5;         <span class="comment">// mutate: change what x points to</span></pre>
</div>

### Clarifying Syntax

What if operations were explicit?

| Current | Clarified | Meaning |
|---------|-----------|---------|
| `let x = y` | `let x = move_or_copy(y)` | compiler chooses |
| `let x = &y` | `let x = shared(y)` | shared IDENTITY |
| `let x = &mut y` | `let x = exclusive(y)` | exclusive IDENTITY |
| `let mut x = 5` | `let rebindable(x) = 5` | binding can change |
| `let &x = r` | `let x = *r` | dereference |

### Framework Translation

| Rust | Framework |
|------|-----------|
| value | Occupies SPACE |
| binding | Name referring to SPACE |
| reference | IDENTITY pointing to SPACE |
| scope | TIME boundary for validity |
| `&x` (expression) | Create shared IDENTITY |
| `&mut x` (expression) | Create exclusive IDENTITY |
| `&x` (pattern) | Follow IDENTITY, bind result |
