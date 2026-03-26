# Learn Rust — Full Node List (Restructured)

Each node is a self-contained section. Edges between nodes are not specified here.

---

# PART 1: THE PROBLEM

---

## P1.1 · Why This Course Exists

This course exists because I cannot memorize rules and never could. If I understand the system that produces the rules, I remember it for years, because systems have structure and structure compresses.

[Rust](https://www.rust-lang.org/) tutorials gave me rules such as "one mutable reference or many immutable," "moves invalidate the source," and "lifetimes must not outlive their referent." Stack architecture, computability constraints, and Rice's theorem sit underneath these rules and explain every decision Rust's designers made. Dig into them and the rules stop requiring memorization because they become derivable. The borrow checker becomes a consequence of what memory is, what compilers can analyze, and what tradeoffs Rust chose.

**Who this course is for**

This course is for experienced programmers, particularly those with a C++ background, who already have a mental model of systems programming. The course updates that model. The density is high and the chapters are short, because every sentence should add to what you already know.

If you are new to programming or new to systems languages, start with [The Rust Book](https://doc.rust-lang.org/book/). It builds the mental model this course assumes you already have. Come back here when you want to understand the system underneath the rules.

**How to read this**

This is an interactive graph, not a linear book. Each section ends with an underlined *next.* — hover it to see where you can go, then click to continue. Underlined words inside the text link to related topics: hover for a preview, click to jump.

---

## P1.2 · Programs That Don't Compile

Most Rust syntax matches C++. Functions, structs, loops, and vectors work the same way. But some code that compiles in C++ does not compile in Rust.

These programs contain memory bugs. Rust rejects them. C++ compiles them.

```rust
let v = vec![1, 2, 3];
let w = v;
println!("{:?}", v);
```

**error[E0382]:** borrow of moved value: `v`

The value moved. `v` is gone. This is ownership in action.

```rust
let mut v = vec![1, 2, 3];
let r = &v[0];
v.push(4);
println!("{}", r);
```

**error[E0502]:** cannot borrow `v` as mutable because it is also borrowed as immutable

A reference to `v[0]` exists while `push` tries to modify `v`. The borrowing rule prevents this.

```rust
let r;
{
    let x = 5;
    r = &x;
}
println!("{}", r);
```

**error[E0597]:** `x` does not live long enough

`x` is gone when the block ends. `r` would point to nothing. This is a dangling reference caught at compile time.

```rust
let mut s = String::from("hello");
let r1 = &mut s;
let r2 = &mut s;
println!("{}, {}", r1, r2);
```

**error[E0499]:** cannot borrow `s` as mutable more than once at a time

Two mutable references to the same data. The compiler forbids this because two writers to the same memory is a data race waiting to happen.

```rust
fn get_string() -> &String {
    let s = String::from("hello");
    &s
}
```

**error[E0106]:** missing lifetime specifier

The function tries to return a reference to a local variable. The local is destroyed when the function returns. The compiler demands a lifetime annotation to verify the reference outlives its scope.

---

## P1.3 · Thread Safety Errors

The compiler prevents data races too.

```rust
use std::thread;
fn main() {
    let v = vec![1, 2, 3];
    thread::spawn(|| {
        println!("{:?}", v);
    });
}
```

**error[E0373]:** closure may outlive the current function

The spawned thread might outlive `main`. The closure captures `v` by reference, but the reference could dangle when `main` returns. The fix: `move` the value into the closure, transferring ownership to the thread.

```rust
use std::rc::Rc;
use std::thread;
fn main() {
    let rc = Rc::new(5);
    thread::spawn(move || {
        println!("{}", rc);
    });
}
```

**error[E0277]:** `Rc<i32>` cannot be sent between threads safely

`Rc` uses non-atomic reference counting. Sharing it across threads would corrupt the count. The type system encodes this: `Rc` does not implement `Send`. The compiler rejects the program at the type level, before any runtime check.

The same borrowing rule that prevents single-threaded bugs also prevents data races.

---

## P1.4 · The Dangling Reference

One rejected program examined in detail.

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

**error[E0597]:** `x` does not live long enough

`r` refers to `x`. The block ends. `x` is gone. `r` points to nothing.

The C++ equivalent compiles and runs. It prints garbage, or crashes, or prints 5 by accident. The C++ standard leaves this behavior undefined.

```cpp
// C++
int main() {
    int* r;
    {
        int x = 5;
        r = &x;
    }
    printf("%d\n", *r);
}
```

Rust rejects the program before it runs. The compiler tracks where every reference points and when the target is reclaimed. A reference that outlives its target is a dangling pointer.

---

## P1.5 · Why Memory Is Hard

A modern CPU has multiple cores. Each core has its own cache. Each cache holds copies of data from RAM. The copies exist because RAM is slow: a core that reached RAM on every read would stall for hundreds of cycles. Caches eliminate the wait.

The problem is writes. When one core writes to its cached copy of an address, every other core that holds a copy of the same address now has stale data. It does not know it.

The system that keeps these copies consistent is called a cache coherence protocol. The most common variant is MESI. This protocol, and the constraint it enforces, turns out to be the same constraint that governs references, pointers, and the borrow checker.

---

## P1.6 · What Happens When Two Cores Touch the Same Address

Two cores, one address. Core 0 reads it. Core 1 reads it. Both hold a cached copy. Both copies are current. No conflict.

Core 0 writes. Its copy now holds the new value. Core 1's copy holds the old value. Core 1 does not know anything changed.

The protocol must intervene. Before Core 0 can write, every other core holding a copy must be notified. Their copies are invalidated. Only then does Core 0 proceed.

If Core 1 wants to read afterward, it must re-fetch the value — either from Core 0's cache or from RAM, depending on whether Core 0 has flushed.

If Core 1 wants to write, it must acquire exclusive access. Core 0's copy is invalidated. Now Core 1 is the sole holder. The write proceeds.

At any moment, the protocol enforces one rule: **either many cores hold a read-only copy, or exactly one core holds a writable copy. Never both.**

Two readers: safe. One writer, no readers: safe. One writer plus any reader: forbidden. Two writers: forbidden.

```
Safe:                           Forbidden:

Core 0   Core 1   Core 2       Core 0   Core 1
┌─────┐  ┌─────┐  ┌─────┐     ┌─────┐  ┌─────┐
│read │  │read │  │read │     │write│  │read │
│copy │  │copy │  │copy │     │     │  │copy │
└─────┘  └─────┘  └─────┘     └─────┘  └─────┘
   ▲        ▲        ▲           ▲        ▲
   └────────┼────────┘           │  stale! │
         shared                  └────┬────┘
                                   conflict
```

The protocol operates on blocks called cache lines, typically 64 bytes. When a core reads one byte, the cache fetches the entire 64-byte block. This means two unrelated variables in the same block are governed by the same protocol state. Two cores writing to different variables in the same block triggers the protocol as if they were writing to the same variable. The hardware calls this false sharing.

---

## P1.7 · This Is a Distributed System

The cache coherence protocol is not a special hardware trick. It is a distributed systems protocol.

Strip the hardware names. What remains:

| Hardware | Abstract |
|---|---|
| Core | Process |
| Cache | Local store (holds copies) |
| RAM | Main store (holds ground truth) |
| Coherence controller | Directory (tracks who holds what) |
| Cache line | Entry (unit of ownership) |
| Invalidation message | Revocation (tell others to discard) |

Multiple processes. Each holds local copies of shared data. Writes make other copies stale. A protocol coordinates who can read, who can write, and what messages are exchanged to keep copies consistent.

This structure is not unique to CPUs. It appears in distributed databases (replicas and primaries), distributed file systems (cached leases), and CDN edges (origin servers and cached pages). The constraint is always the same. The protocol pattern is always the same. The names change.

```
Process 0     Process 1     Process 2     Process 3
    │              │              │              │
    ▼              ▼              ▼              ▼
┌────────┐   ┌────────┐   ┌────────┐   ┌────────┐
│Local   │   │Local   │   │Local   │   │Local   │
│Store 0 │   │Store 1 │   │Store 2 │   │Store 3 │
└───┬────┘   └───┬────┘   └───┬────┘   └───┬────┘
    │            │            │            │
    └────────────┴─────┬──────┴────────────┘
                       │
                ┌──────┴──────┐
                │  Directory  │
                └──────┬──────┘
                       │
                ┌──────┴──────┐
                │ Main Store  │
                └─────────────┘
```

The protocol that keeps a CPU's caches consistent and the protocol that keeps database replicas consistent are instances of the same abstract protocol. The Rust borrow checker enforces the same constraint — at compile time.

The rest of this chapter derives the protocol from the constraint, using the abstract vocabulary. The hardware is one instance. The borrow checker is another.

---

## P1.8 · The Coherence Constraint

A write to any local copy makes all other copies stale. The protocol must ensure no process ever reads a stale copy.

Why does the rule work?

Reads commute with reads. Two processes reading the same value at the same time produce no conflict. Both see the same data. The order does not matter.

Writes commute with nothing. A read concurrent with a write can see either the old value or the new one. Two concurrent writes can produce a value that is the result of neither. The order matters, and without serialization the order is undefined.

The maximal concurrency policy respecting this structure is: permit concurrent reads, require exclusive writes. There is no more permissive policy that avoids conflicts. Any more restrictive policy — such as full mutual exclusion — wastes read concurrency.

The constraint: **either many processes hold a read-only copy, or exactly one process holds a writable copy. Never both.**

This constraint was not invented for any single system. The algebra of read/write conflicts forces the same answer everywhere.

| Year | Domain | Name | Form |
|---|---|---|---|
| 1971 | Concurrent programming | Readers-writers problem | Semaphore-based (Courtois, Heymans, Parnas) |
| 1975 | Databases | Shared/exclusive locking | Transaction isolation (Gray, Eswaran et al.) |
| 1984 | Hardware | SWMR invariant | Cache coherence, MESI (Papamarcos, Patel) |
| ~2012 | Programming languages | Borrowing rule | Compile-time, `&T` / `&mut T` (Rust) |

Same constraint. Same algebra. Different names.

---

## P1.9 · Deriving the States

The constraint says: write permission and the reader set cannot coexist for the same entry. The protocol enforces this with two mechanisms tracked by the directory.

**A token.** One token exists per entry. The token is write permission. Whoever holds the token can write. The directory tracks who holds it.

**A reader set.** The set of processes that currently hold a read-only copy. The directory tracks it because granting the token requires revoking all readers first.

Two rules govern interaction:

1. When the token is held, the reader set must be empty. One writer, no readers.
2. When readers exist, no one holds the token. Many readers, no writer.

The four legal states are the four allowed combinations of token and reader set:

| Token? | Reader set? | What it means | Name |
|---|---|---|---|
| Held, written | Empty | One copy. Sole authority. Main store is stale. | Modified |
| Held, not written | Empty | One copy. Can write silently — no readers to revoke. | Exclusive |
| Not held | Populated | Many copies. All match main store. Read-only. | Shared |
| Not held | Not in set | No copy. Must request on next access. | Invalid |

The initials spell MESI.

**Why must the authority flush to main store before giving up the token?** Because shared copies can be silently discarded. If the main store were still stale and all shared copies were silently discarded, the value would be lost forever. Flushing before sharing ensures the main store can always recover the value.

**Why can shared copies be silently discarded?** Because all shared copies match the main store. The value can always be re-fetched. No message is needed. This means the directory's reader set can be imprecise: it may list processes that have already silently discarded their copies.

---

## P1.10 · The Protocol

The whole protocol is: write permission and the reader set cannot coexist for the same entry. Every message exchange either avoids the conflict or resolves it by revoking one side.

**Read, process holds nothing.** Request goes to the directory. If no one holds anything, the directory supplies the value from the main store and grants the token. If readers exist, the process joins the reader set. If another process holds the token, the directory asks the holder to flush, surrender the token, and join the reader set. The requester joins too. Both shared.

**Write, process holds nothing.** The process needs the token. The directory revokes all readers: invalidation messages, acknowledgments. Grants the token. The process can now write repeatedly with zero messages.

**Write, process holds a shared copy.** The process needs the token. The directory revokes all other readers. Grants the token. No data transfer needed — the process already has the value.

**Write, process holds exclusive (token held, not yet written).** No messages. No readers to revoke. Write directly. Modified.

**Write, process holds modified (token held, already written).** No messages. Write directly. This can happen millions of times with zero protocol traffic.

**Another process reads an entry held as modified.** The directory asks the holder to flush to main store, surrender the token, keep a read copy. The requester gets a read copy. Both shared.

**A shared copy is evicted.** Silently discarded. No message. Safe because the main store is current.

**A modified entry is evicted.** The main store is stale. Silent discard would lose the value. The process must flush and surrender the token. This is the only eviction that requires a message.

Every transition preserves the constraint. The protocol is complete.

---

## P1.11 · Shared Memory Is Message Passing

In the base protocol, all writes to the same entry are fully serialized. Only one process can hold the token at any moment. Every other process that wants to write must wait. Writes to different entries proceed in parallel — each entry has independent state — but writes to the same entry are strictly sequential.

The "sharing" in "shared memory" is reading only. Writing is serialized through message passing between processes.

Consider two ways to pass a value from thread A to thread B.

Go, using a channel:
```
ch <- 42        // thread A sends
val := <-ch     // thread B receives
```

Java, using a shared variable:
```
x = 42;         // thread A writes
val = x;        // thread B reads
```

The Go programmer sees message passing. The Java programmer sees shared memory.

Both produce the same protocol traffic underneath. The assignment `x = 42` triggers a request for write permission, invalidation of all readers, acknowledgments, and a grant. Multiple messages for one assignment statement. "Shared memory" is an API over a message-passing protocol.

Lauer and Needham proved in 1978 that shared-memory concurrency and message-passing concurrency are formally equivalent: any program in one maps to a program in the other. The coherence protocol is that equivalence, running underneath every "shared" variable.

---

## P1.12 · Distance Forces Copies

The cache coherence protocol is one instance of a pattern that appears at every layer of computing.

A CPU cache exists because RAM is far from the core. A database replica exists because users are far from the primary. Thread-local storage exists because shared memory requires synchronization. Every copy is derived data — a representation computed from a source that carries a sync obligation. A change to the source makes the copy stale.

| Layer | Source | Copy | Sync Strategy |
|---|---|---|---|
| CPU cache | RAM | L1/L2/L3 line | MESI protocol |
| Compiler | Memory | Register | Register allocation |
| Language | Original value | Reference/alias | Ownership, locks, GC |
| Thread | Shared heap | Thread-local | Mutex, channels, atomics |
| Process | Shared memory | Process-local | IPC, message passing |
| Database | Primary | Replica | Replication protocol |
| Network | Origin server | CDN edge | TTL, invalidation |

Each layer resolves staleness with a different sync strategy. The pattern is the same. The coherence constraint is the same. The three primitives make the pattern precise.

---

## P1.13 · Three Primitives

The coherence protocol operates on three things. Where copies live. When they change. How you reach them.

**SPACE.** Where data lives. Local store, main store, stack, heap, register, cache line, disk, remote server. The protocol in the previous chapter governs space: which process holds a copy, where the ground truth lives.

**TIME.** When things happen. The write that makes copies stale is a time event. Scope entry to scope exit is a time interval. Compile-time vs runtime. Sequential vs parallel. A value's lifetime.

**COORDINATES.** How we refer to data. The token in the protocol is a coordinate: it tells the directory which process can reach the writable copy. At the language level: names at compile-time, addresses at runtime, references, pointers. Coordinates tell you where space is.

The rest of the course abbreviates these as **S×T×C**. Bugs are failures in one or more of the three. Fixes adjust one or more of the three.

**State** is data that changes over TIME. Stateless computation has no coherence problem. Most coherence problems involve state.

**Derived data** is the mechanism through which coherence problems propagate. A coordinate to a value is derived data. A cached copy is derived data. The source lives in one SPACE, the derived copy in another, and TIME determines when they fall out of sync.

---

## P1.14 · Bugs as Interaction Failures

Every memory bug maps to a specific interaction between primitives.

Interactions between the three primitives produce every category of memory bug.

**SPACE × TIME.** When memory exists. Allocation, deallocation, lifetimes, scope.

**SPACE × COORDINATES.** How many paths lead to memory. Aliasing, ownership, null.

**TIME × COORDINATES.** When a reference is valid. Scope, shadowing, drop order.

| Bug | Primitives | Breakdown |
|---|---|---|
| Use-after-free | S×T×C | Use (COORD) after (TIME) free (SPACE) |
| Dangling pointer | T×C | Pointer (COORD) outlives target (TIME) |
| Double free | S×T | Free (SPACE) twice (TIME) |
| Data race | S×T×C | Data (SPACE) race (TIME) via aliases (COORD) |
| Buffer overflow | S×C | Coord exceeds bounds of SPACE |
| Uninitialized read | S×T×C | Read (COORD) before (TIME) init (SPACE) |
| Memory leak | S×T | Space outlives all coords (TIME) |

Each of these has a timeline that shows exactly how the interaction breaks down.

---

## P1.15 · Use-After-Free

Use (COORDINATES) after (TIME) free (SPACE).

```
TIME  t0        t1              t2          t3
EVENTS  create heap  p ← addr(heap)  free heap   >at_addr(p)<
────────────────────────────────────────────────────────────────────────
HEAP    ████████████████████████████████
COORD (p)            ████████████████░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░
                                                      >BUG<
```

The heap is freed at t2. The coordinate `p` still exists. At t3, the program follows `p` to memory that no longer belongs to it.

In C++, this compiles and runs. The behavior is undefined. In Rust, ownership prevents any coordinate from outliving the space it points to.

---

## P1.16 · Dangling Pointer

Pointer (COORDINATES) dangles (TIME: target gone).

```
TIME  t0        t1              t2            t3
EVENTS  create x  r ← addr(x)  scope ends    >at_addr(r)<
────────────────────────────────────────────────────────────────────────
SPACE (x)  ████████████████████████████████
COORD (r)            ████████████████░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░
                                                      >BUG<
```

The variable `x` lives on the stack. The scope ends at t2. The space is reclaimed. The coordinate `r` now points to reclaimed space.

This is the same bug from the first chapter. The timeline makes the mechanism precise: TIME destroys the SPACE, but the COORDINATE survives.

---

## P1.17 · Double Free

Free (SPACE) twice (TIME).

```
TIME  t0        t1              t2                t3
EVENTS  create heap  p ← addr(heap)  free_at_addr(p)  >free_at_addr(p)<
────────────────────────────────────────────────────────────────────────
HEAP    ████████████████████████████████
COORD (p)            ████████████████░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░
                                                      >BUG<
```

The heap is freed at t2. At t3, the program frees it again. The allocator's bookkeeping is corrupted. The consequences are unpredictable: crashes, silent corruption, security vulnerabilities.

Rust prevents this with single ownership. One owner, one free, automatic.

---

## P1.18 · Data Race

Data (SPACE) race (TIME: parallel) via aliases (COORDINATES).

```
TIME  t0             t1                   t2
EVENTS  create x    p ← addr(x)         ╭─ >write_at_addr(p)<
                    q ← addr(x)         ╰─ >write_at_addr(q)<
────────────────────────────────────────────────────────────────────────
SPACE (x)  ████████████████████████████████▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░
COORD (p)                ████████████████████████████████████████████
COORD (q)                ████████████████████████████████████████████
                                               >BUG< (parallel writes)
```

Two coordinates reach the same space. Two threads write at the same time. No synchronization. The result depends on which write completes first — or whether they interleave at the byte level.

A data race needs three ingredients: shared data (SPACE), parallel access (TIME), at least one writer (COORDINATES). Remove any one and the race disappears.

This is the coherence problem at the language level. The MESI protocol prevents it in hardware by enforcing the constraint: many readers or one exclusive writer. The borrowing rule prevents it at compile time with the same constraint: if any coordinate can write, no other coordinate can exist.

---

## P1.19 · Other Memory Bugs

Three more bugs complete the taxonomy.

**Buffer overflow.** Coordinates that exceed the bounds of their space. The buffer has 5 elements. The coordinate points to index 7. The space at that address belongs to something else. Rust prevents this at runtime: vector and slice indexing checks bounds. The coordinate cannot exceed the space.

**Uninitialized read.** Read (COORDINATES) before (TIME) the value exists (SPACE). The space is allocated. The coordinate is valid. But TIME has not yet given the space a value. The program reads whatever bits happen to be there. Rust prevents this at compile time: every variable must be initialized before use.

**Memory leak.** Space outlives all coordinates to it. The coordinate goes out of scope. The allocation remains. No coordinate can reach it. The ability to free it is lost. Rust prevents most leaks with ownership: when the owner goes out of scope, the value is dropped. Leaks are still possible via `mem::forget` or reference cycles in `Rc`, but they require deliberate effort.

---

## P1.20 · Paradigms and Sync Strategies

Different languages solve the coherence problem differently. Each strategy adjusts one or more of the three primitives.

| Language | Paradigm | Coherence Strategy |
|---|---|---|
| Haskell | Functional | No mutation, no problem |
| Erlang | Actor | Process isolation, message passing |
| Clojure | Functional | Persistent data structures |
| Rust | Linear/ownership | Compile-time proof |
| Go | Imperative | Channels or locks |
| Java | OOP | Locks, volatile |
| C/C++ | Imperative | Programmer responsibility |

Haskell eliminates the problem: functions cannot modify state, so nothing gets out of sync. Erlang eliminates sharing: processes share no memory and communicate by copying messages. Clojure makes mutation structural: modifying a map returns a new map that shares structure with the old one. Rust proves correctness at compile time with zero runtime cost. Go and Java provide runtime tools and trust the programmer to use them correctly. C and C++ provide no enforcement at all.

Each language feature targets a specific interaction between primitives.

**SPACE × TIME** — when memory exists. Garbage collection (the runtime tracks reachable memory), RAII (lifetime tied to scope), reference counting (each allocation tracks its reference count).

**SPACE × COORDINATES** — how many paths lead to memory. Ownership and move semantics (one owner, transfer invalidates the source), value types (data copied, no aliasing), nullable types (references must handle the null case).

**TIME × COORDINATES** — when a reference is valid. Lexical scope (references valid within their declaring scope), lifetime annotations (explicit markers, no runtime cost), closures (the language must decide: copy, borrow, or move captured variables).

**SPACE × TIME × COORDINATES** — the full problem. Mutex/RwLock (runtime enforcement of one writer or many readers), atomics (hardware-level indivisible operations), channels (transfer ownership between threads), immutability (no writes, no conflicts), the Rust borrow checker (compile-time enforcement, zero runtime cost).

The strategies differ in where they place the cost. Static analysis costs nothing at runtime but rejects some valid programs. Garbage collection accepts all programs but costs runtime performance. Programmer responsibility costs nothing until a bug ships.

---

## P1.21 · The Lineage

The constraint is: many readers or one exclusive writer.

Rust's borrowing rules enforce it at compile time. `&T` is shared and read-only. `&mut T` is exclusive and writable. The compiler rejects programs that violate this.

The same constraint, at every layer:

| Layer | Year | Form |
|---|---|---|
| Concurrent programming | 1971 | Readers/writers with semaphores (Courtois, Heymans, Parnas) |
| Databases | 1975 | Shared/exclusive locking (Gray, Eswaran et al.) |
| Hardware | 1984 | Cache coherence, MESI states (Papamarcos, Patel) |
| C++ memory model | 2011 | Happens-before, data race freedom |
| Rust | ~2012 | Borrowing rule, `&T` / `&mut T` |

The algebra forces the same answer at every layer. Reads commute with reads. Writes commute with nothing. The maximal concurrency policy is always: permit concurrent reads, require exclusive writes.

The MESI protocol enforces it at runtime, in hardware, for every address, on every CPU. The borrow checker enforces it at compile time, for every reference, in every Rust program. The cache coherence controller is a runtime borrow checker. The borrow checker is a compile-time coherence protocol.

---

# PART 2: THE COMPILER

---

## P2.1 · Bindings and Addresses

How does the compiler see a dangling reference?

Chapter 1 showed a dangling reference that C++ compiles and Rust rejects. The compiler catches it by analyzing what each binding holds and how long it remains valid.

```cpp
// C++
int* r;
{
    int x = 5;
    r = &x;
}
std::cout << *r;
```

Two bindings. The binding `x` holds a value. The binding `r` holds an address — the location of `x`. When the block ends, `x` is gone. `r` still holds the address and the machine will read whatever bytes sit there.

| Name | Location | Holds |
|---|---|---|
| `x` | A | `5` (a value) |
| `r` | B | address of A (a coordinate) |

We say "coordinate" rather than "pointer" or "reference" because languages define those terms differently. `T*` in C++, `&T` in Rust, `*const T` for raw pointers. The conceptual problem is identical: something that tells you where data lives. "Coordinate" abstracts over the language-specific terms.

Coordinates are not optional. Copying large data costs time and energy, so programs pass addresses instead of values. Graphs, trees, and linked lists require indirection because you cannot inline a cycle. Any language that cares about performance or expressiveness needs coordinates.

The coordinate `r` is syntactically independent — it has its own declaration, its own location, its own scope. It is semantically dependent — its purpose is to refer to `x`, and without `x`, the address in `r` points to nothing meaningful. Coherence fails when these lifetimes fall out of sync.

---

## P2.2 · How Programs Manage Space

Bindings hold values or coordinates. Where does the space live?

The dangling reference exploits a gap between two levels. At the language level, `x`'s space is gone when the scope ends. At the machine level, the bytes are still there. Understanding this gap requires knowing how programs manage memory.

**Stack.** The compiler manages the stack. It determines at compile time how much space each function needs and lays out a stack frame for each call. When a scope ends, the compiler considers those slots available for reuse. The machine does not zero the bytes, does not reclaim them — it adjusts the stack pointer. A coordinate that still holds the old address can read the bytes, and the machine will comply. The language calls this undefined behavior. The machine calls it a normal read.

**Heap.** The heap is managed at runtime. An explicit allocation (`malloc`, `new`, `Box::new`) reserves bytes and returns a coordinate. An explicit deallocation (`free`, `delete`, dropping the owner) marks them reusable. The same gap applies: after deallocation, a coordinate that still holds the address can read stale bytes. The language calls this a use-after-free. The machine calls it a normal read.

The heap adds a problem the stack does not have. Stack space is tied to scope boundaries the compiler can see. Heap space is freed by an operation that can happen anywhere. Heap coordinates can be copied into other variables, passed to functions, stored in data structures. No lexical structure in the source text governs when heap space is freed.

```
STACK (tree)                 HEAP + STACK (hybrid)
main ─────────┐              main
├── foo       │              ├── foo ──────────┐
│   └── bar   │              │   └── bar       │
│       └── baz│                               ▼
└── qux       │              └── qux     [heap]
              │                           ▲
              │                           │
Coordinates point toward     Heap coordinates escape
enclosing scopes.            the scope tree.
Reclamation at scope end.    Reclamation: anywhere.
Compiler sees both.          Compiler sees neither.
```

The rest of this chapter uses "reclaimed" to mean the language no longer considers the space as belonging to any live binding. Stack space is reclaimed when its scope ends. Heap space is reclaimed when something deallocates it. Ownership ties heap reclamation to scope boundaries, making heap behave like stack from the compiler's perspective.

---

## P2.3 · What the Compiler Needs to See

Stack and heap manage space differently. What does the compiler need to catch bugs across both?

Source text has hierarchical structure. Functions contain blocks, blocks contain statements, statements contain expressions. A variable declared inside a block is visible only within that block and any nested blocks. Names respect the scope hierarchy. Coordinates do not.

```rust
fn first(list: &[i32]) -> &i32 {
    &list[0]
}
fn main() {
    let v = vec![1, 2, 3];
    let r = first(&v);
    println!("{}", r);
}
```

The scope hierarchy sees `v` and `r` in `main`'s scope, and `list` in `first`'s scope. Three names, three declarations, three scope boundaries. The scope hierarchy does not see that `r` holds a coordinate into `v`'s space. The coordinate traveled from `main` into `first` as the argument `list`, reached into the vector's first element, and traveled back as the return value bound to `r`. The connection between `r` and `v` crosses two scope boundaries and routes through a function body the caller cannot see.

The analysis needs two things at every point in the program. Where does each coordinate point? When is each piece of space reclaimed? The compiler can answer the first question when every coordinate has a known origin. It can answer the second for stack space, because scope boundaries are visible. It cannot answer it for heap space, because heap deallocation has no lexical structure. Lifetime annotations at function signatures bridge this gap by declaring which input a borrowed output connects to.

The scope hierarchy tracks names and boundaries. The analysis needs data flow and reclamation across boundaries. RAM has no scope structure at all — memory is a sequence of numbered cells, any address can read or write any cell. The compiler bridges these worlds by building a **control flow graph**. Nodes are statements. Edges connect statements where execution can flow from one to the next. Branches create multiple outgoing edges. Loops create backward edges.

---

## P2.4 · Detecting Access to Reclaimed Space

The compiler has a control flow graph. How does it find the bug?

The compiler analyzes the control flow graph by tracking coordinates along every path. It records where each coordinate is created, what space it points to, and when that space is reclaimed. A path connecting a reclamation point to an access point for the same space means the program can reach reclaimed memory. The compiler rejects when any such path exists.

The dangling reference from Chapter 1, traced on the graph:

```rust
fn main() {
    let r;
    {
        let x = 5;      // A
        r = &x;          // B
    }                     // C
    println!("{}", r);   // D
}

    ┌───┐
    │ A │  let x = 5
    └─┬─┘
      ▼
    ┌───┐
    │ B │  r = &x
    └─┬─┘
      ▼
    ┌───┐
    │ C │  x's space reclaimed (scope ends)
    └─┬─┘
      ▼
    ┌───┐
    │ D │  *r  ACCESS with reclaimed space
    └───┘
```

One path. C reclaims the space. D accesses it through `r`. The path from C to D exists. The compiler rejects.

A more complex example with branching shows why the compiler must check all paths:

```rust
fn example(flag: bool) {
    let r;
    if flag {
        let x = 5;                // B
        r = &x;
        println!("{}", *r);       // D: safe, x alive
    } else {
        let y = 10;               // C
        r = &y;
    }                             // y's space reclaimed
    println!("{}", *r);           // G: unsafe if else taken
}

    ┌───┐
    │ A │  if flag
    └─┬─┘
  ┌───┴────┐
  ▼        ▼
┌───┐   ┌───┐
│ B │   │ C │
│x=5│   │y=10│
│r=&x│  │r=&y│
└─┬─┘   │ ✗ │  y reclaimed
  │     └─┬─┘
  ▼       ▼
┌───┐   ┌───┐
│ D │   │ F │
│*r │   │ ✗ │  propagated
│safe│  └─┬─┘
└─┬─┘     │
  └───┬───┘
      ▼
    ┌───┐
    │ G │
    │*r │  unsafe on else path ✗
    └───┘
```

The access at D is safe because no reclamation reaches it along any path. The access at G is unsafe because a path from C reaches it through F. The compiler finds the unsafe path and rejects. It does not matter that the if-branch is safe — one unsafe path is enough.

The compiler checks all paths because it cannot know at compile time which branch the program will take at runtime. This is a sound analysis: it may reject programs that would actually be safe, but it will never accept a program that can reach reclaimed memory.

---

## P2.5 · Why C++ Compilers Skip This Analysis

The analysis from the previous chapter exists. Most compilers don't attempt it.

Three properties of C and C++ prevent building the complete control flow graph the analysis requires.

**Separate compilation.** Each source file becomes an object file independently. A call to a function defined in another file appears as an external symbol, resolved by the linker after analysis is over. The graph has edges leading to functions whose bodies are invisible.

**Function pointers.** The call target is a runtime value. The compiler cannot determine which function executes. The graph has edges to unknown destinations.

**Unconstrained pointers.** Even with a complete graph, the compiler needs to track where every coordinate points. C++ allows coordinates to be created without naming a target.

```cpp
int* p = (int*)0x1234;     // coordinate from integer
int* q = p + 5;            // coordinate from arithmetic
void* v = p;               // type erased
uintptr_t n = (uintptr_t)p; // stored as integer
```

An integer cast to a pointer, pointer arithmetic, `void*` type erasure. The analysis requires knowing where every coordinate points. C++ provides no such guarantee. Rust constrains coordinate creation to fix this.

These are three specific obstacles. They generalize to two deeper problems that any language must address.

---

## P2.6 · RAM Is Flat

One of two deeper problems behind C++'s obstacles.

The machine model has no scope hierarchy. Memory is a sequence of numbered cells. A pointer is a number. Any address can read or write any cell. Nothing in the machine prevents code from reading a cell after the scope that allocated it has ended. Nothing prevents fabricating an address the compiler cannot trace.

C++'s unconstrained pointers are a symptom of this deeper problem. The machine provides no structure for the compiler to exploit. Coordinates can arise from anywhere and point to anything.

A compiler that wants to answer "where does each coordinate point?" needs the language itself to constrain how coordinates are created. The analysis cannot work if coordinates appear from nowhere.

The second deeper problem: data that crosses scope boundaries.

---

## P2.7 · Data Flow Crosses Scope Boundaries

Scopes track names. Data flows between them.

The scope tree tracks where names live. It does not track where data flows between them. A function argument carries a coordinate into a nested scope. A return value carries one out. An assignment can carry a coordinate to a binding with a different lifetime.

A function's caller passes `&v` in and receives a coordinate out. The caller cannot determine whether the returned coordinate still points into `v`'s space. The signature says nothing about the connection. Separate compilation means the compiler cannot see the function body to find out.

C++'s separate compilation and function pointers are symptoms. The deeper problem is that data flow between scopes is invisible to the scope tree. A compiler that wants to trace coordinates across function boundaries needs the language to declare how data flows between scopes.

One more obstacle remains: types that contain pointers.

---

## P2.8 · Invisible Coordinate Copying

Some types hide coordinates inside their stack representation.

A `String`'s stack representation holds a heap pointer, a length, and a capacity. The heap pointer is a coordinate to managed space. Byte-copying the stack representation duplicates that coordinate without duplicating the heap buffer. Two bindings now hold coordinates to the same buffer, and both believe they own it.

```
Byte-copy on i32:          Byte-copy on String (hypothetical):

+----------+               +----------+       +----------+
|    5     | <- x (owner)  | ptr -----|---===>| "hello"  |
+----------+               +----------+  |    |          |
|    5     | <- y (owner)  | ptr -----|--+    +----------+
+----------+               +----------+

Independent space.          Two owners, one space.
```

An `i32` has no heap pointer and no cleanup obligation. Duplicating its bytes produces two independent values. A `String` manages heap space, and byte-copying it duplicates the coordinate without duplicating the space itself. The analysis tracks one owner per allocation. Invisible duplication defeats that tracking by creating a second owner the compiler did not authorize.

The compiler needs to know which types are safe to byte-copy and which are not. Rust prevents accidental byte-copying of types that manage internal space.

Other languages approach these obstacles in entirely different ways.

---

## P2.9 · Where Branches Break the Analysis

Even with every obstacle patched, one problem remains.

Branches create paths that may or may not execute, depending on runtime values.

```
r = uninitialized
{
    x = 5
    if some_condition(n) {
        r = &x
    }
}
// x is gone here
if some_condition(n) {
    print(*r) // r used after x's space is reclaimed?
}
```

Whether `*r` accesses reclaimed space depends on whether `some_condition(n)` returns true. The compiler would need to evaluate it. That evaluation depends on `n`, and `some_condition` might contain arbitrary computation. The analysis checks all paths and rejects when any path is unsafe.

```
// Collatz sequence: does this terminate for all inputs?
n = input
while n != 1 {
    if n is even { n = n / 2 }
    else { n = 3*n + 1 }
}
```

Whether the Collatz sequence terminates for all inputs remains unknown. Mathematicians have tested every number up to 10²⁰. No proof exists. A `some_condition(n)` that checks Collatz termination would force the compiler to solve an open problem in mathematics.

Arbitrary computation resists static prediction. No algorithm can avoid this.

---

## P2.10 · Why Perfect Analysis Is Impossible

Not just hard. Mathematically impossible.

**The Halting Problem (Turing, 1936).** A termination checker must return an answer for any program it examines. The program being examined has no such obligation. It can run forever, halt immediately, or do anything in between.

A program can read the checker's source code, call the checker on itself, and then do the opposite of what the checker predicts. If the checker predicts "terminates," the program loops forever. If the checker predicts "loops forever," the program terminates. The checker is wrong regardless of what it answers. No algorithm can avoid this contradiction.

**Rice's Theorem (1951)** generalizes the result. Any non-trivial semantic property of programs is undecidable. "Non-trivial" means some programs have the property and some do not. "Semantic" means the property depends on what the program does, not how it is written.

"Does this COORDINATE reach reclaimed SPACE?" is such a property. So is "does this program have a data race?" and "does this program dereference a dangling pointer?" These are mathematical impossibilities that apply to any static analysis tool, in any language, built by any team.

The compiler cannot be perfect. It must choose between two failures. When it rejects valid code, escape hatches exist.

---

## P2.11 · Sound or Complete

The compiler must choose which failure to accept.

**Sound** means the compiler never accepts unsafe code. It may reject code that is actually safe.

**Complete** means the compiler never rejects safe code. It may accept code that is actually unsafe.

No compile-time analysis can be both sound and complete for questions about program behavior. Accepting unsafe code means security vulnerabilities and undefined behavior. Rejecting safe code means restructuring or using escape hatches. The asymmetry between failure modes determines the choice.

Rust chooses soundness. The approximation rejects some valid programs, and that is the acceptable cost. What exactly falls on the approximate side?

---

## P2.12 · Names Are Exact, Data Flow Is Approximated

The boundary between what the compiler knows precisely and what it approximates.

The compiler handles names exactly. Scope boundaries, variable declarations, and field names are all visible in source text. The scope tree captures where every name lives and when every name expires. This part of the analysis is decidable and precise.

The approximation falls entirely on data flow. Which path a branch takes, which method a function pointer calls, whether two indices refer to the same element. These depend on runtime values, and Rice's theorem says no algorithm answers correctly for all inputs. The compiler assumes the worst case for every question about data flow.

Four obstacles face the analysis: flat RAM, cross-scope data flow, invisible duplication, and undecidability. One fundamental constraint governs it: the compiler chooses soundness over completeness. The next chapter shows how Rust patches each obstacle with a specific design choice.

---

## P2.13 · Constrained Coordinates

RAM is flat. Pointers are numbers. How does Rust fix this?

In C++, a pointer is a number that happens to serve as an address. The language provides operations on numbers, and pointers inherit them. You can cast an integer to a pointer, perform arithmetic to compute new addresses, erase type information through `void*`, and store pointers in integer variables. Each operation severs the connection between the coordinate and its origin. The compiler cannot trace where the pointer points.

Rust references are not numbers. A reference is a typed relationship between a binding and a target. The language provides one way to create a reference: the `&` operator applied to an existing value. There is no syntax for creating a reference from an integer. There is no reference arithmetic. There is no type erasure for references.

```rust
let x = 5;
let r = &x;     // r points to x, compiler knows this
```

When the compiler sees `&x`, it records that the resulting reference points to `x`. Every reference in the program has a known origin. The compiler can trace any reference back to the space it refers to.

| C++ allows | Rust equivalent | Why it matters |
|---|---|---|
| `(int*)0x1234` | Does not exist | Every reference has a known target |
| `p + 5` | Does not exist | References cannot drift to unknown locations |
| `void* v = p` | Does not exist for references | Type information preserved |
| `(uintptr_t)p` | Does not exist for references | References cannot become integers |

Raw pointers in Rust (`*const T`, `*mut T`) allow some of these operations, but dereferencing a raw pointer requires `unsafe` code. The borrow checker analyzes safe Rust, where all coordinates are references with known origins. This patches the obstacle that RAM is flat: constrained references ensure every coordinate has a known origin on the graph.

---

## P2.14 · Ownership

Heap space has no lexical structure. How does the compiler know when it dies?

Stack space is reclaimed when scopes end, and the compiler sees scope boundaries in the source text. Heap space in C++ has unpredictable lifetimes. A `new` expression allocates, and that space lives until some code calls `delete`. The coordinate to that space can be copied, passed to functions, and stored in data structures. Any code holding the coordinate can free the space. Any code holding a copy can use it afterward and find the space gone. Nothing in the source text tells the compiler when heap space will die.

Rust makes heap lifetimes predictable by assigning exactly one owner to each allocation. The owner is a binding, and that binding has a scope. Rust deallocates the owned space when the owner's scope ends. Heap lifetime follows owner scope, the same way stack lifetime follows function scope.

```rust
fn example() {
    let v = vec![1, 2, 3];   // v owns heap space
    // ... use v ...
}   // v's scope ends, heap space freed
```

Ownership can transfer from one binding to another. This is a move. After the move, the original binding is no longer valid, and the new binding is the owner.

```rust
fn example() {
    let v = vec![1, 2, 3];   // v owns
    let w = v;                // ownership moves to w, v now invalid
    // ... use w ...
}   // w's scope ends, heap space freed
```

At any moment, exactly one binding owns each allocation. The compiler tracks ownership through assignments and function calls. This transforms the analysis: without ownership, heap space can be reclaimed at any point where code calls `free` or `delete`. Ownership ties heap reclamation to scope boundaries, which the compiler already sees. Heap behaves like stack for the purposes of lifetime analysis.

---

## P2.15 · The Notation

Rust syntax hides which binding is owner vs alias. How do you see what is really happening?

Rust's assignment syntax hides which kind of binding a `let` produces. `let result = create()` and `let elem = first(&list)` look identical. One produces an owner. The other produces an alias. The language hides these distinctions deliberately. A learner building the model for the first time does not have the experience to recognize what the syntax conceals.

The course includes a companion crate called `notation` with a macro called `explicit!`. Inside the macro, every operation declares what it does.

```rust
explicit! {
    let owner(s) = take(String::from("hello"));  // s takes ownership
    let owner(t) = take(s);                       // t takes from s, s invalid
    let alias(r) = coord_exclusive(t);            // r is a coordinate to t's space
    println!("{}", at(r));                         // follow the coordinate
}
```

The left side of each `let` declares the kind of binding. `owner` means the binding controls when space is reclaimed. `alias` means the binding holds a coordinate to space owned elsewhere. The right side declares what happens. `take` means ownership transfers; an existing binding passed as the argument becomes invalid. `coord_exclusive` creates an exclusive coordinate. `at` follows a coordinate to its target.

The vocabulary maps to the framework from the earlier chapter. `owner` tracks SPACE × TIME. `alias` tracks COORDINATES. `take` is an ownership transfer in TIME. `coord_exclusive` creates a COORDINATE with exclusive access. Coordinates come in two kinds: `coord_exclusive` grants read and write access, and `coord_shared` grants read access while allowing multiple coordinates to coexist.

---

## P2.16 · Lifetime Annotations

Data flow crosses scope boundaries. Constrained coordinates and ownership make both questions answerable within a single function. Separate compilation limits this. The compiler processes each function independently. A caller cannot see the body of a function that returns a reference and cannot determine where the reference points.

Function signatures solve this. A borrowed output returns a reference that points into space owned by one of the inputs. The `'a` label links an output to its input.

```rust
fn first<'a>(list: &'a [i32]) -> &'a i32 {
    &list[0]
}

fn search<'a, 'b>(map: &'a Map, key: &'b K) -> &'a V
//                                               output borrows from map, not key

fn either<'a>(a: &'a i32, b: &'a i32, flag: bool) -> &'a i32
//                                                    output borrows from both
```

The compiler verifies both directions. Given the function body, it checks that the implementation returns a reference derived from the declared input. Given the call site, it checks that the caller keeps the input alive while using the output. Function pointers, trait objects, and cross-crate calls all work from the signature alone.

The same annotation applies to structs. A struct can hold a coordinate to space owned by a different binding. That coordinate is derived data, and the struct depends on the external space staying alive. The `'a` on the struct declares this dependency.

```rust
struct Viewer<'a> {
    data: &'a [i32],
}
```

The compiler uses `'a` to verify that any code creating a `Viewer` keeps the referenced space alive long enough.

```rust
fn main() {
    let viewer;
    {
        let v = vec![1, 2, 3];
        viewer = Viewer { data: &v };
    } // v's space reclaimed here
    println!("{:?}", viewer.data); // REJECTED: viewer outlives v
}
```

**error[E0597]:** `v` does not live long enough

Function lifetime annotations declare that an output holds a coordinate to an input's space. Struct lifetime annotations declare that the struct holds a coordinate to external space. Both tell the compiler which space a coordinate depends on. The mechanism is the same.

---

## P2.17 · Desugaring and Lifetime Elision

How do operators like `v[0]` connect to lifetimes?

Rust desugars operator syntax to trait method calls. `v[0]` desugars to `*v.index(0)`, and `Index::index` has this signature:

```rust
fn index(&self, index: usize) -> &Self::Output
// With lifetime written out:
fn index<'a>(&'a self, index: usize) -> &'a Self::Output
```

The function takes a `&Vec<i32>` and returns a `&i32`. One coordinate goes in, one coordinate comes out. The shared `'a` declares that the returned element coordinate is valid only while the input Vec coordinate is valid. The compiler reads this temporal dependency from the signature and enforces it at every call site.

Rust calls this inference *lifetime elision*. A function with one reference input and one reference output infers the connection. A method on `&self` or `&mut self` infers the output borrows from `self`. The programmer writes explicit `'a` labels only when the inference is ambiguous: multiple reference inputs, no `self`, or the output needs to declare which specific input it borrows from.

---

## P2.18 · Using Data, Managing Space

What can a coordinate actually do?

An `owner` controls when space is reclaimed. An `alias` holds a coordinate to space owned elsewhere. Using the data and managing the space are different operations, and Rust enforces the difference.

A coordinate, even an exclusive one, lets code read and write the target data. It cannot free the space, move ownership out, or replace the allocation. Only the `owner` can manage the space.

```rust
explicit! {
    let owner(b) = take(Box::new(String::from("hello")));  // b owns heap space
    let alias(r) = coord_exclusive(b);                     // r can read and write
    // take(at(r)) would be invalid: r is an alias, aliases cannot own
}
```

`take(b)` works because `b` is an `owner`. The compiler forbids `take` when the argument is an `alias`. A function that receives an `alias` parameter cannot call `take` on it, cannot destroy the space, cannot move ownership away. The caller knows that after the function returns, the space still exists. The function accessed the data. The `owner` kept control of the space.

| Operation | C++ via `unique_ptr<string>&` | Rust via `alias(r) = coord_exclusive(b)` |
|---|---|---|
| Read value | `*ref` | `at(at(r))` |
| Write value | `*ref = v` | `at(at(r)) = v` |
| Call methods | `ref->method()` | `at(r).method()` |
| Move out | `auto s = move(*ref)` | `take(at(r))` **forbidden** |
| Destroy space | `ref.reset()` | **forbidden** |

The first three rows use data. The last two manage space. C++ allows a coordinate to manage the space. Rust does not. This distinction makes the compiler's analysis airtight: signatures that declare `alias` connections are telling the truth in full, because an `alias` cannot do more than use the data.

---

## P2.19 · Why Rust Moves by Default

Why does assignment invalidate the source?

The fourth obstacle is invisible coordinate duplication. Byte-copying a `String` duplicates the heap pointer, creating two owners of one space. Rust prevents this by making assignment transfer ownership rather than copy bytes.

`let t = s` moves ownership to `t`, and `s` becomes invalid. The compiler does not know whether a type's bytes contain coordinates to managed space. Moving is the safe default.

Types that contain no coordinates to managed space opt into byte duplication by implementing the `Copy` trait. `i32`, `bool`, `f64`, `char`, and tuples of Copy types are all Copy. `String`, `Vec`, and any type that manages heap space are not Copy.

```rust
explicit! {
    let owner(x) = take(5);
    let owner(y) = mem_copy(x);     // duplicate the bytes
    println!("x = {}, y = {}", x, y); // both valid
}
```

`let y = x` does not say whether it copies or moves. The compiler decides based on the type. The notation surfaces this decision:

```rust
explicit! {
    let owner(x1) = take(5);
    let owner(y1) = take_or_mem_copy(x1);  // i32 is Copy: mem_copy
    println!("x1 = {}, y1 = {}", x1, y1);  // both valid

    let owner(x2) = take(String::from("hello"));
    let owner(y2) = take_or_mem_copy(x2);  // String is not Copy: take
    println!("y2 = {}", y2);
    // x2 is invalid here
}
```

`take_or_mem_copy` makes the compiler's decision visible. Standard Rust writes both operations as `let y = x`, and the same syntax produces different behavior depending on the type. For programs that need a full independent copy of a non-Copy type, `.clone()` allocates new space and copies the data.

---

## P2.20 · What mut Means on a Binding

Rebinding and writing through a coordinate are different operations.

Every binding so far has been fixed for its scope. A rebindable binding can point to a different value or a different target. The notation writes this as `rebindable`. Standard Rust writes `let mut`.

```rust
explicit! {
    let owner(rebindable(x)) = take(String::from("hello"));
    x = take(String::from("world"));  // x now holds different space
    println!("{}", at(x));            // prints "world"
}
```

Reassigning `x` frees the old space, because `x` was its owner and the owner has moved to different space. Rebinding changes the binding itself, pointing it to a new target. Changing data at a coordinate modifies the contents of the target without changing what the binding points to. These are different operations.

C++ uses `const` for both purposes: `const int x = 5` prevents reassignment, `const int& r = x` prevents writing through the reference. Rust separates them. `mut` on the binding controls reassignment. `&mut` on the coordinate controls write access. The two are orthogonal.

|  | Can reassign binding | Can write at coordinate |
|---|---|---|
| `let r = &x` | No | No |
| `let mut r = &x` | Yes | No |
| `let r = &mut x` | No | Yes |
| `let mut r = &mut x` | Yes | Yes |

---

## P2.21 · Shared and Exclusive Coordinates

Two kinds of coordinates exist. What are they?

`coord_exclusive` (`&mut T`) grants read and write access to one coordinate at a time. `coord_shared` (`&T`) grants read access and allows multiple coordinates to coexist. Rust calls `&T` a shared reference and `&mut T` a mutable reference.

```rust
explicit! {
    let owner(s) = take(String::from("hello"));
    let alias(r1) = coord_shared(s);
    let alias(r2) = coord_shared(s);
    println!("{}, {}", at(r1), at(r2)); // two shared coordinates, safe
}
```

```rust
explicit! {
    let owner(mut s) = take(String::from("hello"));
    let alias(w) = coord_exclusive(s);
    at(w).push_str(" world"); // one exclusive coordinate, safe
}
```

| Rust | Notation |
|---|---|
| `let r = &s` | `let alias(r) = coord_shared(s)` |
| `let w = &mut s` | `let alias(w) = coord_exclusive(s)` |
| `*r` | `at(r)` |
| `let mut x` | `owner(rebindable(x))` |

This is the coherence constraint from the earlier chapter, enforced at the language level. `coord_shared` corresponds to the Shared state in the protocol: many processes hold a read-only copy. `coord_exclusive` corresponds to the Modified/Exclusive states: one process holds the writable copy. The rule is the same — many readers or one writer — and the compiler enforces it at compile time with the same invariant the hardware enforces at runtime.

Rust's error messages say "borrowed as immutable" for a shared coordinate and "borrowed as mutable" for an exclusive coordinate. The borrowing rule derives the rules governing how shared and exclusive coordinates interact: many shared or one exclusive.

---

## P2.22 · Composing the Vocabulary

How do all these pieces fit together?

`at` works on both sides of an assignment. On the right it follows the coordinate and reads the value. On the left it follows the coordinate and writes a new value.

```rust
explicit! {
    let owner(rebindable(x)) = take(5);
    let alias(r) = coord_exclusive(x);
    at(r) = 10;                        // write: target now holds 10
    let owner(y) = mem_copy(at(r));    // read: y gets a copy of 10
}
```

The owner cannot be used while an exclusive coordinate to its space exists. The compiler enforces this as part of the borrowing rule. The owner resumes when the coordinate expires. `take` permanently invalidates the source. An exclusive coordinate temporarily suspends it.

```rust
explicit! {
    let owner(rebindable(x)) = take(5);
    let alias(r) = coord_exclusive(x);
    // x is suspended here
    at(r) = 10;
    // r goes out of scope, x resumes
    println!("{}", x);    // prints 10
}
```

`Box<T>` owns heap space containing a `T`. Following the coordinate with `at` reaches the inner value, and `take` moves that value out, transferring ownership and invalidating the Box.

```rust
explicit! {
    let owner(b) = take(Box::new(String::from("hello")));
    let owner(s) = take(at(b));    // move String out of the Box
    println!("{}", s);
    // b is invalid
}
```

Standard Rust writes `let s = *b`. Dereferencing a `Box` transfers ownership of the inner value. Regular references do not support this operation.

| Notation | Framework |
|---|---|
| `owner` | SPACE × TIME |
| `alias` | COORDINATES |
| `take` | Ownership transfer in TIME |
| `coord_exclusive` | One COORDINATE, read/write |
| `coord_shared` | Many COORDINATES, read only |
| `at` | Follow a COORDINATE |
| `mem_copy` | Duplicate SPACE, independent owners |
| `take_or_mem_copy` | Compiler decides based on type |
| `rebindable` | Binding changes target over TIME |

The `explicit!` notation reveals what standard Rust leaves implicit. The next chapter derives the rule governing how shared and exclusive coordinates interact.

---

## P2.23 · How a Coordinate Can Dangle While Its Owner Lives

Ownership prevents dangling after free. But coordinates can dangle for a different reason.

A `Vec` manages a growable heap buffer. A coordinate to the Vec struct and a coordinate to one of its elements refer to different space. The Vec struct sits on the stack. The element lives in the heap buffer at an address the Vec computes as `buffer_ptr + index * sizeof(T)`.

```
Stack                         Heap
┌─────────────┐              ┌───┬───┬───┐
│      v      │              │ 1 │ 2 │ 3 │
│  ptr ───────│─────────────>│   │   │   │
│  len: 3     │              └───┴───┴───┘
│  cap: 3     │
└─────────────┘

coord_shared(v)    → the Vec struct on the stack
coord_shared(v[0]) → the first element in the heap buffer
```

`v.push(4)` can reallocate the heap buffer when capacity runs out. The Vec allocates a larger buffer, copies the elements, and frees the old one. A coordinate created before the push still holds the old address. The owner `v` remains alive. A coordinate to the Vec struct would still work. The element coordinate points to reclaimed space because the method changed the internal state that determines the address.

```
    ┌───┐
    │ A │  let r = &v[2]
    └─┬─┘
      ▼
    ┌───┐
    │ B │  v.push(4) — may reallocate, old buffer freed
    └─┬─┘
      ▼
    ┌───┐
    │ C │  println!("{}", r) — r points to freed memory ✗
    └───┘
```

The path from B to C exists. A method changed internal state while a coordinate computed from that state was still alive. This is the same pattern behind the push-while-borrowing error from chapter 1, but now the mechanism is visible: the owner lives, the derived coordinate dangles.

A change to `buffer_ptr` makes the element address stale. The element coordinate depends on state the Vec controls, and push can change that state.

---

## P2.24 · Why Derived Data Needs a Sync Strategy

The element coordinate dangled because it was computed from internal state. This is a general pattern.

The element coordinate is derived data. The address `buffer_ptr + index * sizeof(T)` is computed from the Vec's internal state. When that state changes, the derived value becomes stale. A formula `a = f(b)` creates a dependency: `a` becomes stale if `b` changes and no recomputation follows.

CPU caches, database indexes, element coordinates, and struct field dependencies are all instances of the same pattern. Two kinds of derived data appear in programs. Derived coordinates are addresses computed from internal state. Derived values are fields whose values depend on another field. Both create invalidation problems.

A compiler managing coordinates needs a sync strategy: prevent the source from changing while derived data exists, or recompute derived data after every change. Recomputation requires knowing the dependency formula, which the compiler does not have for arbitrary user types. Prevention is the feasible option.

---

## P2.25 · Why the Compiler Cannot Distinguish Safe Methods

The compiler needs to block dangerous operations. Can it tell which methods are dangerous?

`sort` takes exclusive access to a Vec and rearranges elements in place. `push` takes exclusive access and can reallocate the buffer. Whether `push` reallocates depends on runtime values: the current length, the current capacity, the allocator's behavior. This is undecidable. No static analysis can determine for all programs whether a particular call to `push` will reallocate.

The compiler cannot tell which exclusive-access methods restructure internal space and which merely rearrange it. The distinction depends on runtime state. The compiler blocks any method that takes exclusive access while an element coordinate exists. This is conservative — `sort` would be safe — but conservatism is the price of soundness.

---

## P2.26 · The Borrowing Rule

The compiler blocks exclusive access while a coordinate exists. How long does a coordinate exist?

Three levels of structure shape the answer. Source text organizes names into a scope hierarchy. A name enters scope at its declaration and leaves when the enclosing block ends. The language's semantics operate above that hierarchy: function arguments carry data into nested scopes, return values carry it out, assignments move data between bindings. The RAM machine sits below both levels and has no scope structure at all.

Coordinate liveness is a data-flow property. A coordinate's last use can fall well before its name goes out of scope. The compiler tracks each coordinate from creation to last use on the control flow graph, the same tool used earlier to detect access to reclaimed space. Rust calls this analysis **non-lexical lifetimes** (NLL). Before Rust 2018, the compiler used scope boundaries, which rejected valid programs unnecessarily.

```rust
let r = &v[0];
println!("{}", r);   // last use of r
v.push(4);           // allowed: r is no longer alive
```

```rust
let r = &v[0];
v.push(4);           // REJECTED: r is still alive
println!("{}", r);
```

The only difference is the order of the last two statements. In the first program, `r`'s last use is the `println!`, and `push` comes after. In the second, `push` falls inside `r`'s liveness range. The compiler rejects the second and accepts the first.

This generalizes to the full rule. A space can have many shared coordinates or one exclusive coordinate at any point in the program. Code holding a shared coordinate can read. Methods requiring exclusive access are blocked, so shared coordinates coexist safely. Code holding an exclusive coordinate can call any method, so no other coordinate to the same space can be alive while it exists. This is the borrowing rule.

---

## P2.27 · Why the Borrowing Rule Applies to Every Type

Vec manages heap space. But the rule applies to `i32` too. Why?

An `i32` has no managed internal space. Two coordinates to the same `i32` cannot produce a dangling pointer because there is no internal space to restructure. The compiler could apply the borrowing rule only to types that manage internal allocations and leave simple types unconstrained.

Determining which types manage internal space requires analyzing field dependencies. A struct might hold a raw pointer to a heap allocation, or contain a field whose value depends on another field. Deciding whether a type has such dependencies is undecidable for the same reasons described earlier. The compiler applies the borrowing rule uniformly to every type.

```
The compiler needs a sync strategy for derived data
│
├── Derived from managed space
│     push changes buffer_ptr → element coordinate dangles
│
├── Derived from struct fields
│     field a = f(field b) → changing b makes a stale
│
└── The compiler cannot distinguish these cases
      → uniform rule: many shared or one exclusive
```

The uniform rule is a sync strategy for derived data in general. An element coordinate is derived from the Vec's buffer pointer. A struct field value can be derived from another field. A cache entry is derived from the data it caches. The compiler cannot know which types have these dependencies, so it prevents any source from changing while a coordinate derived from it exists.

---

## P2.28 · The Chapter 1 Programs and the Borrowing Rule

The borrowing rule derived. Now see it applied to every rejected program from chapter 1.

**Push while borrowing.** `r` is a shared coordinate to the first element. `push` requires exclusive access to `v`. A shared coordinate and an exclusive coordinate to overlapping space exist at the same time.

```rust
let mut v = vec![1, 2, 3];
let r = &v[0];
v.push(4);
println!("{}", r);
```

**error[E0502]:** cannot borrow `v` as mutable because it is also borrowed as immutable

**Iterator invalidation.** The `for` loop creates an iterator that holds a shared coordinate to `v`. `push` requires exclusive access.

```rust
let mut v = vec![1, 2, 3];
for x in &v {
    v.push(*x);
}
```

**error[E0502]:** cannot borrow `v` as mutable because it is also borrowed as immutable

**Closure capturing.** The closure captures `v` by exclusive coordinate because it calls `push`. `println!` requires a shared coordinate. The closure exists from creation to last use, and the `println!` falls within that range.

```rust
let mut v = vec![1, 2, 3];
let closure = || v.push(4);
println!("{:?}", v);
closure();
```

**error[E0502]:** cannot borrow `v` as immutable because it is also borrowed as mutable

**Two exclusive coordinates.** Two `&mut` to the same `String`. Either one could call methods that restructure the internal buffer while the other depends on the old state.

```rust
let mut s = String::from("hello");
let r1 = &mut s;
let r2 = &mut s;
println!("{}, {}", r1, r2);
```

**error[E0499]:** cannot borrow `s` as mutable more than once at a time

Each program maps to the same pattern: a coordinate exists while an operation that could invalidate it is attempted.

C++ has the same vocabulary of shared and exclusive access. `const T&` grants read-only access. `T&` grants read and write. C++ does not enforce the borrowing rule at compile time. The push-while-borrowing and iterator invalidation examples compile in C++ without error and produce undefined behavior at runtime.

```cpp
// C++ — compiles, undefined behavior
std::vector<int> v = {1, 2, 3};
int& r = v[0];
v.push_back(4);   // may reallocate
std::cout << r;    // r may point to freed memory
```

The C++ standard calls this the programmer's responsibility. The Rust compiler calls it a rejected program.

---

# PART 3: THE CONSEQUENCES

---

## P3.1 · What the Compiler Can Distinguish

The borrowing rule is an approximation. Where does it lose precision?

The analysis tracks coordinates. The granularity of that tracking determines which simultaneous accesses the compiler can prove safe.

**Struct fields.** `p.x` and `p.y` are distinct. The type declaration places them at distinct offsets, and field names are literal in source text. The compiler can prove that coordinates to `p.x` and coordinates to `p.y` point to disjoint space.

**Array elements with literal indices.** `v[0]` and `v[1]` refer to different elements. The literal indices are visible in source text, so the compiler could distinguish them. Rust treats all index expressions as potentially overlapping. This is a design choice.

**Array elements with computed indices.** `v[i]` and `v[j]` where `i` and `j` come from computation. Whether `i == j` depends on runtime values. Proving it requires solving the halting problem.

| Coordinates to | Distinguishable? | Why |
|---|---|---|
| `p.x` vs `p.y` | Yes | Field names literal in source |
| `v[0]` vs `v[1]` | Could be | Rust's design choice |
| `v[i]` vs `v[j]` | No | May require solving halting problem |

Branches and pointer aliasing share this structure. Whether `*p` and `*q` point to the same space depends on runtime values. The compiler assumes worst case for both. The compiler handles what source text names. Computed values belong to data flow, and data flow is where undecidability enters.

---

## P3.2 · Why Compilers Skip Decidable Cases

Literal indices are decidable. Why does the compiler still reject them?

The literal index `v[0]` is decidable. So is a function call `pick(&a, &b, true)` where the argument is a constant. A human sees which branch executes. The compiler could too, using constant propagation and inlining.

General-purpose compilers favor predictability. Code that compiles when the argument is `true` fails when `true` becomes `some_function()`, and programmers cannot predict which expressions cross the decidability boundary. Identifying which expressions fall into decidable subsets is itself expensive, and coverage is narrow.

Specialized compilers do analyze decidable cases. Fortran compilers use polyhedral analysis for loop parallelization. ML frameworks require static tensor shapes. The techniques exist and work for specific domains.

Rust chose consistent approximate analysis. The compiler treats all index expressions and all branch conditions the same way. A program that compiles with a constant will still compile when that constant becomes a variable.

---

## P3.3 · Signatures Lose Field-Level Information

The compiler distinguishes struct fields inside a body. What happens across a function call?

Inside a function body, the compiler sees field-level coordinate access. `&d.field1` and `&d.field2` point to disjoint space, and the compiler knows it.

```rust
let r1 = &d.field1;    // coordinates to d.field1
let r2 = &d.field2;    // coordinates to d.field2
// disjoint: yes
```

Across a function call, only the signature is visible. A method that borrows `self` borrows the whole struct.

```rust
let r1 = d.get_field1();   // signature says: borrows d
let r2 = d.get_field2();   // signature says: borrows d
// disjoint: the compiler cannot tell
```

Signatures encode coordinate connections at struct granularity. Field-level information does not cross function boundaries. Types describe whole structs, and connections are encoded at the type level. The compiler sees two borrows of `d` where a human sees two borrows of disjoint fields.

---

## P3.4 · The Mirror

The borrowing rule is derived. The compiler tightened three dimensions to make analysis possible.

| Dimension | What was tightened | The patch |
|---|---|---|
| SPACE | Each allocation has one owner | Ownership, move by default |
| TIME | Space is reclaimed when the owner's scope ends | Scope-tied lifetimes |
| COORDINATES | One exclusive or many shared, never both | Constrained references, borrowing rule |

The tightening is the price of compile-time proof. The proof is sound: no unsafe program is accepted. But the proof is approximate: some safe programs are rejected. The rejected programs are safe because the programmer knows something the compiler cannot verify.

Every rejection is an over-constraint in one of the three dimensions. Every fix relaxes that dimension. The question is always: which dimension is too tight, and how far do you need to loosen it?

|  | Type-level encoding | Runtime checking | Manual assertion |
|---|---|---|---|
| **SPACE** | `Cow`, entry API, arena indices | `Rc`, `Arc` | raw pointer, `unsafe` |
| **TIME** | Lifetime annotations, `'static` | `RefCell` borrow tracking | `unsafe` lifetime cast |
| **COORDINATES** | `split_at_mut`, `NonNull` | `Mutex`, `RwLock` | raw pointer deref |

Each column to the right costs more. Type-level encoding costs design effort but nothing at runtime. Runtime checking costs performance. Manual assertion costs safety — the programmer asserts the invariant the compiler cannot prove, and no one checks.

The rest of this chapter walks through each row.

---

## P3.5 · Relaxing Space

**What was tightened.** Each allocation has one owner. When the owner goes out of scope, the space is reclaimed. Assignment moves ownership; the source becomes invalid.

**Where it over-rejects.** Multiple parts of the program need access to the same data, and no single binding is the natural owner. A configuration struct shared by several subsystems. A graph where multiple edges reach the same node. A cache that outlives any single caller.

**Type-level encoding: clone, copy, and arena indices.**

`Clone` duplicates the data. Each copy is independent. Ownership is trivial — each binding owns its own space. The cost is CPU time and memory.

```rust
let config = load_config();
let for_service_a = config.clone();
let for_service_b = config.clone();
```

`Cow` (clone on write) defers the clone until mutation is needed.

```rust
use std::borrow::Cow;
fn process(input: Cow<str>) -> Cow<str> {
    if input.contains("bad") {
        Cow::Owned(input.replace("bad", "good"))
    } else {
        input // no clone
    }
}
```

`Copy` opts small types into automatic duplication. `i32`, `bool`, `f64` — types with no managed space. Duplicating their bytes produces two independent values.

Arena indices eliminate coordinates entirely. A single collection owns all nodes. Edges are pairs of integers — `usize`, freely copyable, no lifetime tracking. The borrow checker does not track indices.

```rust
struct Graph {
    nodes: Vec<NodeData>,
    edges: Vec<(usize, usize)>,
}
```

Index validity becomes the programmer's responsibility, or a runtime check with generational indices:

```rust
struct NodeId {
    index: usize,
    generation: u64,
}
```

This is the only pattern that resolves all three dimensions simultaneously. The single collection centralizes SPACE. All nodes live as long as the collection, making TIME uniform. Indices are `Copy` integers, so COORDINATES disappear entirely. The Rust ecosystem converges on this pattern for anything graph-shaped: `petgraph`, `slotmap`, most game engines.

**Runtime checking: Rc and Arc.**

`Rc` and `Arc` let multiple handles share ownership. The value lives until the last handle drops. A runtime counter replaces the single-owner constraint.

```rust
use std::rc::Rc;
let config = Rc::new(AppConfig::load());
let service_a = ServiceA::new(Rc::clone(&config));
let service_b = ServiceB::new(Rc::clone(&config));
// config lives until all three handles drop
```

Cycles leak memory — each node in the cycle keeps the next alive. `Weak` breaks cycles by providing a non-owning handle.

`Rc` is for single-threaded code. `Arc` is the atomic version for sharing across threads. The cost is the reference count update on every clone and drop.

**Manual assertion: raw pointers.**

`*const T` and `*mut T` have no ownership semantics. Creating them is safe. Dereferencing them requires `unsafe`. The programmer asserts that the pointed-to space exists and is valid. No one checks.

---

## P3.6 · Relaxing Time

**What was tightened.** Space is reclaimed when the owner's scope ends. References must not outlive their target. Lifetimes are checked at compile time.

**Where it over-rejects.** Coordinate lifetimes overlap when they should not, but the compiler cannot see the ordering. A reference created before a mutation is used after it, but on a different branch. A function returns a reference to one of its inputs, and the caller knows which one, but the signature declares both.

**Type-level encoding: reordering, scoping, and lifetime annotations.**

Reorder operations so the first coordinate's lifetime ends before the second begins.

```rust
let mut v = vec![1, 2, 3];
let first = v[0]; // read the value — coordinate ends
v.push(4);        // exclusive access, no conflict
println!("{}", first); // first is a copy, not a coordinate
```

Scoping with `{}` forces a coordinate to die before the next operation.

```rust
let mut v = vec![1, 2, 3];
{
    let r = &v[0];
    println!("{}", r);
} // r dies here
v.push(4); // safe
```

Lifetime annotations declare cross-scope dependencies the compiler cannot infer.

```rust
fn first<'a>(list: &'a [i32]) -> &'a i32 {
    &list[0]
}
```

Adding `T: 'a` constrains a generic. Adding `'a: 'b` declares containment. Same mechanism at different sites.

Separate read and write passes eliminate aliasing conflicts by separating the operations in time.

```rust
// Read pass: shared coordinates
let indices: Vec<usize> = data.iter()
    .enumerate()
    .filter(|(_, v)| v.should_update())
    .map(|(i, _)| i)
    .collect();

// Write pass: exclusive coordinate
for i in indices {
    data[i].update();
}
```

Building new collections avoids the conflict entirely. The iterator holds a shared coordinate to the input for the entire chain. Nothing mutates the input.

```rust
let result: Vec<_> = input.iter()
    .filter(|x| x.is_valid())
    .map(|x| x.transform())
    .collect();
```

**Runtime checking: RefCell.**

`RefCell` enforces the borrowing rule at runtime instead of compile time. `.borrow()` returns a shared guard. `.borrow_mut()` returns an exclusive guard. Multiple shared guards can coexist. An exclusive guard panics if any other guard exists.

```rust
use std::cell::RefCell;
let data = RefCell::new(vec![1, 2, 3]);
let r1 = data.borrow();     // shared: counter increments
let r2 = data.borrow();     // shared: counter increments
drop(r1);                   // counter decrements
drop(r2);                   // counter decrements
let mut w = data.borrow_mut(); // exclusive: panics if counter > 0
w.push(4);
```

The same rule the compiler checks at compile time, enforced at runtime with a borrow counter.

**Message passing: channels.**

Channels serialize access across time by moving data from sender to receiver.

```rust
use std::sync::mpsc;
let (tx, rx) = mpsc::channel();
std::thread::spawn(move || {
    let data = vec![1, 2, 3];
    tx.send(data).unwrap(); // data moves to channel
    // data is invalid here
});
let received = rx.recv().unwrap(); // received owns the data
```

Sending data moves ownership. The sender's binding becomes invalid. The receiver's binding becomes the owner. At any moment, one thread owns the data. This is the same primitive the coherence protocol uses: the old holder surrenders the token, the new holder receives it.

**Drain and rebuild.** `drain()` transfers ownership of each element from the collection to the loop body. The loop can move, modify, or drop each element without conflicting with the collection.

**Swap to extract.** `mem::swap`, `mem::replace`, and `mem::take` extract a value at an exclusive coordinate while leaving valid space behind. The owner always owns something after the operation.

```rust
fn rotate_name(person: &mut Person) -> String {
    let old = std::mem::take(&mut person.name); // leaves String::new()
    person.name = format!("former_{}", old);
    old
}
```

**Manual assertion: unsafe lifetime casts.**

`transmute` and raw pointers can extend or erase lifetimes. The programmer asserts that the reference outlives its use. The compiler cannot verify this.

---

## P3.7 · Relaxing Coordinates

**What was tightened.** One exclusive coordinate or many shared coordinates at any point. No other combination exists.

**Where it over-rejects.** Two parts of the code need exclusive access to different parts of the same space. The compiler sees one coordinate to the whole and rejects the second. Or: a struct needs to mutate an internal field during a shared access — the method takes `&self` but needs to write.

**Type-level encoding: split and partition.**

`split_at_mut` produces two exclusive coordinates to non-overlapping regions. The return type encodes the disjointness.

```rust
let mut v = vec![1, 2, 3, 4, 5, 6];
let (left, right) = v.split_at_mut(3);
left[0] = 10;
right[0] = 40; // different region, no conflict
```

Struct fields split automatically. The compiler knows `person.name` and `person.age` occupy disjoint space.

```rust
let name = &mut person.name;
let age = &mut person.age; // different field, compiler allows
```

Binding struct fields individually before a closure achieves the same at the field level.

Borrowing instead of consuming: pass `&` where ownership is not needed. The caller keeps ownership.

```rust
fn print_len(s: &String) {
    println!("{}", s.len());
}
let s = String::from("hello");
print_len(&s);
println!("{}", s); // s still alive
```

**Runtime checking: interior mutability.**

The aliasing rule guarantees that no code writes during shared access. Sometimes a struct needs to write during shared access — a cache lookup, a lazy initialization, a counter increment.

`Cell<T>` never exposes a coordinate to its contents. `.get()` copies the value out, `.set()` overwrites it. Works for `Copy` types.

```rust
use std::cell::Cell;
let counter = Cell::new(0);
let a = &counter;
let b = &counter;
a.set(a.get() + 1);
b.set(b.get() + 1);
assert_eq!(counter.get(), 2);
```

`Mutex` and `RwLock` enforce the same rule across threads. `Mutex` provides exclusive access; callers block until the lock is free. `RwLock` allows many readers or one writer — the same constraint, enforced at runtime with blocking.

```rust
use std::sync::Mutex;
let data = Mutex::new(Vec::new());
let guard = data.lock().unwrap();
// guard holds exclusive access
// other threads block until guard drops
```

Rust's `Mutex` owns the data it protects. Code cannot access the data without acquiring the lock. This is different from C++, where a mutex and the data it protects are separate objects, and the programmer must remember to lock before accessing.

**Mutability upgrade.** The code requests a shared coordinate where it needs an exclusive one. `.iter()` → `.iter_mut()`. `&` → `&mut`. `.get()` → `.get_mut()`. Adding `mut` to a binding.

```rust
let mut v = vec![1, 2, 3];
for item in v.iter_mut() {
    *item += 1;
}
```

**Manual assertion: raw pointer deref.**

`*const T` and `*mut T` bypass the aliasing rule entirely. The programmer asserts that no conflicting access exists. The `miri` interpreter can check many of these assertions at runtime during testing.

---

## P3.8 · Data Shapes and the Table

Ownership forms a tree. Not all data fits a tree. The table maps common shapes to the dimension that is over-constrained and the position on the spectrum.

| Data shape | Over-constrained dimension | Spectrum position |
|---|---|---|
| DAG, shared config | SPACE — multiple paths, one owner | Clone (type-level) or Rc/Arc (runtime) |
| Graph, doubly linked list | SPACE — cycles, back-pointers | Arena indices (type-level) |
| Cache, lazy init | COORDINATES — mutation behind &self | Cell/RefCell (runtime) |
| Observer, event system | SPACE + COORDINATES — multiple observers | Rc + RefCell or channels |
| Thread communication | TIME — access serialized across threads | Channels (runtime) or Arc + Mutex |
| Self-referential struct | COORDINATES — coordinate to own field | Pin (type-level) or restructure |
| LRU cache | SPACE + COORDINATES — hash map + list reach same entry | Arena indices (type-level) |
| Simulation (particles + grid) | COORDINATES — two fields of same struct | Split fields (type-level) |

The left column is the shape. The middle column is which row of the mirror to look at. The right column is how far right on the spectrum to slide. Most problems have a type-level solution. Slide to runtime only when the type-level solution is too restrictive. Slide to unsafe only when the runtime solution is too expensive.

---

## P3.9 · When to Reach for Unsafe

The six patterns cover most programs. Unsafe serves the remainder.

The standard library uses `unsafe` internally for `Vec`, `HashMap`, `BTreeMap`, `String`, and many other types. The safe APIs that callers use are the product of the "encode invariants in types" escape hatch, where `unsafe` internals expose a safe public interface.

Programs reach for `unsafe` when the safe patterns impose unacceptable performance costs, when a data structure's invariants are provably correct but inexpressible in the type system, at FFI boundaries where Rust's ownership model does not extend, and for intrusive data structures where nodes contain their own link pointers.

The programmer manually asserts the S×T×C invariants that no safe pattern can express. The assertion covers the same three dimensions the compiler checks in safe code. SPACE requires the allocation to be valid. TIME requires the access to happen while the space exists. COORDINATES requires the references to satisfy the aliasing rule.

The `unsafe` keyword marks these assertions as visible and searchable in the codebase, and the `miri` interpreter can check many of them at runtime during testing.
