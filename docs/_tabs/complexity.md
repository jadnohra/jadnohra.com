---
layout: post
title: Complexity
icon: fas fa-project-diagram
order: 4
toc: true
---

<style>
/* Compact layout */
.content p { margin-bottom: 0.5rem; }
.content h2 { margin-top: 1.5rem; margin-bottom: 0.5rem; }
.content h3 { margin-top: 1rem; margin-bottom: 0.3rem; }
.content ul, .content ol { margin-bottom: 0.5rem; }
.content li { margin-bottom: 0.2rem; }
.content blockquote { margin: 0.5rem 0; }
.content hr { margin: 1rem 0; }
</style>

## Part I: Complexity Classes

Computation transforms states. Some target states are easy to reach. Some are hard. Complexity classes characterize *why*.

---

### Computation as Transformation

```
Computation:
  input state → transformation → output state

The question:
  reach a state with property P
```

Every computational problem is: navigate a state space to reach states satisfying some property.

```
Sorting:       reach the one permutation that is ordered
SAT:           reach an assignment that satisfies all clauses
Shortest path: reach the path with minimum weight
Factoring:     reach the pair of numbers whose product is n
```

---

### Why Things Are Hard

Hardness comes from the relationship between:

```
1. State space size       how many possible states exist
2. Solution density       what fraction satisfies the property
3. Structure              does local information help navigate?
4. Convergence            can each step guarantee progress?
```

---

### The Convergence View

Some computations *converge* — each step reduces distance to solution.

```
Sorting:
  state space: n! permutations
  property: sorted order
  density: 1/n! (exactly one solution)

  But: each comparison eliminates half the remaining permutations.
  log(n!) ≈ n log n comparisons → found.

  Convergent: each step halves the space.
```

```
Binary search:
  state space: n positions
  property: target value

  Each comparison halves the space.
  log n steps → found.

  Convergent: exponential shrinkage per step.
```

These are easy not because solutions are dense, but because structure enables convergence.

---

### The Non-Convergent View

Some computations don't guarantee progress.

```
SAT:
  state space: 2^n assignments
  property: satisfies all clauses
  density: sparse (often 0, sometimes tiny fraction)

  Partial assignment tells you:
    - definitely unsatisfiable (contradiction found) — prune
    - definitely satisfiable (all clauses satisfied) — done
    - unknown — keep going

  The "unknown" case: you must commit without knowing if path leads anywhere.
  Wrong commitment might require exponential backtracking.

  Non-convergent: steps might make progress or hit dead end.
```

---

### The Asymmetry

Forward computation is cheap. Backward is expensive.

```
Hash:         input → output    O(n)
              output → input    O(2^n) search

Multiply:     (p, q) → n        O(n²)
              n → (p, q)        sub-exponential (best known)

Circuit:      input → output    O(gates)
              output → input    potentially O(2^inputs)
```

Why?

Forward: follow deterministic steps, one path.
Backward: many inputs could produce this output, which one?

Computation is *lossy* — multiple inputs map to same output. Going backward means recovering lost information.

```
Forward:   many → one (convergent, information lost)
Backward:  one → many (divergent, must search)
```

---

### Properties and Witnesses

A *property* is a predicate: P(state) ∈ {true, false}.

A *witness* is evidence that P holds: something that makes verification easy.

```
SAT:
  property: formula is satisfiable
  witness: a satisfying assignment
  verification: substitute and check — O(clauses)

Composite:
  property: n is composite
  witness: a factor
  verification: divide and check — O(n²)

Graph coloring:
  property: k-colorable
  witness: a valid coloring
  verification: check each edge — O(edges)
```

The asymmetry:
- Finding witness: might require searching exponential space
- Checking witness: polynomial in witness size

NP is precisely: properties where witnesses exist and are efficiently checkable.

---

### Structure and Exploitability

Hardness isn't just about space size. It's about *exploitable structure*.

```
Structure type              Effect on search
─────────────────────────────────────────────────────────
Decomposable                solve parts independently, combine
Monotonic                   partial progress is permanent
Gradient-like               local information guides globally
Constraint propagation      assignment forces other assignments
Symmetry                    equivalence classes reduce space
None                        must enumerate
```

P problems have structure that guarantees polynomial navigation.

NP-hard problems (assuming P≠NP) have structure sufficient for verification but not navigation.

---

### The Gradient Analogy

Optimization landscapes:

```
Convex:
  every local step toward lower loss reaches global optimum
  gradient always points right direction
  polynomial convergence guaranteed

Non-convex:
  local minima trap you
  might need to go uphill to reach global minimum
  local information misleads

  no polynomial guarantee
```

P vs NP is analogous:

```
P:
  there exists a computation path where each step makes provable progress
  "downhill all the way"
  polynomial steps suffice

NP:
  verification is convex (check each clause, combine)
  search is non-convex (partial assignment might lead nowhere)
  local information insufficient for global navigation
```

---

### Information and Location

Finding a solution = locating it in state space.

Location requires information. Each bit of information halves the candidates.

```
Space size 2^n → need n bits to locate a point.

P: each computation step extracts enough bits to make polynomial total.
   sorting: each comparison extracts 1 bit, n log n comparisons → located.

NP: steps might extract bits, but dead ends waste work.
    backtracking = discarding information, must re-acquire.
```

The witness is a compressed representation of the search path — all the information needed to navigate directly.

```
Witness size: poly(n)
Information content: enough to verify
But: extracting this information might require exponential search
```

---

### The Hierarchy Restated

```
Class         Structure                       Convergence
────────────────────────────────────────────────────────────────
P             exploitable, poly navigation    guaranteed progress
NP            verifiable, not navigable       progress or dead end
PSPACE        alternating (games)             adversary blocks progress
EXPTIME       provably unstructured           must enumerate
```

---

### Reductions as Structure Preservation

Reduction from A to B: transform A instances into B instances, preserving structure.

If A reduces to B, then B has at least as much structure (or as little) as A.

NP-complete: all NP problems reduce here. They share a common hard core — the irreducible structure that makes search hard.

```
SAT, 3-SAT, clique, coloring, TSP, knapsack, vertex cover...

Different surfaces, same underlying hardness.
All encode: sparse solution in exponential space, verifiable but not navigable.
```

---

### Concrete Numbers

```
State space              Size at n=100
──────────────────────────────────────
Linear choices           100
Polynomial (n²)          10,000
Polynomial (n³)          1,000,000
Permutations (n!)        10^158
Subsets (2^n)            10^30
```

```
Operations per second:   10^9 (modern CPU)
Seconds in universe:     10^17
Total operations ever:   10^26

2^100 = 10^30 > operations possible in universe lifetime
```

The exponential wall is physical. No speedup crosses it.

---

### The Practical Upshot

When facing a problem:

```
1. Is there convergent structure?
   → Each step guaranteed progress? → probably P

2. Is there verifiable structure?
   → Can check solution in poly time? → in NP

3. Does it reduce from known NP-hard?
   → Stop looking for poly algorithm
   → Use heuristics, approximation, or exploit instance structure

4. SAT solvers work on structured instances
   → Real problems aren't random
   → Structure = pruning opportunities
   → Exponential worst case, often poly in practice
```

---

### Principle

> Hardness is non-convergence: the inability to guarantee progress.

P: structure ensures convergence — each step shrinks the search space by polynomial factor.

NP: structure ensures verifiability — but search might diverge before converging.

The open question (P vs NP): is there always hidden structure that enables convergence, or is non-convergence fundamental?

---

## Part II: Why Polynomial?

### Why Polynomial? (The Real Answer)

The standard answer — "composition works" — is true but doesn't explain *why we care*.

Here's the better angle:

**Polynomial = processing the input itself.**
**Exponential = enumerating what the input could represent.**

```
Input: n bits describing a SAT formula
  Polynomial: process the formula — O(n) clauses, O(n²) pairs of clauses
  Exponential: process the assignments — 2^n possible truth assignments

Input: n nodes in a graph
  Polynomial: process the graph — O(n²) edges, O(n³) triples
  Exponential: process subsets — 2^n subsets of nodes
```

The transition from polynomial to exponential is crossing from "things derived from input" to "things the input quantifies over."

---

### The Exponent's Location

```
Polynomial:  n^k    — exponent k is fixed constant (from problem structure)
Exponential: k^n    — exponent n is the input size

O(n³) — "consider all triples" — 3 is fixed
O(2^n) — "consider all subsets" — n grows with input
```

Polynomial: the structure of what you examine is fixed. Input size just scales it.

Exponential: the structure of what you examine *depends on* input size. More input = qualitatively more structure to explore.

---

### The Physical Angle

Polynomial growth is what physical processes do.

```
Heat diffusion:     spreads proportional to surface area (n²)
Gravitational calc: all pairs interact (n²)
Chemical mixing:    proportional to volume (n³)
```

