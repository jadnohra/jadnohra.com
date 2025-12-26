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

---

## Interior Mutability

### The Rule and Its Escape

The borrow checker enforces:

<div class="derived-box">
!(shared IDENTITY && mutation)
</div>

<table class="derived-table">
<tr><th>Reference</th><th>IDENTITY</th><th>TIME</th><th>Coherence</th></tr>
<tr><td><code>&T</code></td><td>Shared</td><td>Frozen</td><td>Compile-time proof</td></tr>
<tr><td><code>&mut T</code></td><td>Unique</td><td>Flows</td><td>Compile-time proof</td></tr>
</table>

Interior mutability allows **shared IDENTITY + mutation** by shifting coherence from compile-time to runtime.

<table class="derived-table">
<tr><th>Reference</th><th>IDENTITY</th><th>TIME</th><th>Coherence</th></tr>
<tr><td><code>&Cell&lt;T&gt;</code></td><td>Shared</td><td>Flows</td><td>Runtime: copy in/out</td></tr>
<tr><td><code>&RefCell&lt;T&gt;</code></td><td>Shared</td><td>Flows</td><td>Runtime: borrow counting</td></tr>
<tr><td><code>&Mutex&lt;T&gt;</code></td><td>Shared</td><td>Flows</td><td>Runtime: serialize TIME</td></tr>
<tr><td><code>&Atomic*</code></td><td>Shared</td><td>Flows</td><td>Hardware: arbitration</td></tr>
<tr><td><code>&UnsafeCell&lt;T&gt;</code></td><td>Shared</td><td>Flows</td><td>None: you prove it</td></tr>
</table>

---

### Why It Exists

The borrow checker is **sound but incomplete**:
- OK → definitely safe
- ERROR → might be safe (false positives)

Interior mutability is the answer to false positives. When the compiler rejects a safe program, shift verification to runtime.

<div class="rust-code">
<pre><span class="comment">// Borrow checker rejects (can't prove disjoint):</span>
let mut data = vec![1, 2, 3];
let a = &mut data[0];
let b = &mut data[1];  <span class="comment">// ERROR: two &mut to data</span>

<span class="comment">// You know [0] and [1] are disjoint</span>
<span class="comment">// Compiler can't prove it (undecidable for arbitrary indices)</span>
<span class="comment">// Solution: runtime checking or unsafe</span></pre>
</div>

---

### The Hierarchy

All interior mutability builds on `UnsafeCell<T>`:

<div class="pipeline-diagram">
<pre>
UnsafeCell&lt;T&gt;           ← primitive: makes &T + mutation not UB
    │
    ├── Cell&lt;T&gt;         ← single-thread, Copy only, no refs in
    │
    ├── RefCell&lt;T&gt;      ← single-thread, runtime borrow check
    │
    ├── OnceCell&lt;T&gt;     ← single-thread, write-once
    │
    ├── Mutex&lt;T&gt;        ← multi-thread, OS lock
    │
    ├── RwLock&lt;T&gt;       ← multi-thread, many-reader or one-writer
    │
    ├── OnceLock&lt;T&gt;     ← multi-thread, write-once
    │
    └── Atomic*         ← multi-thread, hardware operations
</pre>
</div>

`UnsafeCell<T>` is the foundation. It tells the compiler: "don't assume `&T` means immutable here."

---

### Cell — No References In

<div class="rust-code">
<pre>let x = Cell::new(5);
let y = &x;
let z = &x;  <span class="comment">// Multiple shared refs OK</span>

y.set(10);   <span class="comment">// Mutate through shared ref</span>
z.set(20);   <span class="comment">// Also OK</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Strategy</th></tr>
<tr><td>IDENTITY</td><td>Shared freely</td></tr>
<tr><td>TIME</td><td>Flows</td></tr>
<tr><td>Coherence</td><td>No refs into contents — only copy in/out</td></tr>
</table>

**How it achieves safety:**
- `.get()` returns a copy, not a reference
- `.set()` takes a value, not a reference
- Never two references to the inner data
- No aliasing of contents → no coherence problem

**Limitation:** `T` must be `Copy`. Can't get reference to interior.

---

### RefCell — Runtime Borrow Checker

<div class="rust-code">
<pre>let x = RefCell::new(vec![1, 2, 3]);

