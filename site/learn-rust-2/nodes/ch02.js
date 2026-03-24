// ═══ CHAPTER 2: S×T×C — SPACE, TIME, COORDINATES ═══

// --- Nodes ---

N.physics={ch:'S×T×C',t:'Distance Forces Copies',ctx:'Why do these bugs exist? Physics.',
b:function(){return ''
+'<p>Physics creates distance. Distance forces copies. Copies require coherence.</p>'
+'<p>CPU caches exist because RAM is far from the CPU. Database replicas exist because users are far from the primary. Thread-local storage exists because shared memory requires synchronization.</p>'
+'<p>Every copy is <strong>derived data</strong> — a representation computed from a source that carries a sync obligation. A change to the source makes the derived copy stale. Cache invalidation, stale replicas, '+EA('e24','dangling pointers')+', and torn register reads all follow from the same mechanism.</p>'
+'<table><tr><th>Layer</th><th>Source</th><th>Copy</th><th>Sync Strategy</th></tr>'
+'<tr><td>CPU cache</td><td>RAM</td><td>L1/L2/L3 line</td><td>MESI protocol</td></tr>'
+'<tr><td>Compiler</td><td>Memory</td><td>Register</td><td>Register allocation</td></tr>'
+'<tr><td>Language</td><td>Original value</td><td>Reference/alias</td><td>Ownership, locks, GC</td></tr>'
+'<tr><td>Thread</td><td>Shared heap</td><td>Thread-local</td><td>Mutex, channels, atomics</td></tr>'
+'<tr><td>Process</td><td>Shared memory</td><td>Process-local</td><td>IPC, message passing</td></tr>'
+'<tr><td>Database</td><td>Primary</td><td>Replica</td><td>Replication protocol</td></tr>'
+'<tr><td>Network</td><td>Origin server</td><td>CDN edge</td><td>TTL, invalidation</td></tr>'
+'</table>'
+'<p>Each layer resolves staleness with a different sync strategy. The pattern is the same. The '+EA('e25','three primitives')+' make the pattern precise.</p>';
}};

N.primitives={ch:'S×T×C',t:'Three Primitives',ctx:'The physics made precise.',
b:function(){return ''
+'<p><strong>SPACE.</strong> Where data lives. Stack, heap, register, cache line, disk, remote server. Memory hierarchy is a ladder of spaces with different latencies.</p>'
+'<p><strong>TIME.</strong> When things happen. Compile-time vs runtime. Sequential vs parallel. Scope entry to scope exit. A value\'s lifetime.</p>'
+'<p><strong>COORDINATES.</strong> How we refer to data. Names at compile-time, addresses at runtime, references, pointers. Coordinates tell you where space is.</p>'
+'<p>The rest of the course abbreviates these as <strong>S×T×C</strong>. Bugs are failures in one or more of the three. Fixes adjust one or more of the three.</p>'
+'<p><strong>State</strong> is data that changes over TIME. Stateless computation has no coherence problem. Most coherence problems involve state.</p>'
+'<p><strong>Derived data</strong> is the mechanism through which coherence problems propagate. A coordinate to a value is derived data. A cached copy is derived data. The source lives in one SPACE, the derived copy in another, and TIME determines when they fall out of sync.</p>';
}};

