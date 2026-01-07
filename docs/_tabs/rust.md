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

<div></div>

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

The compiler works in compile-time projection: it tracks names (not addresses), analyzes CFG regions (not execution), and reasons about types (not bytes). It proves coherence there. The proof transfers to runtime.

This is what "zero-cost" means. Verification happens before execution. Nothing carries over except the guarantee.

Confusion arises from conflating projections. "Lifetime" sounds like runtime duration—it's a compile-time region. "&mut" sounds like mutation—it's exclusive IDENTITY. The borrow checker doesn't run at runtime. It operates on names and regions, then erases them.

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

**static** — Bridges projections. Initialized at compile-time, lives in data segment at runtime. Has a stable address. Must be immutable or use interior mutability with atomics.

<div class="rust-code">
<pre>static TABLE: [u8; 256] = [/* ... */];
static COUNTER: AtomicU32 = AtomicU32::new(0);</pre>
</div>

**static mut** — Runtime mutation of global IDENTITY. Rust can't prove safety. Every access requires `unsafe`.

<div class="rust-code">
<pre>static mut COUNTER: u32 = 0;
unsafe { COUNTER += 1; }</pre>
</div>

**let / let mut** — Runtime evaluation, stack SPACE. `mut` allows rebinding the name, not mutation through it.

<div class="rust-code">
<pre>let x = 5;           <span class="comment">// immutable binding</span>
let mut y = 5;       <span class="comment">// can rebind: y = 10;</span></pre>
</div>

---

## References

<table class="derived-table">
<tr><th>Construct</th><th>IDENTITY</th><th>TIME</th><th>Coherence</th></tr>
<tr><td><code>&T</code></td><td>Shared</td><td>Frozen</td><td>Many allowed, no mutation</td></tr>
<tr><td><code>&mut T</code></td><td>Exclusive</td><td>Flows</td><td>One allowed, mutation ok</td></tr>
</table>

**&T — Shared reference:**

<div class="rust-code">
<pre>let x = vec![1, 2, 3];
let r1 = &x;
let r2 = &x;  <span class="comment">// multiple shared refs OK</span></pre>
</div>

IDENTITY is shared (multiple paths to same data). TIME is frozen (no mutation through any path).

**&mut T — Exclusive reference:**

<div class="rust-code">
<pre>let mut x = vec![1, 2, 3];
let r = &mut x;
r.push(4);  <span class="comment">// exclusive access</span></pre>
</div>

IDENTITY is unique (only one path). TIME flows (mutation allowed).

<div class="derived-box">
The rule: !(shared IDENTITY && mutation)
</div>

Expressed as: many `&T` OR one `&mut T`, never both.

---

## IDENTITY at Runtime

**RAM can reference itself.**

RAM locations have addresses. Addresses are numbers. Numbers are data. Data can be stored in RAM. Therefore: RAM can store addresses of its own locations.

```
┌─────────┬───────────────┐
│  100    │   5       ← x │ ←──┐
├─────────┼───────────────┤    │
│  108    │  100      ← r │ ───┘
└─────────┴───────────────┘
```

This enables references, sharing, and complex structures (graphs, trees, linked lists).

**Registers have no address.**

Registers hold bytes but lack addresses. Taking a reference forces the value to stack—the compiler can't point to a register.

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

**Zero-cost safety:**

| Thing | Runtime cost |
|-------|--------------|
| Address | 8 bytes |
| Indirection | Following address |
| Safety rules | Nothing—proven at compile time |

---

## Stack and Heap

**Stack SPACE is automatic.**

Variables live on the stack. When they go out of scope, their SPACE is reclaimed. The compiler knows the size at compile time.

**Heap SPACE is dynamic.**

Sometimes you need SPACE whose size isn't known at compile time, or SPACE that outlives the current scope. This SPACE lives on the heap.

<div class="rust-code">
<pre>let b = Box::new(String::from("hello"));</pre>
</div>

`Box::new(x)` allocates SPACE on the heap, puts x there, and returns owning coordinates.

