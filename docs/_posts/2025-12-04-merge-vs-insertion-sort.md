---
title: "Why Merge Sort Beats Insertion Sort: The Search for Fundamental Reasons"
date: 2025-12-04
categories: [Mathematics, Computer Science]
tags: [algorithms, sorting, information-theory, fundamentals]
math: true
---

## The Search for Fundamental Reasons

Studying, in particular studying math, in particular studying math without constraints guided by my own questions, has been the most rewarding and the most frustrating activity, simultaneously.

![Studying notes](https://github.com/user-attachments/assets/288381f1-4ef0-4b0f-85de-9fb81d1c00d1){: .w-75 .shadow .rounded-10 }
_Scribblings from my mathematical explorations_

Below is a series of screenshots from 2016, from my [TheNotes](https://github.com/jadnohra/TheNotes?tab=readme-ov-file#thenotes) collection in which I hopelessly try to cram linear notation into a graphical proof notation, that tries to be consistent and as powerful as first order logic, but much more dense, carrying a lot of information by being 2 dimensional (graphical) and not purely textual.

![Graphical notation 1](https://github.com/user-attachments/assets/c16af764-34ea-4391-8535-23256923892b){: .w-75 .shadow .rounded-10 }

![Graphical notation 2](https://github.com/user-attachments/assets/8cdea939-7a9b-4e71-8e7d-c5891853b048){: .w-75 .shadow .rounded-10 }

![Graphical notation 3](https://github.com/user-attachments/assets/9c81e7ce-122f-4d0b-ab34-06317d516364){: .w-75 .shadow .rounded-10 }

![Graphical notation 4](https://github.com/user-attachments/assets/cf58d773-b619-4d50-a70a-e6a2ca3290c0){: .w-75 .shadow .rounded-10 }

![Graphical notation 5](https://github.com/user-attachments/assets/a6f8bb14-4b74-48df-8544-4f7ff9d1503c){: .w-75 .shadow .rounded-10 }

I failed miserably, but the scribblings evoke very good feelings of emersion and brain involvement. This led though to some really nice figures, one of them I liked so much that I digitized it (see the [TheNotes README](https://github.com/jadnohra/TheNotes?tab=readme-ov-file#thenotes)) and made it the symbol of my Notes collection — it is a graphical 'machine' that almost mechanistically tries to express how a Taylor series power expansion 'produces' terms.

![TheNotes symbol](https://raw.githubusercontent.com/jadnohra/TheNotes/40948df73393f2c7b06dd7389d826bbbfd4533c8/TheNotes.png){: .w-50 .shadow .rounded-10 .mx-auto .d-block }
_The Taylor series 'machine' — symbol of TheNotes_

## Merge Sort versus Insertion Sort

This studying was always trying to understand the deep reasons why things work out the way they do. And I noticed that my gripe with how computer science is (at least usually) taught, does not really provide such deep reasons. I was reminded of that when I stumbled once more on Insertion sort (O(n²)) versus merge sort (O(n log n)). Why is that so? Almost all answers go into showing that this is true, but do not address the simple fundamental question of **why**. The question a Greek mentor would want their students to ask.

One of the really good uses of how LLMs embed information and the fact that they contain so much information crammed into a single entity, is that sometimes after a lot of iteration, I do manage to refine intuitions I had about fundamental answers to such questions. I wish I had access to LLMs when I was still deeply engaged in studying mathematics.

So let's get to it, **Why?** Here is how I would have wished the two algorithms would be contrasted when teaching them (with Claude).

---

## Why Merge Sort Beats Insertion Sort: The Real Reason

Look at the micro-level efficiency. To merge two sorted arrays of size k/2, you do ~k comparisons and emit k elements — one comparison per element. To insert one element into a sorted array of size k-1, you do up to k-1 comparisons and emit one element — k comparisons per element. Merge is radically more efficient per operation.

But wait: merge sort re-examines elements too. After a merge, those elements aren't in their final position — they'll participate in higher-level merges. So both algorithms re-examine elements. The difference is *how many times*.

In merge sort, each element participates in exactly log n merges — one per level of the tree. In insertion sort, each element, once placed, gets scanned by every future insertion that lands before it. Element #1 might be compared against nearly every other element — that's O(n) comparisons for a single element. Sum across all elements: O(n²). In merge sort, each element is compared O(log n) times. Sum: O(n log n).

**The deep truth is that merge sort's tree structure *caps* per-element re-examination at log n. Insertion sort has no such cap — and that missing ceiling is exactly where you pay.**

## The Information-Theoretic View

There's an even deeper way to see this. To fully sort n elements, you need to determine the total ordering. There are n! possible orderings, so you need log₂(n!) ≈ n log n bits of information. Each comparison yields at most 1 bit. So you need at least n log n comparisons. That's the theoretical floor.

**Merge sort:** when you compare the heads of two sorted arrays, you're at maximum uncertainty — either could be smaller. Each comparison yields a full bit. You do ~n log n comparisons, extract ~n log n bits. Optimal.

**Insertion sort (linear scan):** when you scan to find element X's position, you ask questions whose answers are partially implied by transitivity. If you've established X > A and you already know A > B, then X > B is implied — but you compare X to B anyway. You're asking ~n² comparisons to get n log n bits. Wasteful.

**Insertion sort (binary search):** now you ask only log k comparisons per insertion — no redundant questions. Total: O(n log n) comparisons. Optimal information extraction! But you still do O(n) physical moves per insertion. The data structure can't keep up with the information.

So the information-theoretic view:
- **Merge sort:** efficient questions, efficient physical work
- **Insertion sort (binary search):** efficient questions, inefficient physical work
- **Insertion sort (linear scan):** inefficient questions, inefficient physical work

Merge sort's structure lets you capitalize on information both logically and physically. That's the deep win.

---

## But Wait — What Even IS a "Bit of Information"?

Information is reduction of uncertainty. Before you hear something, there's stuff you don't know. After, you know more. The "information" is the difference — the gap that got closed.

A bit is the information gained from a perfect 50/50 question. You genuinely have no idea. Could be either. You ask, you get the answer. That feeling of "now I know, and I truly didn't before"? That's one bit.

The key insight: **information depends on your prior uncertainty.** If someone tells you "the sun rose today" — that's almost zero information. You already knew. No uncertainty was reduced. If someone tells you the outcome of a truly fair coin flip — that's one bit. You had no idea, now you know. If someone tells you which side a 6-sided die landed on — that's log₂(6) ≈ 2.58 bits. More uncertainty existed, more got resolved.

So "bits of information" is just a measure of how much uncertainty got killed.

The formula: if something had probability p of happening, and you learn it happened, you gained -log₂(p) bits.
- Coin flip (p = 0.5): -log₂(0.5) = 1 bit
- Die roll (p = 1/6): -log₂(1/6) ≈ 2.58 bits
- "Sun rose" (p ≈ 1): -log₂(1) = 0 bits

---

## But Why Log? Why is "N Possibilities" the Same as "log₂(N) Bits"?

Flip it around. If I give you k bits, how many things can you distinguish?
- 1 bit → 2 things (yes/no)
- 2 bits → 4 things
- 3 bits → 8 things
- k bits → 2ᵏ things

So if you have N things, you need k bits where 2ᵏ = N. Solve for k: k = log₂(N).

Bits and possibilities are just two views of the same thing, related by the log.*

_*Note to self: but how come? It's because the position of a bit is information as well — think of tuple constructions from sets._

---

## But Why is Halving the Best You Can Do with One Yes/No Question?

Say you have 100 possibilities. You ask a yes/no question.

**50/50 split:** either answer leaves 50. You always cut in half. Guaranteed progress.

**90/10 split:** 90% of the time you get "yes" → 90 left (learned almost nothing). 10% of the time you get "no" → 10 left (learned a lot!). But that jackpot is rare. Expected remaining: 0.9 × 90 + 0.1 × 10 = 82. Worse than halving.

The lopsided question usually gives you the boring, expected answer. You wasted a question on "yeah, obviously."

**Information is surprise.** A 50/50 question guarantees you'll be surprised — either answer is equally unexpected. A 90/10 question is boring most of the time. "Did you really need to ask that?"

Maximum information = maximum guaranteed surprise = both outcomes equally likely = halving.

That's why 1 bit is the ceiling for a yes/no question. You only hit it when neither answer is obvious.

---

## Back to Sorting: It's Just 20 Questions

Before sorting, your array is in *some* order — one of n! possible permutations. You don't know which. Sorting means figuring out which permutation you're holding.

Each comparison is a yes/no question. A perfect question halves the remaining possibilities. How many halvings to get from n! down to 1?

**log₂(n!) ≈ n log n.**

That's the floor. No comparison-based sort can beat it.

Merge sort asks near-perfect questions — when comparing heads of two sorted arrays, it's genuinely 50/50. It hits the floor.

Insertion sort (linear scan) asks redundant questions — ones whose answers were already implied by transitivity. It pays n² for n log n bits of actual information.

**The algorithm that asks the right questions, at the right time, with data structures that can keep up — that's the one that wins.**
