// ═══ ABOUT ═══

N.about={ch:'Learn Rust',t:'Learn Rust',ctx:'',
b:function(){return ''
+'<p>This course exists because I cannot memorize rules and never could. If I understand the system that produces the rules, I remember it for years, because systems have structure and structure compresses.</p>'
+'<p><a href="https://www.rust-lang.org/">Rust</a> tutorials gave me rules such as \u201cone mutable reference or many immutable,\u201d \u201cmoves invalidate the source,\u201d and \u201clifetimes must not outlive their referent.\u201d Stack architecture, computability constraints, and Rice\u2019s theorem sit underneath these rules and explain every decision Rust\u2019s designers made. Dig into them and the rules stop requiring memorization because they become derivable. The borrow checker becomes a consequence of what memory is, what compilers can analyze, and what tradeoffs Rust chose.</p>'
+'<p>I did not find that path in any existing course. <a href="https://doc.rust-lang.org/book/">The Rust Book</a>, <a href="https://google.github.io/comprehensive-rust/">Google\u2019s Comprehensive Rust</a>, <a href="https://github.com/nrc/r4cppp">r4cppp</a>, <a href="https://www.codecademy.com/learn/rust-for-programmers">Codecademy</a>, and <a href="https://www.udemy.com/courses/search/?src=ukw&q=rust">Udemy</a> teach the rules well. This course derives them. SPACE \u00d7 TIME \u00d7 COORDINATES is a framework for memory bugs. Use-after-free, data races, and dangling references are failures in one of these three dimensions. The course builds the framework first, and the borrow checker falls out of it.</p>'
+'<p><strong>Who this course is for</strong></p>'
+'<p>This course is for experienced programmers, particularly those with a C++ background, who already have a mental model of systems programming. The course updates that model. The density is high and the chapters are short, because we believe every sentence should be valuable and add to what you already know.</p>'
+'<p>If you are new to programming or new to systems languages, start with <a href="https://doc.rust-lang.org/book/">The Rust Book</a>. It builds the mental model this course assumes you already have. Come back here when you want to understand the system underneath the rules.</p>'
+'<p><strong>How to read this</strong></p>'
+'<p>This is an interactive graph, not a linear book. Each section ends with an underlined <em>next.</em> \u2014 hover it to see where you can go, then click to continue. Underlined words inside the text link to related topics: hover for a preview, click to jump. You can also hover any section title to see its connections.</p>'
+'<p>The sidebar on the left lists a guided \u201cTextbook\u201d walk that covers the full course in order. Click \u201cUse this walk\u201d to load all sections at once, collapsed. The sidebar tracks your progress and lets you jump to any section you\u2019ve visited. You can also explore freely \u2014 every path through the graph is valid.</p>';
}};

ED.e_about={from:'about',to:'familiar',t:'Start the course',w:'Familiar C++ syntax, then the programs Rust rejects.',card:true};

// ═══ CHAPTER 1: FIRST CONTACT ═══

// --- Nodes ---

