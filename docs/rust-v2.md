---
layout: post
title: Rust (v2)
toc: true
permalink: /rust-v2/
---

*For the course version, see [Learn Rust](/learn-rust/).*

---

## The Framework

Rust's memory model is a set of choices on the SPACE / TIME / IDENTITY triangle.

- **SPACE**: Where data lives (bytes, layouts, allocations)
- **TIME**: When things happen (compile-time analysis, runtime execution, scope boundaries)
- **IDENTITY**: How we refer to data (names, addresses, ownership relationships)

This framework applies to C++ as well:

| C++ | Framework |
|-----|-----------|
| `T*`, `T&` | IDENTITY pointing to SPACE |
| `const` | TIME frozen (no mutation) |
| Scope, RAII | TIME boundaries |
| `new`/`delete`, stack | SPACE allocation |
| `std::unique_ptr<T>` | Exclusive IDENTITY, owned SPACE |
| `std::shared_ptr<T>` | Shared IDENTITY, reference-counted SPACE |

C++ lets you manage these axes freely—and lets you get them wrong. Rust adds a compile-time verifier called the **borrow checker** that proves your SPACE/TIME/IDENTITY relationships are coherent: no dangling references, no data races, no use-after-free.

The borrow checker has a reputation for being mysterious and combative. Rust's documentation uses precise but sometimes opaque terminology ("lifetime," "borrow," "move") that can feel like jargon. This document aims to demystify it. The borrow checker is not magic—it's a straightforward static analysis operating on a small set of rules. Once you see how it works, the error messages make sense.

---

## The Two Projections

To understand how the borrow checker reasons, you need to distinguish two views of the same program. Think of it like an architectural blueprint versus the finished building: both represent the same structure, but at different levels of abstraction. We call these views "projections":

- **Compile-time projection**: What the compiler sees and analyzes. Variable names, type information, control-flow structure. Abstract and symbolic.
- **Runtime projection**: What actually executes. Memory addresses, bytes, CPU instructions. Concrete and physical.

The borrow checker works entirely in the compile-time projection. It never sees addresses or actual execution—it reasons about names, types, and possible control-flow paths. Then, once it proves safety, the abstractions are erased. The compiled binary has no borrow checking overhead because there's nothing left to check.

Each axis exists in both projections:

| Axis | Compile-time | Runtime |
|------|--------------|---------|
| SPACE | Types, layouts | Bytes in memory |
| TIME | Control-flow regions | Execution sequence |
| IDENTITY | Variable names, paths | Addresses |

---

## Training Wheels: Explicit Syntax

Rust's syntax is terse. Before learning it, we'll use an explicit syntax that says what's actually happening. This is provided by the `explicit!` macro (available as a companion crate for exercises).

```rust
// Explicit syntax (what's happening)
explicit! {
    let name(x) = 5;                      // create name x for this SPACE
    let name(rebindable(y)) = 10;         // rebindable name
    let name(r) = coord_shared(x);        // shared coordinates to x's SPACE
    let name(m) = coord_exclusive(y);     // exclusive coordinates to y's SPACE
}

// Rust syntax (what you write)
let x = 5;
let mut y = 10;
let r = &x;
let m = &mut y;
```

**Why "name" instead of "variable"?**

This departs from standard terminology in both Rust and C++ learning materials. We think it's important and more correct.

"Variable" conflates two projections—it fuses the name (compile-time) with the SPACE it refers to (runtime). This fusion is exactly the mental model that makes the borrow checker feel arbitrary. It's also not how compilers actually work.

When someone says "the variable moved," what moved? The name? The bytes? Both? The confusion is baked into the word.

With our terminology: names exist only at compile time. At runtime, names are erased; only addresses exist. Names can become invalid. SPACE can move. These are independent facts. This is closer to what actually happens in the machine, and it makes the borrow checker's rules obvious rather than mysterious.

This matters when names and SPACE diverge:

```rust
explicit! {
    let name(b) = coord_heap(String::from("hello"));  // b names heap coords
    let name(rebindable(x)) = take(at(b));            // take String from heap
    // b is now invalid, x names the String
}
```

