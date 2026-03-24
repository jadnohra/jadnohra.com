// ═══ CHAPTER 7: WHEN THE APPROXIMATION REJECTS VALID CODE ═══

// --- Nodes ---

N.compiler_granularity={ch:'Practical Guidance',t:'What the Compiler Can Distinguish',ctx:'The borrowing rule is an approximation. Where does it lose precision?',
b:function(){return ''
+'<p>The analysis tracks coordinates. The granularity of that tracking determines which simultaneous accesses the compiler can prove safe.</p>'
+'<p><strong>Struct fields.</strong> <code>p.x</code> and <code>p.y</code> are distinct. The type declaration places them at distinct offsets, and field names are literal in source text. The compiler can prove that coordinates to <code>p.x</code> and coordinates to <code>p.y</code> point to disjoint space.</p>'
+'<p><strong>Array elements with literal indices.</strong> <code>v[0]</code> and <code>v[1]</code> refer to different elements. The literal indices are visible in source text, so the compiler could distinguish them. Rust treats all index expressions as potentially overlapping. This is a design choice.</p>'
+'<p><strong>Array elements with computed indices.</strong> <code>v[i]</code> and <code>v[j]</code> where <code>i</code> and <code>j</code> come from computation. Whether <code>i == j</code> depends on runtime values. Proving it requires solving the halting problem.</p>'
+'<table><tr><th>Coordinates to</th><th>Distinguishable?</th><th>Why</th></tr>'
+'<tr><td><code>p.x</code> vs <code>p.y</code></td><td>Yes</td><td>Field names literal in source</td></tr>'
+'<tr><td><code>v[0]</code> vs <code>v[1]</code></td><td>Could be</td><td>Rust\'s design choice</td></tr>'
+'<tr><td><code>v[i]</code> vs <code>v[j]</code></td><td>No</td><td>May require solving halting problem</td></tr></table>'
+'<p>Branches and pointer aliasing share this structure. Whether <code>*p</code> and <code>*q</code> point to the same space depends on runtime values. The compiler assumes worst case for both. The compiler handles what source text '+EA('e7_8','names')+'. Computed values belong to data flow, and data flow is where undecidability enters.</p>';
}};

N.skip_decidable={ch:'Practical Guidance',t:'Why Compilers Skip Decidable Cases',ctx:'Literal indices are decidable. Why does the compiler still reject them?',
b:function(){return ''
+'<p>The literal index <code>v[0]</code> is decidable. So is a function call <code>pick(&amp;a, &amp;b, true)</code> where the argument is a constant. A human sees which branch executes. The compiler could too, using constant propagation and inlining.</p>'
+'<p>General-purpose compilers favor predictability. Code that compiles when the argument is <code>true</code> fails when <code>true</code> becomes <code>some_function()</code>, and programmers cannot predict which expressions cross the decidability boundary. Identifying which expressions fall into decidable subsets is itself expensive, and coverage is narrow.</p>'
+'<p>Specialized compilers do analyze decidable cases. Fortran compilers use polyhedral analysis for loop parallelization. ML frameworks require static tensor shapes. The techniques exist and work for specific domains.</p>'
+'<p>Rust chose consistent approximate analysis. The compiler treats all index expressions and all branch conditions the same way. A program that compiles with a constant will still compile when that constant becomes a variable. '+EA('e7_1','Where else')+' does the compiler lose information?</p>';
}};

N.sigs_lose_field={ch:'Practical Guidance',t:'Signatures Lose Field-Level Information',ctx:'The compiler distinguishes struct fields inside a body. What happens across a function call?',
b:function(){return ''
+'<p>Inside a function body, the compiler sees field-level coordinate access. <code>&amp;d.field1</code> and <code>&amp;d.field2</code> point to disjoint space, and the compiler knows it.</p>'
+'<div class="cb"><code><span class="k">let</span> r1 = &amp;d.field1;      <span class="c">// coordinates to d.field1</span>\n<span class="k">let</span> r2 = &amp;d.field2;      <span class="c">// coordinates to d.field2</span>\n                          <span class="c">// disjoint: yes</span></code></div>'
+'<p>Across a function call, only the '+EA('e7_10','signature')+' is visible. A method that borrows <code>self</code> borrows the whole struct.</p>'
+'<div class="cb"><code><span class="k">let</span> r1 = d.<span class="m">get_field1</span>(); <span class="c">// signature says: borrows d</span>\n<span class="k">let</span> r2 = d.<span class="m">get_field2</span>(); <span class="c">// signature says: borrows d</span>\n                          <span class="c">// disjoint: the compiler cannot tell</span></code></div>'
+'<p>Signatures encode coordinate connections at struct granularity. Field-level information does not cross function boundaries. Types describe whole structs, and connections are encoded at the type level. The compiler sees two borrows of <code>d</code> where a human sees two borrows of disjoint fields.</p>';
}};

