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

Rust's memory model is a set of choices on the SPACE / TIME / IDENTITY triangle.

<div class="derived-box">
Core strategy: <b>prove coherence at compile time</b>.
</div>

---

### Bindings

<table class="derived-table">
<tr><th>Construct</th><th>TIME</th><th>SPACE</th><th>IDENTITY</th><th>Coherence</th></tr>
<tr><td><code>const</code></td><td>Compile</td><td>Inlined</td><td>N/A</td><td>None needed</td></tr>
<tr><td><code>static</code></td><td>Compile init</td><td>Data segment</td><td>Global</td><td>Immutable or atomic</td></tr>
<tr><td><code>static mut</code></td><td>Runtime</td><td>Data segment</td><td>Global</td><td><code>unsafe</code> — you prove it</td></tr>
<tr><td><code>let</code></td><td>Runtime</td><td>Stack</td><td>Owned</td><td>Scope-limited</td></tr>
<tr><td><code>let mut</code></td><td>Runtime</td><td>Stack</td><td>Owned</td><td>Scope-limited</td></tr>
</table>

---

### const

<div class="rust-code">
<pre>const MAX: u32 = 100;
const BUFFER: [u8; 1024] = [0; 1024];</pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Choice</th></tr>
<tr><td>TIME</td><td>Compile — value must be computable by the compiler</td></tr>
<tr><td>SPACE</td><td>Inlined — substituted at every use site, no address</td></tr>
<tr><td>IDENTITY</td><td>N/A — each use is an independent copy</td></tr>
<tr><td>Coherence</td><td>None needed — no shared location exists</td></tr>
</table>

**Why it exists:**
- Array sizes require compile-time known values: `[T; N]`
- Match arms need compile-time constants
- Zero runtime cost — no memory, no lookup

**Abstraction leak:**

<div class="rust-code">
<pre>const X: u32 = 5;
let a = &X;
let b = &X;
<span class="comment">// a and b may have DIFFERENT addresses — each is a fresh inline copy</span></pre>
</div>

**Contrast — C/C++:**

<div class="rust-code">
<pre><span class="comment">const int MAX = 100;      // C: may still have address, not truly inlined</span>
<span class="comment">constexpr int MAX = 100;  // C++11: equivalent to Rust const</span>
<span class="comment">#define MAX 100           // Preprocessor: textual substitution, no type safety</span></pre>
</div>

---

### static

<div class="rust-code">
<pre>static TABLE: [u8; 256] = [/* ... */];
static COUNTER: AtomicU32 = AtomicU32::new(0);</pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Choice</th></tr>
<tr><td>TIME</td><td>Compile-time initialization, runtime reads</td></tr>
<tr><td>SPACE</td><td>Data segment — one fixed address for program lifetime</td></tr>
<tr><td>IDENTITY</td><td>Global — all code sees same location</td></tr>
<tr><td>Coherence</td><td>Must be immutable OR use interior mutability with atomics</td></tr>
</table>

**Why it exists:**
- Need stable address: `&'static T`
- Large data that shouldn't be duplicated at every use site
- Global state with proper synchronization

**Contrast — const:**

<div class="rust-code">
<pre>const  DATA: [u8; 1000] = [0; 1000];  <span class="comment">// 1000 bytes inlined at EACH use</span>
static DATA: [u8; 1000] = [0; 1000];  <span class="comment">// 1000 bytes in binary ONCE</span></pre>
</div>

---

### static mut

<div class="rust-code">
<pre>static mut COUNTER: u32 = 0;

unsafe {
    COUNTER += 1;  <span class="comment">// Every access requires unsafe</span>
}</pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Choice</th></tr>
<tr><td>TIME</td><td>Runtime mutations</td></tr>
<tr><td>SPACE</td><td>Data segment</td></tr>
<tr><td>IDENTITY</td><td>Global — all threads can alias</td></tr>
<tr><td>Coherence</td><td><code>unsafe</code> — YOU prove no data races</td></tr>
</table>

