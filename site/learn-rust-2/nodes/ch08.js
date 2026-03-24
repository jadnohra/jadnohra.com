// ═══ CHAPTER 8: PROGRAMMING WITH LINEAR TYPES ═══

// --- Nodes ---

N.data_shape={ch:'Linear Types',t:'How Data Shape Determines the Pattern',ctx:'Ownership forms a tree. Not all data fits a tree.',
b:function(){return ''
+'<p>Ownership assigns a single parent to each value. Drop the parent, and the value goes with it. These edges form a hierarchy rooted at the call stack. A Vec owns its elements, a struct owns its fields. Data whose relationships fit this hierarchy needs no additional mechanism.</p>'
+'<p>Not all data fits. Several nodes in a graph can reach the same value, and the edges between them do not form a hierarchy. A doubly linked list has back pointers that are non-tree edges by construction. A HashMap can develop shared values depending on what the program inserts. The type system\'s vocabulary is tree-shaped. Non-tree edges exceed what it can express, and the programmer must choose a representation.</p>'
+'<p>The choice resolves friction along one or more of '+EA('e8_12','three dimensions')+'.</p>'
+'<p><strong>Multiple paths, one owner (SPACE).</strong> Several edges reach the same node, and at most one can be the ownership edge. Node A and node C both connect to node B. A owns B. B\'s space is reclaimed when A drops, regardless of whether C still needs it.</p>'
+'<p><strong>Coordinates across scopes (TIME).</strong> A coordinate from a longer-lived scope to a shorter-lived scope may outlive the space it reaches. The compiler ties the coordinate\'s validity to the owner\'s scope and rejects code where the coordinate might survive the owner.</p>'
+'<p><strong>Multiple coordinates, aliasing conflicts (COORDINATES).</strong> Multiple paths to the same value mean multiple coordinates, and the aliasing rule restricts what those coordinates can do simultaneously. A simulation struct that owns both a particle array and a spatial grid indexing into it has no freeing question and no lifetime question, but reading the grid and writing the particles simultaneously is an aliasing conflict because both coordinates reach the same struct.</p>'
+'<table><tr><th>Constraint</th><th>Relax it</th><th>Work around it</th></tr>'
+'<tr><td>Single owner (SPACE)</td><td>Shared Ownership (Rc/Arc)</td><td>Clone &amp; Diverge</td></tr>'
+'<tr><td>Scope-tied lifetime (TIME)</td><td><em>runtime tracking</em></td><td>Message Passing</td></tr>'
+'<tr><td>Aliasing rule (COORDS)</td><td>Interior Mutability</td><td>Flatten &amp; Index</td></tr>'
+'</table>'
+'<p>Split &amp; Partition resolves a different kind of conflict. The data is disjoint, but the compiler\'s approximation treats it as one unit. Splitting proves disjointness without relaxing any constraint.</p>';
}};

N.flatten_index={ch:'Linear Types',t:'Flatten and Index',ctx:'Replace pointers with integers. Exit the coordinate system entirely.',
b:function(){return ''
+'<p>Multiple paths to the same node require edges outside the ownership tree. A single collection owns all nodes, and those edges become indices: integers, freely copyable, with no lifetime tracking.</p>'
+'<div class="cb"><code><span class="k">struct</span> <span class="m">Graph</span> {\n    nodes: <span class="m">Vec</span>&lt;<span class="m">NodeData</span>&gt;,\n    edges: <span class="m">Vec</span>&lt;(<span class="m">usize</span>, <span class="m">usize</span>)&gt;,\n}</code></div>'
+'<p>The Vec owns the node data. Edges are pairs of indices. Adding an edge does not create a coordinate to any node. Removing a node invalidates indices, and the compiler cannot catch stale indices at compile time. Generational indices address this by pairing each index with a generation counter that increments when a slot is reused.</p>'
+'<div class="cb"><code><span class="k">struct</span> <span class="m">NodeId</span> {\n    index: <span class="m">usize</span>,\n    generation: <span class="m">u64</span>,\n}\n\n<span class="k">struct</span> <span class="m">Arena</span>&lt;T&gt; {\n    entries: <span class="m">Vec</span>&lt;(<span class="m">u64</span>, <span class="m">Option</span>&lt;T&gt;)&gt;,  <span class="c">// (generation, data)</span>\n}</code></div>'
+'<p>This is the only pattern that resolves all three dimensions simultaneously. The single collection centralizes SPACE in one allocation. All nodes live as long as the collection, making TIME uniform. Indices are Copy integers that the borrow checker does not track, so COORDINATES disappear entirely.</p>'
+'<p>The Rust ecosystem converges on this pattern for anything graph-shaped. The <code>petgraph</code> crate, the <code>slotmap</code> crate, and most game engines use this approach. Index validity becomes the programmer\'s responsibility, or a runtime check with generational indices. '+EA('e8_16','Escape hatch')+': restructure the code.</p>';
}};

