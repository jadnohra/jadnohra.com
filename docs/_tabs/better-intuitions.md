---
layout: page
title: Better Intuitions
icon: fas fa-lightbulb
order: 4
---

<style>
.thumb {
  display: block;
  margin: 1rem auto;
  max-width: 200px;
  cursor: zoom-in;
  transition: max-width 0.3s ease;
  border: 1px solid #ddd;
  border-radius: 4px;
}
.thumb.expanded {
  max-width: 100%;
  cursor: zoom-out;
}
</style>

<script>
document.addEventListener('DOMContentLoaded', function() {
  document.querySelectorAll('.thumb').forEach(img => {
    img.addEventListener('click', () => img.classList.toggle('expanded'));
  });
});
</script>

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

Note: we're walking the space of sums in an interesting way — neither from smallest to largest nor largest to smallest, but along a path informed by the min/max elimination.

<img src="/assets/img/better-intuitions/two-pointer-two-sum.png" alt="Two-pointer Two Sum" class="thumb">

The image shows why the typical framing is counterintuitive: if you think of being at some `sᵢ` and comparing to the target, you have 4 choices (increment or decrement either pointer). But from the elimination view, there's only one safe move — eliminate the candidate that cannot possibly work.

<img src="/assets/img/better-intuitions/two-pointer-navigation-path.png" alt="Navigation path through sum space" class="thumb">
