---
layout: course
title: The Framework
short_title: Framework
chapter: 1
permalink: /learn-rust/framework/
---

# The Framework

Rust's memory model is a set of choices on the SPACE / TIME / IDENTITY triangle.

- **SPACE**: Where data lives (bytes, layouts, allocations)
- **TIME**: When things happen (compile-time analysis, runtime execution, scope boundaries)
- **IDENTITY**: How we refer to data (names, addresses, ownership relationships)

---

## The Two Projections

Each axis exists in two forms:

| Axis | Compile-time | Runtime |
|------|-------------|---------|
| SPACE | Types, layouts | Bytes in memory |
| TIME | CFG regions | Execution sequence |
| IDENTITY | Names | Addresses |

The borrow checker works entirely in the compile-time projection. It never sees addresses or actual execution — it reasons about names, types, and possible control-flow paths.

---

## Zero-Cost Abstraction

Once the borrow checker proves safety, the abstractions are erased. The compiled binary has no borrow checking overhead because there's nothing left to check.

The cost is compile time and programmer effort. The runtime pays nothing.

---

## C++ Comparison

This framework applies to C++ as well:

| C++ | Framework |
|-----|-----------|
| `T*`, `T&` | IDENTITY pointing to SPACE |
| `const` | TIME frozen (no mutation) |
| Scope, RAII | TIME boundaries |
| `new`/`delete`, stack | SPACE allocation |
| `std::unique_ptr<T>` | Exclusive IDENTITY, owned SPACE |
| `std::shared_ptr<T>` | Shared IDENTITY, reference-counted SPACE |

C++ lets you manage these axes freely — and lets you get them wrong. Rust adds a compile-time verifier that proves your SPACE/TIME/IDENTITY relationships are coherent.