N.shared_own={ch:'Linear Types',t:'Shared Ownership',ctx:'Multiple handles share ownership. The value lives until the last one drops.',
b:function(){return ''
+'<p>A value with several incoming edges has no single natural owner. Tying its lifetime to any one of them risks dropping it while others still hold a path. <code>Rc</code> and <code>Arc</code> let multiple handles share ownership, and the value lives until the last handle drops. '+EA('e8_15','Reference counting')+' replaces the single-owner constraint with a runtime counter.</p>'
+'<div class="cb"><code><span class="k">use</span> std::rc::<span class="m">Rc</span>;\n\n<span class="k">let</span> config = <span class="m">Rc</span>::new(<span class="m">AppConfig</span>::load());\n<span class="k">let</span> service_a = <span class="m">ServiceA</span>::new(<span class="m">Rc</span>::clone(&amp;config));\n<span class="k">let</span> service_b = <span class="m">ServiceB</span>::new(<span class="m">Rc</span>::clone(&amp;config));\n<span class="c">// config lives until all three handles drop</span></code></div>'
+'<p>The pattern works for DAGs and for immutable shared data where no single owner is natural. Cycles leak memory because each node in the cycle keeps the next alive. <code>Weak</code> breaks cycles by providing a non-owning handle that does not prevent deallocation.</p>'
+'<p><code>Rc</code> hands out shared coordinates, so writing to the data requires interior mutability. <code>Rc&lt;RefCell&lt;T&gt;&gt;</code> combines shared ownership with runtime-checked mutation. <code>Rc</code> relaxes the single-owner constraint on SPACE, and <code>RefCell</code> relaxes the aliasing rule on COORDINATES. Together they handle both dimensions.</p>'
+'<div class="cb"><code><span class="k">use</span> std::rc::<span class="m">Rc</span>;\n<span class="k">use</span> std::cell::<span class="m">RefCell</span>;\n\n<span class="k">let</span> shared = <span class="m">Rc</span>::new(<span class="m">RefCell</span>::new(<span class="m">Vec</span>::new()));\n<span class="k">let</span> handle = <span class="m">Rc</span>::clone(&amp;shared);\nhandle.borrow_mut().push(<span class="n">42</span>);</code></div>';
}};