let r = x.borrow();      <span class="comment">// Returns Ref&lt;Vec&gt;</span>
let s = x.borrow();      <span class="comment">// OK: multiple shared borrows</span>
<span class="comment">// let m = x.borrow_mut(); // PANIC: shared borrow active</span>

drop(r);
drop(s);

let m = x.borrow_mut();  <span class="comment">// OK: no active borrows</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Strategy</th></tr>
<tr><td>IDENTITY</td><td>Shared, but contents tracked</td></tr>
<tr><td>TIME</td><td>Flows, checked at runtime</td></tr>
<tr><td>Coherence</td><td>Borrow counting, panic on violation</td></tr>
</table>

**How it achieves safety:**
- Tracks borrow count at runtime
- `borrow()` → increment shared count
- `borrow_mut()` → check no borrows exist
- Violation → panic (fail-fast)

**It's the same logic as the borrow checker, run at a different TIME:**

<table class="derived-table">
<tr><th>Borrow checker</th><th>RefCell</th></tr>
<tr><td>Checks at compile</td><td>Checks at runtime</td></tr>
<tr><td>Rejects program</td><td>Panics at violation</td></tr>
<tr><td>Zero cost</td><td>Counter overhead</td></tr>
<tr><td>Conservative</td><td>Precise</td></tr>
</table>

---

### Mutex — Serialize TIME

<div class="rust-code">
<pre>let x = Mutex::new(vec![1, 2, 3]);

{
    let mut guard = x.lock().unwrap();
    guard.push(4);
    <span class="comment">// Other threads block here</span>
}  <span class="comment">// Lock released on drop</span>

<span class="comment">// Another thread can now acquire</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Strategy</th></tr>
<tr><td>IDENTITY</td><td>Shared across threads</td></tr>
<tr><td>TIME</td><td>Serialized by lock</td></tr>
<tr><td>Coherence</td><td>Only one accessor at a time</td></tr>
</table>

**How it achieves safety:**
- Lock acquisition blocks other threads
- Only one `MutexGuard` exists at a time
- Guard gives `&mut` to contents
- TIME becomes sequential at lock boundaries

**Trade:** Parallel TIME → sequential TIME. Threads wait.

---

### RwLock — Many Readers or One Writer

<div class="rust-code">
<pre>let x = RwLock::new(vec![1, 2, 3]);

<span class="comment">// Many readers OK</span>
let r1 = x.read().unwrap();
let r2 = x.read().unwrap();

<span class="comment">// Writer must wait for readers</span>
drop(r1);
drop(r2);
let w = x.write().unwrap();</pre>
</div>

<table class="derived-table">
<tr><th>Mode</th><th>IDENTITY</th><th>TIME</th></tr>
<tr><td>Read</td><td>Shared</td><td>Frozen (among readers)</td></tr>
<tr><td>Write</td><td>Exclusive</td><td>Flows</td></tr>
</table>

Same as `&T` vs `&mut T`, enforced at runtime across threads.

---

### Atomic — Hardware Coherence

<div class="rust-code">
<pre>let x = AtomicU64::new(0);

<span class="comment">// Multiple threads can access simultaneously</span>
x.fetch_add(1, Ordering::SeqCst);  <span class="comment">// Hardware guarantees atomicity</span></pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Strategy</th></tr>
<tr><td>IDENTITY</td><td>Shared across threads</td></tr>
<tr><td>TIME</td><td>Hardware arbitrated</td></tr>
<tr><td>Coherence</td><td>CPU cache coherence protocol</td></tr>
</table>

**How it achieves safety:**
- Individual operations are indivisible
- Hardware ensures total order (with SeqCst)
- No locks, but limited to specific operations

**Trade:** Full flexibility → limited operations. Can't atomically modify a Vec.

---

### UnsafeCell — The Primitive

<div class="rust-code">
<pre>let x = UnsafeCell::new(5);

unsafe {
    let ptr = x.get();
    *ptr = 10;
}</pre>
</div>

<table class="derived-table">
<tr><th>Axis</th><th>Strategy</th></tr>
<tr><td>IDENTITY</td><td>Shared</td></tr>
<tr><td>TIME</td><td>Flows</td></tr>
<tr><td>Coherence</td><td>None — you prove it</td></tr>
</table>

`UnsafeCell` is what makes the others possible. It's the opt-out:
- Tells compiler: `&UnsafeCell<T>` does not imply contents are immutable
- All other interior mutability types wrap this
- Raw access via `.get()` → `*mut T`

