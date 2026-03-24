// ═══ CHAPTER 6: THE APPROXIMATION ═══

// --- Nodes ---

N.coord_dangle={ch:'The Approximation',t:'How a Coordinate Can Dangle While Its Owner Lives',ctx:'Ownership prevents dangling after free. But coordinates can dangle for a different reason.',
b:function(){return ''
+'<p>A <code>Vec</code> manages a growable heap buffer. A coordinate to the Vec struct and a coordinate to one of its elements refer to different space. The Vec struct sits on the stack. The element lives in the heap buffer at an address the Vec computes as <code>buffer_ptr + index * sizeof(T)</code>.</p>'
+'<div class="cb"><code>Stack                    Heap\n┌─────────────┐         ┌───┬───┬───┐\n│ v           │         │ 1 │ 2 │ 3 │\n│  ptr ───────│────────&gt;│   │   │   │\n│  len: 3     │         └───┴───┴───┘\n│  cap: 3     │\n└─────────────┘\n\ncoord_shared(v)    → the Vec struct on the stack\ncoord_shared(v[0]) → the first element in the heap buffer</code></div>'
+'<p><code>v.push(4)</code> can reallocate the heap buffer when capacity runs out. The Vec allocates a larger buffer, copies the elements, and frees the old one. A coordinate created before the push still holds the old address. The owner <code>v</code> remains alive. A coordinate to the Vec struct would still work. The element coordinate points to reclaimed space because the method changed the internal state that determines the address.</p>'
+'<div class="cb"><code>     ┌───┐\n     │ A │  <span class="k">let</span> r = &amp;v[<span class="n">2</span>]\n     └─┬─┘\n       ▼\n     ┌───┐\n     │ B │  v.<span class="m">push</span>(<span class="n">4</span>)       — may reallocate, old buffer freed\n     └─┬─┘\n       ▼\n     ┌───┐\n     │ C │  <span class="m">println!</span>(<span class="s">"{}"</span>, r)  — r points to freed memory ✗\n     └───┘</code></div>'
+'<p>The path from B to C exists. A method changed internal state while a coordinate computed from that state was still alive. This is the same pattern behind the '+EA('e6_8','push-while-borrowing error from chapter 1')+', but now the mechanism is visible: the owner lives, the derived coordinate dangles.</p>'
+'<p>A change to <code>buffer_ptr</code> makes the element address stale. The element coordinate depends on state the Vec controls, and push can change that state.</p>';
}};

N.derived_sync={ch:'The Approximation',t:'Why Derived Data Needs a Sync Strategy',ctx:'The element coordinate dangled because it was computed from internal state. This is a general pattern.',
b:function(){return ''
+'<p>The element coordinate is derived data. The address <code>buffer_ptr + index * sizeof(T)</code> is computed from the Vec\'s internal state. When that state changes, the derived value becomes stale. A formula <code>a = f(b)</code> creates a dependency: <code>a</code> becomes stale if <code>b</code> changes and no recomputation follows.</p>'
+'<p>CPU caches, database indexes, element coordinates, and struct field dependencies are all instances of the same pattern. Two kinds of derived data appear in programs. Derived coordinates are addresses computed from internal state. Derived values are fields whose values depend on another field. Both create invalidation problems.</p>'
+'<p>A compiler managing coordinates needs a sync strategy: prevent the source from changing while derived data exists, or recompute derived data after every change. Recomputation requires knowing the dependency formula, which the compiler does not have for arbitrary user types. Prevention is the feasible option.</p>';
}};

N.cant_distinguish={ch:'The Approximation',t:'Why the Compiler Cannot Distinguish Safe Methods',ctx:'The compiler needs to block dangerous operations. Can it tell which methods are dangerous?',
b:function(){return ''
+'<p><code>sort</code> takes exclusive access to a Vec and rearranges elements in place. <code>push</code> takes exclusive access and can reallocate the buffer. Whether <code>push</code> reallocates depends on runtime values: the current length, the current capacity, the allocator\'s behavior. '+EA('e6_9','Chapter 4 showed this is undecidable')+'. No static analysis can determine for all programs whether a particular call to <code>push</code> will reallocate.</p>'
+'<p>The compiler cannot tell which exclusive-access methods restructure internal space and which merely rearrange it. The distinction depends on runtime state. The compiler blocks any method that takes exclusive access while an element coordinate exists. This is conservative — <code>sort</code> would be safe — but conservatism is the price of soundness.</p>';
}};

