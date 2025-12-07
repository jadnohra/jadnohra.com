---
layout: page
title: Missing Intuitions
permalink: /missing-intuitions/
---

## Two-Pointer Technique (Two Sum on Sorted Array)

### About

The two-pointer technique uses two indices traversing a data structure in coordinated ways to solve problems efficiently (often O(n) instead of O(n²)).

Common applications:
- **Two Sum on sorted array** - Pointers at start/end, move inward based on sum
- **Merge operation** - Compare elements from two sorted arrays
- **Palindrome check** - Compare from both ends toward middle
- **Sliding window** - Define expanding/contracting ranges
- **Partitioning** - Swap elements around a pivot
- **Cycle detection** - Slow/fast pointers in linked lists

### Missing Intuition

For Two Sum on a sorted array, explanations say "if sum is too large, move right pointer left" without explaining *why* this is safe.

**Think of it as eliminating candidate elements that cannot possibly be part of the solution.**

Given sorted array `[a₀, a₁, ..., aₙ]` and target sum `T`:

- Start with `s₀ = min + max` (smallest + largest)
- **If s₀ > T:** The sum is too large. But `max` is already paired with the smallest possible element. So `max` cannot be part of any valid pair — safely eliminate it from the candidates.
- **If s₀ < T:** The sum is too small. But `min` is already paired with the largest possible element. So `min` cannot be part of any valid pair — safely eliminate it from the candidates.

After safely eliminating one candidate, check `s₁` on the remaining array. Repeat.

![Merge sort intuition](/assets/img/missing-intuitions/merge-sort-intuition.png)
