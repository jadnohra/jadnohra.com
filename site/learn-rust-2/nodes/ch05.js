// ═══ CHAPTER 5: HOW RUST PATCHES THE OBSTACLES ═══

// --- Nodes ---

N.constrained_refs={ch:'Rust\'s Patches',t:'Constrained Coordinates',ctx:'RAM is flat. Pointers are numbers. How does Rust fix this?',
b:function(){return ''
+'<p>In C++, a pointer is a number that happens to serve as an address. The language provides operations on numbers, and pointers inherit them. You can cast an integer to a pointer, perform arithmetic to compute new addresses, erase type information through <code>void*</code>, and store pointers in integer variables. Each operation severs the connection between the coordinate and its origin. The compiler cannot trace where the pointer points.</p>'
+'<div class="cb"><code><span class="c">// C++</span>\n<span class="m">int</span>* p = (<span class="m">int</span>*)<span class="n">0x1234</span>;           <span class="c">// coordinate from integer</span>\n<span class="m">int</span>* q = p + <span class="n">5</span>;                  <span class="c">// coordinate from arithmetic</span>\n<span class="m">void</span>* v = p;                     <span class="c">// type erased</span>\n<span class="m">uintptr_t</span> n = (<span class="m">uintptr_t</span>)p;      <span class="c">// stored as integer</span></code></div>'
+'<p>Rust references are not numbers. A reference is a typed relationship between a binding and a target. The language provides one way to create a reference: the <code>&amp;</code> operator applied to an existing value. There is no syntax for creating a reference from an integer. There is no reference arithmetic. There is no type erasure for references.</p>'
+'<div class="cb"><code><span class="k">let</span> x = <span class="n">5</span>;\n<span class="k">let</span> r = &amp;x;              <span class="c">// r points to x, compiler knows this</span></code></div>'
+'<p>When the compiler sees <code>&amp;x</code>, it records that the resulting reference points to <code>x</code>. Every reference in the program has a known origin. The compiler can trace any reference back to the space it refers to.</p>'
+'<table><tr><th>C++ allows</th><th>Rust equivalent</th><th>Why it matters</th></tr>'
+'<tr><td><code>(int*)0x1234</code></td><td>Does not exist</td><td>Every reference has a known target</td></tr>'
+'<tr><td><code>p + 5</code></td><td>Does not exist</td><td>References cannot drift to unknown locations</td></tr>'
+'<tr><td><code>void* v = p</code></td><td>Does not exist for references</td><td>Type information preserved</td></tr>'
+'<tr><td><code>(uintptr_t)p</code></td><td>Does not exist for references</td><td>References cannot become integers</td></tr>'
+'</table>'
+'<p>Raw pointers in Rust (<code>*const T</code>, <code>*mut T</code>) allow some of these operations, but dereferencing a raw pointer requires <code><span class="k">unsafe</span></code> code. The borrow checker analyzes safe Rust, where all coordinates are references with known origins. This patches the '+EA('e5_12','obstacle that RAM is flat')+': constrained references ensure every coordinate has a known origin on the graph.</p>';
}};

