---
title: "Distributed Protocol Primitives"
date: 2026-03-08
categories: [Systems, Computing]
tags: [distributed-systems, protocols, consensus, cache-coherence, paxos, raft]
toc: true
description: >-
  Every distributed protocol is built from seven recurring message-exchange
  primitives. This catalog identifies them across MESI, Paxos, Raft, 2PC,
  and others — showing how the same patterns compose under different names.
---

Every distributed protocol is built from a small number of recurring message-exchange patterns. These patterns appear in cache coherence, consensus, replication, locking, file systems, and transaction processing. Each protocol names them differently. An engineer who learns MESI cache coherence and an engineer who learns Paxos encounters the same building blocks under different names.

The distributed systems (DS) literature organizes by problem (consensus, broadcast, mutual exclusion) and by system model (synchronous, asynchronous, Byzantine). The layer between, how the messages actually flow within a protocol, has not been cataloged separately. This document does so.

The primitives are ordered so that each one builds only on primitives the reader already knows.

---

## 1. Request-Grant

One process asks another for something. The receiver decides and responds with a grant or denial.

**1a. Single granter.** One designated process decides. The simplest form.

**1b. Quorum grant.** The request goes to many processes. A majority must grant for the request to succeed.

On denial: the requester's behavior depends on the protocol. It may retry immediately (MESI: the directory queues the request until the resource is available). It may retry with a higher token (Paxos: proposer picks a higher ballot and tries again, see monotonic ordering token, primitive 4). It may give up (database: transaction aborts and reports failure to the application).

| Protocol | Variant | Request | Granter | What is granted |
|---|---|---|---|---|
| MESI | 1a | BusRd / BusRdX | Directory | Read copy / write permission |
| Paxos | 1b | Prepare(n) | Acceptors (majority) | Promise not to accept lower ballots |
| Mutex | 1a | lock() | OS scheduler | Access to critical section |
| Raft | 1b | RequestVote | Followers (majority) | Vote for leadership |
| NFS v4 | 1a | OPEN | Server | File delegation |
| Database | 1a | SELECT FOR UPDATE | Lock manager | Row lock |

---

## 2. Broadcast-and-Collect

One process sends the same request to many processes, then waits for responses. This is N × request-grant (1) with a barrier that blocks until enough responses arrive.

**2a. Unanimity.** All must respond. One slow or crashed process blocks everyone. Same vulnerability as revoke-and-confirm (3a): the broadcaster is stuck until all respond. Protocols that use this variant either assume no crashes (MESI, hardware) or accept being a blocking protocol (2PC).

**2b. Quorum.** A majority must respond. Resilient: the broadcaster can proceed even if a minority of processes crash or are slow. If fewer than a majority respond, the broadcaster retries or aborts, but it is never permanently stuck by a single crash. *(Uses the "quorum intersection" trick: any two quorums overlap, so information accepted by one quorum cannot be missed by the next.)*

| Protocol | Variant | What is broadcast | What is collected |
|---|---|---|---|
| MESI invalidation | 2a | "Discard your copy" | Acknowledgments |
| 2PC Phase 1 | 2a | "Can you commit?" | Votes (yes/no) |
| Paxos Phase 1 | 2b | Prepare(n) | Promises + accepted values |
| Paxos Phase 2 | 2b | Accept(n, v) | Accepted confirmations |
| Raft AppendEntries | 2b | Log entries | Success acknowledgments |

---

## 3. Revoke-and-Confirm

One process tells others to give something up, then waits for confirmations before proceeding. Same message shape as broadcast-and-collect (2), but the semantics differ: broadcast-and-collect gathers information or agreement, revoke-and-confirm takes something away.