N.interior_mut={ch:'Linear Types',t:'Interior Mutability',ctx:'Shared coordinate plus mutation. The aliasing rule says no. These types say yes, at runtime.',
b:function(){return ''
+'<p>The '+EA('e8_13','aliasing rule')+' guarantees that no code writes to a struct\'s fields while shared coordinates to the struct exist. A struct\'s methods sometimes need to write during shared access. The guarantee and the requirement contradict.</p>'
+'<div class="cb"><code><span class="c">// C++</span>\n<span class="k">class</span> <span class="m">Lookup</span> {\n    <span class="k">mutable</span> std::unordered_map&lt;<span class="m">int</span>, <span class="m">int</span>&gt; cache;\n<span class="k">public</span>:\n    <span class="m">int</span> get(<span class="m">int</span> key) <span class="k">const</span> {\n        <span class="k">if</span> (cache.count(key)) <span class="k">return</span> cache[key];\n        <span class="m">int</span> result = expensive(key);\n        cache[key] = result;\n        <span class="k">return</span> result;\n    }\n};</code></div>'
+'<p>C++ exempts <code>mutable</code> fields from <code>const</code> checking. The C++ optimizer does not assume <code>const</code> prevents writes, so the keyword works. Rust cannot do the same. The Rust compiler uses the aliasing guarantee to tell LLVM that data at a shared coordinate will not change. LLVM caches values in registers and skips re-reading from memory. A keyword that silently permitted writes would send the write to memory while the register holds the old value, and the program would read stale data.</p>'
+'<p><strong>UnsafeCell.</strong> The compiler primitive. The compiler recognizes any type containing an <code>UnsafeCell</code> and drops the no-write guarantee for that type\'s contents, so LLVM re-reads from memory on every access. Its <code>get</code> method returns a raw pointer. The caller must ensure no exclusive access overlaps with any other access.</p>'
+'<div class="cb"><code><span class="k">use</span> std::cell::<span class="m">Cell</span>;\n\n<span class="k">let</span> counter = <span class="m">Cell</span>::new(<span class="n">0</span>);\n<span class="k">let</span> a = &amp;counter;\n<span class="k">let</span> b = &amp;counter;\na.set(a.get() + <span class="n">1</span>);     <span class="c">// copies 0 out, computes 1, copies 1 in</span>\nb.set(b.get() + <span class="n">1</span>);     <span class="c">// copies 1 out, computes 2, copies 2 in</span>\n<span class="m">assert_eq!</span>(counter.get(), <span class="n">2</span>);</code></div>'
+'<p><strong>Cell.</strong> Never exposes a coordinate to the stored data. <code>.get()</code> copies the value out, <code>.set()</code> overwrites it. No caller ever holds a coordinate to the interior, so no caller can observe the data changing during access. Works for types that support <code>mem_copy</code>.</p>'
+'<div class="cb"><code><span class="k">use</span> std::cell::<span class="m">RefCell</span>;\n\n<span class="k">let</span> data = <span class="m">RefCell</span>::new(<span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>]);\n<span class="k">let</span> r1 = data.borrow();          <span class="c">// counter: 1</span>\n<span class="k">let</span> r2 = data.borrow();          <span class="c">// counter: 2</span>\ndrop(r1);                         <span class="c">// counter: 1</span>\ndrop(r2);                         <span class="c">// counter: 0</span>\n<span class="k">let mut</span> w = data.borrow_mut();   <span class="c">// counter: -1</span>\nw.push(<span class="n">4</span>);</code></div>'
+'<p><strong>RefCell.</strong> Maintains a runtime borrow counter. <code>.borrow()</code> returns a shared guard, <code>.borrow_mut()</code> returns an exclusive guard. Multiple shared guards can coexist. An exclusive guard panics if any other guard exists. The '+EA('e8_17','same rule')+' the compiler checks at compile time, enforced at runtime.</p>'
+'<div class="cb"><code><span class="c">// Mutex: same rule, across threads</span>\n<span class="k">use</span> std::sync::<span class="m">Mutex</span>;\n\n<span class="k">let</span> data = <span class="m">Mutex</span>::new(<span class="m">Vec</span>::new());\n<span class="k">let</span> guard = data.lock().unwrap();\n<span class="c">// guard holds exclusive access</span>\n<span class="c">// other threads block until guard drops</span></code></div>'
+'<p><strong>Mutex and RwLock.</strong> The same rule enforced across threads. <code>Mutex</code> blocks the calling thread until the lock is free, then returns an exclusive guard. <code>RwLock</code> allows many shared guards or one exclusive guard. Rust\'s <code>Mutex</code> owns the data it protects, so code cannot access the data without acquiring the lock.</p>';
}};