N.escape_hatches={ch:'Practical Guidance',t:'Escape Hatches',ctx:'The compiler rejected valid code. What are the options?',
b:function(){return ''
+'<p>The '+EA('e7_12','borrowing rule')+' is sound. It never accepts unsafe code. It sometimes rejects safe code. The '+EA('e7_9','gap')+' between what is safe and what the compiler can prove safe creates a progression of responses, from least to most powerful.</p>'
+'<p><strong>Restructure the code.</strong> Make the safety visible to the graph. Move the <code>drop</code> after the last use of the reference. Call a function that takes only the input the caller needs. The code was safe, but the signature did not carry enough information for the compiler to verify it. Restructuring makes the safety visible.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">pick</span>&lt;<span class="k">\'a</span>&gt;(a: &amp;<span class="k">\'a</span> [<span class="m">i32</span>], b: &amp;<span class="k">\'a</span> [<span class="m">i32</span>], cond: <span class="m">bool</span>) -> &amp;<span class="k">\'a</span> <span class="m">i32</span> {\n    <span class="k">if</span> cond { &amp;a[<span class="n">0</span>] } <span class="k">else</span> { &amp;b[<span class="n">0</span>] }\n}\n<span class="k">let</span> a = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> b = <span class="m">vec!</span>[<span class="n">4</span>, <span class="n">5</span>, <span class="n">6</span>];\n<span class="k">let</span> r = <span class="m">pick</span>(&amp;a, &amp;b, <span class="k">true</span>);\n<span class="m">drop</span>(b);                  <span class="c">// rejected: r might borrow b</span>\n<span class="m">println!</span>(<span class="s">"{}"</span>, r);</code></div>'
+'<p>The caller knows <code>r</code> comes from <code>a</code> because the argument is <code>true</code>. The signature declares that <code>r</code> might borrow from either <code>a</code> or <code>b</code>. The compiler assumes <code>b</code> might be needed and rejects the <code>drop(b)</code> while <code>r</code> exists.</p>'
+'<p><strong>Encode invariants in types.</strong> The compiler cannot prove that <code>v[0..n]</code> and <code>v[n..len]</code> are disjoint space. A human sees the disjointness immediately. One range ends where the other begins.</p>'
+'<div class="cb"><code><span class="k">let</span> (left, right) = v.<span class="m">split_at_mut</span>(n);\n<span class="c">// left = v[0..n], right = v[n..len]</span>\n<span class="c">// disjoint by construction</span></code></div>'
+'<p>Inside <code>split_at_mut</code>, an <code><span class="k">unsafe</span></code> block asserts what the compiler cannot verify. The function computes two slices that are disjoint by construction, and the return type encodes the disjointness. The caller receives a safe interface. The proof obligation moved from the compiler to the function author.</p>'
+'<p><strong>Move verification to runtime.</strong> Static analysis faces undecidability because it must answer questions about all possible executions. At runtime, only one execution happens. Questions that were undecidable statically become trivial runtime checks.</p>'
+'<table><tr><th>Rust</th><th>C++ equivalent</th><th>What it tracks</th></tr>'
+'<tr><td><code>RefCell&lt;T&gt;</code></td><td>--</td><td>Active borrow count</td></tr>'
+'<tr><td><code>Mutex&lt;T&gt;</code></td><td><code>std::mutex</code> + <code>T</code></td><td>Lock state</td></tr>'
+'<tr><td><code>RwLock&lt;T&gt;</code></td><td><code>std::shared_mutex</code> + <code>T</code></td><td>Reader/writer count</td></tr>'
+'<tr><td><code>Rc&lt;T&gt;</code></td><td><code>std::shared_ptr&lt;T&gt;</code></td><td>Reference count</td></tr>'
+'<tr><td><code>Arc&lt;T&gt;</code></td><td><code>std::shared_ptr&lt;T&gt;</code> (atomic)</td><td>Reference count</td></tr></table>'
+'<p><strong>Cyclic structures.</strong> Ownership is tree-shaped. Each value has one owner, forming a hierarchy. Cyclic structures do not fit. A doubly-linked list node has <code>prev</code> and <code>next</code> coordinates to the same neighbor. Both cannot own the neighbor. The options are reference counting (<code>Rc</code> + <code>RefCell</code>), arenas that use indices as coordinates, or manual management with <code><span class="k">unsafe</span></code>.</p>';
}};