N.familiar={ch:'First Contact',t:'Familiar Syntax',ctx:'',
b:function(){return ''
+'<p>Most Rust syntax matches C++. Functions, structs, loops, and vectors work the same way.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">main</span>() {\n    <span class="m">println!</span>(<span class="s">"Hello, world!"</span>);\n}</code></div>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> x = <span class="n">5</span>;\n    <span class="k">let</span> y = <span class="n">10</span>;\n    <span class="m">println!</span>(<span class="s">"{} + {} = {}"</span>, x, y, x + y);\n}</code></div>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">add</span>(a: <span class="m">i32</span>, b: <span class="m">i32</span>) -> <span class="m">i32</span> {\n    a + b\n}\n\n<span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> result = <span class="m">add</span>(<span class="n">3</span>, <span class="n">4</span>);\n    <span class="m">println!</span>(<span class="s">"Result: {}"</span>, result);\n}</code></div>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>, <span class="n">4</span>, <span class="n">5</span>];\n    <span class="m">println!</span>(<span class="s">"Vector: {:?}"</span>, v);\n    <span class="m">println!</span>(<span class="s">"Length: {}"</span>, v.<span class="m">len</span>());\n}</code></div>'
+'<div class="cb"><code><span class="k">struct</span> <span class="m">Point</span> {\n    x: <span class="m">i32</span>,\n    y: <span class="m">i32</span>,\n}\n\n<span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> p = <span class="m">Point</span> { x: <span class="n">10</span>, y: <span class="n">20</span> };\n    <span class="m">println!</span>(<span class="s">"Point at ({}, {})"</span>, p.x, p.y);\n}</code></div>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let mut</span> count = <span class="n">0</span>;\n    <span class="k">while</span> count &lt; <span class="n">5</span> {\n        <span class="m">println!</span>(<span class="s">"Count: {}"</span>, count);\n        count += <span class="n">1</span>;\n    }\n}</code></div>'
+'<p>All of this compiles and runs. But some code that compiles in C++ '+EA('e1','does not compile in Rust')+'.</p>';
}};

N.rejected={ch:'First Contact',t:'Programs That Don\'t Compile',ctx:'You saw familiar syntax. Now see where it breaks.',
b:function(){return ''
+'<p>These programs contain memory bugs. Rust rejects them. C++ compiles them.</p>'
+'<div class="cb"><code><span class="k">let</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> w = v;\n<span class="m">println!</span>(<span class="s">"{:?}"</span>, v);</code></div>'
+'<div class="er"><b>error[E0382]:</b> borrow of moved value: `v`</div>'
+'<p>The value moved. <code>v</code> is gone. This is '+EA('e5','ownership')+' in action.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> r = &amp;v[<span class="n">0</span>];\nv.<span class="m">push</span>(<span class="n">4</span>);\n<span class="m">println!</span>(<span class="s">"{}"</span>, r);</code></div>'
+'<div class="er"><b>error[E0502]:</b> cannot borrow `v` as mutable because it is also borrowed as immutable</div>'
+'<p>A reference to <code>v[0]</code> exists while <code>push</code> tries to modify <code>v</code>. The '+EA('e6','borrowing rule')+' prevents this.</p>'
+'<div class="cb"><code><span class="k">let</span> r;\n{\n    <span class="k">let</span> x = <span class="n">5</span>;\n    r = &amp;x;\n}\n<span class="m">println!</span>(<span class="s">"{}"</span>, r);</code></div>'
+'<div class="er"><b>error[E0597]:</b> `x` does not live long enough</div>'
+'<p><code>x</code> is gone when the block ends. <code>r</code> would point to nothing. This is a '+EA('e3','dangling reference')+' caught at compile time.</p>';
}};

