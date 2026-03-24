// ═══ CHAPTER 4: WHAT DEFEATS THE ANALYSIS ═══

// --- Nodes ---

N.cpp_skip={ch:'What Defeats the Analysis',t:'Why C++ Compilers Skip This Analysis',ctx:'The analysis from Chapter 3 exists. Most compilers don\'t attempt it.',
b:function(){return ''
+'<p>Three properties of C and C++ prevent building the complete control flow graph the analysis requires.</p>'
+'<p><strong>Separate compilation.</strong> Each source file becomes an object file independently. A call to a function defined in another file appears as an external symbol, resolved by the linker after analysis is over. The graph has edges leading to functions whose bodies are '+EA('e4_16','invisible')+'.</p>'
+'<p><strong>Function pointers.</strong> The call target is a runtime value. The compiler cannot determine which function executes. The graph has edges to unknown destinations.</p>'
+'<p><strong>Unconstrained pointers.</strong> Even with a complete graph, the compiler needs to track where every coordinate points. C++ allows coordinates to be created without naming a target.</p>'
+'<div class="cb"><code><span class="m">int</span>* p = (<span class="m">int</span>*)<span class="n">0x1234</span>;           <span class="c">// coordinate from integer</span>\n<span class="m">int</span>* q = p + <span class="n">5</span>;                   <span class="c">// coordinate from arithmetic</span>\n<span class="k">void</span>* v = p;                      <span class="c">// type erased</span>\n<span class="m">int</span>** stored = &amp;p;                <span class="c">// coordinate to coordinate</span></code></div>'
+'<p>An integer cast to a pointer, pointer arithmetic, <code>void*</code> type erasure. The analysis requires knowing where every coordinate points. C++ provides no such guarantee. Rust '+EA('e4_10','constrains coordinate creation')+' to fix this.</p>'
+'<p>These are three specific obstacles. They generalize to '+EA('e4_1','two deeper problems')+' that any language must address.</p>';
}};

N.ram_flat={ch:'What Defeats the Analysis',t:'RAM Is Flat',ctx:'One of two deeper problems behind C++\'s obstacles.',
b:function(){return ''
+'<p>The machine model has no scope hierarchy. Memory is a sequence of numbered cells. A pointer is a number. Any address can read or write any cell. Nothing in the machine prevents code from reading a cell after the scope that allocated it has ended. Nothing prevents fabricating an address the compiler cannot trace.</p>'
+'<p>C++\'s unconstrained pointers are a symptom of this deeper problem. The machine provides no structure for the compiler to exploit. Coordinates can arise from anywhere and point to anything.</p>'
+'<p>A compiler that wants to answer "where does each coordinate point?" needs the language itself to constrain how coordinates are created. The '+EA('e4_16','analysis')+' cannot work if coordinates appear from nowhere.</p>'
+'<p>The second deeper problem: '+EA('e4_2','data that crosses scope boundaries')+'.</p>';
}};

N.cross_scope={ch:'What Defeats the Analysis',t:'Data Flow Crosses Scope Boundaries',ctx:'Scopes track names. Data flows between them.',
b:function(){return ''
+'<p>The scope tree tracks where names live. It does not track where data flows between them. A function argument carries a coordinate into a nested scope. A return value carries one out. An assignment can carry a coordinate to a binding with a different lifetime.</p>'
+'<p>A function\'s caller passes <code>&amp;v</code> in and receives a coordinate out. The caller cannot determine whether the returned coordinate still points into <code>v</code>\'s space. The signature says nothing about the connection. Separate compilation means the compiler cannot see the function body to find out.</p>'
+'<p>C++\'s separate compilation and function pointers are symptoms. The deeper problem is that data flow between scopes is invisible to the scope tree. A compiler that wants to trace coordinates across function boundaries needs the language to '+EA('e4_11','declare how data flows between scopes')+'.</p>'
+'<p>One more obstacle remains: '+EA('e4_3','types that contain pointers')+'.</p>';
}};