**3a. Unanimity.** Wait for ALL confirmations. One missing confirmation blocks the entire operation. If a process crashes or is unreachable, the revoker is stuck. Protocols handle this differently: MESI assumes the hardware does not crash (the directory waits indefinitely). 2PC has the same vulnerability (a crashed participant blocks the entire transaction, which is why 2PC is called a "blocking" protocol). Systems that cannot afford to block use quorum revocation (3b) or timeouts that treat absence as confirmation. *(Uses the "scope the barrier" trick: the barrier includes only the processes that hold the resource, not all processes in the system.)*

**3b. Quorum.** Wait for a MAJORITY. More resilient but requires reasoning about what the non-responding minority might do.

| Protocol | Variant | Revocation message | Who confirms | What is revoked |
|---|---|---|---|---|
| MESI | 3a | Invalidation | All readers | Read copies of a cache line |
| 2PC | 3a | Prepare | All participants | Right to abort unilaterally |
| Paxos Phase 1 | 3b | Prepare(n) | Majority of acceptors | Ability of lower-ballot proposers to succeed |
| NFS v4 | 3a | CB_RECALL | Delegation holder | File delegation |
| Ceph | 3a | Capability revocation | Clients with caps | Read/write capabilities |

---

## 4. Monotonic Ordering Token

A strictly increasing number attached to authority. Receivers reject requests with a number lower than the highest they've seen.

The motivating scenario: authority transfers from process A to process B. Process A does not know it lost authority yet (lazy discovery, or network delay, or a long pause). Process A sends a request. Process B also sends a request. Both arrive at a recipient. Without a token, the recipient cannot tell which is from the current authority and which is stale. With a monotonic token, the recipient compares: higher number is current, lower number is stale and rejected.

**4a. Centrally generated.** One process issues the numbers. Simple, but the generator is a single point of failure.

**4b. Locally generated.** Each process picks its own number, high enough to supersede all previously seen. No coordination needed to generate, but conflicts are resolved by comparison. *(This is the basis of the "higher number supersedes" trick.)*

**4c. Per-process logical.** Every process maintains its own counter, merged on communication (take the maximum, then increment). Captures causal ordering without a central authority. *(Uses "piggyback state on existing messages": the counter travels on messages already being sent.)*

| Protocol | Variant | Name of the token | Who generates it | Who checks it |
|---|---|---|---|---|
| ZooKeeper | 4a | zxid / znode version | Leader | Followers |
| Kleppmann's fencing | 4a | Fencing token | Lock service | Storage service |
| Paxos | 4b | Ballot number | Proposer | Acceptor |
| Raft | 4b | Term | Candidate | All nodes |
| Lamport clocks | 4c | Logical timestamp | Every process | Every process |
| Database | 4a | Transaction ID / LSN | Transaction manager | Recovery system |

---

## 5. Authority Transfer

Authority over a resource moves from one process to another. The hard problem: at no moment should two processes both believe they hold authority, and at no moment should authority be absent (unless the protocol tolerates a gap). The transfer must appear atomic to the rest of the system.

**5a. Coordinated.** A central process manages the transfer. A requesting process sends a request-grant (1) to the coordinator. The coordinator sends a surrender message to the old holder, waits for confirmation (which may involve flush-before-handoff (6) if the holder has dirty state), then grants authority to the requester. The gap is zero because the coordinator sequences both steps and does not grant until surrender is confirmed.

**5b. Competitive, unique tokens.** No coordinator. Each process generates a globally unique monotonic ordering token (4) using the "local value + global identity" trick (e.g., `(round, process_id)` with lexicographic comparison), so no two processes ever generate the same token. A process that wants authority picks the next token higher than any it has seen and broadcasts its claim (broadcast-and-collect, 2b). Each recipient compares the incoming token with the highest it has previously seen. If the new token is higher, the recipient grants support. If not, it rejects. If the claimant collects enough support (a quorum), it holds authority. There is no explicit surrender step. Authority shifts because the rest of the system stops recognizing the old token ("higher number supersedes" trick). The old holder discovers it lost authority when its next request is rejected. Ties are impossible by construction. *(See Appendix A for a step-by-step derivation.)*