N.ownership_deep={ch:'Rust\'s Patches',t:'Ownership',ctx:'Heap space has no lexical structure. How does the compiler know when it dies?',
b:function(){return ''
+'<p>Stack space is reclaimed when scopes end, and the compiler sees scope boundaries in the source text. Heap space in C++ has unpredictable lifetimes. A <code>new</code> expression allocates, and that space lives until some code calls <code>delete</code>. The coordinate to that space can be copied, passed to functions, and stored in data structures. Any code holding the coordinate can free the space. Any code holding a copy can use it afterward and find the space gone. Nothing in the source text tells the compiler when heap space will die.</p>'
+'<p>Rust makes heap lifetimes predictable by assigning exactly one '+'owner'+' to each allocation. The owner is a binding, and that binding has a scope. Rust deallocates the owned space when the owner\'s scope ends. Heap lifetime follows owner scope, the same way stack lifetime follows function scope.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">example</span>() {\n    <span class="k">let</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];    <span class="c">// v owns heap space</span>\n    <span class="c">// ... use v ...</span>\n}                              <span class="c">// v\'s scope ends, heap space freed</span></code></div>'
+'<p>Ownership can transfer from one binding to another. This is a move. After the move, the original binding is no longer valid, and the new binding is the owner.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">example</span>() {\n    <span class="k">let</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];    <span class="c">// v owns</span>\n    <span class="k">let</span> w = v;                 <span class="c">// ownership moves to w, v now invalid</span>\n    <span class="c">// ... use w ...</span>\n}                              <span class="c">// w\'s scope ends, heap space freed</span></code></div>'
+'<p>At any moment, exactly one binding owns each allocation. The compiler tracks ownership through assignments and function calls. This transforms the analysis: without ownership, heap space can be reclaimed at any point where code calls <code>free</code> or <code>delete</code>. Ownership ties heap reclamation to scope boundaries, which the compiler already sees. Heap behaves like stack for the purposes of lifetime analysis.</p>';
}};

N.notation={ch:'Rust\'s Patches',t:'The Notation',ctx:'Rust syntax hides which binding is owner vs alias. How do you see what is really happening?',
b:function(){return ''
+'<p>Rust\'s assignment syntax hides which kind of binding a <code><span class="k">let</span></code> produces. <code><span class="k">let</span> result = create()</code> and <code><span class="k">let</span> elem = first(&amp;list)</code> look identical. One produces an owner. The other produces an alias. The language hides these distinctions deliberately. A learner building the model for the first time does not have the experience to recognize what the syntax conceals.</p>'
+'<p>The course includes a companion crate called <code>notation</code> with a macro called <code>explicit!</code>. Inside the macro, every operation declares what it does.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(s) = take(<span class="m">String</span>::from(<span class="s">"hello"</span>));       <span class="c">// s takes ownership</span>\n    <span class="k">let</span> owner(t) = take(s);                           <span class="c">// t takes from s, s invalid</span>\n    <span class="k">let</span> alias(r) = coord_exclusive(t);                 <span class="c">// r is a coordinate to t\'s space</span>\n    <span class="m">println!</span>(<span class="s">"{}"</span>, at(r));                             <span class="c">// follow the coordinate</span>\n}</code></div>'
+'<p>The left side of each <code><span class="k">let</span></code> declares the kind of '+EA('e5_17','binding')+'. <code>owner</code> means the binding controls when space is reclaimed. <code>alias</code> means the binding holds a coordinate to space owned elsewhere. The right side declares what happens. <code>take</code> means ownership transfers; an existing binding passed as the argument becomes invalid. <code>coord_exclusive</code> creates an exclusive coordinate. <code>at</code> follows a coordinate to its target.</p>'
+'<p>The vocabulary maps to the framework from Chapter 2. <code>owner</code> tracks SPACE x TIME. <code>alias</code> tracks COORDINATES. <code>take</code> is an ownership transfer in TIME. <code>coord_exclusive</code> creates a COORDINATE with exclusive access. Coordinates come in two kinds: <code>coord_exclusive</code> grants read and write access, and <code>coord_shared</code> grants read access while allowing multiple coordinates to coexist.</p>';
}};