N.borrowing_rule_deep={ch:'The Approximation',t:'The Borrowing Rule',ctx:'The compiler blocks exclusive access while a coordinate exists. How long does a coordinate exist?',
b:function(){return ''
+'<p>Three levels of structure shape the answer. Source text organizes names into a scope hierarchy. A name enters scope at its declaration and leaves when the enclosing block ends. The language\'s semantics operate above that hierarchy: function arguments carry data into nested scopes, return values carry it out, assignments move data between bindings. The RAM machine sits below both levels and has no scope structure at all.</p>'
+'<p>Coordinate liveness is a data-flow property. A coordinate\'s last use can fall well before its name goes out of scope. The compiler tracks each coordinate from creation to last use on the control flow graph, the same tool chapter 3 used to detect access to reclaimed space. Rust calls this analysis <strong>non-lexical lifetimes</strong> (NLL). Before Rust 2018, the compiler used scope boundaries, which rejected valid programs unnecessarily.</p>'
+'<div class="cb"><code><span class="k">let</span> r = &amp;v[<span class="n">0</span>];\n<span class="m">println!</span>(<span class="s">"{}"</span>, r);       <span class="c">// last use of r</span>\nv.<span class="m">push</span>(<span class="n">4</span>);              <span class="c">// allowed: r is no longer alive</span></code></div>'
+'<div class="cb"><code><span class="k">let</span> r = &amp;v[<span class="n">0</span>];\nv.<span class="m">push</span>(<span class="n">4</span>);              <span class="c">// REJECTED: r is still alive</span>\n<span class="m">println!</span>(<span class="s">"{}"</span>, r);</code></div>'
+'<p>The only difference is the order of the last two statements. In the first program, <code>r</code>\'s last use is the <code>println!</code>, and <code>push</code> comes after. In the second, <code>push</code> falls inside <code>r</code>\'s liveness range. The compiler rejects the second and accepts the first.</p>'
+'<p>This generalizes to the full rule. A space can have many '+EA('e6_12','shared coordinates')+' or one exclusive coordinate at any point in the program. Code holding a shared coordinate can read. Methods requiring exclusive access are blocked, so shared coordinates coexist safely. Code holding an exclusive coordinate can call any method, so no other coordinate to the same space can be alive while it exists. This is the borrowing rule.</p>';
}};

N.rule_every_type={ch:'The Approximation',t:'Why the Borrowing Rule Applies to Every Type',ctx:'Vec manages heap space. But the rule applies to i32 too. Why?',
b:function(){return ''
+'<p>An <code>i32</code> has no managed internal space. Two coordinates to the same <code>i32</code> cannot produce a dangling pointer because there is no internal space to restructure. The compiler could apply the borrowing rule only to types that manage internal allocations and leave simple types unconstrained.</p>'
+'<p>Determining which types manage internal space requires analyzing field dependencies. A struct might hold a raw pointer to a heap allocation, or contain a field whose value depends on another field. Deciding whether a type has such dependencies is '+EA('e6_13','undecidable for the same reasons chapter 4 described')+'. The compiler applies the borrowing rule uniformly to every type.</p>'
+'<div class="cb"><code>The compiler needs a sync strategy for derived data\n        │\n        ├── Derived from managed space\n        │    push changes buffer_ptr → element coordinate dangles\n        │\n        ├── Derived from struct fields\n        │    field a = f(field b) → changing b makes a stale\n        │\n        └── The compiler cannot distinguish these cases\n             → uniform rule: many shared or one exclusive</code></div>'
+'<p>The uniform rule is a sync strategy for derived data in general. An element coordinate is derived from the Vec\'s buffer pointer. A struct field value can be derived from another field. A cache entry is derived from the data it caches. The compiler cannot know which types have these dependencies, so it prevents any source from changing while a coordinate derived from it exists.</p>';
}};