These scale polynomially because physical space is 3D and time is 1D. No physical process naturally produces 2^n behavior — that requires *information structure*, not physical structure.

Exponential (2^n) means: the number of *configurations* of n things. This is combinatorial, not physical. It's about *possibilities*, not *objects*.

```
10 light switches: 2^10 = 1024 configurations
100 light switches: 2^100 ≈ 10^30 configurations

The switches are physical (100 objects).
The configurations are combinatorial (10^30 possibilities).
```

Processing objects: polynomial in object count.
Enumerating configurations: exponential in object count.

---

### The Information Angle

To solve a problem, you need enough information to locate the answer.

```
Sorting n elements:
  Answer space: n! permutations ≈ 2^(n log n)
  Information needed: n log n bits
  Each comparison: 1 bit
  Comparisons needed: n log n — polynomial

  Why? Each comparison *guarantees* 1 bit of information.
  Progress is certain.

SAT with n variables:
  Answer space: 2^n assignments
  Information needed: n bits
  Each step: might learn something, might hit dead end

  Why exponential? Dead ends don't give proportional information.
  You might do 2^(n/2) work before learning you're on wrong path.
```

Polynomial: information acquisition is efficient — work translates to progress.

Exponential: information acquisition is inefficient — work might not translate to progress.

---

### The Scaling Test

Here's a concrete test that distinguishes P from NP-hard:

**Can solving size n help you solve size 2n?**

```
Sorting:
  Sort first half (n), sort second half (n), merge (n).
  Size 2n ≈ 2× work of size n.
  Scaling is linear.

SAT:
  Solve first half of variables? Tells you nothing about second half.
  Size 2n = 2^n × work of size n.
  Scaling is exponential.
```

P problems decompose: solving smaller instances builds toward larger.

NP-hard problems don't decompose: instances are coupled globally, small gives no foothold on large.

---

### The Hierarchy Restated (With This Lens)

```
Class         What you process                    Scaling
─────────────────────────────────────────────────────────────────
P             input + polynomial derived structure  size n → work n^k
NP (search)   configurations input quantifies over  size n → work up to 2^n
NP (verify)   single configuration (witness)        witness size m → work m^k
```

NP's asymmetry:
- Verification processes the witness (polynomial-sized object) — P-like
- Search processes the configuration space (exponential-sized space) — possibly not P

---

### Adding to the Full Picture

Here's what we can add to connect everything:

**Why convergent methods land in P:**

Convergence means each step extracts information proportional to its cost. Work translates to progress. Total work = total information needed = polynomial (because you're locating something in the input's structure, not in the space of configurations).

**Why non-convergent search might require exponential:**

Non-convergence means steps might not extract information. Dead ends waste work. In worst case, you must enumerate the configuration space, which is exponential because it's the space of *possibilities*, not the space of *input-derived structures*.

**Why witnesses make verification easy:**

The witness is the compressed form of the search path — all the bits of information you'd need to locate the solution. Given the witness, you process it (polynomial) rather than search for it (exponential).

```
Search: locate witness in 2^n space → might need 2^n steps
Verify: process witness of size poly(n) → poly(n) steps
```

**Why P vs NP is deep:**

It asks: does every search problem have hidden structure that converts it to polynomial information-extraction?

Or equivalently: is there always a way to process the input that implicitly navigates the configuration space?

Or: is non-convergence fundamental, or always avoidable with the right insight?

---

### Principle

```
The state space has structure:
  - derived from input → polynomial-sized
  - configurations of input → exponential-sized

The computation navigates this space:
  - convergent navigation → each step is progress → polynomial total
  - non-convergent navigation → dead ends → potentially exponential

The boundary:
  - P: exploitable structure enables convergent navigation
  - NP-hard: verification structure exists, navigation structure unknown

The witness:
  - encodes the path through configuration space
  - polynomial size (else verification wouldn't be polynomial)
  - finding it = navigating exponential space
  - checking it = processing polynomial object

The open question (P vs NP):
  - is there always hidden structure that enables convergent search?
  - or is non-convergence sometimes fundamental?
```

---

## Part III: Better Class Names

### Better Names

The standard names are historical accidents. Here's what they actually mean:

```
Standard Name    What It Actually Means               Better Name
───────────────────────────────────────────────────────────────────────
L                log space                            pointer-bound
NL               nondeterministic log space           pointer-bound + guessing
P                polynomial time                      input-bound
NP               nondeterministic polynomial          verifiable
co-NP            complement of NP                     refutable
PSPACE           polynomial space                     game-bound
EXPTIME          exponential time                     history-bound
```