N.invisible_dup={ch:'What Defeats the Analysis',t:'Invisible Coordinate Copying',ctx:'Some types hide coordinates inside their stack representation.',
b:function(){return ''
+'<p>A <code>String</code>\'s stack representation holds a heap pointer, a length, and a capacity. The heap pointer is a coordinate to managed space. Byte-copying the stack representation duplicates that coordinate without duplicating the heap buffer. Two bindings now hold coordinates to the same buffer, and both believe they own it.</p>'
+'<div class="cb"><code>Byte-copy on i32:                    Byte-copy on String (hypothetical):\n\n+----------+                         +----------+        +----------+\n|    5     | &lt;- x (owner)            | ptr -----|-======&gt;| "hello"  |\n+----------+                         +----------+   |    |          |\n|    5     | &lt;- y (owner)            | ptr -----|---+    +----------+\n+----------+                         +----------+\nIndependent space.                   Two owners, one space.</code></div>'
+'<p>An <code>i32</code> has no heap pointer and no cleanup obligation. Duplicating its bytes produces two independent values. A <code>String</code> manages heap space, and byte-copying it duplicates the coordinate without duplicating the space itself. The analysis tracks one owner per allocation. Invisible duplication defeats that tracking by creating a second owner the compiler did not authorize.</p>'
+'<p>The compiler needs to know which types are safe to byte-copy and which are not. Rust '+EA('e4_12','prevents accidental byte-copying')+' of types that manage internal space.</p>'
+'<p>'+EA('e4_4','Other languages')+' approach these obstacles in entirely different ways.</p>';
}};

N.other_langs={ch:'What Defeats the Analysis',t:'How Other Languages Approach These Obstacles',ctx:'Five strategies, from doing nothing to doing everything at compile time.',
b:function(){return ''
+'<table><tr><th>Strategy</th><th>Languages</th><th>Tradeoff</th></tr>'
+'<tr><td>No analysis</td><td>C, C++</td><td>Fast, unsafe. Programmer responsibility.</td></tr>'
+'<tr><td>Static analyzers</td><td>Clang SA, Coverity, Infer</td><td>Separate pass, expensive, heuristic. Misses real bugs, reports false positives.</td></tr>'
+'<tr><td>Region annotations</td><td>Cyclone, ATS</td><td>Compile-time safety. High annotation burden, never widely adopted.</td></tr>'
+'<tr><td>Garbage collection</td><td>Java, Go, Python, C#</td><td>Space lives until no coordinates reach it. Runtime cost: GC pauses, memory overhead.</td></tr>'
+'<tr><td>Ownership</td><td>Rust</td><td>Compile-time proof. Zero runtime cost. Requires restructuring when the approximation rejects.</td></tr>'
+'</table>'
+'<p><strong>No analysis.</strong> C and C++ compilers do not attempt systematic tracking. Some warn about obvious cases like returning addresses of locals, but the obstacles described in this chapter prevent anything more.</p>'
+'<p><strong>Static analyzers.</strong> Tools like Clang Static Analyzer, Coverity, and Infer run separately from compilation. They use abstract interpretation and symbolic execution to trace paths. The analysis is expensive and trades precision for scalability. Heuristics reduce false positives, which means real bugs are missed too.</p>'
+'<p><strong>Region annotations.</strong> Cyclone added region types to C: every pointer carries a region annotation, and the compiler tracks region lifetimes. ATS uses dependent and linear types. Both achieve compile-time safety, but annotation requirements and limited C compatibility kept them in research settings.</p>'
+'<p><strong>Garbage collection.</strong> The problem is coordinates pointing to space that no longer exists. GC inverts the relationship: space is not freed until no coordinates point to it. The runtime traces from roots to find reachable objects. The tradeoff is runtime overhead: memory for bookkeeping, CPU for tracing, unpredictable pauses.</p>'
+'<p><strong>Ownership.</strong> Rust patches each obstacle so the compiler can build the complete graph and track every coordinate. Each value has one owner. Transfer invalidates the previous binding. Zero runtime cost. But the analysis faces a '+EA('e4_5','fundamental limit')+' that no language design can remove.</p>'
+'<p>Each '+EA('e4_13','paradigm')+' represents a different point in the design space.</p>';
}};

