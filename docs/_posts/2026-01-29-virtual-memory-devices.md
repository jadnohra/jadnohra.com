---
title: "Abstractor: Virtual Memory and Devices"
date: 2026-01-29
categories: [Systems, Computing]
tags: [linux, memory, virtual-memory, mmu, dma, iommu, devices]
toc: true
---

## Memory Controller - RAM Protocol Abstraction

A CPU reads and writes values at addresses. RAM stores the values, but the protocol for accessing DRAM is complex. DRAM chips organize storage into rows and columns, require periodic refresh to retain data, and impose timing constraints between operations. A CPU cannot issue raw addresses and expect data back.

The memory controller abstracts this complexity. It accepts addresses from the CPU, translates each into the required sequence of DRAM commands (row address strobe, column address strobe, read/write signals), tracks which rows are active, schedules refresh cycles, and interleaves requests across banks. What the CPU sees is a linear address space: issue an address, receive data.

```
CPU ──► Memory Controller ──► RAM
```

**Details:** DRAM stores bits in capacitors arranged in a two-dimensional array. Each capacitor holds one bit. Accessing a bit requires specifying its row and column. The protocol proceeds in phases: the controller asserts RAS (Row Address Strobe) with the row address, which loads the entire row into a sense amplifier buffer. Then it asserts CAS (Column Address Strobe) with the column address, which selects specific bits from the buffer. The data transfers on subsequent clock edges.

Timing parameters constrain every transition. tRCD specifies minimum delay between row activation and column access. tRP specifies minimum delay to precharge before activating a different row. tRFC specifies refresh cycle duration. Violating these timings corrupts data.

Modern DRAM divides memory into independent banks (typically 8-16). Each bank has its own row buffer. The controller interleaves requests across banks: while one bank waits for tRCD, another serves data. This hides latency. The controller maintains state for each bank: idle, active with row N open, precharging. It reorders requests to maximize row buffer hits and minimize precharge cycles.

Refresh runs continuously. Each row must be refreshed within a retention window (typically 64ms). The controller issues REF commands at regular intervals, cycling through all rows. During refresh, the bank is unavailable.

**History:** The PCI bus specification of 1991 established the memory controller as a dedicated chip on the motherboard. AMD moved it onto the CPU die in 2003 with the Athlon 64. Intel followed in 2008 with Nehalem.

**Jargon:** Engineers drew motherboard layouts as maps, CPU at top.

```
        ┌─────┐
        │ CPU │
        └──┬──┘
           │
      ┌────┴────┐
      │Northbridge│  ← memory, graphics (fast, near CPU)
      └────┬────┘
           │
      ┌────┴────┐
      │Southbridge│  ← disk, USB, audio (slow, far from CPU)
      └─────────┘
```

*Northbridge*: the chip near the CPU handling memory and graphics. *Southbridge*: the chip farther away handling disk and USB. The northbridge was the memory controller. When AMD and Intel moved it onto the CPU die, the northbridge disappeared as a separate chip and the term fell out of use.

---

## MMU - Address Space Abstraction

A process wants simple memory: a linear address space starting at zero. It does not want to know which physical RAM chips exist, which addresses are already used by other processes, or where the kernel decided to place its data. Physical memory is the hardware's concern, not the program's.

The MMU (Memory Management Unit) provides this abstraction. It intercepts every memory access—every load, store, and instruction fetch—and translates the virtual address the process uses into a physical address that reaches RAM. Every single memory access goes through this translation. A program running at 3 GHz might issue billions of memory accesses per second, each one translated by the MMU.

```
Process ──► virtual address 0x1000 ──► MMU ──► physical address 0x50000 ──► RAM
```

The process issues address 0x1000. The MMU translates it to physical 0x50000. The process never sees 0x50000—it only knows about its own virtual addresses.

This also enables isolation. Two processes can both use virtual address 0x1000. The MMU gives each its own mapping:

```
Process A (0x1000) ──► MMU ──► Physical 0x50000
Process B (0x1000) ──► MMU ──► Physical 0x80000
```

Each process sees a private linear address space. The MMU multiplexes them onto physical RAM.

**Details:** The MMU maintains a mapping from virtual addresses to physical addresses. This mapping lives in a page table, a data structure stored in RAM. Each process has its own page table, defining its own address space.

