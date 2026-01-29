---
title: "Abstractor: Virtual Memory and Devices"
date: 2026-01-29
categories: [Systems, Computing]
tags: [linux, memory, virtual-memory, mmu, dma, iommu, devices]
toc: true
---

## Physical Memory

RAM chips are complex devices with timing constraints, refresh cycles, banks, and channels. The memory controller sits between the CPU and RAM, handling this complexity. It presents a simpler interface. Any address yields the data stored there.

```
Program ──► Memory Controller ──► RAM
```

**How it works:** The memory controller is hardware on the CPU die (modern systems) or motherboard (older systems). Address lines from the CPU connect to it. When an address appears, the controller translates it to DRAM commands: row address, column address, read/write strobes. It handles timing, refresh, and bank interleaving. The CPU sees none of this. Wires go in with an address, data comes back.

A single program using this interface encounters no difficulty. Multiple programs sharing the same address space encounter several.

## Multiple Programs, One Memory

A program should see a simple model. Memory is a contiguous space starting at address 0, and the program is alone in it. The program should not need to know that other programs exist, that physical memory has a different layout, or that some addresses route to devices instead of RAM.

```
Program A ──┬──► RAM
Program B ──┘
```

Without hardware support, this abstraction is impossible. Programs issue addresses that go directly to RAM. Two programs using address 0x1000 access the same physical location. Programs can read each other's memory, overwrite the kernel, and send commands to devices. The simple model is a fiction that nothing enforces.

## Virtual Addresses

The MMU (Memory Management Unit) sits between the CPU and the memory bus, translating every address before it reaches RAM.

```
Program A ──┬──► MMU ──► RAM
Program B ──┘
```

Each program issues addresses starting at 0. The MMU translates these virtual addresses to physical addresses. Program A's 0x1000 becomes physical 0x50000. Program B's 0x1000 becomes physical 0x80000. Each program sees the simple model it expects. Neither knows the other exists.

**How it works:** The MMU is not software. It is transistors in the path. The CPU does not "call" the MMU. Address lines from the CPU execution unit physically connect to MMU hardware before reaching the memory bus. There is no way around it.

```
CPU execution unit
     │
     │ address lines (wires)
     ▼
┌─────────┐
│   MMU   │  ← physically in the path
└────┬────┘
     │
     │ address lines (wires)
     ▼
Memory bus
```

When the CPU executes `mov eax, [0x1000]`, the address 0x1000 appears on wires. Those wires go into MMU transistors. Different wires come out with the physical address. The translation happens in gate delays, not function calls.

A virtual address splits into page number and offset:

```
Virtual 0x00012345 (4KB pages):
┌─────────────────────┬──────────────┐
│ Page number: 0x12   │ Offset: 0x345│
└─────────────────────┴──────────────┘
```

The MMU looks up page 0x12, finds physical page 0x50. The offset passes through unchanged. Result: physical 0x00050345.

## Page Tables

The MMU needs to know how to translate each virtual address. It reads this information from a table stored in RAM, called a page table.

```
┌─────────────┬─────────────┬─────────────┐
│ Virtual     │ Physical    │ Permissions │
├─────────────┼─────────────┼─────────────┤
│ 0x1000      │ 0x50000     │ user, r/w   │
│ 0x2000      │ 0x51000     │ user, r     │
│ 0x3000      │ —           │ not present │
└─────────────┴─────────────┴─────────────┘
```

Each process has its own page table. A CPU register called CR3 holds the physical address of the current page table. When the kernel switches from one process to another, it loads a new value into CR3, and the MMU begins using the new mapping.

Changing CR3 requires ring 0 privilege. User programs cannot modify their own mappings.

Programs see virtual addresses and do not know where their data physically resides. What leaks through is page granularity, translation cost, and page faults. Protection cannot be finer than a page. Every access needs a lookup. Translation can fail.

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