N.rejected_more={ch:'First Contact',t:'More Rejected Programs',ctx:'Three errors down. Six more categories of rejection.',
b:function(){return ''
+'<div class="cb"><code><span class="k">let mut</span> s = <span class="m">String</span>::from(<span class="s">"hello"</span>);\n<span class="k">let</span> r1 = &amp;<span class="k">mut</span> s;\n<span class="k">let</span> r2 = &amp;<span class="k">mut</span> s;\n<span class="m">println!</span>(<span class="s">"{}, {}"</span>, r1, r2);</code></div>'
+'<div class="er"><b>error[E0499]:</b> cannot borrow `s` as mutable more than once at a time</div>'
+'<p>Two mutable references to the same data. The compiler forbids this because two writers to the same memory is a data race waiting to happen.</p>'
+'<div class="cb"><code><span class="k">fn</span> <span class="m">get_string</span>() -> &amp;<span class="m">String</span> {\n    <span class="k">let</span> s = <span class="m">String</span>::from(<span class="s">"hello"</span>);\n    &amp;s\n}</code></div>'
+'<div class="er"><b>error[E0106]:</b> missing lifetime specifier</div>'
+'<p>The function tries to return a reference to a local variable. The local is destroyed when the function returns. The compiler demands a '+EA('e_rejm_la','lifetime annotation')+' to verify the reference outlives its scope.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">for</span> x <span class="k">in</span> &amp;v {\n    v.<span class="m">push</span>(*x);\n}</code></div>'
+'<div class="er"><b>error[E0502]:</b> cannot borrow `v` as mutable because it is also borrowed as immutable</div>'
+'<p>The iterator borrows <code>v</code> immutably. <code>push</code> borrows it mutably. Same conflict, different shape.</p>'
+'<div class="cb"><code><span class="k">let mut</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n<span class="k">let</span> closure = || v.<span class="m">push</span>(<span class="n">4</span>);\n<span class="m">println!</span>(<span class="s">"{:?}"</span>, v);\nclosure();</code></div>'
+'<div class="er"><b>error[E0502]:</b> cannot borrow `v` as immutable because it is also borrowed as mutable</div>'
+'<p>The closure captures <code>v</code> mutably. <code>println!</code> tries to read it. The borrow conflict spans the closure\'s lifetime.</p>';
}};

N.dangling_detail={ch:'First Contact',t:'The Dangling Reference',ctx:'One rejected program examined in detail.',
b:function(){return ''
+'<div class="cb"><code><span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> r;\n    {\n        <span class="k">let</span> x = <span class="n">5</span>;\n        r = &amp;x;\n    }\n    <span class="m">println!</span>(<span class="s">"{}"</span>, r);\n}</code></div>'
+'<div class="er"><b>error[E0597]:</b> `x` does not live long enough</div>'
+'<p><code>r</code> refers to <code>x</code>. The block ends. <code>x</code> is gone. <code>r</code> points to nothing.</p>'
+'<p>The C++ equivalent compiles and runs. It prints garbage, or crashes, or prints 5 by accident. The C++ standard leaves this behavior undefined.</p>'
+'<div class="cb"><code><span class="c">// C++</span>\n<span class="m">int</span> <span class="m">main</span>() {\n    <span class="m">int</span>* r;\n    {\n        <span class="m">int</span> x = <span class="n">5</span>;\n        r = &amp;x;\n    }\n    <span class="m">printf</span>(<span class="s">"%d\\n"</span>, *r);\n}</code></div>'
+'<p>Rust rejects the program before it runs. The compiler tracks where every reference points and when the target is '+EA('e11','reclaimed')+'. A reference that outlives its target is a '+EA('e12','dangling pointer')+'.</p>';
}};

N.thread_errors={ch:'First Contact',t:'Thread Safety Errors',ctx:'The compiler prevents data races too.',
b:function(){return ''
+'<div class="cb"><code><span class="k">use</span> std::thread;\n\n<span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> v = <span class="m">vec!</span>[<span class="n">1</span>, <span class="n">2</span>, <span class="n">3</span>];\n    thread::spawn(|| {\n        <span class="m">println!</span>(<span class="s">"{:?}"</span>, v);\n    });\n}</code></div>'
+'<div class="er"><b>error[E0373]:</b> closure may outlive the current function</div>'
+'<p>The spawned thread might outlive <code>main</code>. The closure captures <code>v</code> by reference, but the reference could dangle when <code>main</code> returns. The fix: <code>move</code> the value into the closure, transferring '+EA('e16','ownership')+' to the thread.</p>'
+'<div class="cb"><code><span class="k">use</span> std::rc::Rc;\n<span class="k">use</span> std::thread;\n\n<span class="k">fn</span> <span class="m">main</span>() {\n    <span class="k">let</span> rc = <span class="m">Rc</span>::new(<span class="n">5</span>);\n    thread::spawn(<span class="k">move</span> || {\n        <span class="m">println!</span>(<span class="s">"{}"</span>, rc);\n    });\n}</code></div>'
+'<div class="er"><b>error[E0277]:</b> `Rc&lt;i32&gt;` cannot be sent between threads safely</div>'
+'<p><code>Rc</code> uses non-atomic reference counting. Sharing it across threads would corrupt the count. The type system encodes this: <code>Rc</code> does not implement <code>Send</code>. The compiler rejects the program at the type level, before any runtime check.</p>'
+'<p>The same '+EA('e17','borrowing rule')+' that prevents single-threaded bugs also prevents '+EA('e15','data races')+'.</p>';
}};