The CPU needs to know which page table to use. A register (CR3 on x86) holds the physical address of the active process's page table. When the OS switches from process A to process B, it writes B's page table address to CR3. That single register write switches the entire address space—the MMU now translates using B's mappings instead of A's.

Writing to CR3 is a privileged operation. User processes cannot switch their own page tables.

Permission checks happen during translation. If a page is marked read-only and the instruction writes, the MMU raises a fault. If the page is not present, the MMU raises a fault. These faults are synchronous exceptions, delivered before the memory access completes.

**History:** The Atlas computer (1962, Ferranti/Manchester University) introduced the first paged MMU and virtual memory. IBM's System/360 Model 67 (1965) brought it to commercial systems, calling it the Dynamic Address Translation box. Early MMUs were separate chips. Intel integrated the MMU onto the CPU die with the 386 in 1985.

**Jargon:** *Time-sharing*: multiple processes take turns on the CPU, switching fast enough to appear simultaneous. *Concurrent*: interleaved, one at a time, fast switching. *Parallel*: truly simultaneous, multiple CPUs. *MMU*: Memory Management Unit, the hardware performing address translation. *DAT*: Dynamic Address Translation, IBM's term for the same thing. *Virtual address*: the address a process issues. *Physical address*: the address that reaches RAM.

Because the MMU sits in the path of every memory access, translation must be fast. The next sections explain how the mapping is structured (pages, page tables) and how translation is accelerated (TLB).

---

## Pages - Translation Granularity

The MMU translates virtual addresses to physical addresses. But at what granularity?

The extreme option is per-byte mapping: every virtual address has its own entry specifying its physical address. A 48-bit address space has 2^48 addresses. Storing one entry per address is absurd—the mapping metadata would dwarf actual memory.

The solution is pages. Instead of mapping individual addresses, map aligned chunks. A 4KB page contains 4096 contiguous bytes. One page table entry covers all 4096. The mapping metadata shrinks by a factor of 4096.

This forces a structure on addresses. A virtual address splits into two parts:

```
Virtual address (48 bits):
┌────────────────────────────────┬──────────────┐
│         page number            │    offset    │
│          36 bits               │   12 bits    │
└────────────────────────────────┴──────────────┘
```

The split is not arbitrary—it follows directly from page size:
- Page size = 4KB = 2^12 bytes
- Need 12 bits to address any byte within a page (0 to 4095)
- Low 12 bits = offset within the page
- Remaining 36 bits = which page

The MMU translates only the page number. The offset passes through unchanged. If virtual page 5 maps to physical page 0x50, then:
- Virtual 0x5000 → Physical 0x50000
- Virtual 0x5001 → Physical 0x50001
- Virtual 0x5FFF → Physical 0x50FFF

All addresses within the page follow the same translation, differing only in offset.

```
Virtual:  [    page number    |   offset   ]
                   │                 │
                   ▼                 │
             Translation             │
            (page table)             │
                   │                 │
                   ▼                 ▼
Physical: [  physical page   |   offset   ]
                              (unchanged)
```

**Why 4KB?** It's a tradeoff. Smaller pages mean finer granularity (less wasted space when a page is partially used) but more entries to manage. Larger pages mean fewer entries but more internal fragmentation. 4KB became standard in the 1980s and stuck. Modern systems also support huge pages (2MB, 1GB) for workloads that benefit from fewer translations.

**Jargon:** *Page*: a fixed-size aligned chunk of memory (typically 4KB). *Page number*: the high bits of an address identifying which page. *Offset*: the low bits addressing a byte within the page. *Internal fragmentation*: wasted space when allocated memory exceeds what's used.

---

## Page Table - Representation in RAM

The MMU needs a data structure mapping virtual page numbers to physical page numbers. This is the page table, stored in RAM.

A process with a 48-bit virtual address space has 2^36 possible pages (with 4KB pages). Most are unused. A process typically uses thousands of pages, not billions. The mapping is sparse.

A flat array indexed by virtual page number would cost 2^36 entries × 8 bytes = 512 GB per process. This was never practical. The page table must be a sparse data structure.

The page table is a radix tree. It indexes by successive chunks of the page number, allocating nodes only for regions that contain mappings. A process using 1000 pages allocates a few dozen nodes totaling a few hundred KB—not 512 GB.

x86-64 uses a 4-level radix tree. The 36-bit page number splits into four 9-bit indices:

