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

Running multiple processes means multiple processes in memory simultaneously. Each process was compiled to own address zero. Their addresses collide. The physical address space, presented by the memory controller, needed a further abstraction: a separate address space for each process, mapped onto the physical one.

The solution: a hardware component that intercepts every address and translates it before it reaches the memory controller.

```
Process A (0x1000) ──► MMU ──► Physical RAM (0x50000)
Process B (0x1000) ──► MMU ──► Physical RAM (0x80000)
                        │
                 each process gets
                 its own address space
```

Process A's 0x1000 maps to physical 0x50000. Process B's 0x1000 maps to physical 0x80000. Each process sees a linear address space starting at zero. The MMU multiplexes them onto physical RAM.

**Details:** The MMU maintains a mapping from virtual pages to physical pages. This mapping lives in a page table, a data structure stored in RAM. A CPU register (CR3 on x86) holds the physical address of the active page table.

A virtual address splits into two parts: page number and offset. With 4KB pages, the low 12 bits are the offset (2^12 = 4096), the remaining bits are the page number. The MMU uses the page number to index into the page table. Each page table entry (PTE) contains a physical page number, permission bits (read, write, execute), and status flags (present, accessed, dirty). The MMU combines the physical page number from the PTE with the offset from the virtual address. Result: physical address.

```
Virtual:  [    page number    |   offset   ]
                   │                 │
                   ▼                 │
            Page Table              │
          ┌────────────┐            │
          │ PTE: phys  │            │
          │ page 0x50  │            │
          └─────┬──────┘            │
                │                   │
                ▼                   ▼
Physical: [  phys page 0x50  |   offset   ]
```

Permission checks happen during translation. If the PTE marks a page read-only and the instruction writes, the MMU raises a fault. If the present bit is clear, the MMU raises a fault. These faults are synchronous exceptions, delivered before the memory access completes.

Changing CR3 switches the entire address space. Writing to CR3 is a privileged operation. The hardware enforces this.

**History:** The Atlas computer (1962, Ferranti/Manchester University) introduced the first paged MMU and virtual memory. IBM's System/360 Model 67 (1965) brought it to commercial systems, calling it the Dynamic Address Translation box. Early MMUs were separate chips. Intel integrated the MMU onto the CPU die with the 386 in 1985.

**Jargon:** *Time-sharing*: multiple processes take turns on the CPU, switching fast enough to appear simultaneous. *Concurrent*: interleaved, one at a time, fast switching. *Parallel*: truly simultaneous, multiple CPUs. *MMU*: Memory Management Unit, the hardware performing address translation. *DAT*: Dynamic Address Translation, IBM's term for the same thing. *Virtual address*: the address a process issues. *Physical address*: the address that reaches RAM. *Page table*: data structure mapping virtual pages to physical pages. *PTE*: Page Table Entry, one row in the page table.

---

## TLB - Address Translation Optimization

Every address the CPU issues is virtual. Every load, store, and instruction fetch requires translation before reaching RAM. The page table holds the mappings, but it lives in RAM. Fetching from RAM on every access would be slow. The TLB caches recent translations.

The TLB (Translation Lookaside Buffer) is a cache of page table entries built into the CPU. It sits on the same die as the execution units, nanometers away. RAM sits on separate chips, centimeters away across the motherboard. The TLB uses SRAM, which completes a lookup in one CPU cycle. RAM uses DRAM, which takes 50-100 cycles.

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
              │  (RAM)        │
              └───────────────┘
```

Every memory access checks the TLB. A process issues a virtual address. The MMU splits it into page number and offset. The page number indexes into the TLB.

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
       ├── hit: TLB returns physical page (1 cycle)
       │
       └── miss: fetch from page table in RAM (~100 cycles)
                        │
                        ▼
              ┌─────────────────────┐
              │ Page Table (RAM)    │
              │ page 0 → phys 30    │
              │ page 1 → phys 40    │
              │ page 2 → phys 50    │  ← fetch, copy to TLB
              │ ...                 │
              └─────────────────────┘
```

A hit returns the physical page from the TLB in one cycle. A miss fetches the entry from RAM, copies it into the TLB, then returns the physical page. Either way, the MMU combines the physical page with the offset to form the physical address.

Consider a function summing an array. The array starts at virtual address 0x5000. The loop reads element 0 at 0x5000, element 1 at 0x5004, element 2 at 0x5008. Each read triggers a TLB lookup. All three addresses fall within page 5 (0x5000–0x5FFF). The first read misses, fetches the entry from RAM, populates the TLB. Subsequent reads hit.

A 4KB array spans one page. Summing 1000 four-byte integers: one TLB miss, 999 hits.

A 256-entry TLB covering 4KB pages spans 1MB of working memory. Most programs fit their hot path in far less.

**Details:** DRAM stores bits in capacitors that leak and require refresh. SRAM stores bits in flip-flops, six transistors per bit, holding state as long as power flows. SRAM costs more per bit and takes more area, so main memory uses DRAM. SRAM needs no refresh cycles, no row/column protocol, no precharge delays.

The TLB is a content-addressable memory (CAM). Regular RAM takes an address as input and returns data at that location. A CAM takes data as input and searches all entries in parallel. The MMU presents the virtual page number and every TLB entry compares against its stored key simultaneously. Parallel comparison completes in one cycle regardless of TLB size.

Each TLB entry stores: virtual page number, physical page number, permission bits, valid bit, and address space identifier (ASID). The ASID tags entries by process. A lookup matches only when both virtual page number and ASID match. Context switches do not require flushing the TLB.