The intuitions:

```
pointer-bound (L):    can only hold pointers into input, can't accumulate
input-bound (P):      work scales with input structure, guaranteed progress
verifiable (NP):      can check a solution fast, finding it might be hard
refutable (co-NP):    can check a counter-example fast
game-bound (PSPACE):  adversary gets to respond, alternating moves
history-bound (EXPTIME): must remember everything, can't compress or forget
```

---

### The Memory View (Unifies Everything)

The entire hierarchy is about **what the machine can remember**:

```
Machine              Memory                  What It Can Track
─────────────────────────────────────────────────────────────────────────
Finite automaton     finite states           "which of k situations am I in?"
                     (constant)              nothing about input itself

Pushdown automaton   finite states + stack   "nesting depth, what's open"
                     (unbounded but LIFO)

Log-space TM         O(log n) bits           "O(1) pointers into input"
                                             can re-scan but not accumulate

Poly-time TM         O(poly n) bits          "polynomial amount about input"
                                             can accumulate, cross-reference

Poly-space TM        O(poly n) bits          same memory as P, but unlimited time
                     (reusable)              can try all possibilities serially

Exp-time TM          O(2^n) bits             "everything"
                                             must track full history
```

**Memory is the variable that moves.**

---

### Regular Expressions and Finite Automata

A finite automaton has constant memory — just its state. No matter how long the input, it remembers only "which of k states am I in."

```
DFA for "contains 'ab'":

    ┌───┐  a   ┌───┐  b   ┌───┐
───►│ 0 │─────►│ 1 │─────►│ 2 │◄──┐
    └───┘      └───┘      └───┘   │
      │ b        │ a        └──any┘
      └──────────┘

States: {0: haven't seen 'a', 1: just saw 'a', 2: saw 'ab'}
Memory: which state (2 bits, constant forever)
```

The machine "forgets" everything except what state it's in. Input length doesn't matter — memory is fixed.

**What regular expressions can't do:**

```
a^n b^n  (same number of a's then b's)

Why? To check this, you must count how many a's you've seen.
Count can grow to n. But DFA has constant memory.
If n > number of states, you'll confuse different counts.

DFA can only ask: "have I seen 0, 1, 2, or 3+ a's?"
Not: "have I seen exactly 47 a's?"
```

The limitation is **counting**. Finite memory can't count unboundedly.

---

### Pushdown Automata and Context-Free

A pushdown automaton adds a stack — unbounded memory but LIFO access.

```
PDA for a^n b^n:

  see 'a' → push 'a'
  see 'b' → pop 'a'
  accept if stack empty at end

Stack grows to n, but only need top.
Can count by accumulating on stack.
```

**What PDA can do that DFA can't:**

```
✓ a^n b^n          (count and match)
✓ balanced parens  (push open, pop on close)
✓ palindromes      (push first half, match second half)
```

**What PDA can't do:**

```
✗ a^n b^n c^n      (need to count twice, stack is consumed)
✗ ww               (exact copy — need random access to first half)
✗ {a^i b^j c^k : i=j or j=k}  (need to compare two different pairs)
```

The limitation is **LIFO access**. Stack can only see the top. Can't cross-reference or access middle.

---

### The Language Hierarchy

```
Regular  ⊂  Context-Free  ⊂  Context-Sensitive  ⊂  Decidable  ⊂  All Languages

Machine:    DFA         PDA              LBA                TM
Memory:     O(1)        O(n) stack       O(n) tape          unbounded
```

Each level adds memory/access capability.

---

### How Automata Map to Complexity Classes

Here's the connection that's rarely explained clearly:

```
Automaton/Language        Complexity of membership testing
──────────────────────────────────────────────────────────────────────
DFA / Regular             O(n) time, O(1) space — in L
PDA / Context-Free        O(n³) time worst case — in P
LBA / Context-Sensitive   PSPACE-complete
TM / Decidable            can be arbitrarily hard
```

But wait — the complexity depends on **what's fixed vs what's input**:

---

### The Input Matters (This Is Key)

**Scenario 1: Fixed regex, string is input**

```
Regex: /[a-z]+@[a-z]+\.[a-z]+/  (email pattern)
String: "user@domain.com"

Compile regex to DFA once (at compile time).
Run string through DFA: O(n) time, O(1) space.

This is what a lexer does. Blazing fast.
Complexity: O(n) — linear in string length.
```