**Three kinds of coordinates:**

| Rust | Ownership | Cleanup |
|------|-----------|---------|
| `&y` | Borrowing | Not responsible |
| `&mut y` | Borrowing | Not responsible |
| `Box::new(x)` | Owning | Frees heap when dropped |

**When owning coordinates drop, heap SPACE is freed:**

<div class="rust-code">
<pre>{
    let b = Box::new(5);
}  <span class="comment">// b drops → heap freed</span></pre>
</div>

No garbage collector. No manual `free()`. The compiler inserts cleanup at scope end.

---

## Ownership and Move

<div class="rust-code">
<pre>let v = vec![1, 2, 3];
let w = v;  <span class="comment">// MOVE: v is now invalid</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>What happens</th></tr>
<tr><td>IDENTITY</td><td>Transferred — old binding invalid</td></tr>
<tr><td>SPACE</td><td>Same or new (compiler decides)</td></tr>
<tr><td>Coherence</td><td>Only one owner → no aliasing</td></tr>
</table>

Move semantics eliminate shared IDENTITY by default. No coherence problem if only one owner exists.

**C++ contrast:**

<div class="rust-code">
<pre><span class="comment">std::vector<int> w = v;       // COPY by default</span>
<span class="comment">std::vector<int> w = std::move(v);  // explicit move, v "valid but empty"</span></pre>
</div>

In Rust, the compiler enforces that moved-from values cannot be used. In C++, you can still access moved-from values—the compiler doesn't prevent it.

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

Types like `i32`, `bool`, `f64`, `char` implement `Copy`.

**When copying bytes fails:**

For `String`, SPACE contains coordinates to heap SPACE:

```
┌─────────────────┐
│ addr: 0x1234    │     Heap:
│ len: 5          │     ┌───────────┐
│ capacity: 5     │     │ h e l l o │
└─────────────────┘     └───────────┘
```

Copying bytes would create two owners of the same heap SPACE. Instead, Rust requires explicit choice:

| Strategy | Method |
|----------|--------|
| Deep copy | `.clone()` — new heap allocation |
| Share with refcount | `Rc::clone()` |
| Transfer ownership | Move |

<table class="derived-table">
<tr><th>Type</th><th>Copy?</th><th>Clone?</th><th>Why</th></tr>
<tr><td><code>i32</code></td><td>Yes</td><td>Yes</td><td>Self-contained</td></tr>
<tr><td><code>&T</code></td><td>Yes</td><td>Yes</td><td>Coordinates, doesn't own</td></tr>
<tr><td><code>String</code></td><td>No</td><td>Yes</td><td>Owns heap—must choose strategy</td></tr>
<tr><td><code>Vec<T></code></td><td>No</td><td>Yes</td><td>Owns heap</td></tr>
</table>

Copy types get implicit duplication. Clone types require explicit `.clone()`. Expensive operations are visible.

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

To combine shared ownership with mutation:

<table class="derived-table">
<tr><th>Type</th><th>SPACE × IDENTITY</th><th>TIME</th></tr>
<tr><td><code>Rc&lt;T&gt;</code></td><td>Shared ownership</td><td>Frozen</td></tr>
<tr><td><code>Rc&lt;RefCell&lt;T&gt;&gt;</code></td><td>Shared ownership</td><td>Runtime checked</td></tr>
<tr><td><code>Arc&lt;T&gt;</code></td><td>Shared ownership (atomic)</td><td>Frozen</td></tr>
<tr><td><code>Arc&lt;Mutex&lt;T&gt;&gt;</code></td><td>Shared ownership (atomic)</td><td>Serialized</td></tr>
</table>

Ownership handles SPACE × IDENTITY (who can access, who frees). Interior mutability handles TIME (when mutation is safe).

---

## Interior Mutability

Interior mutability moves verification from compile-time to runtime projection. Same rules, different when checked.

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

**The hierarchy:**