N.ch1_and_rule={ch:'The Approximation',t:'The Chapter 1 Programs and the Borrowing Rule',ctx:'The borrowing rule explained. Now see it applied to every rejected program from chapter 1.',
b:function(){return ''
+'<p><strong>Push while borrowing.</strong> <code>r</code> is a shared coordinate to the first element. <code>push</code> requires exclusive access to <code>v</code>. A shared coordinate and an exclusive coordinate to overlapping space exist at the same time.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> r = &amp;v[<span class="n">0</span>];\nv.<span class="m">push</span>(<span class="n">4</span>);\n<span class="m">println!</span>(<span class="s">"{}"</span>, r);</code></div>'
+'<div class="er"><b>error[E0502]:</b> cannot borrow `v` as mutable because it is also borrowed as immutable</div>'
+'<p><strong>Iterator invalidation.</strong> The <code>for</code> loop creates an iterator that holds a shared coordinate to <code>v</code> and a pointer into <code>v</code>\'s heap buffer. <code>push</code> requires exclusive access. The push can reallocate the buffer while the iterator reads from the old one.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">for</span> x <span class="k">in</span> &amp;v {\n    v.<span class="m">push</span>(*x);\n}</code></div>'
+'<div class="er"><b>error[E0502]:</b> cannot borrow `v` as mutable because it is also borrowed as immutable</div>'
+'<p><strong>Closure capturing.</strong> The closure captures <code>v</code> by exclusive coordinate because it calls <code>push</code>. <code>println!</code> requires a shared coordinate. The closure exists from creation to last use, and the <code>println!</code> falls within that range.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> closure = || v.<span class="m">push</span>(<span class="n">4</span>);\n<span class="m">println!</span>(<span class="s">"{:?}"</span>, v);\nclosure();</code></div>'
+'<div class="er"><b>error[E0502]:</b> cannot borrow `v` as immutable because it is also borrowed as mutable</div>'
+'<p><strong>Two exclusive coordinates.</strong> Two <code>&amp;mut</code> to the same <code>String</code>. Either one could call methods that restructure the internal buffer while the other depends on the old state.</p>'
+'<div class="cb"><code><span class="k">let mut</span> s = <span class="m">String</span>::from(<span class="s">"hello"</span>);\n<span class="k">let</span> r1 = &amp;<span class="k">mut</span> s;\n<span class="k">let</span> r2 = &amp;<span class="k">mut</span> s;\n<span class="m">println!</span>(<span class="s">"{}, {}"</span>, r1, r2);</code></div>'
+'<div class="er"><b>error[E0499]:</b> cannot borrow `s` as mutable more than once at a time</div>'
+'<p>Each program maps to the same pattern: a coordinate exists while an operation that could invalidate it is attempted. The '+EA('e6_14','thread safety errors from chapter 1')+' follow from the same rule.</p>';
}};