**Scenario 2: Regex is input, string is input**

```
Given: arbitrary regex R and string S
Question: does S match R?

Must process R to build automaton.
NFA is easy: O(|R|) size
NFA → DFA can explode: up to O(2^|R|) states

But: can simulate NFA directly in O(|R| × |S|) time.
Complexity: P (polynomial in both inputs)
```

**Scenario 3: Two regexes as input, test equivalence**

```
Given: regex R1 and R2
Question: is L(R1) = L(R2)?

Must check: every string matched by R1 is matched by R2, and vice versa.

Turns out: PSPACE-complete

Why? The operations compound:
  L(R1) = L(R2)  iff  (L(R1) ∩ L(R2)') ∪ (L(R1)' ∩ L(R2)) = ∅

  Complementing DFA is easy but intersection can blow up.
  Nested regex operations create exponential state spaces.
```

**Scenario 4: "Regexes" with backreferences**

```
Pattern: (a+)\1
Meaning: some a's followed by the same number of a's (a^n a^n = a^2n)

This is NOT regular! It requires counting and remembering.
Programming language "regexes" aren't theoretical regular expressions.

Matching with backreferences: NP-complete
```

---

### The Compiler Connection

```
Compiler Stage     Language Class      Complexity (fixed grammar)
──────────────────────────────────────────────────────────────────
Lexer              Regular             O(n) per token — linear
Parser             Context-Free        O(n) for LL/LR, O(n³) general
Type checking      Context-Sensitive   depends on type system
                   (or worse)          can be EXPTIME for complex systems
```

Why lexer is separate from parser (revisited):

```
Lexer:
  Language class: Regular
  Machine: DFA
  Memory: O(1)
  Time: O(n)

Parser:
  Language class: Context-Free
  Machine: PDA (or table-driven equivalent)
  Memory: O(n) stack in worst case
  Time: O(n) for practical grammars

Mixed (regex rules in CFG):
  Language class: still Context-Free, but...
  Grammar: huge (every character-level rule)
  Time: much worse constants
```

Separation isn't about language power — it's about keeping each stage in its optimal complexity class.

---

### The Full Picture

```
Memory           Automaton        Languages         Complexity
─────────────────────────────────────────────────────────────────────────
O(1)             DFA              Regular           membership in L
                                                    equivalence in PSPACE

O(n) stack       PDA              Context-Free      membership in P
                                                    equivalence undecidable!

O(log n)         log-space TM     —                 L (between Regular and P)

O(n) tape        LBA              Context-Sensitive PSPACE

O(poly n)        poly-time TM     P                 definition of P

O(poly n) +      poly-time NTM    NP                witness checkable
guess

O(poly n) +      poly-space TM    PSPACE            game trees,
reuse time                                          alternation

O(2^n)           exp-time TM      EXPTIME           full enumeration
                                                    provably hard
```

---

### The Naming (Unified)

By memory/capability:

```
Class         Memory              Access           Better Name
───────────────────────────────────────────────────────────────
Regular       O(1) states         none             stateless
Context-Free  O(n) stack          LIFO             stack-bound
L             O(log n) tape       random           pointer-bound
P             O(poly n) tape      random           input-bound
NP            O(poly n) + guess   random           verifiable
PSPACE        O(poly n) reused    random           game-bound
EXPTIME       O(2^n) tape         random           history-bound
```

By what the computation processes:

```
Class         Processes                             Name
───────────────────────────────────────────────────────────────
Regular       characters, no memory                 streaming
Context-Free  characters, with nesting              nested
P             input structures                      structural
NP            witness (finding: configuration)      verifiable
PSPACE        configurations, adversarial           adversarial
EXPTIME       configurations, with full history     archival
```

---

### Principle

```
Regular:     "I process characters but immediately forget them."
             "I only know which state I'm in — nothing about the past."
             Can't count. Can't match nesting.

Context-Free: "I can remember nesting depth via a stack."
              "But I can't cross-reference or look back."
              Can match parens. Can't match a^n b^n c^n.

L:           "I can hold a few pointers into the input."
             "I can re-read but can't accumulate."
             Between Regular and P.

P:           "I can accumulate polynomial information about input."
             "Each step makes progress."
             Processing the input itself.

NP:          "I can verify a polynomial-sized proof."
             "But finding that proof might require search."
             Processing witnesses, searching configurations.

PSPACE:      "I can play games — consider your move, my response, your counter..."
             "Memory is reused. Time is spent enumerating."
             Adversarial alternation.

EXPTIME:     "I must track everything. No forgetting allowed."
             "The history IS the problem."
             Full enumeration with memory.
```