N.sigs_lifetimes={ch:'Rust\'s Patches',t:'Signatures and Lifetime Annotations',ctx:'Data flow crosses scope boundaries. How does the compiler track it?',
b:function(){return ''
+'<p>The '+EA('e5_14','third obstacle')+' is that data flow crosses scope boundaries. Constrained coordinates and ownership make both questions answerable within a single function. Separate compilation limits this. The compiler processes each function independently. It sees the signature of each function it calls but not the body. A caller has no access to the body of a function that returns a reference and cannot determine where the reference points.</p>'
+'<p>Function signatures solve this. The signature declares, for each output, what kind of thing the caller receives. An owned output returns new space to the caller. A value output returns a self-contained value. A borrowed output returns a reference that points into space owned by one of the inputs.</p>'
+'<div class="cb"><code><span class="c">// Owned output: caller receives ownership</span>\n<span class="k">fn</span> <span class="m">create</span>() -&gt; <span class="m">Vec</span>&lt;<span class="m">i32</span>&gt; {\n    <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>]\n}\n\n<span class="c">// Value output: self-contained</span>\n<span class="k">fn</span> <span class="m">length</span>(list: &amp;[<span class="m">i32</span>]) -&gt; <span class="m">usize</span> {\n    list.<span class="m">len</span>()\n}\n\n<span class="c">// Borrowed output: points into input space</span>\n<span class="k">fn</span> <span class="m">first</span>(list: &amp;[<span class="m">i32</span>]) -&gt; &amp;<span class="m">i32</span> {\n    &amp;list[<span class="n">0</span>]\n}</code></div>'
+'<p>Borrowed outputs require the signature to declare which input the output connects to. This declaration is what Rust calls a '+'lifetime annotation'+'. The <code>\'a</code> label links an output to its input. The same label on an input and an output means "the output borrows from this input."</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">first</span>&lt;<span class="k">\'a</span>&gt;(list: &amp;<span class="k">\'a</span> [<span class="m">i32</span>]) -&gt; &amp;<span class="k">\'a</span> <span class="m">i32</span>\n<span class="c">//                  ^^              ^^  same label: output borrows from list</span>\n\n<span class="k">fn</span> <span class="m">search</span>&lt;<span class="k">\'a</span>, <span class="k">\'b</span>&gt;(map: &amp;<span class="k">\'a</span> <span class="m">Map</span>, key: &amp;<span class="k">\'b</span> K) -&gt; &amp;<span class="k">\'a</span> V\n<span class="c">//                      ^^                       ^^  output borrows from map, not key</span>\n\n<span class="k">fn</span> <span class="m">either</span>&lt;<span class="k">\'a</span>&gt;(a: &amp;<span class="k">\'a</span> <span class="m">i32</span>, b: &amp;<span class="k">\'a</span> <span class="m">i32</span>, flag: <span class="m">bool</span>) -&gt; &amp;<span class="k">\'a</span> <span class="m">i32</span>\n<span class="c">//                ^^           ^^                      ^^  output borrows from both</span></code></div>'
+'<p>C++ carries no information about where a returned reference points. The Rust signatures encode it.</p>'
+'<table><tr><th>C++ signature</th><th>What Rust adds</th></tr>'
+'<tr><td><code>const int&amp; first(const vector&lt;int&gt;&amp;)</code></td><td>Output borrows from <code>list</code></td></tr>'
+'<tr><td><code>const V&amp; search(const Map&amp;, const K&amp;)</code></td><td>Output borrows from <code>map</code>, not <code>key</code></td></tr>'
+'<tr><td><code>const int&amp; either(const int&amp;, const int&amp;, bool)</code></td><td>Output borrows from both</td></tr>'
+'</table>'
+'<p>The compiler verifies both directions. Given the function body, it checks that the implementation returns a reference derived from the declared input. Given the call site, it checks that the caller keeps the input alive while using the output. Function pointers, trait objects, and cross-crate calls all work from the signature alone.</p>';
}};