```
UnsafeCell<T>           ← primitive: makes &T + mutation not UB
    │
    ├── Cell<T>         ← single-thread, Copy only
    ├── RefCell<T>      ← single-thread, runtime borrow check
    ├── Mutex<T>        ← multi-thread, OS lock
    ├── RwLock<T>       ← multi-thread, many-read or one-write
    └── Atomic*         ← multi-thread, hardware
```

---

**Cell** — No references into contents. Only copy values in/out.

<div class="rust-code">
<pre>let x = Cell::new(5);
x.set(10);       <span class="comment">// mutate through shared ref</span>
let v = x.get(); <span class="comment">// returns copy</span></pre>
</div>

---

**RefCell** — Runtime borrow checker. Panics on violation.

<div class="rust-code">
<pre>let x = RefCell::new(vec![1, 2, 3]);
let r = x.borrow();      <span class="comment">// shared</span>
let s = x.borrow();      <span class="comment">// OK</span>
<span class="comment">// x.borrow_mut();       // PANIC: already borrowed</span></pre>
</div>

Same logic as compile-time borrow checker, run at runtime.

---

**Mutex** — Serialize TIME. Only one accessor.

<div class="rust-code">
<pre>let x = Mutex::new(vec![1, 2, 3]);
let mut guard = x.lock().unwrap();
guard.push(4);
<span class="comment">// other threads block until guard drops</span></pre>
</div>

---

**Atomic** — Hardware arbitration. Individual operations indivisible.

<div class="rust-code">
<pre>let x = AtomicU64::new(0);
x.fetch_add(1, Ordering::SeqCst);</pre>
</div>

---

**When to use:**

| Situation | Use |
|-----------|-----|
| Simple flag, single thread | `Cell<T>` |
| Complex data, single thread | `RefCell<T>` |
| Shared across threads | `Mutex<T>` |
| Read-heavy, multi-thread | `RwLock<T>` |
| Counter, multi-thread | `AtomicU64` |
| Building sync primitives | `UnsafeCell<T>` |
| Borrow checker accepts code | None needed |

---

## Thread Safety

<table class="derived-table">
<tr><th>Trait</th><th>Meaning</th></tr>
<tr><td><code>Send</code></td><td>IDENTITY can transfer across thread boundary</td></tr>
<tr><td><code>Sync</code></td><td>Shared IDENTITY (<code>&T</code>) safe across threads</td></tr>
</table>

<div class="rust-code">
<pre><span class="comment">// Rc<T> is !Send and !Sync — single-thread only</span>
<span class="comment">// Arc<T> is Send and Sync — multi-thread safe</span>
<span class="comment">// Mutex<T>: T is Send → Mutex<T> is Send + Sync</span></pre>
</div>

The compiler checks these at compile time. If your code compiles, thread safety is proven.

---

## Summary

<table class="derived-table">
<tr><th>Problem</th><th>Rust's Solution</th></tr>
<tr><td>When to evaluate?</td><td><code>const</code> (compile) vs <code>let</code> (runtime)</td></tr>
<tr><td>Where to store?</td><td>Inlined / stack / heap / static</td></tr>
<tr><td>Aliasing + mutation?</td><td>Borrow checker: <code>&T</code> OR <code>&mut T</code></td></tr>
<tr><td>Global state?</td><td><code>static</code> with atomics or interior mutability</td></tr>
<tr><td>Cross-thread sharing?</td><td><code>Send</code> / <code>Sync</code> traits</td></tr>
<tr><td>Escaping the rules?</td><td><code>unsafe</code> — you prove coherence</td></tr>
</table>

<div class="derived-box">
Rust's bet: prove coherence in compile-time projection. Pay with complexity. Gain zero-cost guarantees at runtime.
</div>

---

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

## Lifetimes & Borrow Checker

### Lifetimes as Compile-time TIME

A lifetime is a set of CFG points where IDENTITY remains valid. It's compile-time TIME, not runtime duration.

<div class="derived-box">
'a = region in compile-time TIME where reference is valid
</div>

`&'a T` means: shared IDENTITY to SPACE, valid during region 'a.