N.cpp_threads_rule={ch:'The Approximation',t:'How C++ Handles Multiple Coordinates',ctx:'Rust enforces the borrowing rule at compile time. What does C++ do?',
b:function(){return ''
+'<p>C++ has the same vocabulary of shared and exclusive access. <code>const T&amp;</code> grants read-only access. <code>T&amp;</code> grants read and write. C++ does not enforce the borrowing rule at compile time. The push-while-borrowing and iterator invalidation examples from chapter 1 compile in C++ without error and produce undefined behavior at runtime.</p>'
+'<div class="cb"><code><span class="c">// C++ — compiles, undefined behavior</span>\nstd::vector&lt;<span class="m">int</span>&gt; v = {<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>};\n<span class="m">int</span>&amp; r = v[<span class="n">0</span>];\nv.<span class="m">push_back</span>(<span class="n">4</span>);   <span class="c">// may reallocate</span>\nstd::cout &lt;&lt; r;   <span class="c">// r may point to freed memory</span></code></div>'
+'<p>The C++ standard says this is the programmer\'s responsibility. The compiler accepts the code. Whether the program crashes, prints garbage, or appears to work depends on the allocator\'s behavior at runtime.</p>'
+'<p>Parallel execution introduces a separate, independent reason for the same rule. A '+EA('e6_11','data race')+' requires parallel TIME, shared SPACE, and multiple COORDINATES where at least one writes. One thread reading an <code>i32</code> while another writes to it can produce a torn read, depending on the hardware. The <code>i32</code> has no managed internal space. Parallel access to the same space creates the danger regardless of whether any type manages internal allocations.</p>'
+'<p>The borrowing rule prevents both dangers with the same mechanism. Shared coordinates allow parallel reads because no writer can change the state they depend on. An exclusive coordinate prevents parallel access by ensuring no other coordinate exists. The rule protects managed space from restructuring in sequential code and prevents data races when execution is parallel.</p>'
+'<p>The approximation sometimes rejects valid code. The compiler\'s '+EA('e6_7_inline','tracking granularity')+' creates false rejections, and Rust provides several ways to work around that conservatism.</p>';
}};

// --- Within-chapter edges (choice cards) ---

ED.e6_1={from:'coord_dangle',to:'derived_sync',t:'Why is this a general pattern?',w:'Element coordinates are derived data. Any derived value becomes stale when its source changes.',card:true};
ED.e6_2={from:'derived_sync',to:'cant_distinguish',t:'Can the compiler tell which methods restructure?',w:'No. Whether push reallocates depends on runtime values. The compiler blocks all exclusive access.',card:true};
ED.e6_3={from:'cant_distinguish',to:'borrowing_rule_deep',t:'So what does the compiler actually block?',w:'Exclusive access while any coordinate exists. The remaining question is how long a coordinate exists.',card:true};
ED.e6_4={from:'borrowing_rule_deep',to:'rule_every_type',t:'Why does the rule apply to simple types too?',w:'The compiler cannot determine which types manage internal space. Uniform rule for all types.',card:true};
ED.e6_5={from:'rule_every_type',to:'ch1_and_rule',t:'What does this look like in practice?',w:'Every rejected program from chapter 1, explained by the borrowing rule.',card:true};
ED.e6_6={from:'ch1_and_rule',to:'cpp_threads_rule',t:'How does C++ handle this?',w:'C++ has shared and exclusive access but no enforcement. Same programs compile and produce undefined behavior.',card:true};
ED.e6_7={from:'cpp_threads_rule',to:'compiler_granularity',t:'When does the approximation reject valid code?',w:'The compiler\'s tracking granularity creates false rejections.',card:true};

// --- Cross-chapter inline edges ---

ED.e6_8={from:'coord_dangle',to:'rejected',t:'which ch1 error is this?',w:'Which chapter 1 error does this explain?'};
ED.e6_9={from:'cant_distinguish',to:'perfect_impossible',t:'why is this undecidable?',w:'Whether push reallocates depends on runtime values. No algorithm can decide this for all programs.'};
ED.e6_11={from:'cpp_threads_rule',to:'data_race',t:'what exactly is a data race?',w:'A data race occurs when two threads access the same memory, at least one writes, and no synchronization governs the access.'};
ED.e6_12={from:'borrowing_rule_deep',to:'shared_exclusive',t:'how do the two kinds of coordinates work?',w:'What can each kind of coordinate do?'};
ED.e6_13={from:'rule_every_type',to:'sound_complete',t:'why can\'t the compiler be more precise?',w:'A sound compiler never accepts unsafe code. A complete compiler never rejects safe code. No compiler can be both.'};
ED.e6_14={from:'ch1_and_rule',to:'thread_errors',t:'what about thread safety?',w:'The same borrowing rule prevents data races.'};

// Inline edge for final paragraph of cpp_threads_rule
ED.e6_7_inline={from:'cpp_threads_rule',to:'compiler_granularity',t:'what does tracking granularity mean?',w:'The compiler tracks at the level of entire bindings, not individual fields or elements.'};
