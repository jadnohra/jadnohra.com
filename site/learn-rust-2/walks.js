// ═══ WALKS ═══
// Each walk is a sequence of edge IDs tracing a path through the graph.
// The first edge's `from` is the entry node.

W.textbook=[
  // About
  'e_about', // about → familiar
  // Ch01: First Contact
  'e1',     // familiar → rejected
  'e2',     // rejected → rejected_more
  'e7',     // rejected_more → thread_errors
  'e13',    // thread_errors → dangling_detail

  // Ch02: S×T×C
  'e9',     // dangling_detail → physics
  'e25',    // physics → primitives
  'e27',    // primitives → bugs_table
  'e29',    // bugs_table → uaf
  'e38',    // uaf → dangling_ptr
  'e40',    // dangling_ptr → double_free
  'e44',    // double_free → data_race
  'e48',    // data_race → buffer_overflow
  'e51',    // buffer_overflow → uninit_read
  'e53',    // uninit_read → memory_leak
  'e55',    // memory_leak → paradigms
  'e58',    // paradigms → features

  // Ch03: The Analysis
  'e3_13',  // features → bindings
  'e3_1',   // bindings → manage_space
  'e3_2',   // manage_space → compiler_needs
  'e3_3',   // compiler_needs → detecting_access

  // Ch04: What Defeats the Analysis
  'e3_4',   // detecting_access → cpp_skip
  'e4_1',   // cpp_skip → ram_flat
  'e4_2',   // ram_flat → cross_scope
  'e4_3',   // cross_scope → invisible_dup
  'e4_4',   // invisible_dup → other_langs
  'e4_5',   // other_langs → branches_break
  'e4_6',   // branches_break → perfect_impossible
  'e4_7',   // perfect_impossible → sound_complete
  'e4_8',   // sound_complete → names_exact

  // Ch05: How Rust Patches the Obstacles
  'e4_9',   // names_exact → constrained_refs
  'e5_1',   // constrained_refs → ownership_deep
  'e5_2',   // ownership_deep → notation
  'e5_3',   // notation → sigs_lifetimes
  'e5_4',   // sigs_lifetimes → desugaring
  'e5_5',   // desugaring → using_managing
  'e5_6',   // using_managing → move_default
  'e5_7',   // move_default → struct_lifetimes
  'e5_8',   // struct_lifetimes → mut_binding
  'e5_9',   // mut_binding → shared_exclusive
  'e5_10',  // shared_exclusive → composing_vocab

  // Ch06: The Approximation
  'e5_11',  // composing_vocab → coord_dangle
  'e6_1',   // coord_dangle → derived_sync
  'e6_2',   // derived_sync → cant_distinguish
  'e6_3',   // cant_distinguish → borrowing_rule_deep
  'e6_4',   // borrowing_rule_deep → rule_every_type
  'e6_5',   // rule_every_type → ch1_and_rule
  'e6_6',   // ch1_and_rule → cpp_threads_rule

  // Ch07: When the Approximation Rejects Valid Code
  'e6_7',   // cpp_threads_rule → compiler_granularity
  'e7_1',   // compiler_granularity → skip_decidable
  'e7_2',   // skip_decidable → sigs_lose_field
  'e7_3',   // sigs_lose_field → escape_hatches
  'e7_4',   // escape_hatches → hints_space
  'e7_5',   // hints_space → hints_time
  'e7_6',   // hints_time → hints_coords

  // Ch08: Programming with Linear Types
  'e7_7',   // hints_coords → data_shape
  'e8_1',   // data_shape → flatten_index
  'e8_3',   // flatten_index → shared_own
  'e8_4',   // shared_own → interior_mut
  'e8_5',   // interior_mut → split_partition
  'e8_6',   // split_partition → clone_diverge
  'e8_7',   // clone_diverge → msg_passing
  'e8_8',   // msg_passing → ds_linearity
  'e8_9',   // ds_linearity → algo_patterns
  'e8_10',  // algo_patterns → hints_patterns
  'e8_11',  // hints_patterns → when_unsafe
];