N.desugaring={ch:'Rust\'s Patches',t:'Desugaring and Lifetime Elision',ctx:'How do operators like v[0] connect to lifetimes?',
b:function(){return ''
+'<p>Rust desugars operator syntax to trait method calls. <code>v[<span class="n">0</span>]</code> desugars to <code>*v.index(<span class="n">0</span>)</code>, and <code>Index::index</code> has this signature:</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">index</span>(&amp;<span class="k">self</span>, index: <span class="m">usize</span>) -&gt; &amp;Self::<span class="m">Output</span>\n\n<span class="c">// With lifetime written out:</span>\n<span class="k">fn</span> <span class="m">index</span>&lt;<span class="k">\'a</span>&gt;(&amp;<span class="k">\'a</span> <span class="k">self</span>, index: <span class="m">usize</span>) -&gt; &amp;<span class="k">\'a</span> Self::<span class="m">Output</span></code></div>'
+'<p>The function takes a <code>&amp;<span class="m">Vec</span>&lt;<span class="m">i32</span>&gt;</code> and returns a <code>&amp;<span class="m">i32</span></code>. One coordinate goes in, one coordinate comes out. The shared <code>\'a</code> declares that the returned element coordinate is valid only while the input Vec coordinate is valid. The compiler reads this temporal dependency from the signature and enforces it at every call site.</p>'
+'<p>Rust calls this inference <em>lifetime elision</em>. A function with one reference input and one reference output infers the connection. A method on <code>&amp;<span class="k">self</span></code> or <code>&amp;<span class="k">mut</span> <span class="k">self</span></code> infers the output borrows from <code><span class="k">self</span></code>. The programmer writes explicit <code>\'a</code> labels only when the inference is ambiguous: multiple reference inputs, no <code><span class="k">self</span></code>, or the output needs to declare which specific input it borrows from.</p>';
}};

N.using_managing={ch:'Rust\'s Patches',t:'Using Data, Managing Space',ctx:'What can a coordinate actually do?',
b:function(){return ''
+'<p>An <code>owner</code> controls when '+EA('e5_18','space')+' is reclaimed. An <code>alias</code> holds a coordinate to space owned elsewhere. Using the data and managing the space are different operations, and Rust enforces the difference.</p>'
+'<p>A coordinate, even an exclusive one, lets code read and write the target data. It cannot free the space, move ownership out, or replace the allocation. Only the <code>owner</code> can manage the space.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(b) = take(<span class="m">Box</span>::new(<span class="m">String</span>::from(<span class="s">"hello"</span>)));  <span class="c">// b owns heap space</span>\n    <span class="k">let</span> alias(r) = coord_exclusive(b);                      <span class="c">// r can read and write</span>\n    <span class="c">// take(at(r)) would be invalid: r is an alias, aliases cannot own</span>\n}</code></div>'
+'<p><code>take(b)</code> works because <code>b</code> is an <code>owner</code>. The compiler forbids <code>take</code> when the argument is an <code>alias</code>. A function that receives an <code>alias</code> parameter cannot call <code>take</code> on it, cannot destroy the space, cannot move ownership away. The caller knows that after the function returns, the space still exists. The function accessed the data. The <code>owner</code> kept control of the space.</p>'
+'<table><tr><th>Operation</th><th>C++ via <code>unique_ptr&lt;string&gt;&amp;</code></th><th>Rust via <code>alias(r) = coord_exclusive(b)</code></th></tr>'
+'<tr><td>Read value</td><td><code>*ref</code></td><td><code>at(at(r))</code></td></tr>'
+'<tr><td>Write value</td><td><code>*ref = v</code></td><td><code>at(at(r)) = v</code></td></tr>'
+'<tr><td>Call methods</td><td><code>ref-&gt;method()</code></td><td><code>at(r).method()</code></td></tr>'
+'<tr><td>Move out</td><td><code>auto s = move(*ref)</code></td><td><code>take(at(r))</code> <strong>forbidden</strong></td></tr>'
+'<tr><td>Destroy space</td><td><code>ref.reset()</code></td><td><strong>forbidden</strong></td></tr>'
+'</table>'
+'<p>The first three rows use data. The last two manage space. C++ allows a coordinate to manage the space. Rust does not. This distinction makes the compiler\'s analysis airtight: signatures that declare <code>alias</code> connections are telling the truth in full, because an <code>alias</code> cannot do more than use the data.</p>';
}};