// --- Edges from ch01 nodes ---
// card:true = appears as a choice card at the bottom of the source node

// From: familiar
ED.e1={from:'familiar',to:'rejected',t:'What doesn\'t compile?',w:'The compiler rejects code that looks right.',card:true};

// From: rejected (choice cards first, then prose-only)
ED.e2={from:'rejected',to:'rejected_more',t:'Are there more errors like these?',w:'What other code does the compiler reject?',card:true};
ED.e3={from:'rejected',to:'dangling_detail',t:'What\'s a dangling reference?',w:'C++ compiles it. Rust refuses. The details reveal why.',card:true};
ED.e4={from:'rejected',to:'physics',t:'Why do these bugs exist at all?',w:'Three primitives rooted in physics explain every category.',card:true};
ED.e5={from:'rejected',to:'ownership_deep',t:'why does my value vanish?',w:'Why does assigning a value invalidate the original?'};
ED.e6={from:'rejected',to:'borrowing_rule_deep',t:'how does the compiler know this is wrong?',w:'It tracks which borrows are live at each point and prevents conflicting access.'};

// From: rejected_more
ED.e7={from:'rejected_more',to:'thread_errors',t:'Does the compiler catch thread bugs too?',w:'Does the type system prevent concurrency bugs too?',card:true};
ED.e8={from:'rejected_more',to:'physics',t:'Why do all these bugs exist?',w:'Three primitives rooted in physics explain every category.',card:true};
ED.e_rejm_la={from:'rejected_more',to:'sigs_lifetimes',t:'what are those \'a things I keep seeing?',w:'An explicit marker that tells the compiler how long a reference lives.'};

// From: dangling_detail
ED.e9={from:'dangling_detail',to:'physics',t:'Why does this class of bug exist?',w:'Three primitives rooted in physics explain every category.',card:true};
ED.e10={from:'dangling_detail',to:'rejected_more',t:'Are there more rejected programs?',w:'What other patterns does the compiler forbid?',card:true};
ED.e11={from:'dangling_detail',to:'uaf',t:'what happens to that memory?',w:'The space is returned to the stack. The bytes are still there, but the allocation has ended.'};
ED.e12={from:'dangling_detail',to:'dangling_ptr',t:'how common is this bug?',w:'One of the most common memory bugs in C and C++. The timeline shows exactly how it happens.'};

// From: thread_errors
ED.e13={from:'thread_errors',to:'dangling_detail',t:'What does a dangling reference look like up close?',w:'The same bug, Rust vs C++, side by side.',card:true};
ED.e14={from:'thread_errors',to:'physics',t:'Why do all these bugs keep happening?',w:'Three primitives rooted in physics explain every category.',card:true};
ED.e15={from:'thread_errors',to:'data_race',t:'What exactly is a data race?',w:'Two threads accessing the same memory, at least one writing, no synchronization. The timeline shows the mechanics.',card:true};
ED.e16={from:'thread_errors',to:'ownership_deep',t:'what does move actually do?',w:'Where does the value go, and why can\'t the old scope see it?'};
ED.e17={from:'thread_errors',to:'borrowing_rule_deep',t:'remind me how borrowing works?',w:'How does one rule prevent both single-threaded and multi-threaded bugs?'};