**Why unsafe:**
- Global IDENTITY + free TIME = coherence problem
- Rust can't prove safety → you must
- Prefer `AtomicU32`, `Mutex<T>`, or `OnceLock<T>`

---

### let (immutable)

<div class="rust-code">
<pre>let x = compute_something();
let y = &x;  <span class="comment">// Borrow: shared reference</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Choice</th></tr>
<tr><td>TIME</td><td>Runtime — evaluated when reached</td></tr>
<tr><td>SPACE</td><td>Stack — lives in current frame</td></tr>
<tr><td>IDENTITY</td><td>Owned — one owner, borrowable</td></tr>
<tr><td>Coherence</td><td>Borrow checker proves safety</td></tr>
</table>

**Why immutable by default:**
- Shared references (`&T`) are safe with no mutation
- Enables the rule: many `&T` OR one `&mut T`

---

### let mut

<div class="rust-code">
<pre>let mut x = 5;
x += 1;

let r = &mut x;  <span class="comment">// Exclusive borrow</span>
*r += 1;</pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Choice</th></tr>
<tr><td>TIME</td><td>Runtime, mutable</td></tr>
<tr><td>SPACE</td><td>Stack</td></tr>
<tr><td>IDENTITY</td><td>Unique access enforced</td></tr>
<tr><td>Coherence</td><td>Borrow checker: <code>&mut</code> is exclusive</td></tr>
</table>

**The rule:**

<div class="derived-box">
!(shared_identity && mutation)
</div>

Expressed as: many `&T` OR one `&mut T`, never both.

---

### References

<table class="derived-table">
<tr><th>Construct</th><th>IDENTITY</th><th>TIME</th><th>Coherence</th></tr>
<tr><td><code>&T</code></td><td>Shared</td><td>Frozen</td><td>Many allowed, no mutation</td></tr>
<tr><td><code>&mut T</code></td><td>Exclusive</td><td>Flows</td><td>One allowed, mutation ok</td></tr>
</table>

**&T — Shared Reference:**

<div class="rust-code">
<pre>let x = vec![1, 2, 3];
let r1 = &x;
let r2 = &x;  <span class="comment">// OK: multiple shared refs</span>
<span class="comment">// x.push(4);  // ERROR: can't mutate while borrowed</span></pre>
</div>

- IDENTITY is shared (multiple paths to same data)
- TIME is frozen (no mutation through any path)
- Coherence: no problem — frozen TIME

**&mut T — Exclusive Reference:**

<div class="rust-code">
<pre>let mut x = vec![1, 2, 3];
let r = &mut x;
r.push(4);    <span class="comment">// OK: exclusive access</span>
<span class="comment">// let r2 = &x;  // ERROR: can't alias while mutably borrowed</span></pre>
</div>

- IDENTITY is unique (only one path)
- TIME flows (mutation allowed)
- Coherence: no problem — unique IDENTITY

---

### Ownership and Move

<div class="rust-code">
<pre>let v = vec![1, 2, 3];
let w = v;  <span class="comment">// MOVE: v is now invalid</span>
<span class="comment">// println!("{:?}", v);  // ERROR: use of moved value</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Choice</th></tr>
<tr><td>IDENTITY</td><td>Transferred — old binding invalid</td></tr>
<tr><td>SPACE</td><td>Same or new (compiler decides)</td></tr>
<tr><td>Coherence</td><td>Only one owner → no aliasing</td></tr>
</table>

**Why move semantics:**
- Eliminates shared IDENTITY by default
- No coherence problem if only one owner exists

**Contrast — C++:**

<div class="rust-code">
<pre>std::vector&lt;int&gt; v = {1, 2, 3};
std::vector&lt;int&gt; w = v;  <span class="comment">// COPY by default</span>
<span class="comment">// Both v and w valid — independent copies</span>
std::vector&lt;int&gt; w = std::move(v);  <span class="comment">// Explicit move</span>
<span class="comment">// v is valid but "empty" — not enforced by compiler</span></pre>
</div>

---