N.move_default={ch:'Rust\'s Patches',t:'Why Rust Moves by Default',ctx:'Why does assignment invalidate the source?',
b:function(){return ''
+'<p>The '+EA('e5_16','fourth obstacle')+' is invisible coordinate duplication. Byte-copying a <code><span class="m">String</span></code> duplicates the heap pointer, creating two owners of one space. Rust prevents this by making assignment transfer ownership rather than copy bytes.</p>'
+'<p><code><span class="k">let</span> t = s</code> moves ownership to <code>t</code>, and <code>s</code> becomes invalid. The compiler does not know whether a type\'s bytes contain coordinates to managed space. Moving is the safe default.</p>'
+'<p>Types that contain no coordinates to managed space opt into byte duplication by implementing the <code><span class="m">Copy</span></code> trait. <code><span class="m">i32</span></code>, <code><span class="m">bool</span></code>, <code><span class="m">f64</span></code>, <code><span class="m">char</span></code>, and tuples of Copy types are all Copy. <code><span class="m">String</span></code>, <code><span class="m">Vec</span></code>, and any type that manages heap space are not Copy.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(x) = take(<span class="n">5</span>);\n    <span class="k">let</span> owner(y) = mem_copy(x);       <span class="c">// duplicate the bytes</span>\n    <span class="m">println!</span>(<span class="s">"x = {}, y = {}"</span>, x, y); <span class="c">// both valid</span>\n}</code></div>'
+'<p><code><span class="k">let</span> y = x</code> does not say whether it copies or moves. The compiler decides based on the type. The notation surfaces this decision:</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(x1) = take(<span class="n">5</span>);\n    <span class="k">let</span> owner(y1) = take_or_mem_copy(x1);         <span class="c">// i32 is Copy: mem_copy</span>\n    <span class="m">println!</span>(<span class="s">"x1 = {}, y1 = {}"</span>, x1, y1);         <span class="c">// both valid</span>\n\n    <span class="k">let</span> owner(x2) = take(<span class="m">String</span>::from(<span class="s">"hello"</span>));\n    <span class="k">let</span> owner(y2) = take_or_mem_copy(x2);          <span class="c">// String is not Copy: take</span>\n    <span class="m">println!</span>(<span class="s">"y2 = {}"</span>, y2);\n    <span class="c">// x2 is invalid here</span>\n}</code></div>'
+'<p><code>take_or_mem_copy</code> makes the compiler\'s decision visible. Standard Rust writes both operations as <code><span class="k">let</span> y = x</code>, and the same syntax produces different behavior depending on the type. For programs that need a full independent copy of a non-Copy type, <code>.clone()</code> allocates new space and copies the data.</p>';
}};

N.struct_lifetimes={ch:'Rust\'s Patches',t:'Struct Lifetime Annotations',ctx:'What about structs that hold references?',
b:function(){return ''
+'<p>A struct can hold a coordinate to space owned by a different binding. That coordinate is derived data, and the struct depends on the external space staying alive. Rust requires the struct to declare this dependency. The <code>\'a</code> on the struct states that it contains a coordinate that must not outlive space with lifetime <code>\'a</code>.</p>'
+'<div class="cb"><code><span class="k">struct</span> <span class="m">Viewer</span>&lt;<span class="k">\'a</span>&gt; {\n    data: &amp;<span class="k">\'a</span> [<span class="m">i32</span>],\n}</code></div>'
+'<p>The compiler uses <code>\'a</code> to verify that any code creating a <code><span class="m">Viewer</span></code> keeps the referenced space alive long enough. The rejection looks like this:</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> viewer;\n    {\n        <span class="k">let</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n        viewer = <span class="m">Viewer</span> { data: &amp;v };\n    }                                     <span class="c">// v\'s space reclaimed here</span>\n    <span class="m">println!</span>(<span class="s">"{:?}"</span>, viewer.data);        <span class="c">// REJECTED: viewer outlives v</span>\n}</code></div>'
+'<div class="er"><b>error[E0597]:</b> `v` does not live long enough</div>'
+'<p>Function lifetime annotations declare that an output holds a coordinate to an input\'s space. Struct lifetime annotations declare that the struct holds a coordinate to external space. Both tell the compiler which space a coordinate depends on. The solution is the same: the <code>\'a</code> annotation declares the cross-scope dependency.</p>';
}};