**5c. Competitive, with split vote.** Same as 5b but tokens are not globally unique (the "local value + global identity" trick is not used). Two processes may pick the same token. Each recipient votes for at most one claimant per token value (first come, first served). If neither claimant collects a quorum, the round fails: a split vote. Both claimants wait a random duration and retry with a higher token. The randomness makes repeated collisions unlikely. Eventually one goes first and wins. *(Raft uses this variant. Paxos avoids it by using 5b.)*

| Protocol | Variant | What transfers | From | To | Via |
|---|---|---|---|---|---|
| MESI | 5a | Write permission | Current holder | Requester | Directory |
| Raft | 5c | Leadership | Old leader | New leader | Election (highest term wins) |
| Suzuki-Kasami | 5a | Token | Holder | Requester | Token message via coordinator |
| Paxos | 5b | Proposer role | Old proposer | New proposer | Higher ballot supersedes |
| ZooKeeper | 5a | Ephemeral lock | Crashed holder | Next in queue | Session expiry + watch |

---

## 6. Flush-Before-Handoff

Before surrendering authority, the holder must make its private state visible. Without this, the private state is lost when authority transfers. In MESI: if the holder surrenders write permission without flushing, the main store is stale. Shared copies can later be silently evicted. If all copies are evicted while the main store is still stale, the value is gone permanently. The flush prevents this by making the main store current before anyone else gets a copy.

**6a. Flush to shared store.** The holder writes to a common location before surrendering. Any future reader can fetch from the shared store. *(Uses two tricks: "flush enables free eviction" of copies later, and "messages establish causality for free" since the flush creates a happens-before edge between the writer and all future readers.)*

**6b. Flush to successor directly.** The holder sends its state directly to the next authority, bypassing the shared store. *(Uses "messages establish causality for free": the direct transfer carries both data and a happens-before edge.)*

| Protocol | Variant | What is flushed | Where | Why |
|---|---|---|---|---|
| MESI | 6a | Dirty cache line | Main store (RAM) | Shared copies can be silently evicted; if main store is stale and all copies are evicted, the value is lost |
| MOESI | 6b | Dirty cache line | Requesting cache directly | Avoids expensive write to main store |
| Database | 6a | Write-ahead log | Disk | Writes must be recoverable after coordinator crash |
| Raft | 6b | Uncommitted log entries | Follower logs | A new leader must have all committed entries |

---

## 7. Silent Withdrawal

A process leaves a group without notifying anyone. The group's tracking metadata becomes imprecise.

The alternative is explicit notification: send a message on every departure. The tradeoff: explicit notification keeps metadata accurate but costs a message every time a process leaves. Silent withdrawal saves that message at the cost of occasional imprecision, handled when discovered. In systems where departures are frequent and cheap (MESI: cache eviction happens constantly), the savings dominate.

**7a. Discovered on next interaction.** The protocol encounters the stale metadata and handles it gracefully. *(Uses "lazy metadata" trick: the metadata is allowed to be imprecise, and staleness is resolved on discovery rather than prevented.)*

**7b. Discovered by timeout.** Absence is detected by time passing without a heartbeat or renewal.

| Protocol | Variant | Who leaves | What they held | Consequence |
|---|---|---|---|---|
| MESI | 7a | Controller evicting a shared entry | Read copy | Directory sends invalidation, controller acks immediately (nothing to discard) |
| TCP | 7b | Crashed peer | Connection state | Timeout detects absence |
| Lease systems | 7b | Expired lease holder | Lease | Lease expires, coordinator reclaims |
| Gossip protocols | 7b | Crashed node | Membership | Failure detector eventually notices |

---

## Smart Tricks

The primitives alone do not explain why protocols work well. What makes protocols elegant is a set of recurring tricks that reduce messages, avoid coordination, or get guarantees for free. These tricks feel clever the first time you encounter them. They appear across unrelated protocols because the same constraints produce the same solutions.