### Copy vs Clone

<table class="derived-table">
<tr><th>Trait</th><th>IDENTITY</th><th>When</th></tr>
<tr><td><code>Copy</code></td><td>Creates new, implicit</td><td>Bitwise copy, cheap</td></tr>
<tr><td><code>Clone</code></td><td>Creates new, explicit</td><td>Potentially expensive</td></tr>
</table>

<div class="rust-code">
<pre>let a: i32 = 5;
let b = a;      <span class="comment">// Copy: both valid, independent</span>
<span class="comment">// i32 is Copy — stack-only, no heap resources</span>

let v = vec![1, 2, 3];
let w = v.clone();  <span class="comment">// Clone: explicit, allocates new heap</span></pre>
</div>

**Copy = value semantics:**
- New IDENTITY — no shared state
- No coherence problem — independent copies

---

### Interior Mutability

When you need shared IDENTITY + mutation:

<table class="derived-table">
<tr><th>Type</th><th>SPACE</th><th>Coherence Strategy</th></tr>
<tr><td><code>Cell&lt;T&gt;</code></td><td>Stack/heap</td><td>Single-thread, no refs into</td></tr>
<tr><td><code>RefCell&lt;T&gt;</code></td><td>Stack/heap</td><td>Single-thread, runtime borrow check</td></tr>
<tr><td><code>Mutex&lt;T&gt;</code></td><td>Heap</td><td>Multi-thread, serialize TIME</td></tr>
<tr><td><code>RwLock&lt;T&gt;</code></td><td>Heap</td><td>Multi-thread, many-read or one-write</td></tr>
<tr><td><code>Atomic*</code></td><td>Stack/heap</td><td>Multi-thread, hardware arbitration</td></tr>
</table>

<div class="rust-code">
<pre>use std::cell::RefCell;

let data = RefCell::new(vec![1, 2, 3]);
let r1 = data.borrow();      <span class="comment">// Shared borrow at runtime</span>
let r2 = data.borrow();      <span class="comment">// OK</span>
<span class="comment">// let w = data.borrow_mut(); // PANIC: already borrowed</span></pre>
</div>

**Why it exists:**
- Borrow checker is conservative — rejects some safe programs
- Interior mutability opts into runtime checking
- `unsafe` under the hood, safe API on top

---

### Thread Safety Markers

<table class="derived-table">
<tr><th>Trait</th><th>Meaning</th></tr>
<tr><td><code>Send</code></td><td>Safe to transfer to another thread (move IDENTITY across threads)</td></tr>
<tr><td><code>Sync</code></td><td>Safe to share reference across threads (<code>&T</code> can be sent)</td></tr>
</table>

<div class="rust-code">
<pre><span class="comment">// Rc&lt;T&gt; is !Send and !Sync — single-thread only</span>
<span class="comment">// Arc&lt;T&gt; is Send and Sync — multi-thread safe</span>

<span class="comment">// Mutex&lt;T&gt;: T is Send → Mutex&lt;T&gt; is Send + Sync</span>
<span class="comment">// The Mutex provides the coherence strategy</span></pre>
</div>

**The insight:**
- `Send` = IDENTITY can cross thread boundary
- `Sync` = shared IDENTITY across threads is safe
- Compiler checks these at compile time

---

### Summary — Rust's Strategy

<table class="derived-table">
<tr><th>Problem</th><th>Rust's Solution</th></tr>
<tr><td>When to evaluate?</td><td><code>const</code> (compile) vs <code>let</code> (runtime)</td></tr>
<tr><td>Where to store?</td><td>Inlined / stack / heap / static — explicit control</td></tr>
<tr><td>Aliasing + mutation?</td><td>Borrow checker: <code>&T</code> OR <code>&mut T</code>, never both</td></tr>
<tr><td>Global state?</td><td><code>static</code> with atomics or interior mutability</td></tr>
<tr><td>Cross-thread sharing?</td><td><code>Send</code> / <code>Sync</code> traits, checked at compile time</td></tr>
<tr><td>Escaping the rules?</td><td><code>unsafe</code> — you prove coherence</td></tr>
</table>

