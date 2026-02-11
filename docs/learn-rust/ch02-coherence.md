---
layout: course
title: The Coherence Problem
short_title: Coherence
chapter: 2
permalink: /learn-rust/coherence/
---

# The Coherence Problem

Two paths to the same SPACE. One writes, one reads. The reader sees partial state. This is incoherence.

> Sharing freezes. Mutation isolates.

Either:
- **Shared IDENTITY** — many can access — TIME must freeze (no mutation)
- **Exclusive IDENTITY** — one can access — TIME can flow (mutation ok)

---

## How Rust Enforces This

The borrow checker analyzes your code at compile time:

1. **Tracks names** — which variables alias which SPACE
2. **Builds a graph** — possible execution paths (the CFG)
3. **Computes regions** — when each reference is live
4. **Checks for conflicts** — shared + mutation overlapping?

```rust
let mut x = 5;
let r = &x;        // shared IDENTITY starts
println!("{}", r);  // shared IDENTITY used, then ends
let m = &mut x;     // exclusive starts — OK, no overlap
```

The borrow checker sees: shared region ends before exclusive begins. No conflict.

```rust
let mut x = 5;
let r = &x;        // shared IDENTITY starts
let m = &mut x;     // exclusive starts — ERROR
println!("{}", r);  // shared still live here
```

The borrow checker sees: shared and exclusive regions overlap. Conflict. Rejected.

---

## SPACE Granularity

The borrow checker tracks IDENTITY at path granularity:

| Path type | Can prove disjoint? | Why |
|-----------|--------------------|----|
| `p.x` vs `p.y` | Yes | Struct fields known at compile time |
| `v[0]` vs `v[1]` | No | Indices are runtime values |

```rust
let mut p = Point { x: 1, y: 2 };
let rx = &mut p.x;
let ry = &mut p.y;  // OK: different fields
```

```rust
let mut v = vec![1, 2, 3];
let r = &mut v[0];
let s = &mut v[1];  // ERROR: both "borrow v"
```

The checker sees both as "borrow of v" — it can't prove `v[0] != v[1]` in general.