N.branches_break={ch:'What Defeats the Analysis',t:'Where Branches Break the Analysis',ctx:'Even with every obstacle patched, one problem remains.',
b:function(){return ''
+'<p>Branches create paths that may or may not execute, depending on runtime values.</p>'
+'<div class="cb"><code>r = uninitialized\n{\n    x = <span class="n">5</span>\n    <span class="k">if</span> some_condition(n) {\n        r = &amp;x\n    }\n}\n<span class="c">// x is gone here</span>\n<span class="k">if</span> some_condition(n) {\n    print(*r)      <span class="c">// r used after x\'s space is reclaimed?</span>\n}</code></div>'
+'<p>Whether <code>*r</code> accesses reclaimed space depends on whether <code>some_condition(n)</code> returns true. The compiler would need to evaluate it. That evaluation depends on <code>n</code>, and <code>some_condition</code> might contain arbitrary computation. The '+EA('e4_17','analysis checks all paths')+' and rejects when any path is unsafe.</p>'
+'<div class="cb"><code><span class="c">// Collatz sequence: does this terminate for all inputs?</span>\nn = input\n<span class="k">while</span> n != <span class="n">1</span> {\n    <span class="k">if</span> n is even { n = n / <span class="n">2</span> }\n    <span class="k">else</span> { n = <span class="n">3</span>*n + <span class="n">1</span> }\n}</code></div>'
+'<p>Whether the Collatz sequence terminates for all inputs remains unknown. Mathematicians have tested every number up to 10<sup>20</sup>. No proof exists. A <code>some_condition(n)</code> that checks Collatz termination would force the compiler to solve an open problem in mathematics.</p>'
+'<p>Arbitrary computation resists static prediction. '+EA('e4_6','No algorithm can avoid this')+'.</p>';
}};

N.perfect_impossible={ch:'What Defeats the Analysis',t:'Why Perfect Analysis Is Impossible',ctx:'Not just hard. Mathematically impossible.',
b:function(){return ''
+'<p><strong>The Halting Problem (Turing, 1936).</strong> A termination checker must return an answer for any program it examines. The program being examined has no such obligation. It can run forever, halt immediately, or do anything in between.</p>'
+'<p>A program can read the checker\'s source code, call the checker on itself, and then do the opposite of what the checker predicts. If the checker predicts "terminates," the program loops forever. If the checker predicts "loops forever," the program terminates. The checker is wrong regardless of what it answers. No algorithm can avoid this contradiction.</p>'
+'<p><strong>Rice\'s Theorem (1951)</strong> generalizes the result. Any non-trivial semantic property of programs is undecidable. "Non-trivial" means some programs have the property and some do not. "Semantic" means the property depends on what the program does, not how it is written.</p>'
+'<p>"Does this COORDINATE reach reclaimed SPACE?" is such a property. So is "does this program have a data race?" and "does this program dereference a dangling pointer?" These are mathematical impossibilities that apply to any static analysis tool, in any language, built by any team.</p>'
+'<p>The compiler cannot be perfect. It must '+EA('e4_7','choose between two failures')+'. When it rejects valid code, '+EA('e4_14','escape hatches')+' exist.</p>';
}};

N.sound_complete={ch:'What Defeats the Analysis',t:'Sound or Complete',ctx:'The compiler must choose which failure to accept.',
b:function(){return ''
+'<p><strong>Sound</strong> means the compiler never accepts unsafe code. It may reject code that is actually safe.</p>'
+'<p><strong>Complete</strong> means the compiler never rejects safe code. It may accept code that is actually unsafe.</p>'
+'<p>No compile-time analysis can be both sound and complete for questions about program behavior. Accepting unsafe code means security vulnerabilities and undefined behavior. Rejecting safe code means restructuring or using escape hatches. The asymmetry between failure modes determines the choice.</p>'
+'<p>Rust chooses soundness. The '+EA('e4_15','approximation')+' rejects some valid programs, and that is the acceptable cost. '+EA('e4_8','What exactly')+' falls on the approximate side?</p>';
}};