`&'a mut T` means: exclusive IDENTITY to SPACE, valid during region 'a.

The borrow checker proves:
1. IDENTITY doesn't outlive SPACE
2. Shared IDENTITY + mutation don't overlap in TIME

---

### The Borrow Checker

The borrow checker operates entirely in compile-time projection. It tracks names (not addresses), analyzes CFG regions (not execution), reasons about types (not bytes).

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

---

### Phase 1: MIR

MIR makes IDENTITY operations explicit:

<div class="rust-code">
<pre>_2 = &(*_1)     <span class="comment">// borrow: create IDENTITY</span>
_3 = use(_2)   <span class="comment">// use: access through IDENTITY</span>
drop(_2)       <span class="comment">// end: IDENTITY released</span></pre>
</div>

---

### Phase 2: CFG

The function becomes a directed graph. Each node is a program point, edges are possible transitions.

<div class="rust-code">
<pre>fn example(cond: bool) {
    let mut x = 5;       <span class="comment">// BB0</span>
    let r = &mut x;      <span class="comment">// BB0</span>
    if cond {
        *r += 1;         <span class="comment">// BB1</span>
    }
    println!("{}", r);   <span class="comment">// BB2</span>
}</pre>
</div>

TIME in a program is not linear. The CFG captures all possible orderings.

---

### Phase 3: Liveness

Liveness asks: "From this point, on some future path, will this IDENTITY be used?"

<div class="rust-code">
<pre>LIVE(r, P) ≡ "reference r is used on some path from P forward"</pre>
</div>

Computed via backward dataflow. The IDENTITY must be valid at all points where it's live.

---

### Phase 4: Region Inference

A region is a set of CFG points. Regions form a lattice under subset ordering.

The solver finds the minimal region satisfying all constraints:
- `'a: P` — region 'a includes point P
- `'a: 'b` — region 'a contains region 'b

Fixed-point iteration: expand regions until stable. Guaranteed to terminate (finite lattice, monotonic operations).

---

### Phase 5: Conflict Detection

With regions computed, check if SPACE × TIME × IDENTITY conflict:

<div class="rust-code">
<pre>ERROR if:
  ∃ borrows B1, B2 of same location where:
    - regions overlap
    - AND one is mutable
    - AND B1 ≠ B2</pre>
</div>

---

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

---

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
let s = &mut v[1];  <span class="comment">// ERROR: two &mut into v</span></pre>
</div>

The checker sees both as "borrow of v". Proving `[0] ≠ [1]` requires value tracking.

---

### Curry-Howard

Types are propositions. Programs are proofs.

`&'a T` is a proof of: "There exists SPACE containing T, and IDENTITY to it is valid for TIME span 'a."

A successful borrow check means: "All IDENTITY/TIME claims are consistent."

---

### Equivalences

<div class="equivalences-box">
lifetime 'a       ≅  validity region    ≅  compile-time TIME<br>
liveness          ≅  "replica in use?"  ≅  IDENTITY tracking<br>
outlives 'a: 'b   ≅  TIME containment   ≅  'a ⊇ 'b<br>
conflict          ≅  coherence violation<br>
abstract interp.  ≅  decidable approximation
</div>

---

## Memory Ordering

Atomics make operations indivisible. But when does a write in one TIME line become visible in another?

**Hardware defers coherence:**

- **Store buffers:** Writes queue locally before reaching RAM. Other cores see old values until drain.
- **Caches:** Each core copies values locally. Writes don't instantly propagate.
- **Reordering:** CPU executes out of order for performance.

All three: optimizations assuming single TIME line. Multiple TIME lines expose the deferred sync.

---

### The Orderings

**Relaxed** — Atomic access only. No visibility guarantees. Other TIME lines may see writes in any order.

**Acquire/Release** — Coherence at sync points.

Release: drain buffer, flush writes. Everything before this point visible before this write.

Acquire: invalidate cache, pull fresh. Everything after this sees writes from before a Release.

```
TIME line A:                TIME line B:

write x = 1
write y = 2
Release(flag = true)  ───>  Acquire(flag)
                            read y  // sees 2
                            read x  // sees 1
```