The MMU splits each virtual address into page number and offset:

```
page_number = virtual_address / page_size
offset      = virtual_address % page_size
```

With 4KB pages, the low 12 bits are the offset, the remaining bits are the page number. The physical address is:

```
physical_address = (physical_page * page_size) + offset
```

Modern CPUs split the TLB into levels. The L1 TLB is tiny (16-64 entries) and completes in 1 cycle. The L2 TLB is larger (512-2048 entries) and completes in 4-8 cycles. A miss in both triggers a page table walk.

**ASID** extends the per-process address space abstraction into the TLB. The MMU gives each process its own virtual address space. The TLB caches translations from all running processes. ASID is a field in each TLB entry tagging which process owns that translation. A lookup matches only when both the virtual page number and the ASID match. Entries from different processes coexist in the TLB without interference. Without ASIDs, the OS must flush the TLB on every context switch.

On a TLB miss, the **page table walker** fetches the mapping from RAM. The page table is a tree, typically four levels deep on x86-64. The walker reads the first-level table to find a pointer to the second-level, reads that to find the third, and so on until it reaches the entry containing the physical page number. Each level requires a memory read. A full walk costs four RAM accesses.

**Huge pages** reduce TLB pressure for large working sets. Standard pages are 4KB. A 256-entry TLB covers 1MB. Huge pages are 2MB or 1GB. A single 2MB entry covers 512 times more memory than a 4KB entry. The same 256-entry TLB now covers 512MB. Database servers and virtual machines use huge pages. The tradeoff: huge pages waste memory when partially used, and the OS must find contiguous physical memory to back them.

**History:** The Atlas computer (1962) included the first TLB, called "page address registers." Without it, every memory access required two RAM fetches: one for the page table entry, one for the data. The TLB eliminated the translation fetch for 95-99% of memory references. Virtual memory went from a 2x slowdown to near-zero overhead. The optimization made virtual memory practical. The term TLB emerged in IBM documentation during the 1970s. Modern x86 processors have separate TLBs for instructions and data, with multiple levels.

**Jargon:** *TLB*: Translation Lookaside Buffer, a cache for page table entries. *CAM*: Content-Addressable Memory, memory that searches by content rather than address. *SRAM*: Static RAM, fast memory using flip-flops. *DRAM*: Dynamic RAM, dense memory using capacitors. *ASID*: Address Space Identifier, tag distinguishing entries from different processes. *Page table walk*: traversing the page table in RAM to find a translation. *Huge pages*: pages larger than 4KB (2MB or 1GB) to reduce TLB pressure.

---

## Pages

Memory is divided into fixed-size units called pages, typically 4KB. The page table maps virtual pages to physical pages, not individual bytes.

Per-byte mapping would require a table entry for every address. A 4GB address space would need 4 billion entries. At 8 bytes per entry, the table would be 32GB, larger than the memory it describes.

Per-page mapping at 4KB granularity reduces this to 1 million entries for 4GB. The cost is that protection cannot be finer than a page.

## Translation Speed

Every memory access requires a page table lookup. The page table is in RAM. A memory access to perform a memory access would double latency at minimum.

The MMU contains a cache called the TLB (Translation Lookaside Buffer) that holds recent translations.

```
MMU: ┌─────────────────────┐
     │ TLB (hit: ~1 cycle) │
     │ Walker (miss: ~100) │
     └─────────────────────┘
```

A TLB hit completes in roughly one cycle. A TLB miss requires walking the page table in RAM, roughly 100 cycles.

The translation cache is small, typically 64 to 1024 entries. Programs that touch many different pages see frequent misses.

Large pages (2MB or 1GB instead of 4KB) reduce cache pressure by covering more memory per entry. The cost is internal fragmentation when a large page is only partially used.

## Hierarchical Page Tables

A 64-bit address space contains 2^52 pages at 4KB granularity. A flat table would require petabytes of storage.

```
Flat table (impossible):
┌─────────────────────────────────────────┐
│ Entry 0 │ Entry 1 │ ... │ Entry 2^52   │
└─────────────────────────────────────────┘
```

Modern systems use multi-level page tables.

```
Multi-level (sparse):
CR3 → L1 → L2 → L3 → L4 → Physical
           ↓
          NULL (unmapped: no deeper tables)
```

A 4-level structure has 512 entries at each level. Unmapped regions require no storage beyond a null pointer at the level where they diverge.

| Approach | Space | Lookup cost |
|----------|-------|-------------|
| Flat table | Huge (impractical) | 1 memory access |
| 4-level table | Sparse, small | 4 memory accesses |

**How it works:** On a translation cache miss, the MMU hardware walks the table. The virtual address splits into four 9-bit indices (plus 12-bit offset):

```
Virtual address (48 bits used):
┌────────┬────────┬────────┬────────┬─────────────┐
│ L1 idx │ L2 idx │ L3 idx │ L4 idx │ Offset      │
│ 9 bits │ 9 bits │ 9 bits │ 9 bits │ 12 bits     │
└────────┴────────┴────────┴────────┴─────────────┘
```

The table walker hardware reads CR3 to find the L1 table, uses the L1 index to find an entry, reads that entry to find the L2 table address, and so on. Four memory accesses in sequence. Modern CPUs have dedicated hardware for this walk. Some cache intermediate results (page structure caches).

The translation cache hides the lookup cost most of the time.

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