N.split_partition={ch:'Linear Types',t:'Split and Partition',ctx:'The data is disjoint. Prove it to the compiler.',
b:function(){return ''
+'<p>Two parts of the code need exclusive access to different parts of the same space. The compiler sees one coordinate to the whole and rejects the second. Splitting produces two coordinates to non-overlapping regions.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>, <span class="n">4</span>, <span class="n">5</span>, <span class="n">6</span>];\n<span class="k">let</span> (left, right) = v.split_at_mut(<span class="n">3</span>);\nleft[<span class="n">0</span>] = <span class="n">10</span>;\nright[<span class="n">0</span>] = <span class="n">40</span>;    <span class="c">// different region, no conflict</span></code></div>'
+'<p>Struct fields split automatically. The compiler knows that <code>person.name</code> and <code>person.age</code> occupy disjoint space.</p>'
+'<div class="cb"><code><span class="k">let</span> name = &amp;<span class="k">mut</span> person.name;\n<span class="k">let</span> age = &amp;<span class="k">mut</span> person.age;    <span class="c">// different field, compiler allows</span></code></div>'
+'<p>This pattern resolves conflicts where the data is genuinely disjoint but the compiler\'s approximation treats it as one unit. Signatures encode coordinate connections at struct granularity, and field-level information does not cross function boundaries. Splitting provides the field-level information the compiler needs. Escape hatch: encode invariants in types.</p>';
}};

N.clone_diverge={ch:'Linear Types',t:'Clone and Diverge',ctx:'Duplicate the data. Eliminate sharing entirely.',
b:function(){return ''
+'<p>Shared ownership and interior mutability add runtime overhead. Duplicating the data eliminates the sharing entirely and avoids both costs. Each copy is independent, and ownership is trivial because each binding owns its own space.</p>'
+'<div class="cb"><code><span class="k">let</span> original = expensive_to_build_config();\n<span class="k">let</span> for_thread_a = original.clone();\n<span class="k">let</span> for_thread_b = original.clone();</code></div>'
+'<p>Cloning trades memory and CPU time for simplicity. The copies diverge after creation, so changes to one do not affect the others. '+EA('e8_20','Move by default')+' prevents accidental duplication of managed types. Cloning is always explicit.</p>'
+'<p>The <code>Cow</code> type (clone on write) defers the clone until mutation is needed.</p>'
+'<div class="cb"><code><span class="k">use</span> std::borrow::<span class="m">Cow</span>;\n\n<span class="k">fn</span> <span class="m">process</span>(input: <span class="m">Cow</span>&lt;<span class="m">str</span>&gt;) -&gt; <span class="m">Cow</span>&lt;<span class="m">str</span>&gt; {\n    <span class="k">if</span> input.contains(<span class="s">"bad"</span>) {\n        <span class="m">Cow</span>::Owned(input.replace(<span class="s">"bad"</span>, <span class="s">"good"</span>))\n    } <span class="k">else</span> {\n        input    <span class="c">// no clone, original returned</span>\n    }\n}</code></div>';
}};

N.msg_passing={ch:'Linear Types',t:'Message Passing',ctx:'Threads share data by moving it. One owner at a time.',
b:function(){return ''
+'<p>Threads that need to operate on the same data face an aliasing conflict that no compile-time pattern resolves across thread boundaries. Channels serialize access across time by '+EA('e8_18','moving data')+' from sender to receiver, so that at any moment one thread owns the data.</p>'
+'<div class="cb"><code><span class="k">use</span> std::sync::<span class="m">mpsc</span>;\n\n<span class="k">let</span> (tx, rx) = <span class="m">mpsc</span>::channel();\n\nstd::thread::spawn(<span class="k">move</span> || {\n    <span class="k">let</span> data = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n    tx.send(data).unwrap();    <span class="c">// data moves to channel</span>\n    <span class="c">// data is invalid here</span>\n});\n\n<span class="k">let</span> received = rx.recv().unwrap();    <span class="c">// received owns the data</span></code></div>'
+'<p>The pattern restructures the program around data flow. Threads send and receive owned values. No thread shares mutable state. Sending data moves ownership from the sender to the receiver, a <code>take</code> in the notation vocabulary. The sender\'s binding becomes invalid, and the receiver\'s binding becomes the owner.</p>'
+'<p>Escape hatch: restructure the code.</p>';
}};