---

### The Spectrum

From compile-time to trust-the-programmer:

<div class="pipeline-diagram">
<pre>
COMPILE-TIME                              RUNTIME                                 UNSAFE
     │                                        │                                      │
     ▼                                        ▼                                      ▼
  &T / &mut T      Cell&lt;T&gt;      RefCell&lt;T&gt;      Mutex&lt;T&gt;      Atomic*      UnsafeCell&lt;T&gt;
     │               │              │             │              │               │
  Borrow         Copy in/out    Borrow count   OS lock       Hardware       You prove it
  checker        No refs in     Panic on err   Blocking      CPU cycles     Zero cost
  Zero cost      Copy types     Flexibility    Multi-thread  Limited ops    Unlimited
</pre>
</div>

<table class="derived-table">
<tr><th>Type</th><th>Check TIME</th><th>Cost</th><th>Flexibility</th><th>Thread</th></tr>
<tr><td><code>&T</code>/<code>&mut T</code></td><td>Compile</td><td>Zero</td><td>Conservative</td><td>Any</td></tr>
<tr><td><code>Cell&lt;T&gt;</code></td><td>Runtime</td><td>Cheap</td><td>Copy only</td><td>Single</td></tr>
<tr><td><code>RefCell&lt;T&gt;</code></td><td>Runtime</td><td>Counter</td><td>Full, panics</td><td>Single</td></tr>
<tr><td><code>Mutex&lt;T&gt;</code></td><td>Runtime</td><td>Lock</td><td>Full, blocks</td><td>Multi</td></tr>
<tr><td><code>RwLock&lt;T&gt;</code></td><td>Runtime</td><td>Lock</td><td>Full, blocks</td><td>Multi</td></tr>
<tr><td><code>Atomic*</code></td><td>Runtime</td><td>CPU</td><td>Limited ops</td><td>Multi</td></tr>
<tr><td><code>UnsafeCell&lt;T&gt;</code></td><td>Never</td><td>Zero</td><td>Unlimited</td><td>Any</td></tr>
</table>

---

### Triangle View

Interior mutability changes which axis is constrained:

<table class="derived-table">
<tr><th>Type</th><th>SPACE</th><th>TIME</th><th>IDENTITY</th><th>Coherence</th></tr>
<tr><td><code>&T</code></td><td>Fixed</td><td>Frozen</td><td>Shared</td><td>No mutation → no problem</td></tr>
<tr><td><code>&mut T</code></td><td>Fixed</td><td>Flows</td><td>Unique</td><td>No sharing → no problem</td></tr>
<tr><td><code>Cell</code></td><td>Fixed</td><td>Flows</td><td>Shared</td><td>No refs in → no aliasing of contents</td></tr>
<tr><td><code>RefCell</code></td><td>Fixed</td><td>Flows (checked)</td><td>Shared (counted)</td><td>Runtime borrow rules</td></tr>
<tr><td><code>Mutex</code></td><td>Fixed</td><td>Serialized</td><td>Shared</td><td>One at a time</td></tr>
<tr><td><code>Atomic</code></td><td>Fixed</td><td>Hardware</td><td>Shared</td><td>Hardware arbitration</td></tr>
</table>

Standard references: constrain TIME (freeze) or IDENTITY (unique).
Interior mutability: constrain TIME at runtime or use hardware.

---

### Derived Data View

From the coherence pattern:

<div class="derived-box">
Shared IDENTITY + Mutation + TIME overlap = Coherence Problem
</div>

Interior mutability solves this in different ways:

<table class="derived-table">
<tr><th>Type</th><th>How it breaks the problem</th></tr>
<tr><td><code>Cell</code></td><td>No IDENTITY into contents (copy values)</td></tr>
<tr><td><code>RefCell</code></td><td>Detect overlap, panic (fail-fast)</td></tr>
<tr><td><code>Mutex</code></td><td>Prevent TIME overlap (serialize)</td></tr>
<tr><td><code>RwLock</code></td><td>Allow parallel frozen TIME, serialize mutation</td></tr>
<tr><td><code>Atomic</code></td><td>Hardware ensures no overlap (atomic ops)</td></tr>
<tr><td><code>UnsafeCell</code></td><td>Trust programmer to prevent overlap</td></tr>
</table>

---

### When to Use What