---

### Triangle Position

<div class="triangle-diagram">
<pre>
                      TIME
                       △
                      /|\
                     / | \
         "let mut"  /  |  \  "const" (frozen at compile time)
         "&mut T"  /   |   \
                  /    |    \
                 / "&T"     \
                /  (frozen)   \
               /       |       \
            SPACE -----+------ IDENTITY
                       |
              "Ownership: unique IDENTITY"
              "Borrowing: controlled sharing"
              "Move: transfer IDENTITY"
</pre>
</div>

<div class="derived-box">
Rust's bet: constrain IDENTITY (ownership) and TIME (borrowing rules) at compile time. Pay with complexity. Gain zero-cost coherence guarantees.
</div>

---

### Equivalences

<div class="equivalences-box">
Rust const         ≅  inlined value      ≅  no derived data exists<br>
Rust static        ≅  single replica     ≅  one source of truth<br>
Rust &T            ≅  read replica       ≅  frozen TIME, shared IDENTITY<br>
Rust &mut T        ≅  exclusive lock     ≅  unique IDENTITY, TIME flows<br>
Rust Mutex&lt;T&gt;      ≅  distributed lock   ≅  serialize TIME<br>
Rust move          ≅  migration          ≅  transfer IDENTITY<br>
Rust Clone         ≅  snapshot           ≅  new independent IDENTITY<br>
Rust unsafe        ≅  "trust me"         ≅  you prove coherence
</div>

---

## Lifetimes & Borrow Checker

### Lifetimes in the Triangle

A lifetime is not about memory allocation. It's about **IDENTITY validity in TIME**.

<div class="derived-box">
LIFETIME 'a = the set of program points where IDENTITY to a SPACE remains valid
</div>

A reference `&'a T` means:

<table class="derived-table">
<tr><th>Axis</th><th>Meaning</th></tr>
<tr><td>IDENTITY</td><td>This reference points to some SPACE</td></tr>
<tr><td>TIME</td><td>That SPACE is valid for duration <code>'a</code></td></tr>
<tr><td>Coherence</td><td>Frozen — no mutation through other paths during <code>'a</code></td></tr>
</table>

A reference `&'a mut T` means:

<table class="derived-table">
<tr><th>Axis</th><th>Meaning</th></tr>
<tr><td>IDENTITY</td><td>This reference points to some SPACE, exclusively</td></tr>
<tr><td>TIME</td><td>That SPACE is valid for duration <code>'a</code></td></tr>
<tr><td>Coherence</td><td>Exclusive — no other IDENTITY to this SPACE during <code>'a</code></td></tr>
</table>

The borrow checker proves:
1. **IDENTITY doesn't outlive SPACE** — no dangling pointers
2. **Shared IDENTITY + mutation don't overlap in TIME** — no data races

---

### Overview

<div class="pipeline-diagram">
<pre>
              Source Code
                   │
                   ▼
           ┌───────────────┐
           │  Lower to MIR │ ◄── IDENTITY ops explicit
           └───────────────┘     (operational semantics)
                   │
                   ▼
           ┌───────────────┐
           │   Build CFG   │ ◄── branching TIME
           └───────────────┘     (automata theory)
                   │
                   ▼
           ┌───────────────┐
           │   Liveness    │ ◄── TIME × IDENTITY
           │   Analysis    │     (temporal logic, ◇)
           └───────────────┘
                   │
                   ▼
           ┌───────────────┐
           │    Region     │ ◄── TIME spans
           │   Inference   │     (lattice theory)
           └───────────────┘
                   │
                   ▼
           ┌───────────────┐
           │   Conflict    │ ◄── SPACE × TIME × IDENTITY
           │   Detection   │     (coherence check)
           └───────────────┘
                   │
              ▼         ▼
            OK        ERROR
         (proof)    (might be valid,
                     undecidable to prove)
</pre>
</div>