N.ds_linearity={ch:'Linear Types',t:'Data Structures Under Linearity',ctx:'Which pattern fits which data structure.',
b:function(){return ''
+'<p>Each pattern suits different data structures. The table maps common structures to the conflicts they create and the patterns that resolve them.</p>'
+'<table><tr><th>Data Structure</th><th>Conflict</th><th>Pattern</th></tr>'
+'<tr><td>Singly linked list</td><td>Works, but Vec is faster</td><td>Flatten and Index</td></tr>'
+'<tr><td>Doubly linked list</td><td>Back-pointers: two paths per node</td><td>Flatten and Index</td></tr>'
+'<tr><td>Graph (adjacency)</td><td>Cycles, shared nodes, multiple paths</td><td>Flatten and Index</td></tr>'
+'<tr><td>Tree with parent pointers</td><td>Parent pointer is a second path</td><td>Flatten and Index, or Weak</td></tr>'
+'<tr><td>Cache / memoization</td><td>Mutation behind shared access</td><td>Interior Mutability</td></tr>'
+'<tr><td>Observer / event system</td><td>Multiple observers coordinate to subject</td><td>Shared Ownership, or channels</td></tr>'
+'<tr><td>Self-referential struct</td><td>Coordinate points to own field</td><td>Pin, or restructure</td></tr>'
+'<tr><td>LRU cache</td><td>Hash map and recency list both reach same entry</td><td>Flatten and Index</td></tr>'
+'</table>'
+'<p>A singly linked list fits tree-shaped ownership, where each node owns the next. The borrow checker accepts it. The hardware punishes it. Linked list traversal scatters across memory, defeating cache prefetching. <code>Vec</code> stores elements contiguously, and modern CPUs process contiguous memory an order of magnitude faster than pointer-chased memory.</p>'
+'<p>Self-referential structs are a special case. A struct that contains a coordinate to its own field cannot be moved, because moving changes the address of the field while the coordinate still holds the old address. <code>Pin</code> prevents the struct from moving. Most Rust programmers restructure the design to eliminate the self-reference. '+EA('e8_21','Threads')+' add one more dimension of complexity to every entry in this table.</p>';
}};

N.algo_patterns={ch:'Linear Types',t:'Algorithm Patterns Under Linearity',ctx:'Data structure patterns adjust SPACE and COORDINATES. Algorithm patterns adjust TIME.',
b:function(){return ''
+'<p>The data structure patterns change how data is structured. Algorithm patterns change <em>when</em> operations happen so that coordinate lifetimes do not overlap. Five patterns appear repeatedly.</p>'
+'<p><strong>Separate read and write passes.</strong> An algorithm that reads and modifies a collection in the same loop creates an aliasing conflict. Separating into a read pass that collects information and a write pass that applies changes avoids it.</p>'
+'<div class="cb"><code><span class="c">// Read pass: shared coordinates</span>\n<span class="k">let</span> indices: <span class="m">Vec</span>&lt;<span class="m">usize</span>&gt; = data.iter()\n    .enumerate()\n    .filter(|(_, v)| v.should_update())\n    .map(|(i, _)| i)\n    .collect();\n\n<span class="c">// Write pass: exclusive coordinate</span>\n<span class="k">for</span> i <span class="k">in</span> indices {\n    data[i].update();\n}</code></div>'
+'<p><strong>Build new collections.</strong> Functional-style transformations sidestep borrow conflicts by producing new owned collections. The iterator holds a shared coordinate to the input for the entire chain. Nothing mutates the input.</p>'
+'<div class="cb"><code><span class="k">let</span> result: <span class="m">Vec</span>&lt;_&gt; = input.iter()\n    .filter(|x| x.is_valid())\n    .map(|x| x.transform())\n    .collect();</code></div>'
+'<p><strong>Index-based graph traversal.</strong> Graph algorithms use indices to traverse while holding a shared coordinate to the graph. The algorithm\'s own state (visited set, queue, result list) lives in separate owned space.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">bfs</span>(graph: &amp;<span class="m">Graph</span>, start: <span class="m">NodeId</span>) -&gt; <span class="m">Vec</span>&lt;<span class="m">NodeId</span>&gt; {\n    <span class="k">let mut</span> visited = <span class="m">HashSet</span>::new();\n    <span class="k">let mut</span> queue = <span class="m">VecDeque</span>::new();\n    queue.push_back(start);\n    visited.insert(start);\n    <span class="k">while let</span> Some(node) = queue.pop_front() {\n        <span class="k">for</span> &amp;neighbor <span class="k">in</span> &amp;graph.edges[node.index] {\n            <span class="k">if</span> visited.insert(neighbor) {\n                queue.push_back(neighbor);\n            }\n        }\n    }\n    visited.into_iter().collect()\n}</code></div>'
+'<p><strong>Drain and rebuild.</strong> <code>drain()</code> transfers ownership of each element from the collection to the loop body. The loop can move, modify, or drop each element without conflicting with the collection.</p>'
+'<p><strong>Swap to extract.</strong> <code>mem::swap</code>, <code>mem::replace</code>, and <code>mem::take</code> extract a value at an exclusive coordinate while leaving valid space behind. The owner always owns something after the operation.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">rotate_name</span>(person: &amp;<span class="k">mut</span> <span class="m">Person</span>) -&gt; <span class="m">String</span> {\n    <span class="k">let</span> old = std::mem::take(&amp;<span class="k">mut</span> person.name);  <span class="c">// leaves String::new()</span>\n    person.name = <span class="m">format!</span>(<span class="s">"former_{}"</span>, old);\n    old\n}</code></div>'
+'<p>'+EA('e8_19','Partitioning')+' works on the same principle: prove to the compiler that two accesses do not overlap.</p>';
}};