N.mut_binding={ch:'Rust\'s Patches',t:'What mut Means on a Binding',ctx:'Rebinding and writing through a coordinate are different operations.',
b:function(){return ''
+'<p>Every binding so far has been fixed for its scope. A rebindable binding can point to a different value or a different target. The notation writes this as <code>rebindable</code>. Standard Rust writes <code><span class="k">let</span> <span class="k">mut</span></code>.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(rebindable(x)) = take(<span class="m">String</span>::from(<span class="s">"hello"</span>));\n    x = take(<span class="m">String</span>::from(<span class="s">"world"</span>));    <span class="c">// x now holds different space</span>\n    <span class="m">println!</span>(<span class="s">"{}"</span>, at(x));              <span class="c">// prints "world"</span>\n}</code></div>'
+'<p>Reassigning <code>x</code> frees the old space, because <code>x</code> was its owner and the owner has moved to different space. Rebinding changes the binding itself, pointing it to a new target. Changing data at a coordinate modifies the contents of the target without changing what the binding points to. These are different operations.</p>'
+'<p>C++ uses <code>const</code> for both purposes: <code>const int x = 5</code> prevents reassignment, <code>const int&amp; r = x</code> prevents writing through the reference. Rust separates them. <code><span class="k">mut</span></code> on the binding controls reassignment. <code>&amp;<span class="k">mut</span></code> on the coordinate controls write access. The two are orthogonal.</p>'
+'<table><tr><th></th><th>Can reassign binding</th><th>Can write at coordinate</th></tr>'
+'<tr><td><code><span class="k">let</span> r = &amp;x</code></td><td>No</td><td>No</td></tr>'
+'<tr><td><code><span class="k">let</span> <span class="k">mut</span> r = &amp;x</code></td><td>Yes</td><td>No</td></tr>'
+'<tr><td><code><span class="k">let</span> r = &amp;<span class="k">mut</span> x</code></td><td>No</td><td>Yes</td></tr>'
+'<tr><td><code><span class="k">let</span> <span class="k">mut</span> r = &amp;<span class="k">mut</span> x</code></td><td>Yes</td><td>Yes</td></tr>'
+'</table>';
}};

N.shared_exclusive={ch:'Rust\'s Patches',t:'Shared and Exclusive Coordinates',ctx:'Two kinds of coordinates exist. What are they?',
b:function(){return ''
+'<p><code>coord_exclusive</code> (<code>&amp;<span class="k">mut</span> T</code>) grants read and write access to one coordinate at a time. <code>coord_shared</code> (<code>&amp;T</code>) grants read access and allows multiple coordinates to coexist. Rust calls <code>&amp;T</code> a shared reference and <code>&amp;<span class="k">mut</span> T</code> a mutable reference.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(s) = take(<span class="m">String</span>::from(<span class="s">"hello"</span>));\n    <span class="k">let</span> alias(r1) = coord_shared(s);\n    <span class="k">let</span> alias(r2) = coord_shared(s);\n    <span class="m">println!</span>(<span class="s">"{}, {}"</span>, at(r1), at(r2));    <span class="c">// two shared coordinates, safe</span>\n}</code></div>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(<span class="k">mut</span> s) = take(<span class="m">String</span>::from(<span class="s">"hello"</span>));\n    <span class="k">let</span> alias(w) = coord_exclusive(s);\n    at(w).push_str(<span class="s">" world"</span>);              <span class="c">// one exclusive coordinate, safe</span>\n}</code></div>'
+'<table><tr><th>Rust</th><th>Notation</th></tr>'
+'<tr><td><code><span class="k">let</span> r = &amp;s</code></td><td><code><span class="k">let</span> alias(r) = coord_shared(s)</code></td></tr>'
+'<tr><td><code><span class="k">let</span> w = &amp;<span class="k">mut</span> s</code></td><td><code><span class="k">let</span> alias(w) = coord_exclusive(s)</code></td></tr>'
+'<tr><td><code>*r</code></td><td><code>at(r)</code></td></tr>'
+'<tr><td><code><span class="k">let</span> <span class="k">mut</span> x</code></td><td><code>owner(rebindable(x))</code></td></tr>'
+'</table>'
+'<p>Rust\'s error messages say "borrowed as immutable" for a shared coordinate and "borrowed as mutable" for an exclusive coordinate. The '+EA('e5_19','borrowing rule')+' derives the rules governing how shared and exclusive coordinates interact: many shared or one exclusive.</p>';
}};