N.bugs_table={ch:'S×T×C',t:'Bugs as Interaction Failures',ctx:'Every memory bug maps to a specific interaction between primitives.',
b:function(){return ''
+'<p>Interactions between the three primitives produce every category of memory bug.</p>'
+'<p><strong>SPACE × TIME.</strong> When memory exists. Allocation, deallocation, lifetimes, scope.</p>'
+'<p><strong>SPACE × COORDINATES.</strong> How many paths lead to memory. Aliasing, ownership, null.</p>'
+'<p><strong>TIME × COORDINATES.</strong> When a reference is valid. Scope, shadowing, drop order.</p>'
+'<table><tr><th>Bug</th><th>Primitives</th><th>Breakdown</th></tr>'
+'<tr><td>'+EA('e29','Use-after-free')+'</td><td>S×T×C</td><td>Use (COORD) after (TIME) free (SPACE)</td></tr>'
+'<tr><td>'+EA('e30','Dangling pointer')+'</td><td>T×C</td><td>Pointer (COORD) outlives target (TIME)</td></tr>'
+'<tr><td>'+EA('e33','Double free')+'</td><td>S×T</td><td>Free (SPACE) twice (TIME)</td></tr>'
+'<tr><td>'+EA('e31','Data race')+'</td><td>S×T×C</td><td>Data (SPACE) race (TIME) via aliases (COORD)</td></tr>'
+'<tr><td>'+EA('e34','Buffer overflow')+'</td><td>S×C</td><td>Coord exceeds bounds of SPACE</td></tr>'
+'<tr><td>'+EA('e35','Uninitialized read')+'</td><td>S×T×C</td><td>Read (COORD) before (TIME) init (SPACE)</td></tr>'
+'<tr><td>'+EA('e36','Memory leak')+'</td><td>S×T</td><td>Space outlives all coords (TIME)</td></tr>'
+'</table>'
+'<p>Each of these has a timeline that shows exactly how the interaction breaks down.</p>';
}};

N.uaf={ch:'S×T×C',t:'Use-After-Free',ctx:'Use (COORDINATES) after (TIME) free (SPACE).',
b:function(){return ''
+'<div class="cb"><code>TIME        t0              t1              t2              t3\nEVENTS      create heap     p ← addr(heap)  free heap       >at_addr(p)&lt;\n────────────────────────────────────────────────────────────────────────\nHEAP        ████████████████████████████████\nCOORD (p)                   ████████████████░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░\n                                                            >BUG&lt;</code></div>'
+'<p>The heap is freed at t2. The coordinate <code>p</code> still exists. At t3, the program follows <code>p</code> to memory that no longer belongs to it.</p>'
+'<p>In C++, this compiles and runs. The behavior is undefined. In Rust, '+EA('e37','ownership')+' prevents any coordinate from outliving the space it points to.</p>';
}};

N.dangling_ptr={ch:'S×T×C',t:'Dangling Pointer',ctx:'Pointer (COORDINATES) dangles (TIME: target gone).',
b:function(){return ''
+'<div class="cb"><code>TIME        t0              t1              t2              t3\nEVENTS      create x        r ← addr(x)     scope ends      >at_addr(r)&lt;\n────────────────────────────────────────────────────────────────────────\nSPACE (x)   ████████████████████████████████\nCOORD (r)                   ████████████████░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░\n                                                            >BUG&lt;</code></div>'
+'<p>The variable <code>x</code> lives on the stack. The scope ends at t2. The space is reclaimed. The coordinate <code>r</code> now points to reclaimed space.</p>'
+'<p>This is the same bug from '+EA('e43','the first chapter')+'. The timeline makes the mechanism precise: TIME destroys the SPACE, but the COORDINATE survives.</p>';
}};

N.double_free={ch:'S×T×C',t:'Double Free',ctx:'Free (SPACE) twice (TIME).',
b:function(){return ''
+'<div class="cb"><code>TIME        t0              t1              t2              t3\nEVENTS      create heap     p ← addr(heap)  free_at_addr(p) >free_at_addr(p)&lt;\n────────────────────────────────────────────────────────────────────────\nHEAP        ████████████████████████████████\nCOORD (p)                   ████████████████░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░\n                                                            >BUG&lt;</code></div>'
+'<p>The heap is freed at t2. At t3, the program frees it again. The allocator\'s bookkeeping is corrupted. The consequences are unpredictable: crashes, silent corruption, security vulnerabilities.</p>'
+'<p>Rust prevents this with '+EA('e47','single ownership')+'. One owner, one free, automatic.</p>';
}};