---

## Part IV: Canonical Algorithms

### The Core Insight

Hardness comes from the relationship between:

```
1. State space size       how many possible states exist
2. Solution density       what fraction satisfies the property
3. Structure              does local information help navigate?
4. Convergence            can each step guarantee progress?
```

The key asymmetry:

```
Forward:   follow deterministic steps, one path, information lost
Backward:  many inputs could produce this output, must search
```

Computation is lossy — multiple inputs map to same output. Going backward means recovering lost information.

---

### The Hierarchy

Each class is defined by what the machine can remember:

```
Name           Memory                    Canonical Structure      Canonical Algorithm
─────────────────────────────────────────────────────────────────────────────────────────
lookup         O(1) states               transition table         DFA traversal
stack          O(n) stack                call stack               recursive descent
scanner        O(log n) pointers         two pointers             Floyd's cycle detection
accumulator    O(n^k) facts              visited set              BFS / Dijkstra
verifier       O(n^k) witness            certificate              SAT solver (backtracking)
explorer       O(n^k) reused             unmemoized recursion     minimax (no transposition)
archivist      O(2^n) everything         memoized recursion       minimax (full transposition)
```

---

### Lookup (Regular)

**Memory:** O(1) — fixed number of states, no matter input size.

**Structure:** Transition table. State × symbol → state.

```
        a    b    c
      ┌────┬────┬────┐
  S0  │ S1 │ S0 │ S0 │
  S1  │ S1 │ S2 │ S0 │
  S2  │ S1 │ S0 │ S3 │  ← accept
      └────┴────┴────┘

Size: |states| × |alphabet| — constant
Memory: one cell — "which row am I in?"
```

**Behavior:** Input flows through. State updates. Nothing accumulates.

**Limitation:** Can't count. Can't match nesting. The machine "forgets" everything except which state it's in.

```
Can:    "contains 'ab'"
        "ends with '.com'"
        "valid identifier"

Can't:  a^n b^n (equal counts)
        balanced parens (nesting)
        palindromes (need to remember first half)
```

**Algorithm:** DFA traversal.

```python
def match(table, accept_states, input):
    state = 0
    for char in input:
        state = table[state][char]  # just a lookup
    return state in accept_states
```

**One-liner:** "I'm a table. Input goes in, answer comes out. I remember nothing."

---

### Stack (Context-Free)

**Memory:** O(n) — stack grows with nesting depth, but LIFO only.

**Structure:** Call stack. Recursion itself IS the pushdown automaton.

```python
def parse_expr():
    parse_term()
    while see('+'):
        consume('+')
        parse_term()

def parse_term():
    parse_factor()
    while see('*'):
        consume('*')
        parse_factor()

def parse_factor():
    if see('('):
        consume('(')
        parse_expr()     # stack grows — recursion!
        consume(')')     # stack shrinks — return!
    else:
        parse_number()
```

**Behavior:** Nesting pushes. Return pops. When you return, you forget.

**Limitation:** Can't cross-reference. Can't look back. Only see top of stack.

```
Can:    balanced parens ( )
        a^n b^n (push a's, pop on b's)
        nested structures

Can't:  a^n b^n c^n (need to count twice)
        ww (exact copy — need random access)
        cross-references between parts
```

**Algorithm:** Recursive descent parsing.

**One-liner:** "I'm a stack. I track what's open. When it closes, I forget."

---

### Scanner (L — Log Space)

**Memory:** O(log n) — enough for constant number of pointers into input.

**Structure:** Two pointers. Positions only, no storage.

```python
def has_cycle(head):
    slow = head
    fast = head
    while fast and fast.next:
        slow = slow.next          # one step
        fast = fast.next.next     # two steps
        if slow == fast:
            return True
    return False

# Memory: two pointers
# Can re-read input, can't store anything about it
```

**Behavior:** Scan input. Compare positions. Can re-read but can't accumulate findings.

**Limitation:** Can't mark "visited." Can't build up information. Just fingers on the page.

```
Can:    string equality (two pointers, compare char by char)
        cycle detection (fast/slow pointers)
        palindrome check (two pointers inward)

Can't:  track visited nodes (need O(n) bits)
        accumulate statistics
        remember what you've seen
```

**Algorithm:** Floyd's cycle detection, two-pointer techniques.