N.composing_vocab={ch:'Rust\'s Patches',t:'Composing the Vocabulary',ctx:'How do all these pieces fit together?',
b:function(){return ''
+'<p><code>at</code> works on both sides of an assignment. On the right it follows the coordinate and reads the value. On the left it follows the coordinate and writes a new value.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(rebindable(x)) = take(<span class="n">5</span>);\n    <span class="k">let</span> alias(r) = coord_exclusive(x);\n    at(r) = <span class="n">10</span>;                         <span class="c">// write: target now holds 10</span>\n    <span class="k">let</span> owner(y) = mem_copy(at(r));     <span class="c">// read: y gets a copy of 10</span>\n}</code></div>'
+'<p>The owner cannot be used while an exclusive coordinate to its space exists. The compiler enforces this as part of the borrowing rule. The owner resumes when the coordinate expires. <code>take</code> permanently invalidates the source. An exclusive coordinate temporarily suspends it.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(rebindable(x)) = take(<span class="n">5</span>);\n    <span class="k">let</span> alias(r) = coord_exclusive(x);\n    <span class="c">// x is suspended here</span>\n    at(r) = <span class="n">10</span>;\n    <span class="c">// r goes out of scope, x resumes</span>\n    <span class="m">println!</span>(<span class="s">"{}"</span>, x);                  <span class="c">// prints 10</span>\n}</code></div>'
+'<p><code><span class="m">Box</span>&lt;T&gt;</code> owns heap space containing a <code>T</code>. Following the coordinate with <code>at</code> reaches the inner value, and <code>take</code> moves that value out, transferring ownership and invalidating the Box.</p>'
+'<div class="cb"><code><span class="m">explicit!</span> {\n    <span class="k">let</span> owner(b) = take(<span class="m">Box</span>::new(<span class="m">String</span>::from(<span class="s">"hello"</span>)));\n    <span class="k">let</span> owner(s) = take(at(b));         <span class="c">// move String out of the Box</span>\n    <span class="m">println!</span>(<span class="s">"{}"</span>, s);\n    <span class="c">// b is invalid</span>\n}</code></div>'
+'<p>Standard Rust writes <code><span class="k">let</span> s = *b</code>. Dereferencing a <code><span class="m">Box</span></code> transfers ownership of the inner value. Regular references do not support this operation.</p>'
+'<table><tr><th>Notation</th><th>Framework</th></tr>'
+'<tr><td><code>owner</code></td><td>SPACE x TIME</td></tr>'
+'<tr><td><code>alias</code></td><td>COORDINATES</td></tr>'
+'<tr><td><code>take</code></td><td>Ownership transfer in TIME</td></tr>'
+'<tr><td><code>coord_exclusive</code></td><td>One COORDINATE, read/write</td></tr>'
+'<tr><td><code>coord_shared</code></td><td>Many COORDINATES, read only</td></tr>'
+'<tr><td><code>at</code></td><td>Follow a COORDINATE</td></tr>'
+'<tr><td><code>mem_copy</code></td><td>Duplicate SPACE, independent owners</td></tr>'
+'<tr><td><code>take_or_mem_copy</code></td><td>Compiler decides based on type</td></tr>'
+'<tr><td><code>rebindable</code></td><td>Binding changes target over TIME</td></tr>'
+'</table>'
+'<p>The <code>explicit!</code> notation reveals what standard Rust leaves implicit. The next chapter derives the rule governing how shared and exclusive coordinates interact.</p>';
}};