N.names_exact={ch:'What Defeats the Analysis',t:'Names Are Exact, Data Flow Is Approximated',ctx:'The boundary between what the compiler knows precisely and what it approximates.',
b:function(){return ''
+'<p>The compiler handles names exactly. Scope boundaries, variable declarations, and field names are all visible in source text. The scope tree captures where every name lives and when every name expires. This part of the analysis is decidable and precise.</p>'
+'<p>The approximation falls entirely on data flow. Which path a branch takes, which method a function pointer calls, whether two indices refer to the same element. These depend on runtime values, and Rice\'s theorem says no algorithm answers correctly for all inputs. The compiler assumes the worst case for every question about data flow.</p>'
+'<p>Four obstacles face the analysis: flat RAM, cross-scope data flow, invisible duplication, and undecidability. One fundamental constraint governs it: the compiler chooses soundness over completeness. The next chapter shows '+EA('e4_9','how Rust patches each obstacle')+' with a specific design choice.</p>';
}};

// --- Edges ---

// Within-chapter choice cards
ED.e4_1={from:'cpp_skip',to:'ram_flat',t:'What\'s the deeper problem?',w:'Two obstacles that any language must address, not just C++.',card:true};
ED.e4_2={from:'ram_flat',to:'cross_scope',t:'What about data that crosses scope boundaries?',w:'The scope tree tracks names, not data flow between them.',card:true};
ED.e4_3={from:'cross_scope',to:'invisible_dup',t:'What about types that contain pointers?',w:'Byte-copying a String duplicates the coordinate without duplicating the space.',card:true};
ED.e4_4={from:'invisible_dup',to:'other_langs',t:'How do other languages handle this?',w:'Five strategies from doing nothing to compile-time proofs.',card:true};
ED.e4_5={from:'other_langs',to:'branches_break',t:'Even with all obstacles patched, what remains?',w:'Branches create paths that depend on runtime values.',card:true};
ED.e4_6={from:'branches_break',to:'perfect_impossible',t:'Can any algorithm predict which path executes?',w:'No. Turing proved it in 1936.',card:true};
ED.e4_7={from:'perfect_impossible',to:'sound_complete',t:'What does the compiler do about this?',w:'Choose between two failures: accept unsafe or reject safe.',card:true};
ED.e4_8={from:'sound_complete',to:'names_exact',t:'What exactly is approximate?',w:'Names are exact. Data flow is approximated.',card:true};
ED.e4_9={from:'names_exact',to:'constrained_refs',t:'How does Rust patch each obstacle?',w:'Four design choices that make the analysis possible: constrained references, ownership, function signatures, and move-by-default.',card:true};

// Cross-chapter inline edges
ED.e4_10={from:'cpp_skip',to:'constrained_refs',t:'how does Rust fix unconstrained pointers?',w:'How does Rust make every reference traceable?'};
ED.e4_11={from:'cross_scope',to:'sigs_lifetimes',t:'how does Rust declare data flow?',w:'How do function signatures declare where a reference points?'};
ED.e4_12={from:'invisible_dup',to:'move_default',t:'how does Rust prevent this?',w:'Why does Rust move by default instead of copying?'};
ED.e4_13={from:'other_langs',to:'paradigms',t:'which paradigms use which strategies?',w:'Which languages use which strategies?'};
ED.e4_14={from:'perfect_impossible',to:'escape_hatches',t:'what do you do when the compiler rejects valid code?',w:'Restructure, encode in types, runtime checks, or unsafe.'};
ED.e4_15={from:'sound_complete',to:'borrowing_rule_deep',t:'what does the approximation look like?',w:'What does the compiler actually enforce?'};
ED.e4_16={from:'ram_flat',to:'detecting_access',t:'how does the analysis work when it can?',w:'The compiler builds the control flow graph and checks whether any path leads from reclamation to access.'};
ED.e4_17={from:'branches_break',to:'detecting_access',t:'remind me how the analysis handles branches',w:'The compiler checks all paths and rejects when any path is unsafe.'};

// (e3_4 in ch03.js already connects detecting_access → cpp_skip)