N.data_race={ch:'S×T×C',t:'Data Race',ctx:'Data (SPACE) race (TIME: parallel) via aliases (COORDINATES).',
b:function(){return ''
+'<div class="cb"><code>TIME        t0              t1              t2\nEVENTS      create x        p ← addr(x)     ╭─ >write_at_addr(p)&lt;\n                            q ← addr(x)     ╰─ >write_at_addr(q)&lt;\n────────────────────────────────────────────────────────────────────────\nSPACE (x)   ████████████████████████████████▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░\nCOORD (p)                   ████████████████████████████████████████████\nCOORD (q)                   ████████████████████████████████████████████\n                                            >BUG&lt; (parallel writes)</code></div>'
+'<p>Two coordinates reach the same space. Two threads write at the same time. No synchronization. The result depends on which write completes first — or whether they interleave at the byte level.</p>'
+'<p>A data race needs three ingredients: shared data (SPACE), parallel access (TIME), at least one writer (COORDINATES). Remove any one and the race disappears.</p>'
+'<p>The '+EA('e50','borrowing rule')+' removes the third ingredient at compile time: if any coordinate can write, no other coordinate can exist.</p>';
}};

N.buffer_overflow={ch:'S×T×C',t:'Buffer Overflow',ctx:'Coordinates that exceed the bounds of their space.',
b:function(){return ''
+'<div class="cb"><code>TIME        t0              t1\nEVENTS      create buf[5]   >at_addr(buf[7])&lt;\n────────────────────────────────────────────────────────────\nSPACE       ████████████████████████████████ [0..4]\nCOORD [7]                   ▓▓▓▓ (no SPACE here)\n                            >BUG&lt;</code></div>'
+'<p>The buffer has 5 elements. The coordinate points to index 7. The space at that address belongs to something else — or to nothing. Whatever is there gets read or overwritten.</p>'
+'<p>Rust prevents this at runtime: vector and slice indexing checks bounds. The coordinate cannot exceed the space.</p>';
}};

N.uninit_read={ch:'S×T×C',t:'Uninitialized Read',ctx:'Read (COORDINATES) before (TIME) the value exists (SPACE).',
b:function(){return ''
+'<div class="cb"><code>TIME        t0                      t1\nEVENTS      declare x, p ← addr(x)  >at_addr(p)&lt;\n────────────────────────────────────────────────────────────\nSPACE (x)   ░░░░░░░░░░░░░░░░░░░░░░░░▓▓▓▓░░░░░░░░░░░░░░░░░░░░\nCOORD (p)   ████████████████████████████████████████████████\n                                    >BUG&lt; (SPACE has no value)</code></div>'
+'<p>The space is allocated. The coordinate is valid. But TIME has not yet given the space a value. The program reads whatever bits happen to be in that memory.</p>'
+'<p>Rust prevents this at compile time: every variable must be initialized before use. The compiler tracks initialization along the control flow graph.</p>';
}};

N.memory_leak={ch:'S×T×C',t:'Memory Leak',ctx:'Space outlives all coordinates to it.',
b:function(){return ''
+'<div class="cb"><code>TIME        t0              t1              t2\nEVENTS      create heap     p ← addr(heap)  >p scope ends&lt;\n────────────────────────────────────────────────────────────────────────\nHEAP        ████████████████████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░→\nCOORD (p)                   ████████████████\n                                            >BUG&lt; (ability to free heap through p is lost)</code></div>'
+'<p>The coordinate <code>p</code> goes out of scope. The heap allocation remains. No coordinate can reach it. The ability to free it is lost forever.</p>'
+'<p>Rust prevents most leaks with '+EA('e57','ownership')+': when the owner goes out of scope, the value is dropped automatically. Leaks are still possible (via <code>mem::forget</code> or reference cycles in <code>Rc</code>) but they require deliberate effort.</p>';
}};