**One-liner:** "I'm a finger on the page. I can re-read, but I can't take notes."

---

### Accumulator (P — Polynomial Time)

**Memory:** O(n^k) — polynomial facts accumulated.

**Structure:** Visited set. Hash table. Growing collection of facts.

```python
def bfs(graph, start):
    visited = {start}          # accumulates!
    queue = [start]
    while queue:
        node = queue.pop(0)
        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)  # progress!
                queue.append(neighbor)
    return visited

# Each step: one new fact
# Never backtrack
# Work = progress
```

**Behavior:** Add facts. Never remove. Monotonic progress. Each step shrinks the remaining problem.

**Why polynomial is natural:**

```
Polynomial = processing the input itself
Exponential = enumerating what the input represents

Input: n-node graph
Polynomial work: O(n²) edges, O(n³) triples — derived from input
Exponential work: O(2^n) subsets — configurations the input quantifies over
```

```
Can:    shortest path
        sorting
        matrix multiplication
        primality testing

All share: each step = guaranteed progress toward answer
```

**Algorithm:** BFS, Dijkstra, dynamic programming.

**One-liner:** "I'm a notebook. I write facts down. I never erase. Every note helps."

---

### Verifier (NP — Nondeterministic Polynomial)

**Memory:** O(n^k) for checking — but finding the witness might require searching 2^n.

**Structure:** Witness / certificate. The proof you can check.

```python
# Sudoku: hard to solve, easy to check

def verify_sudoku(grid):          # O(n²) — polynomial
    for row in grid:
        if not valid_set(row): return False
    for col in columns(grid):
        if not valid_set(col): return False
    for box in boxes(grid):
        if not valid_set(box): return False
    return True

def solve_sudoku(grid):           # exponential search
    # try configurations until one verifies
    ...
```

**Behavior:** Given proof, check fast. Finding proof, search hard.

**The asymmetry:**

```
Verification: process witness (polynomial-sized) — accumulator-like
Search: explore configurations (exponential-sized) — might not converge
```

```
SAT:       verify assignment in O(n), find assignment in O(2^n)?
Sudoku:    verify grid in O(n²), solve in O(k^n)?
Factoring: verify factors in O(n²), find factors in ???
```

**Why this matters:**

```
Witness encodes the search path — compressed directions to the answer.
Given witness: verify = polynomial (check each step)
Without witness: search = exponential? (we don't know)
```

**Algorithm:** SAT solver, backtracking with pruning.

**One-liner:** "I'm a proof-checker. Give me the proof, I'll check it fast. Don't ask me to find it."

---

### Explorer (PSPACE — Polynomial Space)

**Memory:** O(n^k) — but reused across branches. Same space, many paths.

**Structure:** Unmemoized recursion. Game tree DFS. Forget on backtrack.

```python
def minimax(position, depth, maximizing):
    if depth == 0 or game_over(position):
        return evaluate(position)

    if maximizing:
        value = -infinity
        for move in legal_moves(position):
            child = make_move(position, move)
            value = max(value, minimax(child, depth-1, False))
            # child is FORGOTTEN here — space reused!
        return value
    else:
        value = +infinity
        for move in legal_moves(position):
            child = make_move(position, move)
            value = min(value, minimax(child, depth-1, True))
            # child is FORGOTTEN here — space reused!
        return value

# Space: O(depth) — polynomial
# Time: O(2^depth) — exponential
# Key: forgetting enables space reuse
```

**Behavior:** Explore all paths. Backtrack = forget. Reuse space. Spend time instead.

**Why PSPACE contains NP:**

```
NP: "guess" the right path (nondeterminism)
PSPACE: "try" every path, one at a time (enumeration)

Same configurations explored.
NP does it with magic guessing.
PSPACE does it with time and forgetting.
```

**The game connection:**

```
NP:     ∃ — "does there exist a winning move?"
PSPACE: ∀∃∀∃ — "for all opponent moves, exists my response, for all counters..."

Alternation = adversary takes turns.
Can't just guess — opponent responds.
```

```
Can:    QBF (quantified boolean formula)
        optimal game play (polynomial-bounded)
        regex equivalence

All share: adversarial, alternating, must consider all responses
```

**Algorithm:** Minimax, game tree search, DPLL without learning.

**One-liner:** "I'm a maze-walker. I try every path. When I backtrack, I forget where I've been. I have time."

---

### Archivist (EXPTIME — Exponential Time)