N.hints_space={ch:'Practical Guidance',t:'Hints About Space',ctx:'The compiler rejected code. The problem is about SPACE.',
b:function(){return ''
+'<p>The wrong binding owns the space, or earlier code '+EA('e7_13','moved')+' the space away. Two adjustments fix this.</p>'
+'<p><strong>Duplication.</strong> Two sites need the same data. One takes ownership, the other still needs the value. <code>clone()</code> allocates new space and copies the data. Deriving <code>Copy</code> makes the compiler duplicate automatically for small stack types. <code>Rc</code> and <code>Arc</code> share the space and track owners with a reference count.</p>'
+'<div class="cb"><code><span class="k">let</span> s = <span class="m">String</span>::from(<span class="s">"hello"</span>);\n<span class="k">let</span> t = s.<span class="m">clone</span>();           <span class="c">// new space, independent owner</span>\n<span class="m">consume</span>(s);\n<span class="m">println!</span>(<span class="s">"{}"</span>, t);           <span class="c">// t\'s space is still alive</span></code></div>'
+'<p><strong>Ownership transfer.</strong> The space exists in the wrong place. A closure needs to own a value from the enclosing scope. A function needs to return data it created. <code>mem::replace</code> and <code>mem::take</code> transfer at an exclusive coordinate. They swap the contents and leave valid space behind.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">take_name</span>(person: &amp;<span class="k">mut</span> <span class="m">Person</span>) -> <span class="m">String</span> {\n    std::mem::<span class="m">take</span>(&amp;<span class="k">mut</span> person.name)  <span class="c">// extracts name, leaves String::new()</span>\n}\n\n<span class="k">let</span> s = <span class="m">String</span>::from(<span class="s">"hello"</span>);\n<span class="k">let</span> closure = <span class="k">move</span> || <span class="m">println!</span>(<span class="s">"{}"</span>, s);  <span class="c">// s moves into the closure</span></code></div>';
}};

N.hints_time={ch:'Practical Guidance',t:'Hints About Time',ctx:'The compiler rejected code. The problem is about TIME.',
b:function(){return ''
+'<p>Coordinate lifetimes overlap when they should not, or the compiler cannot see when a coordinate\'s target space is reclaimed. Two adjustments fix this. Every hint adjusts one of the '+EA('e7_14','three dimensions')+'.</p>'
+'<p><strong>Reordering.</strong> The fix reorders operations so the first coordinate\'s lifetime ends before the second begins. Scoping with <code>{}</code> achieves the same result. Binding a temporary to a <code>let</code> extends its lifetime so coordinates to that space survive longer.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> first = v[<span class="n">0</span>];         <span class="c">// read the value, coordinate ends</span>\nv.<span class="m">push</span>(<span class="n">4</span>);                <span class="c">// exclusive access, no conflict</span>\n<span class="m">println!</span>(<span class="s">"{}"</span>, first);    <span class="c">// first is a copy, no coordinate</span></code></div>'
+'<p><strong>Scope declarations.</strong> The compiler cannot verify that a coordinate\'s lifetime fits inside its target space\'s lifetime because the relationship is invisible. Lifetime annotations make it explicit.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">first</span>&lt;<span class="k">\'a</span>&gt;(list: &amp;<span class="k">\'a</span> [<span class="m">i32</span>]) -> &amp;<span class="k">\'a</span> <span class="m">i32</span> {\n    &amp;list[<span class="n">0</span>]\n}</code></div>'
+'<p>The <code>\'a</code> label on both the input and the output tells the compiler that the output coordinate points into the input\'s space. Adding <code>T: \'a</code> to a generic parameter and adding <code>\'a: \'b</code> to declare containment are the same hint at different sites.</p>';
}};