```
48-bit virtual address:
┌────────┬────────┬────────┬────────┬──────────────┐
│ L1 idx │ L2 idx │ L3 idx │ L4 idx │    offset    │
│ 9 bits │ 9 bits │ 9 bits │ 9 bits │   12 bits    │
└────────┴────────┴────────┴────────┴──────────────┘

CR3 ──► L1[idx] ──► L2[idx] ──► L3[idx] ──► L4[idx] ──► physical page
```

Each table level occupies one 4KB page containing 512 entries (512 × 8 bytes = 4KB). Each entry holds either:
- A pointer to the next-level table, or
- At the final level (L4), the physical page number plus permission bits

The radix tree is the page table. There is no other representation. When the MMU needs to translate a virtual page number, it walks this tree in RAM.

**Why radix trees over hash tables?** Both can represent sparse mappings. Radix trees win here for three reasons:

1. **Predictable traversal.** Fixed depth. Four levels, four memory reads, always. Hash tables probe until finding the entry or an empty slot—variable cost.

2. **Subtree deletion.** Unmap a region: clear one pointer, the entire subtree is gone. Hash tables must visit every entry in the range.

3. **Subtree sharing.** Two processes mapping the same library can share subtree nodes. Hash tables cannot share structure.

**Details:** Each page table entry (PTE) contains: physical address of next-level table (or final physical page), present bit, read/write permission, user/supervisor permission, accessed bit, dirty bit. The present bit indicates whether the mapping exists. Permission bits enable read-only mappings, kernel-only pages, and execute-disable.

**History:** Multics (1965) introduced multi-level page tables. x86 used 2-level tables with 32-bit addresses, extended to 3-level with PAE (1995), and 4-level with x86-64 (2003). Intel introduced 5-level paging (2017) to support 57-bit virtual addresses.

**Jargon:** *Page table*: data structure mapping virtual pages to physical pages. *Radix tree*: a tree indexing by successive chunks of the key. *Page table entry (PTE)*: one entry containing a physical address and permission bits. *CR3*: x86 register holding the physical address of the L1 table.

---

## TLB - Caching Translation Results

Walking the page table costs four RAM accesses—one per tree level. Every load, store, and instruction fetch requires translation. Paying four RAM accesses per memory access would be catastrophic.

The TLB (Translation Lookaside Buffer) caches the results of page table walks. It stores final mappings: virtual page number → physical page number. Not tree nodes. Not pointers. Just the end result.

```
┌────────────────────────────────────────┐
│                CPU die                 │
│  ┌──────────┐  ┌──────────┐           │
│  │Execution │  │   TLB    │           │
│  │  Units   │──│  (SRAM)  │           │
│  └──────────┘  └────┬─────┘           │
│                     │                  │
│                ┌────┴─────┐           │
│                │   MMU    │           │
│                └────┬─────┘           │
│                     │                  │
│                ┌────┴─────┐           │
│                │ Memory   │           │
│                │Controller│           │
│                └────┬─────┘           │
└─────────────────────┼──────────────────┘
                      │ memory bus
              ┌───────┴───────┐
              │  DRAM chips   │
              └───────────────┘
```

Every memory access checks the TLB first:

```
Virtual address from process
           │
           ▼
┌─────────────────────────────┐
│ page number  │    offset    │
└──────┬──────────────────────┘
       │
       ▼
   TLB lookup
       │
       ├── hit: return physical page (1 cycle)
       │
       └── miss: walk page table in RAM
                        │
                        ▼
              ┌─────────────────────────────────┐
              │ Page table walk (4 RAM accesses)│
              │                                 │
              │ CR3 → L1 → L2 → L3 → L4        │
              │                                 │
              │ Result: page 2 → phys 0x50     │
              │ (cache this mapping in TLB)    │
              └─────────────────────────────────┘
```

A hit returns the physical page in one cycle. A miss walks the radix tree in RAM, taking roughly 100 cycles, then caches the result for future accesses.

Consider summing an array starting at virtual 0x5000. Element 0 at 0x5000, element 1 at 0x5004, element 2 at 0x5008. All fall within page 5 (0x5000–0x5FFF). The first access misses, walks the tree, caches "page 5 → physical 0x50". Subsequent accesses hit. One miss, many hits.

A 256-entry TLB covering 4KB pages spans 1MB of working memory. Most programs fit their hot path in far less.

