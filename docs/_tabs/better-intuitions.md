---
layout: post
title: Better Intuitions
icon: fas fa-lightbulb
order: 4
toc: true
---

<style>
.thumb {
  display: block;
  margin: 1rem auto;
  width: 200px;
  cursor: zoom-in;
  border: 1px solid #ddd;
  border-radius: 4px;
}
.lightbox {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background: rgba(0,0,0,0.9);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999;
  cursor: zoom-out;
}
.lightbox img {
  max-width: 90%;
  max-height: 90%;
  border-radius: 4px;
}
</style>

<script>
document.addEventListener('DOMContentLoaded', function() {
  document.querySelectorAll('.thumb').forEach(img => {
    img.addEventListener('click', function(e) {
      e.preventDefault();
      e.stopPropagation();
      if (document.querySelector('.lightbox')) return;
      var lightbox = document.createElement('div');
      lightbox.className = 'lightbox';
      var bigImg = document.createElement('img');
      bigImg.src = this.src;
      lightbox.appendChild(bigImg);
      document.body.appendChild(lightbox);
      lightbox.addEventListener('click', function() {
        this.remove();
      });
    });
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

### Better Intuition (close to a proof)

**TL;DR:** It's an elimination argument, not a search. Each step safely eliminates one element that cannot possibly be part of any solution.

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

---

## [DRAFT] Why Merge Sort Beats Insertion Sort

### About

This studying was always trying to understand the deep reasons why things work out the way they do. And I noticed that my gripe with how computer science is (at least usually) taught, does not really provide such deep reasons. I was reminded of that when I stumbled once more on Insertion sort (O(n²)) versus merge sort (O(n log n)). Why is that so? Almost all answers go into showing that this is true, but do not address the simple fundamental question of **why**. The question a Greek mentor would want their students to ask.

### Better Intuition (close to a proof)

**TL;DR:** Merge sort's tree structure caps per-element re-examination at log n. Insertion sort has no such cap — and that missing ceiling is exactly where you pay.

Look at the micro-level efficiency. To merge two sorted arrays of size k/2, you do ~k comparisons and emit k elements — one comparison per element. To insert one element into a sorted array of size k-1, you do up to k-1 comparisons and emit one element — k comparisons per element. Merge is radically more efficient per operation.

But wait: merge sort re-examines elements too. After a merge, those elements aren't in their final position — they'll participate in higher-level merges. So both algorithms re-examine elements. The difference is *how many times*.

In merge sort, each element participates in exactly log n merges — one per level of the tree. In insertion sort, each element, once placed, gets scanned by every future insertion that lands before it. Element #1 might be compared against nearly every other element — that's O(n) comparisons for a single element. Sum across all elements: O(n²). In merge sort, each element is compared O(log n) times. Sum: O(n log n).

---

> Shouldn't merge sorting two arrays of size k/2 be MORE expensive than insertion sorting k-1 with 1 new?

Here's the insight: look at what fraction of the O(k) work is "new progress."

**Insertion sort:** You pay O(k) to place 1 new element. The other k-1 elements were already sorted — you're re-scanning them just to find where the new guy goes. **Efficiency: 1/k of your work is "new."**

**Merge sort:** You pay O(k) to place k elements. Both halves need interleaving — every comparison emits one element into its final position at this level. **Efficiency: k/k = 100% of your work is "new."**

So the deep answer: insertion sort keeps re-examining already-sorted elements. Every time you insert, you're paying a "tax" to traverse the sorted prefix again. Element #1 gets looked at n-1 times. Element #2 gets looked at n-2 times. Etc.

Merge sort never re-examines within a level. Each element participates in exactly log n merge operations total (once per level), and at each level the total work across all merges is O(n).

**The root cause:** insertion is "read-heavy" on old data. Merge is "write-balanced" — every comparison emits one element, no wasted scans.

---

### The Information-Theoretic View

There's an even deeper way to see this. To fully sort n elements, you need to determine the total ordering. There are n! possible orderings, so you need log₂(n!) ≈ n log n bits of information. Each comparison yields at most 1 bit. So you need at least n log n comparisons. That's the theoretical floor.

**Merge sort:** when you compare the heads of two sorted arrays, you're at maximum uncertainty — either could be smaller. Each comparison yields a full bit. You do ~n log n comparisons, extract ~n log n bits. Optimal.

**Insertion sort (linear scan):** when you scan to find element X's position, you ask questions whose answers are partially implied by transitivity. If you've established X > A and you already know A > B, then X > B is implied — but you compare X to B anyway. You're asking ~n² comparisons to get n log n bits of actual information. Wasteful.

**Insertion sort (binary search):** now you ask only log k comparisons per insertion — no redundant questions. Total: O(n log n) comparisons. Optimal information extraction! But you still do O(n) physical moves per insertion. The data structure can't keep up with the information.

So the information-theoretic view:
- **Merge sort:** efficient questions, efficient physical work
- **Insertion sort (binary search):** efficient questions, inefficient physical work
- **Insertion sort (linear scan):** inefficient questions, inefficient physical work

Merge sort's structure lets you capitalize on information both logically and physically. That's the deep win.

---

### But Wait — What Even IS a "Bit of Information"?

Information is reduction of uncertainty. Before you hear something, there's stuff you don't know. After, you know more. The "information" is the difference — the gap that got closed.

A bit is the information gained from a perfect 50/50 question. You genuinely have no idea. Could be either. You ask, you get the answer. That feeling of "now I know, and I truly didn't before"? That's one bit.

The key insight: **information depends on your prior uncertainty.** If someone tells you "the sun rose today" — that's almost zero information. You already knew. No uncertainty was reduced. If someone tells you the outcome of a truly fair coin flip — that's one bit. You had no idea, now you know. If someone tells you which side a 6-sided die landed on — that's log₂(6) ≈ 2.58 bits. More uncertainty existed, more got resolved.

So "bits of information" is just a measure of how much uncertainty got killed.

The formula: if something had probability p of happening, and you learn it happened, you gained -log₂(p) bits.
- Coin flip (p = 0.5): -log₂(0.5) = 1 bit
- Die roll (p = 1/6): -log₂(1/6) ≈ 2.58 bits
- "Sun rose" (p ≈ 1): -log₂(1) = 0 bits

---

### But Why Log? Why is "N Possibilities" the Same as "log₂(N) Bits"?

Flip it around. If I give you k bits, how many things can you distinguish?
- 1 bit → 2 things (yes/no)
- 2 bits → 4 things
- 3 bits → 8 things
- k bits → 2ᵏ things

So if you have N things, you need k bits where 2ᵏ = N. Solve for k: k = log₂(N).

Bits and possibilities are just two views of the same thing, related by the log.*

_*Note to self: but how come? It's because the position of a bit is information as well — think of tuple constructions from sets._

---

### But Why is Halving the Best You Can Do with One Yes/No Question?

Say you have 100 possibilities. You ask a yes/no question.

**50/50 split:** either answer leaves 50. You always cut in half. Guaranteed progress.

**90/10 split:** 90% of the time you get "yes" → 90 left (learned almost nothing). 10% of the time you get "no" → 10 left (learned a lot!). But that jackpot is rare. Expected remaining: 0.9 × 90 + 0.1 × 10 = 82. Worse than halving.

The lopsided question usually gives you the boring, expected answer. You wasted a question on "yeah, obviously."

**Information is surprise.** A 50/50 question guarantees you'll be surprised — either answer is equally unexpected. A 90/10 question is boring most of the time. "Did you really need to ask that?"

Maximum information = maximum guaranteed surprise = both outcomes equally likely = halving.

That's why 1 bit is the ceiling for a yes/no question. You only hit it when neither answer is obvious.

---

### Back to Sorting: It's Just 20 Questions

Before sorting, your array is in *some* order — one of n! possible permutations. You don't know which. Sorting means figuring out which permutation you're holding.

Each comparison is a yes/no question. A perfect question halves the remaining possibilities. How many halvings to get from n! down to 1?

**log₂(n!).**

That's it. That's the whole argument.

And log₂(n!) ≈ n log n (Stirling's approximation: n! ≈ (n/e)ⁿ, take the log).

So the "n log n bits" just means: there are n! possible inputs, and you're playing 20 Questions to figure out which one you have. You can't win in fewer than log₂(n!) questions. Each comparison is one question.

That's the floor. No comparison-based sort can beat it.

Merge sort asks near-perfect questions — when comparing heads of two sorted arrays, it's genuinely 50/50. It hits the floor.

Insertion sort (linear scan) asks redundant questions — ones whose answers were already implied by transitivity. It pays n² for n log n bits of actual information.

**The algorithm that asks the right questions, at the right time, with data structures that can keep up — that's the one that wins.**