N.paradigms={ch:'S×T×C',t:'Paradigms and Languages',ctx:'Different paradigms solve the coherence problem differently.',
b:function(){return ''
+'<table><tr><th>Language</th><th>Paradigm</th><th>Coherence Strategy</th></tr>'
+'<tr><td>Haskell</td><td>Functional</td><td>No mutation, no problem</td></tr>'
+'<tr><td>Erlang</td><td>Actor</td><td>Process isolation, message passing</td></tr>'
+'<tr><td>Clojure</td><td>Functional</td><td>Persistent data structures</td></tr>'
+'<tr><td>Rust</td><td>Linear/ownership</td><td>Compile-time proof</td></tr>'
+'<tr><td>Go</td><td>Imperative</td><td>Channels or locks</td></tr>'
+'<tr><td>Java</td><td>OOP</td><td>Locks, volatile</td></tr>'
+'<tr><td>C/C++</td><td>Imperative</td><td>Programmer responsibility</td></tr>'
+'</table>'
+'<p><strong>Haskell.</strong> Functions cannot modify state. Data structures are immutable. Nothing changes, so nothing gets out of sync.</p>'
+'<p><strong>Erlang.</strong> Processes share no memory. Communication happens by copying messages. Each process owns its data exclusively.</p>'
+'<p><strong>Clojure.</strong> Data structures appear mutable but are immutable. Modifying a map returns a new map that shares structure with the old one.</p>'
+'<p><strong>Rust.</strong> Each value has one '+EA('e60','owner')+'. '+EA('e61','Borrowing rules')+' ensure one writer or many readers. Enforced at compile time with no runtime cost.</p>'
+'<p><strong>Go.</strong> Goroutines share memory. Channels transfer data. Mutexes protect shared access. The programmer chooses.</p>'
+'<p><strong>Java.</strong> Objects live on a shared heap. Locks and volatile exist but are not required. Correct synchronization is the programmer\'s job.</p>'
+'<p><strong>C/C++.</strong> Any pointer can alias any memory. Any thread can write anywhere. No compiler enforcement. Coherence is entirely manual.</p>';
}};

N.features={ch:'S×T×C',t:'Features as Interaction Solutions',ctx:'Each language feature targets a specific interaction between primitives.',
b:function(){return ''
+'<p><strong>SPACE × TIME</strong> — when memory exists.</p>'
+'<p><strong>Garbage collection</strong> (Lisp, 1959). The runtime tracks reachable memory and reclaims the rest. Eliminates use-after-free and double-free. Cost: unpredictable pauses.</p>'
+'<p><strong>RAII / destructors</strong> (C++, 1980s). Memory lifetime tied to scope. Destructor runs when the scope ends. Deterministic, no runtime tracking.</p>'
+'<p><strong>Reference counting</strong> (1960s). Each allocation tracks how many references point to it. Count reaches zero, memory is freed. Cycles are a problem.</p>'

+'<p><strong>SPACE × COORDINATES</strong> — how many paths lead to memory.</p>'
+'<p><strong>'+EA('e64','Ownership / move semantics')+'</strong> (C++11, central to Rust). Each value has one owner. Passing transfers ownership. Aliasing eliminated by default.</p>'
+'<p><strong>Value types</strong> (ancient). Data copied rather than referenced. Each copy independent. No aliasing.</p>'
+'<p><strong>Nullable types / Option</strong> (ML, 1970s). References must be explicitly nullable. Code must handle the null case to compile.</p>'

+'<p><strong>TIME × COORDINATES</strong> — when a reference is valid.</p>'
+'<p><strong>Lexical scope</strong> (ALGOL, 1958). References valid only within their declaring scope.</p>'
+'<p><strong>'+EA('e66','Lifetime annotations')+' </strong> (Rust, 2010). Explicit markers for how long references live. No runtime cost.</p>'
+'<p><strong>Closures</strong> (Lisp, 1958). Captures force the language to decide: copy, borrow, or move the captured variables.</p>'

+'<p><strong>SPACE × TIME × COORDINATES</strong> — the full problem.</p>'
+'<p><strong>Mutex / RwLock</strong> (1960s). Runtime enforcement of one writer or many readers. Deadlocks possible.</p>'
+'<p><strong>Atomics</strong> (1970s). Hardware-level indivisible operations. Limited to simple operations.</p>'
+'<p><strong>Channels / message passing</strong> (CSP, 1978). Transfer ownership between threads. Sender loses access.</p>'
+'<p><strong>Immutability</strong> (ancient). No mutation means no write-read conflicts.</p>'
+'<p><strong>'+EA('e65','Rust borrow checker')+' </strong> (2010). One writer or many readers at compile time. No runtime cost. The practical form of linear types.</p>';
}};