<table class="derived-table">
<tr><th>Situation</th><th>Use</th></tr>
<tr><td>Simple counter/flag, single thread</td><td><code>Cell&lt;T&gt;</code></td></tr>
<tr><td>Complex data, single thread, need refs</td><td><code>RefCell&lt;T&gt;</code></td></tr>
<tr><td>Shared across threads, any data</td><td><code>Mutex&lt;T&gt;</code></td></tr>
<tr><td>Shared across threads, read-heavy</td><td><code>RwLock&lt;T&gt;</code></td></tr>
<tr><td>Shared across threads, simple counter</td><td><code>AtomicU64</code> etc</td></tr>
<tr><td>Building your own sync primitive</td><td><code>UnsafeCell&lt;T&gt;</code></td></tr>
<tr><td>Borrow checker accepts your code</td><td>None needed</td></tr>
</table>

---

### Common Pattern: Shared Ownership + Interior Mutability

Neither alone is enough for shared mutable data:

<table class="derived-table">
<tr><th>Type</th><th>Ownership</th><th>Mutability</th></tr>
<tr><td><code>Rc&lt;T&gt;</code></td><td>Shared, single-thread</td><td>Immutable</td></tr>
<tr><td><code>Arc&lt;T&gt;</code></td><td>Shared, multi-thread</td><td>Immutable</td></tr>
<tr><td><code>Rc&lt;RefCell&lt;T&gt;&gt;</code></td><td>Shared, single-thread</td><td>Mutable</td></tr>
<tr><td><code>Arc&lt;Mutex&lt;T&gt;&gt;</code></td><td>Shared, multi-thread</td><td>Mutable</td></tr>
</table>

Ownership handles SPACE × IDENTITY (who can access).
Interior mutability handles TIME (when mutation is safe).

---

### The Insight

Interior mutability is not an escape from the rules. It's **the same rules, checked at a different TIME**.

<table class="derived-table">
<tr><th></th><th>Borrow checker</th><th>Interior mutability</th></tr>
<tr><td>Rule</td><td>No shared+mutable</td><td>No shared+mutable</td></tr>
<tr><td>Check</td><td>Compile time</td><td>Runtime</td></tr>
<tr><td>Failure</td><td>Won't compile</td><td>Panic or block</td></tr>
<tr><td>Cost</td><td>Zero</td><td>Runtime overhead</td></tr>
<tr><td>Precision</td><td>Conservative</td><td>Exact</td></tr>
</table>

The borrow checker is an ahead-of-time approximation. Interior mutability is just-in-time verification. Same coherence problem, different verification strategy.

---

### Interior Mutability — Equivalences

<div class="equivalences-box">
Cell&lt;T&gt;               ≅  value cache         ≅  no refs, copy in/out<br>
RefCell&lt;T&gt;            ≅  runtime mutex       ≅  single-thread lock<br>
Mutex&lt;T&gt;              ≅  distributed lock    ≅  serialize TIME<br>
RwLock&lt;T&gt;             ≅  read replica + lock ≅  many-read or one-write<br>
Atomic*               ≅  hardware CAS        ≅  CPU coherence<br>
UnsafeCell&lt;T&gt;         ≅  trust-me token      ≅  unchecked coherence<br>
interior mutability   ≅  runtime coherence   ≅  deferred verification
</div>

---

## Pedantic Naming

Rust's naming creates a large [abstraction distance](/abstractor/) between terminology and the underlying primitives. This distance hinders understanding — every concept requires extra mental mapping through jargon before reaching fundamentals.

---

### References

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td><code>&T</code></td><td>"Reference to T"</td><td>Shared IDENTITY, frozen TIME</td><td>Shared alias, read-only</td><td><code>&shared T</code></td></tr>
<tr><td><code>&mut T</code></td><td>"Mutable reference"</td><td>Exclusive IDENTITY, TIME flows</td><td>Unique alias, read-write</td><td><code>&unique T</code></td></tr>
<tr><td>"Borrow"</td><td>Lending temporarily</td><td>Creating scoped IDENTITY</td><td>Scoped alias</td><td>"Alias"</td></tr>
<tr><td>"Reborrow"</td><td>Borrowing again</td><td>Narrowing IDENTITY scope</td><td>Nested alias</td><td>"Narrow"</td></tr>
</table>

**The `&mut` problem:**

`&mut T` sounds like "the reference is mutable" or "a reference you can mutate." Neither is right.