**Higher number supersedes.** No need to ask the old holder to surrender. Just show a higher number. Every receiver enforces this independently. The old holder is not contacted, not asked, not even aware it lost authority. Authority shifts because the rest of the system stops recognizing the old number. Used in: Paxos ballots, Raft terms, fencing tokens. This is what makes competitive authority transfer (5b) possible without a coordinator.

**Quorum intersection.** Any two majorities share at least one member. So if a value was accepted by a majority, any future majority must include someone who knows about it. No process needs to talk to every other process. A majority is enough because any two majorities overlap. Used in: Paxos (both phases), Raft (election and replication), quorum reads/writes in distributed databases.

**Lazy metadata.** Don't keep metadata perfectly accurate. Let it go stale. Handle the staleness gracefully when discovered. In MESI: the directory's reader set lists a controller that already evicted its copy. When the directory sends an invalidation, the controller has nothing to discard and acknowledges immediately. The protocol works. The cost of perfect metadata (a message on every eviction) is higher than the cost of occasional wasted messages. Used in: MESI silent eviction, gossip protocol membership, lease-based systems where the coordinator discovers expiry lazily.

**Flush enables free eviction.** Silent withdrawal (7) is safe only because flush-before-handoff (6) happened earlier. The main store was made current, so any copy can vanish without loss. The expensive operation (flush) happens once, enabling the cheap operation (silent discard) to happen many times. One payment, many benefits. Used in: MESI (flush to main store enables silent eviction of shared copies), database checkpointing (flush enables log truncation).

**Piggyback state on existing messages.** Instead of sending separate tracking messages, attach your current knowledge to messages you are already sending. No extra round trips; the information travels for free. Lamport clocks: every message carries the sender's logical timestamp, so the receiver inherits causal history without requesting it. Paxos: promises carry the highest accepted value, so the proposer learns the system's state from the same messages that grant it permission. Used in: Lamport clocks, vector clocks, Paxos promise responses, MESI snoop responses that carry the cache line data along with the state change.

**Messages establish causality for free.** A protocol message that transfers data simultaneously establishes a Lamport happens-before edge between sender and receiver. The flush in MESI is not just "make the main store current." It also establishes that everything the writer did before the flush causally precedes everything any future reader of that value does. The protocol does not need a separate mechanism for causal ordering; the message exchanges that enforce the coherence constraint provide it as a side effect. This is why Release/Acquire atomics work: Release is a flush, Acquire is a read from the flushed location, and the happens-before edge comes for free from the message the hardware already sent.

**Local value + global identity = globally unique without coordination.** A local value (counter, timestamp, round number) is unique within one process but can collide across processes. Append the process identity. Now it is globally unique. But uniqueness alone is not enough: every participant must be able to compare any two tokens and get the same answer. This requires a total ordering rule.

Two implementations. *Lexicographic comparison*: the token is a tuple `(round, process_id)`. Compare round first; higher round wins. If rounds are equal, compare process ID; higher ID wins. Every participant applies the same two-step comparison and reaches the same conclusion. This is what Paxos uses. *Modular arithmetic*: flatten the tuple into a single number. Process `i` out of `N` processes only uses numbers where `number mod N == i`. Process 0 uses 0, 3, 6, 9... Process 1 uses 1, 4, 7, 10... The process identity is baked into the number. Comparison is plain integer comparison. Simpler but wastes number space.

Either way, any two tokens from different processes are always different and every participant computes the same ordering without coordination.

Lamport 1978 used this to turn a partial order into a total order: logical timestamps can tie, so append the process ID and break ties deterministically. Paxos ballots use the same structure. Twitter's Snowflake IDs apply it at scale: `(timestamp, machine_id, sequence)`, producing globally unique sortable IDs across thousands of machines with zero coordination. Database sharded keys, UUIDs, and distributed sequence generators all use the same trick.

Used in: Lamport total order (1978), Paxos ballot numbers, Raft (does NOT use this trick, allowing ties which it handles with random backoff and retry), Snowflake IDs, sharded database keys.