// --- Edges from ch02 nodes ---

// From: physics
ED.e24={from:'physics',to:'dangling_ptr',t:'what does a dangling pointer look like?',w:'A pointer whose target has been freed. The timeline shows the mechanics.'};
ED.e25={from:'physics',to:'primitives',t:'Can you make this pattern precise?',w:'Three axes that explain every coherence failure.',card:true};
ED.e26={from:'physics',to:'paradigms',t:'How do other languages handle coherence?',w:'Seven languages solve the coherence problem with seven different strategies, from garbage collection to compile-time proofs.',card:true};

// From: primitives
ED.e27={from:'primitives',to:'bugs_table',t:'How do real bugs map to S×T×C?',w:'Every memory bug is a specific interaction failure between primitives.',card:true};
ED.e28={from:'primitives',to:'features',t:'How do language features map to S×T×C?',w:'Each feature targets a specific interaction between primitives.',card:true};

// From: bugs_table (card edges first, then prose-only)
ED.e29={from:'bugs_table',to:'uaf',t:'What does use-after-free look like?',w:'A heap pointer followed after the heap is freed.',card:true};
ED.e30={from:'bugs_table',to:'dangling_ptr',t:'What does a dangling pointer look like?',w:'A reference that outlives the thing it points to.',card:true};
ED.e31={from:'bugs_table',to:'data_race',t:'What does a data race look like?',w:'Two threads writing the same memory with no synchronization.',card:true};
ED.e32={from:'bugs_table',to:'paradigms',t:'How do different languages prevent these?',w:'Seven programming paradigms each use a different coherence strategy to prevent memory bugs.',card:true};
ED.e33={from:'bugs_table',to:'double_free',t:'What happens when you free twice?',w:'The allocator\'s bookkeeping is corrupted. Anything can happen.'};
ED.e34={from:'bugs_table',to:'buffer_overflow',t:'What happens beyond the bounds?',w:'The coordinate exceeds the allocated space. Whatever is at that address gets read or written.'};
ED.e35={from:'bugs_table',to:'uninit_read',t:'Why is reading uninitialized memory bad?',w:'The space exists but has no meaningful value yet. The program reads garbage.'};
ED.e36={from:'bugs_table',to:'memory_leak',t:'Is a leak really a bug?',w:'Space outlives all coordinates to it. The ability to free it is lost forever.'};

// From: uaf
ED.e37={from:'uaf',to:'ownership_deep',t:'how does Rust prevent this?',w:'One owner. When the owner goes out of scope, the value is dropped. No coordinate can outlive the owner.'};
ED.e38={from:'uaf',to:'dangling_ptr',t:'How is a dangling pointer different from this?',w:'Same mechanism, but the target goes out of scope instead of being freed.',card:true};
ED.e39={from:'uaf',to:'double_free',t:'What if you free the same memory twice?',w:'The allocator\'s bookkeeping corrupts. Anything can happen.',card:true};

// From: dangling_ptr
ED.e40={from:'dangling_ptr',to:'double_free',t:'What if you free the same memory twice?',w:'The allocator\'s bookkeeping corrupts. Anything can happen.',card:true};
ED.e41={from:'dangling_ptr',to:'uaf',t:'How is use-after-free different from this?',w:'Explicit free instead of scope end. Same dangling coordinate.',card:true};
ED.e42={from:'dangling_ptr',to:'data_race',t:'What happens when two threads hit the same memory?',w:'Two threads access the same memory with no coordination. The outcome changes between runs.',card:true};
ED.e43={from:'dangling_ptr',to:'dangling_detail',t:'remind me of the Rust vs C++ comparison',w:'Rust rejects it at compile time. C++ prints garbage, crashes, or works by accident.'};