What it means:
- IDENTITY: exclusive (no other aliases exist)
- TIME: flows (mutation allowed)
- Coherence: uniqueness guarantees no conflicts

It's not about mutability of the reference. It's about **exclusivity of the IDENTITY**.

<div class="rust-code">
<pre>let mut x = 5;
let r = &mut x;  <span class="comment">// "mutable reference" - misleading</span>
                 <span class="comment">// Actually: exclusive alias to x</span>
                 <span class="comment">// Better: &unique x or &excl x</span></pre>
</div>

Compare to:

<div class="rust-code">
<pre>let mut r = &x;  <span class="comment">// Actually mutable: r can be reassigned</span>
r = &y;          <span class="comment">// The binding r changed, not what it points to</span></pre>
</div>

---

### Lifetimes

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td>"Lifetime"</td><td>Memory duration</td><td>IDENTITY validity span</td><td>Reference validity</td><td>"Validity"</td></tr>
<tr><td><code>'a</code></td><td>Some lifetime a</td><td>TIME region where IDENTITY valid</td><td>Validity interval</td><td><code>#a</code></td></tr>
<tr><td><code>'static</code></td><td>Lives forever</td><td>Valid for all program TIME</td><td>Global validity</td><td><code>'always</code></td></tr>
<tr><td><code>'a: 'b</code></td><td>a outlives b</td><td>TIME span a contains b</td><td>Interval containment</td><td><code>'a ⊇ 'b</code></td></tr>
</table>

**The "lifetime" problem:**

"Lifetime" sounds like memory duration — when SPACE exists. But it's actually about IDENTITY validity — when a reference is usable.

<div class="rust-code">
<pre>fn example&lt;'a&gt;(x: &'a i32) -> &'a i32 { x }</pre>
</div>

Reading: "x has lifetime 'a"
Misleading: suggests x lives for duration 'a
Actually: the IDENTITY relationship (reference) is valid during TIME span 'a

The value's SPACE might exist longer. The lifetime bounds the reference (IDENTITY), not the value (SPACE).

---

### Ownership

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td>"Ownership"</td><td>Possession</td><td>Unique IDENTITY + dealloc responsibility</td><td>Unique handle</td><td>"Unique binding"</td></tr>
<tr><td>"Move"</td><td>Physical relocation</td><td>IDENTITY transfer + source invalidation</td><td>Linear consumption</td><td>"Transfer"</td></tr>
<tr><td>"Copy"</td><td>Make a copy</td><td>New SPACE, new IDENTITY, bitwise</td><td>Value duplication</td><td>"Duplicate"</td></tr>
<tr><td>"Clone"</td><td>Make a clone</td><td>Explicit deep copy</td><td>Deep duplication</td><td>"Deep copy"</td></tr>
<tr><td>"Drop"</td><td>Drop it</td><td>Free SPACE at TIME boundary</td><td>Destructor</td><td>"Destruct"</td></tr>
</table>

**The "move" problem:**

"Move" suggests data physically relocates. It doesn't.

<div class="rust-code">
<pre>let x = vec![1, 2, 3];  <span class="comment">// x has IDENTITY to heap SPACE</span>
let y = x;              <span class="comment">// IDENTITY transfers to y, x invalidated</span>
                        <span class="comment">// The heap data didn't move</span>
                        <span class="comment">// Only the IDENTITY relationship changed</span></pre>
</div>

What happens:
- SPACE: unchanged (heap data stays put)
- IDENTITY: transferred from x to y
- x: name exists, IDENTITY severed

"Transfer" or "consume" would be clearer than "move."

---

### Borrow Checker

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td>"Borrow checker"</td><td>Checks borrowing</td><td>Proves IDENTITY×TIME coherence</td><td>Alias-validity prover</td><td>"Coherence prover"</td></tr>
<tr><td>"Borrow error"</td><td>Borrowing mistake</td><td>IDENTITY×TIME conflict</td><td>Coherence violation</td><td>"Alias conflict"</td></tr>
<tr><td>"Cannot borrow"</td><td>Borrowing forbidden</td><td>IDENTITY conflict detected</td><td>Alias violation</td><td>"Conflicting alias"</td></tr>
</table>

**The "borrow checker" problem:**

"Borrow checker" sounds like a simple rule checker. It's actually:
- A dataflow analyzer (CFG, liveness)
- A constraint solver (region inference)
- A proof system (verifying coherence)