N.hints_patterns={ch:'Linear Types',t:'Hints and Patterns Correspond',ctx:'Hints are reactive. Patterns are proactive. Same adjustments.',
b:function(){return ''
+'<p>The hints from the previous chapter are the reactive version of the patterns here. The compiler rejected your code, and the hint tells you which dimension to adjust. The patterns here are the proactive version. You are designing a program, and the pattern tells you how to structure it so the compiler accepts it from the start.</p>'
+'<table><tr><th>Hint (reactive)</th><th>Pattern (proactive)</th><th>Dimension</th></tr>'
+'<tr><td>Duplication</td><td>Clone &amp; Diverge</td><td>SPACE</td></tr>'
+'<tr><td>Ownership Transfer</td><td>Message Passing</td><td>TIME</td></tr>'
+'<tr><td>Reordering</td><td>Separate Read/Write Passes</td><td>TIME</td></tr>'
+'<tr><td>Partitioning</td><td>Split &amp; Partition</td><td>COORDINATES</td></tr>'
+'<tr><td>Mutability Upgrade</td><td>Interior Mutability</td><td>COORDINATES</td></tr>'
+'</table>'
+'<p>A programmer who internalizes the patterns will recognize the hints when the compiler suggests them, because both are adjustments along the same three axes.</p>';
}};

N.when_unsafe={ch:'Linear Types',t:'When to Reach for Unsafe',ctx:'The six patterns cover most programs. Unsafe serves the remainder.',
b:function(){return ''
+'<p>The standard library uses <code>unsafe</code> internally for <code>Vec</code>, <code>HashMap</code>, <code>BTreeMap</code>, <code>String</code>, and many other types. The safe APIs that callers use are the product of the "encode invariants in types" '+EA('e8_14','escape hatch')+', where <code>unsafe</code> internals expose a safe public interface.</p>'
+'<p>Programs reach for <code>unsafe</code> when the safe patterns impose unacceptable performance costs, when a data structure\'s invariants are provably correct but inexpressible in the type system, at FFI boundaries where Rust\'s ownership model does not extend, and for intrusive data structures where nodes contain their own link pointers.</p>'
+'<p>The programmer manually asserts the S×T×C invariants that no safe pattern can express. The assertion covers the same three dimensions the compiler checks in safe code. SPACE requires the allocation to be valid. TIME requires the access to happen while the space exists. COORDINATES requires the references to satisfy the aliasing rule.</p>'
+'<p>The <code>unsafe</code> keyword marks these assertions as visible and searchable in the codebase, and the <code>miri</code> interpreter can check many of them at runtime during testing.</p>';
}};