<table class="derived-table">
<tr><th>Phase</th><th>Triangle Axis</th><th>Computes</th><th>CS Foundation</th></tr>
<tr><td>MIR lowering</td><td>IDENTITY</td><td>Explicit borrow/move/drop</td><td>Operational semantics</td></tr>
<tr><td>CFG construction</td><td>TIME</td><td>Possible execution orderings</td><td>Automata theory (NFA)</td></tr>
<tr><td>Liveness analysis</td><td>TIME × IDENTITY</td><td>"Will this IDENTITY be used in future?"</td><td>Temporal logic (◇)</td></tr>
<tr><td>Region inference</td><td>TIME</td><td>Minimal TIME span for each IDENTITY</td><td>Lattice theory</td></tr>
<tr><td>Conflict detection</td><td>SPACE × TIME × IDENTITY</td><td>All three axes intersect?</td><td>Coherence check</td></tr>
</table>

---

### Computational Model

**Turing machine:**
- Tape → SPACE
- Head position → IDENTITY (what we're pointing to)
- Steps → TIME (linear)

But programs branch. TIME is not a line — it's a **directed graph of possible futures**.

<div class="derived-box">
Turing: TIME is linear         ──────────────────►<br><br>
Programs: TIME branches         ────┬────►<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;│<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└────►
</div>

The **Control Flow Graph** models branching TIME. Each node is a program point (moment in TIME), edges are possible transitions.

The borrow checker **tracks IDENTITY structure, forgets values**. It doesn't know `x == 5`, only that `r` points to `x`.

---

### Phase 1: MIR — Surfacing IDENTITY

MIR (Mid-level IR) makes IDENTITY operations explicit:

<div class="rust-code">
<pre><span class="comment">// Source</span>
fn example(x: &mut i32) {
    let y = &*x;
    println!("{}", y);
}

<span class="comment">// MIR (simplified)</span>
bb0:
    _2 = &(*_1)          <span class="comment">// borrow: create IDENTITY to SPACE</span>
    _3 = println(_2)     <span class="comment">// use: access through IDENTITY</span>
    drop(_2)             <span class="comment">// end: IDENTITY released</span>
    return</pre>
</div>

Every borrow, move, and drop is a statement about IDENTITY:
- `&x` → create IDENTITY to x's SPACE
- `move x` → transfer IDENTITY, invalidate old
- `drop(x)` → IDENTITY ends

**CS foundation:** Operational semantics — formally defining what each operation does to the IDENTITY structure.

---

### Phase 2: CFG — Modeling Branching TIME

The function becomes a directed graph:

<div class="rust-code">
<pre>fn example(cond: bool) {
    let mut x = 5;       <span class="comment">// BB0</span>
    let r = &mut x;      <span class="comment">// BB0: IDENTITY created</span>
    if cond {
        *r += 1;         <span class="comment">// BB1: IDENTITY used</span>
    } else {
        <span class="comment">// nothing       // BB2</span>
    }
    println!("{}", r);   <span class="comment">// BB3: IDENTITY used</span>
}</pre>
</div>

<div class="pipeline-diagram">
<pre>
        BB0
     (x, r = &mut x)
          │
        cond?
        /   \
      BB1   BB2
     (*r)   (-)
        \   /
         BB3
     (print r)
</pre>
</div>

TIME in a program is not linear. The CFG captures all possible TIME orderings.

**CS foundation:** Automata theory — the CFG is a non-deterministic finite automaton (NFA) over program points. The borrow checker must reason about all paths.

---

### Phase 3: Liveness — TIME × IDENTITY

Liveness asks: "From this point, on SOME future path, will this IDENTITY be used?"

<div class="derived-box">
LIVE(r, P) = "reference r is used on some path from P forward"
</div>

This is a temporal logic question. The modal operator ◇ means "eventually/possibly":

<div class="rust-code">
<pre>LIVE(r, P)  ≡  ◇(r is used)   at point P
            ≡  "there EXISTS a future where r is used"</pre>
</div>

Computed via backward dataflow:

<div class="rust-code">
<pre>LIVE_in(B) = USE(B) ∪ (LIVE_out(B) - DEF(B))
LIVE_out(B) = ∪ { LIVE_in(S) : S is successor of B }</pre>
</div>

For the example:
- `r` is live at: BB0 (after borrow), BB1, BB2, BB3
- The IDENTITY must be valid at all these TIME points

**CS foundation:** Temporal logic — reasoning about properties over branching time. Liveness is the ◇ operator computed via fixed-point iteration.

---

### Phase 4: Region Inference — TIME as Lattice

A **region** (lifetime) is a set of CFG points — a discrete representation of TIME.

**Constraint types:**

<table class="derived-table">
<tr><th>Constraint</th><th>Meaning</th><th>Formal</th></tr>
<tr><td><code>'a: P</code></td><td>Region 'a includes point P</td><td>P ∈ 'a</td></tr>
<tr><td><code>'a: 'b</code></td><td>Region 'a outlives 'b</td><td>'a ⊇ 'b</td></tr>
</table>

Regions form a **lattice** under subset ordering:

<div class="pipeline-diagram">
<pre>
        'static (all points)
           /    \
         'a      'b
           \    /
          'c (fewer points)
              |
             {} (empty)
</pre>
</div>

The solver finds the **minimal region** satisfying all constraints.

**Fixed-point iteration:**

1. Initialize all regions to minimal (empty or single point)
2. For each constraint, expand regions as needed
3. Repeat until no changes
4. Guaranteed to terminate: lattice is finite, operations are monotonic

**CS foundation:** Lattice theory — regions form a subset lattice. Fixed-point theorem guarantees termination for monotonic functions on finite lattices. Same math underlies abstract interpretation.

---

### Phase 5: Conflict Detection — Coherence Check

With regions computed, check if SPACE × TIME × IDENTITY conflict:

<div class="rust-code">
<pre>ERROR if:
  ∃ location L, borrows B1, B2 of L where:
    - regions(B1) ∩ regions(B2) ≠ ∅     <span class="comment">// TIME overlap</span>
    - AND (B1 is mut OR B2 is mut)      <span class="comment">// mutation</span>
    - AND B1 ≠ B2                       <span class="comment">// different IDENTITY paths</span></pre>
</div>

This is the coherence check: when all three axes intersect improperly, STATE can diverge.

Example:

<div class="rust-code">
<pre>let mut x = 5;
let r = &mut x;     <span class="comment">// B1: mut borrow, region starts</span>
let s = &x;         <span class="comment">// B2: shared borrow — ERROR</span>
println!("{}", r);  <span class="comment">// B1 region includes this point</span></pre>
</div>

The checker sees:
- Same SPACE: both borrow `x`
- TIME overlap: B2 starts while B1's region is active
- IDENTITY conflict: B1 is mut, B2 exists

**CS foundation:** This is the derived data coherence problem — shared IDENTITY + mutation in overlapping TIME = conflict.

---

### Decidability

Why does the borrow checker reject valid programs?

Perfect analysis would require solving:

<table class="derived-table">
<tr><th>Question</th><th>Difficulty</th></tr>
<tr><td>"Will this branch execute?"</td><td>Halting problem</td></tr>
<tr><td>"Do <code>v[i]</code> and <code>v[j]</code> alias?"</td><td>Undecidable for arbitrary i, j</td></tr>
<tr><td>"Will this value be used?"</td><td>Halting problem</td></tr>
</table>

The borrow checker uses **decidable approximation**:

<table class="derived-table">
<tr><th>Property</th><th>Perfect</th><th>Approximation</th></tr>
<tr><td>Branch</td><td>"Will this run?"</td><td>"Assume all branches might run"</td></tr>
<tr><td>Alias</td><td>"Same address?"</td><td>"Same base → might alias"</td></tr>
<tr><td>Use</td><td>"Will this be used?"</td><td>"Might be used on some path"</td></tr>
</table>

This is **abstract interpretation** — analyze over abstract domains (sets of points) instead of concrete values.

**Sound but incomplete:**
- OK → definitely safe (no false negatives)
- ERROR → might be safe (false positives exist)

**CS foundation:** Abstract interpretation, Rice's theorem — non-trivial semantic properties are undecidable for general programs. The borrow checker trades precision for decidability.

---

### Rejected Valid Programs

**Path-insensitive (can't prove branch won't run):**

<div class="rust-code">
<pre>let mut x = 5;
let r = &mut x;
if false {
    println!("{}", r);  <span class="comment">// Never runs, but checker doesn't know</span>
}
let s = &x;  <span class="comment">// ERROR: r might be used</span></pre>
</div>

Checker sees: r's region includes the (unreachable) use, extends past `s` borrow.

**Alias imprecision (can't distinguish indices):**

<div class="rust-code">
<pre>let mut v = vec![1, 2, 3];
let r = &mut v[0];
let s = &mut v[1];  <span class="comment">// ERROR: two &mut into v</span></pre>
</div>

Checker sees: both borrow "v", might be same SPACE. Proving `[0] ≠ [1]` would require value tracking.

**Conditional initialization:**

<div class="rust-code">
<pre>let r: &i32;
if cond {
    r = &x;
}
use(r);  <span class="comment">// ERROR: r might be uninitialized</span></pre>
</div>

Checker sees: r's IDENTITY might not exist on some paths.

---

### Curry-Howard — Lifetimes as Proofs

Types are propositions. Programs are proofs.

A reference `&'a T` is a proof of:

<div class="derived-box">
"There exists a SPACE containing T,<br>
 and IDENTITY to it is valid for TIME span 'a"
</div>

A lifetime bound `'a: 'b` is a proof obligation:

<div class="derived-box">
"TIME span 'a contains TIME span 'b"
</div>

A successful borrow check means:

<div class="derived-box">
"All IDENTITY/TIME claims are consistent.<br>
 No path exists where IDENTITY outlives SPACE<br>
 or where shared IDENTITY + mutation overlap."
</div>

The borrow checker is a **proof verifier** for memory safety propositions.

**CS foundation:** Curry-Howard correspondence — the type system is a logic, lifetime annotations are propositions about TIME, the checker verifies proofs.

---

### Polonius — Future Direction

Current NLL: **location-based** — tracks which SPACEs are borrowed.

Polonius: **origin-based** — tracks where IDENTITY comes from.

<div class="rust-code">
<pre>let mut x = 5;
let mut y = 6;
let r = if cond { &mut x } else { &mut y };
*r = 7;
x = 8;  <span class="comment">// NLL: might error (conservative)</span>
        <span class="comment">// Polonius: OK if cond was false (tracks origin)</span></pre>
</div>

Polonius uses **Datalog** (logic programming) for constraint solving:

<div class="rust-code">
<pre><span class="comment">% Datalog rules (simplified)</span>
borrow_live_at(B, P) :-
    borrow_region(B, R),
    region_contains(R, P).

error(P) :-
    borrow_live_at(B1, P),
    borrow_live_at(B2, P),
    borrow_conflicts(B1, B2).</pre>
</div>

**CS foundation:** Logic programming, Datalog — rules define relations, engine computes fixed-points. More expressive than manual dataflow.

---

### Equivalences

<div class="equivalences-box">
lifetime 'a           ≅  TTL on cache           ≅  bounded TIME<br>
region (CFG points)   ≅  validity window        ≅  discrete TIME<br>
liveness              ≅  "replica in use?"      ≅  IDENTITY tracking<br>
outlives 'a: 'b       ≅  dependency order       ≅  TIME containment<br>
conflict detection    ≅  write-write conflict   ≅  coherence violation<br>
NLL                   ≅  precise invalidation   ≅  fine-grained TIME<br>
abstract interp.      ≅  conservative approx.   ≅  decidable coherence<br>
proof (types)         ≅  safety certificate     ≅  verified IDENTITY/TIME
</div>