// --- Edges ---

// Within-chapter choice cards
ED.e5_1={from:'constrained_refs',to:'ownership_deep',t:'How does Rust handle heap lifetime?',w:'Exactly one owner per allocation. Heap reclamation tied to scope.',card:true};
ED.e5_2={from:'ownership_deep',to:'notation',t:'How do I see what\'s really happening?',w:'The notation crate makes owner, alias, and take visible in the syntax.',card:true};
ED.e5_3={from:'notation',to:'sigs_lifetimes',t:'How do coordinates cross function boundaries?',w:'Signatures declare which input the output borrows from.',card:true};
ED.e5_4={from:'sigs_lifetimes',to:'desugaring',t:'How do operators connect through signatures?',w:'v[0] desugars to *v.index(0). Lifetime elision infers the connection.',card:true};
ED.e5_5={from:'desugaring',to:'using_managing',t:'What can a coordinate actually do?',w:'Read and write data, but never manage the space. Only the owner can do that.',card:true};
ED.e5_6={from:'using_managing',to:'move_default',t:'Why does assignment invalidate the source?',w:'Byte-copying would duplicate the heap pointer. Move is the safe default.',card:true};
ED.e5_7={from:'move_default',to:'struct_lifetimes',t:'What about structs that hold references?',w:'A struct holding derived data must declare the dependency with \'a.',card:true};
ED.e5_8={from:'struct_lifetimes',to:'mut_binding',t:'What does mut really mean?',w:'Rebinding and writing through a coordinate are separate operations.',card:true};
ED.e5_9={from:'mut_binding',to:'shared_exclusive',t:'What are the two kinds of coordinates?',w:'coord_shared for reading, coord_exclusive for writing. Two different access levels.',card:true};
ED.e5_10={from:'shared_exclusive',to:'composing_vocab',t:'How do all these pieces fit together?',w:'How do reading, writing, borrowing, and Box interact in the notation?',card:true};
ED.e5_11={from:'composing_vocab',to:'coord_dangle',t:'What happens when the patches aren\'t enough?',w:'The fourth obstacle remains: undecidability.',card:true};

// Cross-chapter edges (inline, via EA)
ED.e5_12={from:'constrained_refs',to:'ram_flat',t:'what was the obstacle?',w:'Why can\'t the compiler just use RAM addresses as scope?'};
ED.e5_14={from:'sigs_lifetimes',to:'cross_scope',t:'what was the obstacle?',w:'Data flow crosses scope boundaries. The scope tree can\'t track it.'};
ED.e5_16={from:'move_default',to:'invisible_dup',t:'what was the obstacle?',w:'Byte-copying a managed type duplicates the heap pointer, creating two owners for the same space.'};
ED.e5_17={from:'notation',to:'bindings',t:'where did bindings come from?',w:'What is a binding, and what can it hold?'};
ED.e5_18={from:'using_managing',to:'manage_space',t:'how does the machine handle space?',w:'How do stack and heap differ at the machine level?'};
ED.e5_19={from:'shared_exclusive',to:'borrowing_rule_deep',t:'how do shared and exclusive interact?',w:'How do shared and exclusive coordinates interact?'};