What happens in memory:

```
Before - b names Box coordinates to heap String:
  Stack                 Heap
┌───────┐         ┌─────────────┐         ┌─────────┐
│ ptr ──│────────>│ ptr/len/cap │────────>│ "hello" │
└───────┘ <── b   └─────────────┘         └─────────┘
  (Box)             (String)                (chars)

After take(at(b)) - String moved to stack, Box freed:
  Stack                                     Heap
┌─────────────┐                       ┌─────────┐
│ ptr/len/cap │──────────────────────>│ "hello" │
└─────────────┘ <── x                 └─────────┘
                <── b (invalid)
```

The SPACE moved. The name `b` became invalid. The name `x` now refers to the String. "Variable" would obscure this: did the variable move? No—SPACE moved, names changed validity.

Similarly, "rebindable" is precise where "mutable" is ambiguous:

```rust
explicit! {
    let name(rebindable(x)) = String::from("hello");
    x = String::from("world");  // rebind x to new SPACE
}
```

```
During rebinding - new String created, old still exists:
  Stack                                     Heap
┌─────────────┐                       ┌─────────┐
│ ptr/len/cap │──────────────────────>│ "hello" │ <── x (current)
├─────────────┤                       ├─────────┤
│ ptr/len/cap │──────────────────────>│ "world" │ (new, unnamed)
└─────────────┘                       └─────────┘

After rebinding completes:
  Stack                                     Heap
┌─────────────┐                       ┌─────────┐
│ ptr/len/cap │──────────────────────>│ "world" │
└─────────────┘ <── x                 └─────────┘
  ("hello" dropped, freed)
```

The name `x` is rebound to different SPACE. The old SPACE is dropped. This is not "mutating a variable"—it's rebinding a name.

**Coordinates** instead of "reference" for the same reason: coordinates tell you where SPACE is, they don't give you the SPACE itself. They exist in both projections (compile-time: tracked regions; runtime: addresses).

The explicit operations:

| Explicit | Meaning |
|----------|---------|
| `name(x)` | Create a name for some SPACE |
| `name(rebindable(x))` | Name that can be rebound to different SPACE |
| `coord_shared(y)` | Get coordinates to y's SPACE (many allowed, read-only) |
| `coord_exclusive(y)` | Get coordinates to y's SPACE (one allowed, read-write) |
| `coord_heap(y)` | Allocate y on heap, get owning coordinates |
| `take(y)` | Take SPACE from y (y's name becomes invalid) |
| `mem_copy(y)` | Copy bytes of y (y still valid, requires Copy type) |
| `clone_copy(y)` | Call y.clone() (y still valid, requires Clone type) |
| `take_or_mem_copy(y)` | Compiler decides: mem_copy if Copy, else take |
| `at(r)` | Access SPACE at coordinates r |

We'll use explicit syntax to introduce concepts, then show the Rust equivalent.

---

## The Coherence Problem

If two coordinates point to the same SPACE, and one writes while the other reads, the reader may see partially-updated data. This is the coherence problem.

Rust's solution: shared coordinates can coexist only if none of them allow mutation. Mutation requires exclusive coordinates.

- **Shared coordinates** (`coord_shared`): many allowed, none can mutate
- **Exclusive coordinates** (`coord_exclusive`): one allowed, can mutate

```rust
explicit! {
    let name(rebindable(x)) = 5;
    let name(r) = coord_shared(x);        // shared coordinates
    println!("{}", at(r));                // use shared coordinates
    let name(m) = coord_exclusive(x);     // exclusive — OK, shared no longer used
}

// Rust equivalent:
let mut x = 5;
let r = &x;
println!("{}", *r);
let m = &mut x;
```

```rust
explicit! {
    let name(rebindable(x)) = 5;
    let name(r) = coord_shared(x);        // shared coordinates start
    let name(m) = coord_exclusive(x);     // ERROR: exclusive while shared exists
    println!("{}", at(r));                // shared still needed here
}

// Rust equivalent:
let mut x = 5;
let r = &x;
let m = &mut x;    // ERROR
println!("{}", *r);
```

The borrow checker enforces this at compile time by:
1. Tracking which names have coordinates to which SPACE
2. Building a graph of possible execution paths
3. Computing TIME regions when each set of coordinates is live
4. Checking for conflicts: shared + exclusive overlapping

**C++ comparison**: C++ allows both patterns. The second one compiles, and whether it causes problems depends on what you do. Rust rejects it statically.

[Exercises 1-8]

---

## SPACE Granularity

The borrow checker tracks IDENTITY at path granularity:

| Path type | Can prove disjoint? | Why |
|-----------|---------------------|-----|
| `p.x` vs `p.y` | Yes | Struct fields known at compile time |
| `v[0]` vs `v[1]` | No | Indices are runtime values |

```rust
let mut p = Point { x: 1, y: 2 };
let rx = &mut p.x;
let ry = &mut p.y;  // OK: different fields, provably disjoint
```

```rust
let mut v = vec![1, 2, 3];
let r = &mut v[0];
let s = &mut v[1];  // ERROR: both "borrow v"
```

The checker sees both index accesses as "borrow of v" because it cannot prove `v[0] ≠ v[1]` in general.

[Exercises 9-12]

---

## Bindings

A binding is a name for some SPACE.

```rust
explicit! {
    let name(x) = 5;              // x names SPACE containing 5
    let name(rebindable(y)) = 10; // y can be rebound to different SPACE
    y = 20;                       // rebind y to SPACE containing 20
}

// Rust:
let x = 5;
let mut y = 10;
y = 20;
```

| Construct | Explicit | SPACE | Rebindable |
|-----------|----------|-------|------------|
| `const` | — | Inlined (no runtime SPACE) | No |
| `static` | — | Data segment | No |
| `static mut` | — | Data segment | Yes (unsafe) |
| `let` | `name(x)` | Stack | No |
| `let mut` | `name(rebindable(x))` | Stack | Yes |

**const**: Purely compile-time. No runtime SPACE exists. Each use is inlined. Taking `&CONST` may yield different addresses because there is no single location.

**static**: Bridges projections. Initialized at compile time, lives in data segment at runtime. Has a stable address. C++ equivalent: `const` global or `constexpr`.

**static mut**: Runtime mutation of global SPACE. The borrow checker cannot prove safety across the whole program, so every access requires `unsafe`. C++ equivalent: global variable (but C++ doesn't require you to mark access as unsafe).

**let / let mut**: Runtime evaluation, stack SPACE. The `mut` allows rebinding the name to different SPACE. C++ equivalent: local variable, `const` vs non-`const`.

[Exercises 13-16]

---

## Coordinates (References)

Coordinates create IDENTITY to existing SPACE without owning it.

| Type | Explicit | How many | Mutation |
|------|----------|----------|----------|
| Shared | `coord_shared(x)` | Many | No |
| Exclusive | `coord_exclusive(x)` | One | Yes |

```rust
explicit! {
    let name(x) = vec![1, 2, 3];
    let name(r1) = coord_shared(x);
    let name(r2) = coord_shared(x);   // OK: multiple shared
}

// Rust:
let x = vec![1, 2, 3];
let r1 = &x;
let r2 = &x;
```

```rust
explicit! {
    let name(rebindable(x)) = vec![1, 2, 3];
    let name(r) = coord_exclusive(x);
    at(r).push(4);   // OK: exclusive access allows mutation
}

// Rust:
let mut x = vec![1, 2, 3];
let r = &mut x;
r.push(4);
```

**C++ comparison**: Both patterns work identically in C++. The difference is that Rust prevents you from having shared and exclusive coordinates alive simultaneously, while C++ allows `const T&` and `T&` to coexist.

---

## IDENTITY at Runtime

At runtime, names are erased. Only addresses remain.

RAM can store addresses of its own locations. This is what enables references, sharing, and complex data structures:

```
┌─────────┬───────────────┐
│  100    │   5       ← x │ ←──┐
├─────────┼───────────────┤    │
│  108    │  100      ← r │ ───┘
└─────────┴───────────────┘
```

Registers hold values but lack addresses. Taking a reference forces the value to the stack:

```rust
let x = 5;      // might live in register
let r = &x;     // x now has address (on stack)
```

**Cost of sharing**: 8 bytes for the address, plus indirection to reach data, plus forcing values out of registers. For small types, copying is cheaper than referencing. This is why `Copy` exists.

---

## Stack and Heap

**Stack SPACE** is automatic. When a scope ends, its SPACE is reclaimed. The compiler knows the size at compile time.

**Heap SPACE** is dynamic. For SPACE whose size isn't known at compile time, or SPACE that must outlive the current scope.

```rust
explicit! {
    let name(b) = coord_heap(String::from("hello"));
}

// Rust:
let b = Box::new(String::from("hello"));
```

`coord_heap` (Box::new) allocates SPACE on the heap and returns owning coordinates.

| Rust | Owns SPACE? | Cleanup |
|------|-------------|---------|
| `coord_shared(y)` / `&y` | No | Not responsible |
| `coord_exclusive(y)` / `&mut y` | No | Not responsible |
| `coord_heap(x)` / `Box::new(x)` | Yes | Frees heap when dropped |

No garbage collector. No manual `free()`. The compiler inserts cleanup at scope end.

---

## Ownership and Move

```rust
explicit! {
    let name(v) = vec![1, 2, 3];
    let name(w) = take(v);   // SPACE ownership transfers to w
    // v is now invalid — using it is a compile error
}

// Rust:
let v = vec![1, 2, 3];
let w = v;
// v is now invalid
```

| Axis | What happens |
|------|--------------|
| IDENTITY | Transferred to new name, old name invalid |
| SPACE | May or may not physically move (compiler decides) |
| Coherence | Only one owner exists, no aliasing possible |

**C++ comparison**: `auto w = std::move(v)` is similar, but C++ lets you keep using `v` afterward (it's in a "valid but unspecified state"). This is a source of bugs. Rust makes the old name unusable at compile time.

[Exercises 17-24]

---

## Copy vs Clone

```rust
explicit! {
    // For i32 (Copy type): mem_copy happens automatically
    let name(x) = 5;
    let name(y) = mem_copy(x);   // copy bytes, x still valid
    
    // For String (non-Copy): must choose
    let name(s) = String::from("hello");
    let name(t) = clone_copy(s); // explicit clone, s still valid
    // OR
    let name(t) = take(s);       // take ownership, s invalid
}

// Rust:
let x = 5;
let y = x;           // Copy: implicit byte copy

let s = String::from("hello");
let t = s.clone();   // Clone: explicit
// OR
let t = s;           // Move: s now invalid
```

**mem_copy** (Copy trait): Duplicate the bytes implicitly on assignment. Only for types where byte-copying produces an independent value.

**clone_copy** (Clone trait): Call `.clone()` explicitly. Required when byte-copying would create aliasing problems.

For `i32`, SPACE is self-contained. Copying bytes works:
```
┌─────────┐    ┌─────────┐
│    5    │    │    5    │
└─────────┘    └─────────┘
```

For `String`, SPACE contains coordinates to heap allocation:
```
┌─────────────────┐
│ ptr: 0x1234     │     Heap:
│ len: 5          │     ┌───────────┐
│ capacity: 5     │     │ h e l l o │
└─────────────────┘     └───────────┘
```

Copying bytes would create two owners of the same heap SPACE. Rust requires explicit choice: `clone_copy` for deep copy, or `take`.

| Type | Copy? | Clone? | Why |
|------|-------|--------|-----|
| `i32` | Yes | Yes | Self-contained |
| `&T` | Yes | Yes | Just coordinates, doesn't own |
| `String` | No | Yes | Owns heap, must choose strategy |
| `Vec<T>` | No | Yes | Owns heap |

**C++ comparison**: C++ copy constructors can do either shallow or deep copy depending on implementation. Rust makes the distinction explicit in the type system.

[Exercises 25-32]

---

## Vocabulary: Explicit → Rust → C++

Now that you've seen the concepts, here's the mapping to Rust's actual syntax and C++ equivalents:

| Explicit | Rust | C++ | Notes |
|----------|------|-----|-------|
| `name(x)` | `let x` | `const T x` | Binding (immutable by default in Rust) |
| `name(rebindable(x))` | `let mut x` | `T x` | Rebindable binding |
| `coord_shared(y)` | `&y` | `const T&` or `const T*` | Rust enforces no aliased mutation |
| `coord_exclusive(y)` | `&mut y` | `T&` or `T*` | Rust guarantees exclusivity |
| `coord_heap(y)` | `Box::new(y)` | `std::make_unique<T>(y)` | Owning heap coordinates |
| `take(y)` | `y` (move) | `std::move(y)` | Rust enforces y is dead after |
| `mem_copy(y)` | `y` (Copy type) | `y` (trivial copy) | Implicit byte copy |
| `clone_copy(y)` | `y.clone()` | `y` (copy ctor) | Explicit deep copy |
| `take_or_mem_copy(y)` | `y` | — | Compiler chooses based on type |
| `at(r)` | `*r` | `*r` | Dereference |

**Rust's terminology quirks**:

| Rust says | Actually means |
|-----------|----------------|
| "reference" | Coordinates with compile-time validity tracking |
| "mutable reference" | Exclusive coordinates (mutation is consequence, not definition) |
| "lifetime" | Compile-time TIME region (not runtime duration) |
| "borrow" | Creating coordinates to existing SPACE |
| "move" | Transferring ownership (SPACE may or may not physically move) |

---

## Smart Pointers

`Box<T>` provides unique IDENTITY to heap-allocated SPACE. One owner, clear responsibility.

`Rc<T>` and `Arc<T>` provide shared IDENTITY with reference counting. When count reaches zero, SPACE is freed.

| Type | IDENTITY | SPACE lifetime | Thread safety |
|------|----------|----------------|---------------|
| `Box<T>` | Unique | Owner ends it | Any |
| `Rc<T>` | Shared, counted | Last reference ends it | Single-thread only |
| `Arc<T>` | Shared, atomic counted | Last reference ends it | Multi-thread safe |

**C++ comparison**:
- `Box<T>` ≈ `std::unique_ptr<T>`
- `Rc<T>` ≈ `std::shared_ptr<T>` with non-atomic count (C++ doesn't have this; `shared_ptr` is always atomic)
- `Arc<T>` ≈ `std::shared_ptr<T>`

With `Rc` and `Arc`, contents are immutable through the shared reference. To mutate, you need interior mutability.

[Exercises 33-40]

---

## Interior Mutability

The borrow checker is sound but incomplete. If it accepts code, that code is definitely safe. If it rejects code, the code might still be safe—the checker just cannot prove it.

Interior mutability handles these cases by moving the check to runtime. Same coherence rule, different enforcement time.

| Type | Checked when | Cost | Thread safety |
|------|--------------|------|---------------|
| `&T` / `&mut T` | Compile time | Zero | Any |
| `Cell<T>` | Runtime | Copy in/out | Single-thread |
| `RefCell<T>` | Runtime | Borrow count | Single-thread |
| `Mutex<T>` | Runtime | Lock | Multi-thread |
| `RwLock<T>` | Runtime | Lock | Multi-thread |
| `Atomic*` | Runtime | Hardware | Multi-thread |

**Cell**: Copy values in and out. No references into contents.
```rust
let x = Cell::new(5);
x.set(10);       // mutate through shared reference
let v = x.get(); // returns copy
```

**RefCell**: Runtime borrow checking. Panics on violation.
```rust
let x = RefCell::new(vec![1, 2, 3]);
let r = x.borrow();      // shared, returns Ref<T>
let s = x.borrow();      // OK: multiple shared
// x.borrow_mut();       // PANIC: already borrowed
```

**Mutex**: Serialize access across threads. Only one accessor at a time.
```rust
let x = Mutex::new(vec![1, 2, 3]);
let mut guard = x.lock().unwrap();
guard.push(4);
// other threads block until guard drops
```

**C++ comparison**:
- `Cell<T>` ≈ no equivalent (C++ just lets you mutate through any pointer)
- `RefCell<T>` ≈ no equivalent (runtime borrow checking)
- `Mutex<T>` ≈ `std::mutex` + data, but Rust bundles them and enforces that you hold the lock to access the data

[Exercises 41-52]

---

## Thread Safety

| Trait | Meaning |
|-------|---------|
| `Send` | IDENTITY can transfer across thread boundary |
| `Sync` | Shared IDENTITY (`&T`) is safe across threads |

```rust
// Rc<T> is !Send and !Sync — single-thread only
// Arc<T> is Send and Sync — multi-thread safe
// Mutex<T>: if T is Send, then Mutex<T> is Send + Sync
```

The compiler checks these at compile time. If your code compiles, thread safety is proven.

**C++ comparison**: C++ has no equivalent. You use `std::shared_ptr` and hope you don't create data races. Rust makes thread safety part of the type system.

[Exercises 53-58]

---

## Memory Ordering

Atomics make operations indivisible. But when does a write in one thread become visible to another?

Hardware defers coherence for performance:
- **Store buffers**: Writes queue locally before reaching RAM
- **Caches**: Each core has local copies; writes don't instantly propagate
- **Reordering**: CPU executes out of order

| Ordering | Guarantee | Use case |
|----------|-----------|----------|
| Relaxed | Operation is atomic | Counters |
| Release | Writes before this are visible to Acquire | Producer |
| Acquire | Reads after this see writes before Release | Consumer |
| AcqRel | Both | Read-modify-write |
| SeqCst | Global total order | When in doubt |

```
Thread A:                    Thread B:
write x = 1
write y = 2
Release(flag = true)  ───>   Acquire(flag)
                             read y  // sees 2
                             read x  // sees 1
```

**C++ comparison**: Direct equivalents exist.

| Rust | C++ |
|------|-----|
| `Ordering::Relaxed` | `std::memory_order_relaxed` |
| `Ordering::Acquire` | `std::memory_order_acquire` |
| `Ordering::Release` | `std::memory_order_release` |
| `Ordering::AcqRel` | `std::memory_order_acq_rel` |
| `Ordering::SeqCst` | `std::memory_order_seq_cst` |

[Exercises 59-64]

---

## Syntax

Rust overloads `&` and `mut` based on position.

| Position | `&mut x` means |
|----------|----------------|
| Expression (right of `=`) | Produce exclusive reference to x |
| Pattern (left of `=`) | Expect reference, bind target |
| Type (right of `:`) | The type "exclusive reference to T" |

**Primitives**:
- `&` creates or matches shared IDENTITY
- `&mut` creates or matches exclusive IDENTITY
- `mut` on a binding allows rebinding the name

`&mut` is one token. There is no `mut&`.

**Rebind vs mutate**:
```rust
let mut x = &y;   // x is rebindable
x = &z;           // rebind: x now points elsewhere

let x = &mut y;   // x points to y exclusively
*x = 5;           // mutate: change what y contains
```

[Exercises 65-72]

---

## Borrow Checker Internals

The borrow checker operates entirely in compile-time projection through this pipeline:

```
Source Code
     │
     ▼
┌───────────────┐
│  Lower to MIR │  ← IDENTITY operations made explicit
└───────────────┘
     │
     ▼
┌───────────────┐
│   Build CFG   │  ← Control flow graph (branching TIME)
└───────────────┘
     │
     ▼
┌───────────────┐
│   Liveness    │  ← "Will this IDENTITY be used later?"
└───────────────┘
     │
     ▼
┌───────────────┐
│    Region     │  ← Compute minimal TIME spans
│   Inference   │
└───────────────┘
     │
     ▼
┌───────────────┐
│   Conflict    │  ← Check SPACE × TIME × IDENTITY
│   Detection   │
└───────────────┘
     │
  ▼       ▼
 OK     ERROR
```

**MIR** makes IDENTITY operations explicit:
```
_2 = &(*_1)     // borrow: create IDENTITY
_3 = use(_2)    // use: access through IDENTITY
drop(_2)        // end: IDENTITY released
```

**Liveness** asks: from this point forward, on some possible path, will this reference be used? Computed via backward dataflow analysis.

**Region inference** finds minimal TIME spans satisfying all constraints. Fixed-point iteration expands regions until stable.

**Conflict detection** checks: do any two borrows of the same SPACE have overlapping regions where at least one is exclusive?

The analysis is decidable because it uses conservative approximation:

| Perfect question | Approximation |
|------------------|---------------|
| "Will this branch execute?" | "Assume all branches might execute" |
| "Same address?" | "Same base variable → might alias" |
| "Will this be used?" | "Might be used on some path" |

This makes the checker sound (accepted code is safe) but incomplete (some safe code is rejected). Interior mutability exists to handle the false positives.

[Exercises 73-80]

---

## Summary

| Problem | Rust's Solution |
|---------|-----------------|
| When to evaluate? | `const` (compile) vs `let` (runtime) |
| Where to store? | Inlined / stack / heap / static |
| Aliasing + mutation? | Borrow checker: shared OR exclusive |
| Global state? | `static` with atomics or interior mutability |
| Cross-thread sharing? | `Send` / `Sync` traits |
| Escaping the rules? | `unsafe` — you prove coherence |

Rust's bet: prove coherence in compile-time projection. Pay with complexity. Gain zero-cost guarantees at runtime.

**Equivalences**:

| Concept | Equivalent to | Framework |
|---------|---------------|-----------|
| `const` | inlined value | no runtime IDENTITY |
| `static` | single replica | bridges projections |
| `&T` | read replica | shared IDENTITY, frozen TIME |
| `&mut T` | exclusive lock | unique IDENTITY, TIME flows |
| `Mutex<T>` | distributed lock | serialize TIME |
| move | transfer | IDENTITY changes hands |
| `Clone` | snapshot | new IDENTITY, new SPACE |
| `unsafe` | "trust me" | you prove coherence |

---

## Appendix: Switcher Pain Points

**Things that won't compile (and what to do)**:

| C++ pattern | Problem | Rust solution |
|-------------|---------|---------------|
| Self-referential struct | Can't prove reference outlives struct | Use indices, or `Pin` |
| Store iterator, mutate container | Iterator borrows container | Use indices |
| Observer pattern with raw pointers | Aliased mutation | Use channels, `Rc<RefCell<T>>`, or redesign |
| Return reference to local | Reference outlives data | Return owned data, or take output reference as parameter |
| Two mutable references "I know they're disjoint" | Compiler can't prove it | Use `split_at_mut`, indices, or unsafe |

**Things that feel wrong but are fine**:

| Pattern | Why it's OK |
|---------|-------------|
| Clone liberally | Profile first. Cloning is often fast enough. |
| `Rc<RefCell<T>>` everywhere | Fine for prototyping. Refactor later if needed. |
| `unsafe` in leaf code | Not cheating if you prove correctness locally. |
| Many small functions to satisfy borrow checker | Often improves design anyway. |

**Mental model shifts**:

| C++ thinking | Rust thinking |
|--------------|---------------|
| "I'll be careful with this pointer" | "The compiler will verify I'm careful" |
| "This is obviously safe" | "Can I make the safety obvious to the compiler?" |
| "I need shared mutable state" | "Do I? Can I restructure to avoid it?" |
| "I'll add `mut` to fix the error" | "`mut` won't help; the issue is aliasing" |

---

## Appendix: First Contact (Optional)

If you've never touched Rust, spend 20 minutes here first. Run these examples. The first few work fine—Rust isn't alien. Then you'll hit walls. Don't try to understand the errors—just feel the confusion. Then read the main content.

If you've already fought the borrow checker, skip this.

---

**These work fine:**

```rust
fn main() {
    println!("Hello, world!");
}
```

```rust
fn main() {
    let x = 5;
    let y = x + 10;
    println!("{}", y);  // 15
}
```

```rust
fn add(a: i32, b: i32) -> i32 {
    a + b
}

fn main() {
    let result = add(3, 4);
    println!("{}", result);  // 7
}
```

```rust
fn main() {
    let v = vec![1, 2, 3, 4, 5];
    for i in &v {
        println!("{}", i);
    }
}
```

```rust
struct Point {
    x: f64,
    y: f64,
}

fn main() {
    let p = Point { x: 3.0, y: 4.0 };
    let distance = (p.x * p.x + p.y * p.y).sqrt();
    println!("{}", distance);  // 5.0
}
```

```rust
fn main() {
    let mut count = 0;
    while count < 5 {
        println!("{}", count);
        count += 1;
    }
}
```

Normal language. Normal constructs. Now try these:

---

**Example 1: The value vanished**
```rust
fn main() {
    let v = vec![1, 2, 3];
    let w = v;
    println!("{:?}", v);
}
```
```
error[E0382]: borrow of moved value: `v`
```

---

**Example 2: Can't borrow as mutable**
```rust
fn main() {
    let mut v = vec![1, 2, 3];
    let r = &v[0];
    v.push(4);
    println!("{}", r);
}
```
```
error[E0502]: cannot borrow `v` as mutable because it is also borrowed as immutable
```

---

**Example 3: Doesn't live long enough**
```rust
fn main() {
    let r;
    {
        let x = 5;
        r = &x;
    }
    println!("{}", r);
}
```
```
error[E0597]: `x` does not live long enough
```

---

**Example 4: Can't have two mutable references**
```rust
fn main() {
    let mut s = String::from("hello");
    let r1 = &mut s;
    let r2 = &mut s;
    println!("{}, {}", r1, r2);
}
```
```
error[E0499]: cannot borrow `s` as mutable more than once at a time
```

---

**Example 5: Can't return reference to local**
```rust
fn get_string() -> &String {
    let s = String::from("hello");
    &s
}
```
```
error[E0106]: missing lifetime specifier
```

---

**Example 6: The iterator and the mutation**
```rust
fn main() {
    let mut v = vec![1, 2, 3];
    for i in &v {
        v.push(*i + 1);
    }
}
```
```
error[E0502]: cannot borrow `v` as mutable because it is also borrowed as immutable
```

---

**Example 7: Struct holding reference to itself**
```rust
struct SelfRef {
    value: String,
    pointer: &String,
}
```
```
error[E0106]: missing lifetime specifier
```

---

**Example 8: Closure captures**
```rust
fn main() {
    let mut v = vec![1, 2, 3];
    let mut f = || v.push(4);
    println!("{:?}", v);
    f();
}
```
```
error[E0502]: cannot borrow `v` as immutable because it is also borrowed as mutable
```

---

**Example 9: Thread needs 'static**
```rust
fn main() {
    let v = vec![1, 2, 3];
    std::thread::spawn(|| {
        println!("{:?}", v);
    });
}
```
```
error[E0373]: closure may outlive the current function, but it borrows `v`
```

---

**Example 10: Rc is not Send**
```rust
use std::rc::Rc;
use std::thread;

fn main() {
    let r = Rc::new(5);
    thread::spawn(move || {
        println!("{}", r);
    });
}
```
```
error[E0277]: `Rc<i32>` cannot be sent between threads safely
```

---

Each error points to a concept in the main content. After reading the framework, come back. The errors will make sense.

---

## What's Next

You get it now.

Not all of Rust—there's error handling, traits, generics, iterators, async, macros, and more. But those are just syntax and libraries. Other tutorials cover them well, and now they'll click.

What you have is what other tutorials don't give you: the mental model that makes the borrow checker obvious instead of arbitrary. The framework that turns cryptic errors into clear statements. The understanding that lets you predict what the compiler wants before it tells you.

You can read three books and complete five courses and still feel like you're guessing. That's what this fixes.

You're ready to expand.