N.hints_coords={ch:'Practical Guidance',t:'Hints About Coordinates',ctx:'The compiler rejected code. The problem is about COORDINATES.',
b:function(){return ''
+'<p>The code requests the wrong kind of coordinate, or uses ownership where a coordinate would suffice. Three adjustments fix this.</p>'
+'<p><strong>Borrowing.</strong> The code transfers ownership where a coordinate would suffice. Pass <code>&amp;</code> instead of consuming the value. The caller keeps ownership.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">print_len</span>(s: &amp;<span class="m">String</span>) {\n    <span class="m">println!</span>(<span class="s">"{}"</span>, s.<span class="m">len</span>());\n}\n<span class="k">let</span> s = <span class="m">String</span>::from(<span class="s">"hello"</span>);\n<span class="m">print_len</span>(&amp;s);                <span class="c">// coordinate to s, ownership stays</span>\n<span class="m">println!</span>(<span class="s">"{}"</span>, s);            <span class="c">// s still alive</span></code></div>'
+'<p><strong>Partitioning.</strong> Two parts of the code need exclusive access to different parts of the same space. The compiler sees one coordinate to the whole and rejects the second. <code>split_at_mut</code> produces two exclusive coordinates to non-overlapping regions. Binding struct fields individually before a closure achieves the same at the field level.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>, <span class="n">4</span>];\n<span class="k">let</span> (left, right) = v.<span class="m">split_at_mut</span>(<span class="n">2</span>);\nleft[<span class="n">0</span>] = <span class="n">10</span>;\nright[<span class="n">0</span>] = <span class="n">30</span>;\n\n<span class="k">let</span> name = &amp;<span class="k">mut</span> person.name;\n<span class="k">let</span> age = &amp;<span class="k">mut</span> person.age;    <span class="c">// different field, no conflict</span></code></div>'
+'<p><strong>Mutability upgrade.</strong> The code requests a '+EA('e7_11','shared coordinate')+' where it needs an exclusive one, or the binding lacks <code><span class="k">mut</span></code>. <code>.iter()</code> produces shared coordinates. <code>.iter_mut()</code> produces exclusive ones. Adding <code><span class="k">mut</span></code> to a binding, changing <code>&amp;</code> to <code>&amp;<span class="k">mut</span></code>, and using <code>.get_mut()</code> on a map are the same hint at different sites.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">for</span> item <span class="k">in</span> v.<span class="m">iter_mut</span>() {    <span class="c">// exclusive coordinates to each element</span>\n    *item += <span class="n">1</span>;\n}</code></div>'
+'<p>The seven hints across SPACE, TIME, and COORDINATES are reactive. The compiler rejected code, and the hint tells you which dimension to adjust. The '+EA('e7_7','proactive counterpart')+' is structuring programs so the compiler accepts them from the start.</p>';
}};

// --- Within-chapter edges (choice cards) ---

ED.e7_1={from:'compiler_granularity',to:'skip_decidable',t:'Why doesn\'t the compiler handle the decidable cases?',w:'Predictability: code that compiles with a constant still compiles when it becomes a variable.',card:true};
ED.e7_2={from:'skip_decidable',to:'sigs_lose_field',t:'Where else does the compiler lose information?',w:'Signatures encode at struct granularity. Field-level info does not cross function boundaries.',card:true};
ED.e7_3={from:'sigs_lose_field',to:'escape_hatches',t:'What do I do when the compiler rejects valid code?',w:'Restructure, encode invariants in types, or move verification to runtime.',card:true};
ED.e7_4={from:'escape_hatches',to:'hints_space',t:'What adjustments work for SPACE problems?',w:'Duplication (clone, Copy, Rc) or ownership transfer (move, mem::take).',card:true};
ED.e7_5={from:'hints_space',to:'hints_time',t:'What about TIME problems?',w:'Reorder operations or declare scope relationships with lifetime annotations.',card:true};
ED.e7_6={from:'hints_time',to:'hints_coords',t:'What about COORDINATE problems?',w:'Borrow instead of owning, partition disjoint regions, or upgrade mutability.',card:true};
ED.e7_7={from:'hints_coords',to:'data_shape',t:'How do I structure programs proactively?',w:'Data shape determines the pattern.',card:true};

// --- Cross-chapter inline edges ---

ED.e7_8={from:'compiler_granularity',to:'names_exact',t:'where does the approximation fall?',w:'The compiler handles names and scope boundaries exactly. Data flow between scopes is where the approximation enters.'};
ED.e7_9={from:'escape_hatches',to:'sound_complete',t:'why does the compiler reject valid code?',w:'Rust chose soundness: the compiler never accepts unsafe code, but sometimes rejects code that is actually safe.'};
ED.e7_10={from:'sigs_lose_field',to:'sigs_lifetimes',t:'how do signatures work?',w:'Lifetime annotations declare which input a borrowed output connects to.'};
ED.e7_11={from:'hints_coords',to:'shared_exclusive',t:'what are the two kinds of coordinates?',w:'What can each kind of coordinate do?'};
ED.e7_12={from:'escape_hatches',to:'borrowing_rule_deep',t:'remind me of the rule',w:'What does the rule actually say?'};
ED.e7_13={from:'hints_space',to:'move_default',t:'why does assignment invalidate the source?',w:'Byte-copying a String would duplicate the heap pointer, creating two owners for one allocation. Move-by-default prevents this.'};
ED.e7_14={from:'hints_time',to:'primitives',t:'what are the three dimensions?',w:'Which dimension does each hint adjust?'};