**Memory:** O(2^n) — must remember everything. Can't forget, can't compress.

**Structure:** Memoized recursion. Full transposition table.

```python
transposition = {}  # THIS is the exponential memory

def minimax_memo(position, depth, maximizing):
    key = (position, depth, maximizing)
    if key in transposition:
        return transposition[key]

    if depth == 0 or game_over(position):
        return evaluate(position)

    if maximizing:
        value = -infinity
        for move in legal_moves(position):
            child = make_move(position, move)
            value = max(value, minimax_memo(child, depth-1, False))
        transposition[key] = value  # NEVER FORGET
        return value
    else:
        # ... similar

# Space: O(positions) — exponential
# Time: O(positions) — exponential
# Key: remembering IS the problem
```

**Behavior:** Explore all paths. Remember every position. Can't forget — problem structure requires the history.

**Why provably hard:**

```
Unlike NP-hard (where we just can't prove P≠NP),
EXPTIME-complete problems are provably not in P.

The game tree IS the problem.
Every position matters.
No shortcut exists.
```

```
Examples:
    generalized chess (n×n board)
    generalized Go
    full game trees without repetition
```

**Algorithm:** Minimax with full memoization.

**One-liner:** "I'm a maze-walker with a map. I try every path. I mark every room. I need a very big map."

---

### The Convergence View

What separates the classes:

```
Class        Convergence                          Progress Guarantee
───────────────────────────────────────────────────────────────────────────
lookup       input flows through                  O(1) per character
stack        nesting resolves                     O(1) per token
scanner      pointers traverse                    O(1) per step
accumulator  facts accumulate                     each step = permanent progress
verifier     verify converges, search might not   verification: yes, search: ???
explorer     enumerate all, forget between        no per-step guarantee, eventual completion
archivist    enumerate all, remember all          no per-step guarantee, exponential total
```

---

### The Information View

How much information can be extracted:

```
Class        Information Capacity         Extraction Rate
──────────────────────────────────────────────────────────────────────
lookup       O(log k) bits — which state   O(1) per input symbol
stack        O(n) bits — stack contents    O(1) per token
scanner      O(log n) bits — positions     O(1) per comparison
accumulator  O(n^k) bits — all findings    O(1) per step, guaranteed
verifier     witness has O(n^k) bits       O(1) per step to verify; search: ???
explorer     O(n^k) bits active at once    O(1) per step, but restarting
archivist    O(2^n) bits total             O(1) per step, never discard
```

---

### The Physical View

```
lookup       "streaming — input through, nothing saved"
stack        "call stack — nesting tracked, return forgets"
scanner      "two fingers on page — scan, compare, no notes"
accumulator  "notebook — write facts, never erase"
verifier     "proof checker — verify easy, discovery hard"
explorer     "maze walker — all paths, no map"
archivist    "maze walker — all paths, full map"
```

---

### The Compiler Connection

```
Stage               Class           Why
─────────────────────────────────────────────────────────────────────
Lexer               lookup          DFA, fixed table, input flows through
Parser              stack           recursive descent, nesting = stack depth
Name resolution     accumulator     build symbol table, monotonic
Type inference      accumulator     (usually) — propagate types
                    archivist       (F-omega) — can blow up
Register allocation verifier        NP-hard (graph coloring), use heuristics
Instruction sched   verifier        NP-hard, use heuristics
Optimization        accumulator     dataflow = fixed-point over lattice
```

---

### The Practical Implications

```
You hit lookup:      use DFA, will be fast
You hit stack:       use recursive descent or table parser
You hit scanner:     use pointer tricks, very constrained
You hit accumulator: use BFS/Dijkstra/DP, will converge
You hit verifier:    stop looking for poly algorithm
                     use heuristics, approximation, SAT solvers
You hit explorer:    game tree, exponential time, be patient
You hit archivist:   provably exponential, limit scope or approximate
```

---

### Principle

```
lookup       "I'm a table."
             input flows through, state updates, nothing accumulates.

stack        "I'm a stack."
             nesting pushes, return pops, pop forgets.

scanner      "I'm pointers."
             scan and compare, can't take notes.

accumulator  "I'm a notebook."
             write facts, never erase, every step = progress.

verifier     "I'm a proof-checker."
             give me proof, I verify fast. finding it is your problem.

explorer     "I'm a maze-walker without a map."
             try all paths, forget on backtrack, reuse space, spend time.

archivist    "I'm a maze-walker with a full map."
             try all paths, remember all positions, exponential map required.
```