**Details:** The TLB is a content-addressable memory (CAM). Regular RAM takes an address and returns data. A CAM takes data and searches all entries in parallel. The MMU presents a virtual page number; every TLB entry compares against it simultaneously. Parallel comparison completes in one cycle.

Each TLB entry stores: virtual page number, physical page number, permission bits, valid bit, and address space identifier (ASID). The ASID tags which process owns the entry, allowing multiple processes to share the TLB without collisions.

The TLB uses SRAM—fast, expensive, on-die. RAM uses DRAM—slower, denser, off-chip. SRAM completes a lookup in one cycle. DRAM takes 50-100 cycles.

Modern CPUs have multi-level TLBs. L1 TLB: 16-64 entries, 1 cycle. L2 TLB: 512-2048 entries, 4-8 cycles. Miss in both triggers a page table walk.

**Huge pages** reduce TLB pressure. A 2MB page covers 512× more memory than a 4KB page. The same TLB covers 512× more address space. Database servers and VMs use huge pages heavily.

**Paging structure caches** are separate from the TLB. They cache intermediate page table nodes (L1, L2, L3 entries), reducing walk cost when only the final level differs.

**History:** The Atlas computer (1962) included the first TLB. Without it, virtual memory meant two RAM accesses per memory access—one for translation, one for data. The TLB eliminated the translation access 95-99% of the time, making virtual memory practical.

**Jargon:** *TLB*: Translation Lookaside Buffer, caches virtual→physical mappings. *CAM*: Content-Addressable Memory. *ASID*: Address Space Identifier, tags entries by process. *Huge pages*: 2MB or 1GB pages reducing TLB pressure.

---

## Page Faults

When the MMU cannot complete a translation, it raises an exception called a page fault. The CPU saves the faulting address in a register called CR2 and traps to the kernel.

**How it works:** The MMU has an output line for exceptions. When a lookup fails (page not present) or permission check fails (user accessing supervisor page, write to read-only), the MMU asserts this line instead of completing the access.

```
MMU lookup fails
      │
      ▼
┌─────────────────────────────────────────┐
│ MMU raises exception:                   │
│   1. Block the memory access            │
│   2. Store faulting address in CR2      │
│   3. Assert interrupt line              │
└────────────────────┬────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────┐
│ CPU sees interrupt:                     │
│   1. Save current state                 │
│   2. Switch to ring 0                   │
│   3. Jump to fault handler (from IDT)   │
└─────────────────────────────────────────┘
```

The kernel examines CR2 (faulting address) and the error code (why it faulted) and decides how to proceed. A reference to an unmapped address in user code typically terminates the process with a segmentation fault. A reference to a page marked "not present" may trigger allocation or loading from disk.

## Kernel Techniques

The kernel configures the MMU, so programs do not need to know about physical addresses or page tables. What leaks through are page faults and timing. Programs see segfaults when they access invalid memory. Mapped and unmapped access times differ measurably.

Page faults enable several memory management techniques.

**Lazy allocation** delays physical memory allocation until first access.

```
1. Process starts. Pages marked "not present" in table.
2. Process accesses 0x1000.
3. Page fault.
4. Kernel allocates physical page, updates table, restarts instruction.
5. Process continues, unaware.
```

Most programs do not use all their memory. Lazy allocation allocates only what is touched.

**Lazy copying** makes fork efficient.

```
1. Fork: child gets same page table, pages marked read-only.
2. Either process writes.
3. Page fault (write to read-only).
4. Kernel copies just that page, updates tables, restarts.
5. Process continues, unaware.
```

Most forked processes call exec() immediately. Lazy copying copies nothing until needed.

**Disk overflow** extends physical memory onto disk.

```
1. RAM full, need a page.
2. Pick a victim page, write to disk.
3. Mark victim's entry "not present".
4. Use freed physical page.
5. Later: victim accessed, page fault, load from disk.
```

Disk overflow allows more programs to run than fit in RAM. The cost is substantial. Disk access is roughly 100,000 times slower than RAM. Overcommitting memory leads to thrashing.

## Devices

Some physical addresses do not correspond to RAM. The memory bus routes addresses to their destinations.

```
              ┌──► RAM     (0x00000000 - 0xBFFFFFFF)
MMU ──► Bus ──┼──► NIC     (0xC0000000 - 0xC00000FF)
              └──► Disk    (0xC0000100 - 0xC00001FF)
```