Better name: **Alias-Validity Prover** or **Coherence Analyzer**

---

### Interior Mutability Terms

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td>"Interior mutability"</td><td>Mutating insides</td><td>Runtime coherence</td><td>Deferred verification</td><td>"Runtime coherence"</td></tr>
<tr><td><code>Cell&lt;T&gt;</code></td><td>A cell</td><td>Copy-only shared mutation</td><td>Value-only access</td><td><code>CopyMut&lt;T&gt;</code></td></tr>
<tr><td><code>RefCell&lt;T&gt;</code></td><td>Reference cell</td><td>Runtime borrow checker</td><td>Checked alias</td><td><code>RuntimeChecked&lt;T&gt;</code></td></tr>
<tr><td><code>UnsafeCell&lt;T&gt;</code></td><td>Unsafe cell</td><td>Raw opt-out of alias rules</td><td>Unchecked interior</td><td><code>RawMut&lt;T&gt;</code></td></tr>
</table>

---

### Thread Safety

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td><code>Send</code></td><td>Can be sent</td><td>IDENTITY can cross thread TIME</td><td>Thread-transferable</td><td><code>ThreadTransfer</code></td></tr>
<tr><td><code>Sync</code></td><td>Synchronizes</td><td>Shared IDENTITY safe in parallel TIME</td><td>Thread-shareable</td><td><code>ThreadShare</code></td></tr>
<tr><td><code>Mutex&lt;T&gt;</code></td><td>Mutual exclusion</td><td>TIME serializer</td><td>Lock</td><td><code>Lock&lt;T&gt;</code></td></tr>
<tr><td><code>Arc&lt;T&gt;</code></td><td>Atomic ref count</td><td>Shared IDENTITY across threads</td><td>Thread-shared handle</td><td><code>SharedHandle&lt;T&gt;</code></td></tr>
</table>

**The Send/Sync problem:**

`Send` and `Sync` are opaque. They don't reveal what they mean.

- `Send`: IDENTITY can be transferred across thread boundary
- `Sync`: shared IDENTITY (`&T`) can exist in parallel TIME

Better names: `ThreadTransfer` / `ThreadShare`

---

### Smart Pointers

<table class="derived-table">
<tr><th>Rust</th><th>Suggests</th><th>Actually</th><th>Fundamental</th><th>Clearer Name</th></tr>
<tr><td><code>Box&lt;T&gt;</code></td><td>A box</td><td>Unique heap IDENTITY</td><td>Heap-unique</td><td><code>HeapUnique&lt;T&gt;</code></td></tr>
<tr><td><code>Rc&lt;T&gt;</code></td><td>Reference counted</td><td>Shared IDENTITY, counted SPACE lifetime</td><td>Shared handle (counted)</td><td><code>SharedLocal&lt;T&gt;</code></td></tr>
<tr><td><code>Arc&lt;T&gt;</code></td><td>Atomic RC</td><td>Thread-safe shared IDENTITY</td><td>Shared handle (atomic)</td><td><code>SharedAtomic&lt;T&gt;</code></td></tr>
<tr><td><code>Weak&lt;T&gt;</code></td><td>Weak reference</td><td>Non-owning IDENTITY</td><td>Observer handle</td><td><code>Observer&lt;T&gt;</code></td></tr>
</table>

---

### Error Messages

<table class="derived-table">
<tr><th>Rust Says</th><th>Actually Means</th><th>Clearer Message</th></tr>
<tr><td>"cannot borrow `x` as mutable because it is also borrowed as immutable"</td><td>Exclusive IDENTITY requested but shared IDENTITY exists</td><td>"Cannot create exclusive alias: shared alias exists"</td></tr>
<tr><td>"borrowed value does not live long enough"</td><td>IDENTITY validity exceeds SPACE validity</td><td>"Reference outlives the value it refers to"</td></tr>
<tr><td>"cannot move out of borrowed content"</td><td>Can't transfer IDENTITY while alias exists</td><td>"Cannot consume: alias exists"</td></tr>
<tr><td>"use of moved value"</td><td>IDENTITY accessed after transfer</td><td>"Value already consumed"</td></tr>
</table>

---

### The Jargon Layers

Rust terminology forms layers that hide fundamentals:

<div class="pipeline-diagram">
<pre>
RUST JARGON LAYER
    │
    │  "borrow", "lifetime", "move", "interior mutability"
    │
    ▼
RUST SEMANTIC LAYER
    │
    │  Scoped alias, validity span, transfer, runtime check
    │
    ▼
CS CONCEPT LAYER
    │
    │  Alias analysis, dataflow, constraint solving, coherence
    │
    ▼
FUNDAMENTAL LAYER

    SPACE × TIME × IDENTITY
</pre>
</div>

Each layer adds terminology that obscures the layer below.

---

### Translation Table

<table class="derived-table">
<tr><th>Rust Concept</th><th>CS Concept</th><th>Fundamental (Triangle)</th></tr>
<tr><td>Ownership</td><td>Unique pointer/handle</td><td>Unique IDENTITY to SPACE</td></tr>
<tr><td>Borrowing</td><td>Aliasing</td><td>Creating IDENTITY relationship</td></tr>
<tr><td>Lifetime</td><td>Reference validity interval</td><td>TIME span of IDENTITY</td></tr>
<tr><td>Move</td><td>Linear consumption</td><td>IDENTITY transfer</td></tr>
<tr><td>Copy</td><td>Value semantics</td><td>New SPACE, new IDENTITY</td></tr>
<tr><td>Borrow checker</td><td>Alias analysis + dataflow</td><td>IDENTITY×TIME coherence proof</td></tr>
<tr><td>Interior mutability</td><td>Runtime checks</td><td>Deferred coherence verification</td></tr>
<tr><td><code>&T</code></td><td>Shared/const pointer</td><td>Shared IDENTITY, frozen TIME</td></tr>
<tr><td><code>&mut T</code></td><td>Unique/exclusive pointer</td><td>Exclusive IDENTITY, TIME flows</td></tr>
<tr><td>Send</td><td>Thread-transferable</td><td>IDENTITY can cross thread</td></tr>
<tr><td>Sync</td><td>Thread-shareable</td><td>Shared IDENTITY parallel-safe</td></tr>
<tr><td>Drop</td><td>Destructor</td><td>SPACE freed at TIME boundary</td></tr>
<tr><td><code>'static</code></td><td>Global validity</td><td>IDENTITY valid for all TIME</td></tr>
</table>

---

### Why This Matters

**For learning:**
- Jargon creates artificial barriers
- Fundamentals transfer between languages
- Correct mental models prevent confusion

**For debugging:**
- "Borrow error" is vague
- "IDENTITY×TIME conflict" tells you what to fix
- Understanding the triangle helps resolve issues

**For design:**
- Jargon hides tradeoffs
- Fundamentals reveal what's actually constrained
- Better names would teach concepts

---

### What Rust Could Have Named

<table class="derived-table">
<tr><th>Current</th><th>Alternative</th><th>Why Better</th></tr>
<tr><td><code>&T</code></td><td><code>&shared T</code></td><td>Reveals: IDENTITY is shared</td></tr>
<tr><td><code>&mut T</code></td><td><code>&unique T</code></td><td>Reveals: IDENTITY is exclusive</td></tr>
<tr><td>Lifetime <code>'a</code></td><td>Validity <code>#a</code></td><td>Not confused with memory duration</td></tr>
<tr><td>Borrow checker</td><td>Coherence prover</td><td>Reveals: it proves safety properties</td></tr>
<tr><td>Interior mutability</td><td>Runtime coherence</td><td>Reveals: same rules, different TIME</td></tr>
<tr><td>Move</td><td>Transfer / Consume</td><td>Reveals: IDENTITY moves, not data</td></tr>
<tr><td>Send</td><td>ThreadTransfer</td><td>Reveals: what can cross threads</td></tr>
<tr><td>Sync</td><td>ThreadShare</td><td>Reveals: what's safe to share</td></tr>
</table>

---

### Naming — Summary

Rust's naming choices:
1. Create a jargon barrier to entry
2. Suggest wrong mental models (`&mut` = mutable, "lifetime" = duration)
3. Hide CS fundamentals (dataflow, coherence, aliasing)
4. Prevent knowledge transfer to/from other systems

The fundamentals are simple:
- SPACE: where data lives
- TIME: when things happen
- IDENTITY: which references point where
- Coherence: keeping IDENTITY×TIME consistent

Rust's features are combinations of these. The names obscure this. Understanding the mapping lets you see through the jargon to the principles.