**SeqCst** — Total order. All TIME lines observe same sequence. Full visibility. Expensive.

---

### Summary

| Ordering | Guarantee | Use |
|----------|-----------|-----|
| Relaxed | Operation is atomic | Counters |
| Release | Writes before visible to Acquire | Producer |
| Acquire | Reads after see writes before Release | Consumer |
| AcqRel | Both | Read-modify-write |
| SeqCst | Global total order | When in doubt |

Memory ordering is choosing how much coherence to force.

---

## Pedantic Naming

Rust's terminology conflates compile-time and runtime projections.

### Core Terms

| Rust | Suggests | Actually |
|------|----------|----------|
| "lifetime" | runtime duration | compile-time region |
| `&mut T` | mutation allowed | exclusive IDENTITY |
| "borrow" | runtime lending | compile-time IDENTITY relationship |
| "move" | data relocation | IDENTITY transfer |
| "borrow checker" | rule checker | coherence prover |

### References

| Rust | Clearer |
|------|---------|
| `&T` | `&shared T` |
| `&mut T` | `&unique T` |

`&mut T` is about exclusive IDENTITY, not mutability. The name obscures this.

### Ownership

| Rust | Clearer |
|------|---------|
| Ownership | Unique binding |
| Move | Transfer |
| Copy | Duplicate |
| Drop | Destruct |

"Move" suggests data relocates. It doesn't. IDENTITY transfers.

### Thread Safety

| Rust | Clearer |
|------|---------|
| `Send` | `ThreadTransfer` |
| `Sync` | `ThreadShare` |

### Error Messages

| Rust says | Means |
|-----------|-------|
| "cannot borrow as mutable because borrowed as immutable" | Exclusive IDENTITY requested but shared exists |
| "borrowed value does not live long enough" | IDENTITY outlives SPACE |
| "use of moved value" | IDENTITY accessed after transfer |

### Translation Table

| Rust | CS Concept | Triangle |
|------|------------|----------|
| Ownership | Unique handle | Unique IDENTITY |
| Borrowing | Aliasing | Creating IDENTITY |
| Lifetime | Reference validity interval | TIME span of IDENTITY |
| Move | Linear consumption | IDENTITY transfer |
| Borrow checker | Alias analysis + dataflow | IDENTITY×TIME coherence proof |
| Interior mutability | Runtime checks | Deferred verification |
| `Send` | Thread-transferable | IDENTITY crosses thread |
| `Sync` | Thread-shareable | Shared IDENTITY parallel-safe |

---

## Pedantic Syntax

Rust overloads `&` and `mut` based on position.

### Position Rules

| Position | `&mut x` means |
|----------|----------------|
| Expression (right of `=`) | Produce reference to x |
| Pattern (left of `=`) | Expect reference, bind target |
| Type (right of `:`) | Type "reference to T" |

### Examples

**Type:**
<div class="rust-code">
<pre>let x: &mut i32 = ...;  <span class="comment">// x is reference to i32</span></pre>
</div>

**Expression:**
<div class="rust-code">
<pre>let p = &mut x;  <span class="comment">// produce reference to x</span></pre>
</div>

**Pattern:**
<div class="rust-code">
<pre>let &mut target = some_ref;  <span class="comment">// follow reference, bind target</span></pre>
</div>

### Primitives

| Primitive | What |
|-----------|------|
| `&` | shared reference |
| `&mut` | exclusive reference |
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

Current syntax overloads position. Clarified syntax makes operations explicit.

### Framework Translation

| Rust | Framework |
|------|-----------|
| value | Occupies SPACE |
| binding | Name referring to SPACE |
| reference | IDENTITY pointing to SPACE |
| scope | TIME boundary for validity |
| `&x` (expression) | Create shared IDENTITY to x's SPACE |
| `&mut x` (expression) | Create exclusive IDENTITY to x's SPACE |
| `&x` (pattern) | Follow IDENTITY, bind result |