**How it works:** Address decoding is hardware. Comparators examine the high bits of the physical address and activate the appropriate chip-select line.

```
Physical address arrives on bus
         │
         ▼
┌─────────────────────────────────────────┐
│ Address decoder (combinational logic)   │
│                                         │
│  if addr < 0xC0000000 → RAM_CS = 1      │
│  if addr in NIC range → NIC_CS = 1      │
│  if addr in Disk range → DISK_CS = 1    │
│                                         │
└─────────────────────────────────────────┘
         │
         ├──► RAM chip (listens if RAM_CS high)
         ├──► NIC chip (listens if NIC_CS high)
         └──► Disk chip (listens if DISK_CS high)
```

Only one chip responds. This is fixed in hardware. The mapping is determined by motherboard design and configured at boot via firmware.

A write to 0xC0000000 does not store data. It sends a command to the network card. A read from 0xC0000004 does not retrieve stored data. It queries device status.

This is the same pattern as syscalls. The syscall API presents `read(fd, buf, n)` as "read bytes," but fd is a handle and the kernel decides what read means for that handle. MMIO presents `mov [addr], val` as "store to memory," but the address is a handle and hardware decides what the store means for that address. Both are thin APIs. Uniform syntax hides arbitrary operations behind a handle.

| Interface | Syntax | Handle | Who interprets |
|-----------|--------|--------|----------------|
| Syscall | read/write/ioctl | file descriptor | Kernel |
| MMIO | load/store | address | Hardware routing |

User processes cannot access device addresses because the kernel does not create mappings for them in user page tables. An attempt faults, and the kernel terminates the process.

## DMA

Devices that transfer bulk data do not use the CPU as an intermediary. A network card receiving packets or a disk controller reading sectors writes directly to RAM through a mechanism called DMA (Direct Memory Access).

```
1. Kernel tells network card: write incoming packets to physical 0x50000
2. Packet arrives
3. Network card writes to 0x50000 directly
4. Network card interrupts CPU
5. Kernel reads packet from 0x50000
```

The CPU does not copy bytes one at a time. The device transfers data directly to RAM.

**How it works:** DMA-capable devices are bus masters. They can drive the memory bus themselves, not just respond to CPU requests.

```
┌─────────────────────────────────────────────────┐
│               Memory Bus                        │
│                                                 │
│  Normal: CPU drives bus                         │
│    CPU ──────► address + data ──────► RAM       │
│                                                 │
│  DMA: Device drives bus                         │
│    Device ──────► address + data ──────► RAM    │
│    (CPU is idle or doing other work)            │
│                                                 │
└─────────────────────────────────────────────────┘
```

The kernel programs the device with a physical address and a length. The device has its own logic to increment addresses and move data. When done, it asserts an interrupt line. The CPU sees the interrupt and knows the transfer completed.

DMA leaks several concerns. Devices use physical addresses. A device configured with an address can access any physical memory, including kernel data structures and page tables. Cache coherency becomes an issue because the CPU cache, RAM, and device may have different views of the same memory.

## IOMMU

An IOMMU provides address translation for devices, analogous to the MMU for the CPU.

```
CPU ────► MMU ───┬──► RAM
Device ► IOMMU ──┘
```

Each device sees its own address space, just as each process sees its own virtual address space. The kernel configures the IOMMU with tables that restrict which physical addresses each device can access.

**How it works:** The IOMMU sits on the path between devices and the memory bus. When a device issues a DMA request, the address goes through the IOMMU first.

```
┌─────────────────────────────────────────────────┐
│ Device issues DMA to address 0x1000             │
│         │                                       │
│         ▼                                       │
│  ┌─────────────────────────────────────┐        │
│  │ IOMMU                               │        │
│  │                                     │        │
│  │ 1. Identify device (which tables?) │        │
│  │ 2. Look up 0x1000 in device's table│        │
│  │ 3. Check permission                 │        │
│  │ 4. Translate to physical            │        │
│  └──────────────┬──────────────────────┘        │
│                 │                               │
│                 ▼                               │
│  ┌─────────────────────────────────────┐        │
│  │ Allowed: forward 0x50000 to bus     │        │
│  │ Blocked: signal error, no bus access│        │
│  └─────────────────────────────────────┘        │
└─────────────────────────────────────────────────┘
```