**Scope the barrier.** Revoke-and-confirm (3a) waits for N acknowledgments. But N is not all processes in the system. It is only the processes in the reader set for that specific entry. The directory scopes the barrier to exactly the processes that matter. Every other process in the system is unaffected. This is why MESI scales: a write to an uncontended entry involves zero messages, because the barrier has zero participants.

---

## Conservation of Difficulty

A distributed protocol enforces a constraint on distributed state. The difficulty of enforcing it cannot be eliminated. It can only be moved.

Every primitive has variants. The variants are not ranked: none is strictly better than another. They differ in where the difficulty lives.

| Where difficulty goes | What it costs | Who pays |
|---|---|---|
| A coordinator | Single point of failure, bottleneck | System reliability |
| Message count | More rounds, more bandwidth | Network and latency |
| Time | Wasted rounds, retries | Progress speed |
| Design complexity | Clever token schemes, quorum arithmetic | Protocol designer |
| Assumptions | Simpler protocol, but only correct if assumptions hold | Whoever guarantees the assumptions |

Assumptions are the most powerful way to move difficulty. MESI uses all the simplest variants because the hardware guarantees no crashes (silicon doesn't stop mid-protocol), no message loss (the bus/interconnect is reliable), and bounded latency (electrical signals have predictable timing). Those guarantees aren't free. They are paid for by the hardware engineers who designed the silicon. The difficulty moved to them. The protocol is simple because someone else absorbed the hard parts.

Paxos uses harder variants because it assumes crashes happen, messages can be arbitrarily delayed, and only a majority survives. It cannot rely on anyone else's guarantees. The difficulty stays in the protocol itself. DS theory calls these choices "system models" (crash-stop, crash-recovery, Byzantine, synchronous, asynchronous, partially synchronous). Each model is a set of assumptions. Choosing a stronger model (fewer failures possible) simplifies the protocol. Choosing a weaker model (more failures possible) forces the protocol to handle more, using harder variants of the same primitives.

Authority transfer illustrates this clearly:

| Variant | What you pay | Where difficulty lives | Progress |
|---|---|---|---|
| 5a Coordinated | Single point of failure, bottleneck | Coordinator reliability | Deterministic |
| 5b Unique tokens | Token design, more messages | Token generation + quorum logic | Deterministic |
| 5c Split vote | Wasted rounds on ties | Retry + random backoff | Probabilistic |

No variant has zero cost. If one were strictly better, the others would not exist. They exist because different systems have different answers to "where can I afford to put the difficulty?"

MESI uses all the simplest variants (all "a" variants: single granter, unanimity, coordinated transfer, flush to shared store) because the hardware can afford a coordinator. The directory is built into silicon. It does not crash. It does not partition. The difficulty is placed in a coordinator that the hardware guarantees is reliable.

Paxos uses harder variants (quorum grant, competitive transfer, quorum collect) because its coordinator CAN crash. It cannot afford to place the difficulty in a single reliable process, so it distributes the difficulty across quorums. The cost shows up as more messages and more design complexity.

The impossibility results in distributed systems (FLP, 1985) are the formal proof that the difficulty is conserved: no asynchronous protocol can guarantee agreement, termination, and fault tolerance simultaneously. One must be weakened. The choice of which one to weaken IS the choice of where to put the difficulty.

---

## Relations

The seven primitives are not all independent. Some are compositions of others, some are sequential dependencies, and some are degenerate cases.

**Atomic primitives.** Request-grant (1) is the elementary message exchange: one request, one response. Monotonic ordering token (4) is not a message exchange but metadata attached to other primitives, giving receivers a way to reject stale requests. These two are the true atoms.

**Shared structure.** Broadcast-and-collect (2) and revoke-and-confirm (3) are the same message shape: send to N targets, wait for a threshold of responses. The difference is semantic. Revoke-and-confirm asks targets to give something up. Broadcast-and-collect asks targets for information or agreement. Both decompose into: N parallel request-grants (1) plus a barrier that blocks until enough responses arrive.

**Sequential dependency.** Flush-before-handoff (6) is a precondition on authority transfer (5). When the current holder has dirty state, the flush must complete before the transfer can proceed. The ordering is strict: 6 then 5, never reversed.

**Degenerate case.** Silent withdrawal (7) is what happens when a process skips its role in revoke-and-confirm (3). Instead of waiting for a revocation message and sending a confirmation, the process simply disappears. The protocol must handle discovering the missing participant later, typically by treating a revocation to an absent process as an immediate acknowledgment.

---

## Composition

Every protocol composes these primitives.

**MESI cache coherence** uses: request-grant (1a), broadcast-and-collect (2a), revoke-and-confirm (3a), authority transfer (5a), flush-before-handoff (6a), silent withdrawal (7a). All simplest variants.

**Paxos** uses: request-grant (1b), broadcast-and-collect (2b), authority transfer (5b), monotonic ordering token (4b).

**Two-phase commit** uses: broadcast-and-collect (2a), revoke-and-confirm (3a).

**Raft** uses: request-grant (1b), broadcast-and-collect (2b), authority transfer (5c), flush-before-handoff (6b), monotonic ordering token (4b).

**Distributed file locking (NFS v4)** uses: request-grant (1a), authority transfer (5a), revoke-and-confirm (3a), silent withdrawal (7b).

The protocols look different because they use different names for the same primitives, solve different high-level problems, and operate under different failure models. The message-exchange patterns underneath are the same seven.

---

## Note

Implementation-level distributed systems patterns have been cataloged, notably by Unmesh Joshi in *Patterns of Distributed Systems* (Addison-Wesley, 2023), which describes patterns like Write-Ahead Log, Generation Clock, Leader and Followers, and Consistent Core. These describe what a system component does.

This catalog operates one level below: the message-exchange shapes that appear inside those solutions. Joshi's "Generation Clock" uses what we call a monotonic ordering token (primitive 4). His "Leader and Followers" composes authority transfer (5) with request-grant (1). The implementation patterns are built from these primitives, but the primitives themselves have not been separately cataloged.

The distributed systems literature organizes knowledge by problem (consensus, broadcast, mutual exclusion) and by system model (synchronous, asynchronous, crash-fault, Byzantine). This catalog addresses the layer between: the recurring message-exchange patterns that compose to build every protocol.

---

## Appendix A: Deriving Competitive Authority Transfer

How would you design authority transfer without a coordinator? Walk through it step by step.

**Step 1: The intuition.** If everyone agrees on who has the highest number, that process is the authority. Simple. Numbers are totally ordered. Comparison is deterministic. Every participant independently reaches the same conclusion about who wins. No coordination needed for the decision itself.

**Step 2: Problem: two processes pick the same number.** Two solutions exist. Make numbers globally unique by combining the round number with the process identity ("local value + global identity" trick), so ties are structurally impossible (Paxos: `(round, process_id)`, every pair of tokens has a deterministic winner). Or allow ties and handle them with retry (Raft: each participant votes for at most one claimant per number, if nobody gets a quorum, both retry with a random delay).

**Step 3: Problem: concurrent claims.** Two processes broadcast claims at the same time, with different numbers. Each recipient sees both. The higher one wins everywhere, because comparison is local and deterministic. The lower claimant fails to collect a quorum because the recipients already committed to the higher number.

**Step 4: Problem: the old authority doesn't know it lost.** It is not informed. It finds out lazily, when its next request is rejected because its token is stale. This is safe because every other participant already enforces the higher number. The old authority's actions are rejected by the rest of the system even before it knows it lost. Safety does not depend on the old holder stopping promptly.

Every step is a problem and a trick that solves it. The core insight: comparison is local and deterministic, so consensus on who holds authority emerges from each participant independently applying the same rule.