// From: double_free
ED.e44={from:'double_free',to:'data_race',t:'What happens with parallel access instead?',w:'Two threads access the same memory without synchronization. The outcome is nondeterministic.',card:true};
ED.e45={from:'double_free',to:'uaf',t:'How is use-after-free related?',w:'Access after free instead of freeing twice. Same stale coordinate.',card:true};
ED.e46={from:'double_free',to:'memory_leak',t:'What if memory is never freed at all?',w:'The opposite problem. Space outlives every coordinate to it.',card:true};
ED.e47={from:'double_free',to:'ownership_deep',t:'how does one owner prevent double free?',w:'Each value has exactly one owner. The owner frees the value once, automatically, when it goes out of scope. No second free is possible.'};

// From: data_race
ED.e48={from:'data_race',to:'buffer_overflow',t:'What happens when a coordinate goes out of bounds?',w:'The index exceeds the allocated space. Whatever lives there gets hit.',card:true};
ED.e49={from:'data_race',to:'paradigms',t:'How do languages prevent data races?',w:'Channels, locks, immutability, and ownership are among seven strategies languages use to prevent data races.',card:true};
ED.e50={from:'data_race',to:'borrowing_rule_deep',t:'wait, one rule handles all of this?',w:'How does one rule handle both single-threaded and multi-threaded bugs?'};

// From: buffer_overflow
ED.e51={from:'buffer_overflow',to:'uninit_read',t:'What if the memory was never initialized?',w:'Space exists but TIME has not yet given it a value.',card:true};
ED.e52={from:'buffer_overflow',to:'uaf',t:'What if the memory was freed instead?',w:'The coordinate outlives the space. Same dangling pattern.',card:true};

// From: uninit_read
ED.e53={from:'uninit_read',to:'memory_leak',t:'What if memory is never freed at all?',w:'Space outlives every coordinate. The ability to free it is gone.',card:true};
ED.e54={from:'uninit_read',to:'bugs_table',t:'How do all seven bugs compare?',w:'The full table mapping every bug to S×T×C interactions.',card:true};

// From: memory_leak
ED.e55={from:'memory_leak',to:'paradigms',t:'How do languages deal with all of this?',w:'Seven programming paradigms each handle memory management with a different coherence strategy.',card:true};
ED.e56={from:'memory_leak',to:'bugs_table',t:'How do all seven bugs compare?',w:'The full table mapping every bug to S×T×C interactions.',card:true};
ED.e57={from:'memory_leak',to:'ownership_deep',t:'how does ownership prevent leaks?',w:'When the owner goes out of scope, the value is automatically dropped. No coordinate needs to explicitly free it.'};

// From: paradigms
ED.e58={from:'paradigms',to:'features',t:'Which specific features solve which interactions?',w:'Which feature targets which dimension?',card:true};
ED.e59={from:'paradigms',to:'primitives',t:'Remind me what S×T×C stands for?',w:'What are the three dimensions of memory?',card:true};
ED.e60={from:'paradigms',to:'ownership_deep',t:'so what happens when I assign a value?',w:'What happens to the old binding after assignment?'};
ED.e61={from:'paradigms',to:'borrowing_rule_deep',t:'how does Rust enforce one-writer-or-many-readers?',w:'How does one rule enforce memory safety at compile time?'};

// From: features
ED.e62={from:'features',to:'bugs_table',t:'Which bugs does each feature actually prevent?',w:'The bug table maps every category to its S×T×C interaction.',card:true};
ED.e63={from:'features',to:'paradigms',t:'Which languages use which of these features?',w:'Seven paradigms and their coherence strategies compared.',card:true};
ED.e64={from:'features',to:'ownership_deep',t:'what does this look like when I write code?',w:'let s2 = s1 moves the value. s1 is invalid afterward. One owner at a time.'};
ED.e65={from:'features',to:'borrowing_rule_deep',t:'can a compiler really catch all of this?',w:'One writer or many readers, enforced at compile time. No runtime cost.'};
ED.e66={from:'features',to:'sigs_lifetimes',t:'what are those tick-a things in function signatures?',w:'fn longest<\'a>(x: &\'a str, y: &\'a str) -> &\'a str. The \'a declares how long the return lives.'};