// --- Edges ---

// Within-chapter choice cards
ED.e8_1={from:'data_shape',to:'flatten_index',t:'How do graphs and cycles work in Rust?',w:'Single collection owns all nodes. Edges become indices.',card:true};
ED.e8_2={from:'data_shape',to:'shared_own',t:'What if multiple things need to own the same value?',w:'Rc/Arc: shared handles, value lives until the last one drops.',card:true};
ED.e8_3={from:'flatten_index',to:'shared_own',t:'What if indices aren\'t practical?',w:'Reference counting gives shared ownership with runtime cost.',card:true};
ED.e8_4={from:'shared_own',to:'interior_mut',t:'How do I mutate shared data?',w:'Interior mutability moves the aliasing check to runtime.',card:true};
ED.e8_5={from:'interior_mut',to:'split_partition',t:'What if the data is actually disjoint?',w:'Split the data and prove disjointness to the compiler.',card:true};
ED.e8_6={from:'split_partition',to:'clone_diverge',t:'Can I just copy the data instead?',w:'Clone eliminates sharing entirely. Each copy independent.',card:true};
ED.e8_7={from:'clone_diverge',to:'msg_passing',t:'How do threads share data?',w:'Channels move data between threads. One owner at a time.',card:true};
ED.e8_8={from:'msg_passing',to:'ds_linearity',t:'Which patterns fit which data structures?',w:'A table mapping structures to conflicts and patterns.',card:true};
ED.e8_9={from:'ds_linearity',to:'algo_patterns',t:'What about algorithm patterns?',w:'Five patterns that adjust when operations happen.',card:true};
ED.e8_10={from:'algo_patterns',to:'hints_patterns',t:'How do patterns relate to the hints?',w:'Same adjustments, different direction. Reactive vs proactive.',card:true};
ED.e8_11={from:'hints_patterns',to:'when_unsafe',t:'When is none of this enough?',w:'Performance, inexpressible invariants, FFI, intrusive structures.',card:true};

// Cross-chapter inline edges
ED.e8_12={from:'data_shape',to:'primitives',t:'what are the three dimensions?',w:'What are the three dimensions of memory?'};
ED.e8_13={from:'interior_mut',to:'borrowing_rule_deep',t:'remind me of the aliasing rule',w:'What does the rule actually say?'};
ED.e8_14={from:'when_unsafe',to:'escape_hatches',t:'what are the escape hatches?',w:'Four options when the compiler rejects valid code: restructure the program, encode invariants in types, move checks to runtime, or use unsafe.'};
ED.e8_15={from:'shared_own',to:'features',t:'where does reference counting fit?',w:'How does reference counting map to the framework?'};
ED.e8_16={from:'flatten_index',to:'escape_hatches',t:'is this an escape hatch?',w:'Flattening data into a single collection and using indices is restructuring that makes the access pattern visible to the compiler.'};
ED.e8_17={from:'interior_mut',to:'cant_distinguish',t:'why can\'t the compiler handle this?',w:'Whether a method changes internal layout depends on runtime values. The compiler cannot determine this statically.'};
ED.e8_18={from:'msg_passing',to:'ownership_deep',t:'how does move work?',w:'What happens to the original when ownership transfers?'};
ED.e8_19={from:'algo_patterns',to:'hints_coords',t:'how does partitioning work?',w:'How do you get two exclusive references to different parts?'};
ED.e8_20={from:'clone_diverge',to:'move_default',t:'why can\'t I just copy?',w:'Move by default prevents accidental duplication of managed types.'};
ED.e8_21={from:'ds_linearity',to:'data_race',t:'why do threads complicate things?',w:'A data race needs shared data, parallel access, and at least one writer. The borrowing rule eliminates the combination.'};