Device identity comes from the bus. PCIe devices have a requester ID (bus/device/function). The IOMMU maintains separate page tables per device or device group.

```
Device A's IOMMU table:
  0x0000 → Physical 0x50000, read/write
  0x1000 → Physical 0x51000, read only
  0x2000 → BLOCKED
```

Device A attempting to access 0x2000 triggers an error. This protects against malicious or malfunctioning devices and enables safe device assignment to virtual machines.

What leaks is configuration complexity. The kernel must set up and maintain IOMMU tables for each device.

## Architecture

Each component emerged from a specific problem. Address translation gives each program its own address space. The translation cache hides the latency of translation. Hierarchical tables reduce memory consumption. MMIO extends memory instructions to device access. DMA offloads bulk transfers from the CPU. IOMMU restricts what devices can access.

```
Programs ──► CPU ──► MMU ──► Bus ──┬──► RAM
                                   ├──► Device ──► IOMMU ──┐
                                   └──► Device ──► IOMMU ──┴──► RAM
```

Kernel configures: page tables, CR3, IOMMU tables.

## Summary

**Abstractions:**

| Component | Problem | Solution | Abstraction |
|-----------|---------|----------|-------------|
| Memory Controller | RAM chips have complex timing, refresh, banks | Handle DRAM protocol | Linear address space |
| MMU | Multiple programs issue same addresses | Translate virtual to physical per-process | Per-program address space |
| Memory Bus | CPU doesn't know physical layout | Route addresses to RAM or devices | Uniform address space |
| IOMMU | Devices can access any physical memory | Translate and restrict device addresses | Per-device address space |

**Optimizations:**

| Component | Problem | Solution | Benefit |
|-----------|---------|----------|---------|
| Translation cache | Translation needs memory access | Cache recent translations | Fast translation |
| Hierarchical tables | Flat table too large for 64-bit | Sparse multi-level structure | Compact tables |
| DMA | CPU copying bytes is slow | Devices transfer directly to RAM | Fast bulk transfer |

## Costs

| Mechanism | Cost |
|-----------|------|
| Translation | ~100 cycles on cache miss |
| Multi-level tables | 4 memory accesses per miss |
| Small pages | Translation cache pressure |
| Large pages | Internal fragmentation |
| Lazy allocation | Fault latency on first access |
| Lazy copying | Fault latency on first write |
| Disk overflow | Disk latency (milliseconds) |
| IOMMU | Configuration complexity, some latency |

## Terminology

| Term used here | Official term |
|----------------|---------------|
| virtual address | virtual address |
| physical address | physical address |
| page table | page table |
| map pointer | CR3, PTBR (page table base register) |
| translation hardware | MMU (Memory Management Unit) |
| translation cache | TLB (Translation Lookaside Buffer) |
| table lookup | page table walk |
| translation failure | page fault |
| permission violation | protection fault |
| device memory mapping | MMIO (Memory-Mapped I/O) |
| device RAM access | DMA (Direct Memory Access) |
| device translation | IOMMU, VT-d (Intel), AMD-Vi |

---

## Appendix: Radix Trees

**Note:** The compression comes from storing shared prefixes once instead of repeating them.

```
We want to store:
  0000 → 1
  0001 → 1
  0010 → 1
  0011 → 1
  (all other 4-bit keys → 0)

Flat array:
  Any 4-bit key might be looked up → 16 slots

  index:  0000 0001 0010 0011 0100 0101 ... 1111
  value:    1    1    1    1    0    0  ...   0

  16 slots allocated, 12 hold zeros

Radix tree:
  All 4 keys share prefix 00.
  Store 00 once, then store only the suffixes.

  00 ─────► 00 → 1
            01 → 1
            10 → 1
            11 → 1

  01 ─────► null (no table allocated)
  10 ─────► null (no table allocated)
  11 ─────► null (no table allocated)

  4 entries in L1 + 4 entries in one L2 = 8 slots
```

The flat array allocates 16 slots because any 4-bit key might be looked up. The radix tree allocates only the branch that contains data. Prefixes 01, 10, 11 point to null. No subtables behind them.

A fully allocated radix tree is worse than a flat array: same leaf entries plus intermediate nodes. The tree saves space only when branches are null. Sparse, clustered keys make branches null. Page tables fit this pattern: processes use thousands of pages clustered in a few regions, leaving most of the address space empty.
