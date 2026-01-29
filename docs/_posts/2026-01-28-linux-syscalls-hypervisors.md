---
title: "Abstractor: Linux, Syscalls and Hypervisors"
date: 2026-01-28
categories: [Systems, Computing]
tags: [linux, hypervisors, virtualization, kernel, memory, cpu]
toc: true
---

## Part 1: Why Do We Even Need VMs?

### What Linux Already Does

```
┌──────────┐ ┌──────────┐ ┌──────────┐
│ Process A│ │ Process B│ │ Process C│
│ (user 1) │ │ (user 1) │ │ (user 2) │
└────┬─────┘ └────┬─────┘ └────┬─────┘
     │            │            │
┌────┴────────────┴────────────┴────┐
│            KERNEL                  │
│  - Schedules CPU time              │
│  - Manages memory (page tables)    │
│  - Controls file access            │
│  - Enforces user permissions       │
└────────────────┬──────────────────┘
                 │
            Hardware
```

**Already isolated:**

| Resource | How Linux Isolates |
|----------|-------------------|
| CPU time | Scheduler gives each process time slices |
| Memory | Page tables — process A can't see process B's RAM |
| Files | Permissions — user 2 can't read user 1's files |
| Network | Ports, firewall rules |

### The Gaps — Why Linux Multi-User Isn't Enough

| Problem | Why Linux Multi-User Isn't Enough |
|---------|----------------------------------|
| **Different OS** | Process can't be Windows. It's all Linux. |
| **Kernel bugs** | One kernel. Bug affects everyone. |
| **Root escape** | If process gets root, game over for ALL users. |
| **Kernel version** | Everyone shares same kernel. Can't mix versions. |
| **Full isolation** | Processes share kernel memory, syscalls, timing. Information can leak. |
| **Resource guarantees** | Scheduler is "fair" but not guaranteed. No hard limits. |

### What VMs Add

Each VM gets its own kernel. Total separation.

```
┌─────────────────┐  ┌─────────────────┐
│      VM 1       │  │      VM 2       │
│  ┌───────────┐  │  │  ┌───────────┐  │
│  │ Process A │  │  │  │ Process X │  │
│  └─────┬─────┘  │  │  └─────┬─────┘  │
│        │        │  │        │        │
│  ┌─────┴─────┐  │  │  ┌─────┴─────┐  │
│  │  Kernel 1 │  │  │  │  Kernel 2 │  │
│  │  (Linux)  │  │  │  │ (Windows) │  │
│  └───────────┘  │  │  └───────────┘  │
└────────┬────────┘  └────────┬────────┘
         │                    │
┌────────┴────────────────────┴────────┐
│            HYPERVISOR                 │
└──────────────────┬───────────────────┘
                   │
               Hardware
```

**Now:**
- VM 1 root ≠ VM 2 root
- VM 1 kernel crash ≠ VM 2 crash
- VM 1 can run Linux, VM 2 can run Windows
- Isolation is at hardware boundary, not syscall boundary

**The Short Answer:**
- **Linux multi-user:** Trust the kernel. Isolation via permissions and page tables.
- **VMs:** Don't trust anything. Each tenant gets their own kernel. Isolation via fake hardware.

---

## Part 2: Linux Internals — The Three Boundaries

Everything in Linux is about enforcing three boundaries:

```
┌─────────────────────────────────────────────────┐
│                                                 │
│   BOUNDARY 1: Process ←→ Process               │
│   (Memory isolation via page tables)           │
│                                                 │
│   BOUNDARY 2: Process ←→ Kernel                │
│   (Ring 3 vs Ring 0, syscalls)                 │
│                                                 │
│   BOUNDARY 3: Kernel ←→ Hardware               │
│   (Drivers, interrupts)                        │
│                                                 │
└─────────────────────────────────────────────────┘
```

---

## Part 3: CPU Has Two Modes (Hardware Enforced)

```
┌─────────────────────────────────────┐
│         RING 3 (User Mode)          │
│                                     │
│   - Your process runs here          │
│   - CANNOT execute privileged ops   │
│   - CANNOT access hardware directly │
│   - CANNOT see other process memory │
│                                     │
│   If you try → CPU raises exception │
└──────────────────┬──────────────────┘
                   │ syscall instruction
                   │ (controlled gate)
                   ▼
┌─────────────────────────────────────┐
│         RING 0 (Kernel Mode)        │
│                                     │
│   - Kernel runs here                │
│   - CAN touch hardware              │
│   - CAN see all memory              │
│   - CAN do anything                 │
└─────────────────────────────────────┘
```

**The CPU itself enforces this.** A register holds current ring. Hardware checks every instruction.

**What you remember:**
> "CPU has a mode bit. Ring 0 vs Ring 3. Hardware checks every instruction. Some instructions only work in ring 0."

**If they ask details:**
> "I don't remember which register, but there's a bit that says 'kernel mode' or 'user mode'. CPU checks it on privileged operations."

---

## Part 4: The Thin API (Syscalls)

Your process wants things. Hardware has things. Kernel is the middleman.

```
┌──────────────────────────────────────┐
│            YOUR PROCESS              │
│                                      │
│   You can:                           │
│     - Do math                        │
│     - Access your own memory         │
│     - Call the thin API              │
│                                      │
│   You cannot:                        │
│     - Touch hardware                 │
│     - See other processes            │
│     - Do anything privileged         │
│                                      │
└──────────────────┬───────────────────┘
                   │
                   │  THE THIN API
                   │
                   │  get_handle(thing) → int
                   │  bytes_in(int) → data
                   │  bytes_out(int, data)
                   │  release(int)
                   │  do_anything(int, ???)
                   │
                   │  (officially: open, read, write, close, ioctl)
                   │
┌──────────────────▼───────────────────┐
│         KERNEL + HARDWARE            │
│                                      │
│   Figures out what handle means.     │
│   Does the actual work.              │
│   Returns result.                    │
│                                      │
└──────────────────────────────────────┘
```

### How read(fd, buf, 100) Actually Works

```c
read(fd, buf, 100);  // This calls a function in libc. That's it.
```

**The layers:**

```
YOUR CODE:
    read(fd, buf, 100);
         │
         │ normal function call
         ▼
LIBC (glibc, musl, etc):
    ssize_t read(int fd, void *buf, size_t count) {
        // Put args in registers
        // Do the actual syscall instruction
        return syscall(SYS_read, fd, buf, count);
    }
         │
         │ syscall instruction
         ▼
KERNEL:
    sys_read(fd, buf, count) {
        // actual work
    }
```

### Why SYSCALL Is A CPU Instruction

Like `ADD` or `MOV`, there is literally an instruction called `SYSCALL`. It's opcode `0x0F 0x05`. Hardware knows what to do.

**What SYSCALL does (hardware, atomically):**
1. Save rip → rcx (so kernel knows where to return)
2. Save flags → r11
3. Load rip from special register (MSR_LSTAR) — kernel set this at boot
4. Set mode = privileged
5. Continue at new rip (now in kernel)

**Software can't change the mode bit.** That would require... privileged mode. Chicken and egg. So CPU provides a special instruction that does all of this atomically, safely.

---

## Part 4.5: The Trap Table

SYSCALL is one entry point, but the CPU handles many other events like divide by zero, page fault, debug breakpoint, and timer tick. Each of these must transfer control to kernel code.

This transfer has to be indirect. If unprivileged code could jump directly to any kernel address, it would skip validation and corrupt state. So the CPU uses a table of fixed entry points that only the kernel can configure.

When event N happens, the CPU looks up entry N, switches to privileged mode, and jumps to that address.

```
┌────────┬─────────────────────┐
│ Number │ Jump address        │
├────────┼─────────────────────┤
│   0    │ 0x80100100          │  divide by zero
│   1    │ 0x80100200          │  debug
│  13    │ 0x80100D00          │  general protection fault
│  14    │ 0x80100E00          │  page fault
│  ...   │ ...                 │
│  64    │ 0x80103000          │  syscall (legacy)
└────────┴─────────────────────┘
```

The kernel writes this table at boot, and the `lidt` instruction tells the CPU where to find it. Both operations require privileged mode.

| Who | Does | Privileged? |
|-----|------|-------------|
| Kernel | Writes table, runs `lidt` | Yes |
| User code | Triggers entry (`int N`, or fault) | No |
| Hardware | Looks up, switches mode, jumps | — |

User code triggers. Kernel code configures. Hardware enforces.

(officially: Interrupt Descriptor Table / IDT)

---

## Part 5: Page Tables (Memory Isolation)

### The Lie

Every process thinks: "I have all memory to myself. I start at address 0. I'm alone."

### The Truth

```
Process A's view:          Process B's view:

0x1000 → my variable       0x1000 → my variable

        │                          │
        ▼                          ▼
    ┌───────┐                  ┌───────┐
    │ MMU   │                  │ MMU   │
    │(CPU)  │                  │(CPU)  │
    └───┬───┘                  └───┬───┘
        │                          │
        ▼                          ▼
Physical 0x50000           Physical 0x80000
(different!)               (different!)
```

**Page table** = a map from virtual → physical

```
Virtual Address      Physical Address     Permissions
─────────────────────────────────────────────────────
0x0000 - 0x0FFF  →   UNMAPPED            (null trap)
0x1000 - 0x1FFF  →   0x50000             read, write
0x2000 - 0x2FFF  →   0x51000             read, execute
```

**Key insight:**
- CPU has a register (CR3 on x86) pointing to current page table
- On EVERY memory access, hardware translates
- Process can't change CR3 — it's privileged (ring 0 only)
- Kernel swaps CR3 when switching processes

### Process Virtual Address Space

```
0xFFFFFFFF  ┌─────────────────┐
            │  Kernel memory  │ ← Process CANNOT access
            │  (mapped but    │   (page table says no)
            │   protected)    │
0xC0000000  ├─────────────────┤
            │     Stack       │ ← Grows down
            │       ↓         │
            ├─────────────────┤
            │       ↑         │
            │     Heap        │ ← malloc() comes from here
            │  (brk / mmap)   │
            ├─────────────────┤
            │    .bss         │ ← Uninitialized globals
            ├─────────────────┤
            │    .data        │ ← Initialized globals
            ├─────────────────┤
            │    .text        │ ← Your code (read + execute)
0x00400000  ├─────────────────┤
            │   Unmapped      │ ← NULL pointer catches
0x00000000  └─────────────────┘
```

---

## Part 6: What A Process Actually Is

### Kernel's View

```c
struct task_struct {
    // IDENTITY
    pid_t pid;
    uid_t uid;
    gid_t gid;

    // SPACE
    struct mm_struct *mm;       // → page tables, mappings

    // STATE
    struct pt_regs regs;        // saved registers

    // RESOURCES (more IDENTITY)
    struct files_struct *files; // open file descriptors

    // SCHEDULING (TIME)
    int prio;
    u64 vruntime;               // how much time used

    // ...hundreds more fields
};
```

**A process is just a struct.** Kernel allocates it, fills it in, links it to scheduler.

---

## Part 7: Context Switch — The Big Moment

### There's ONE Physical CPU. ONE Set of Registers.

When process is NOT running, its registers live in memory:

```
MEMORY (kernel heap):

    Process A's task_struct:       Process B's task_struct:
    ┌─────────────────────┐        ┌─────────────────────┐
    │ saved_rax = 5       │        │ saved_rax = 99      │
    │ saved_rbx = 10      │        │ saved_rbx = 200     │
    │ saved_rip = 0x4000  │        │ saved_rip = 0x5000  │
    │ saved_rsp = 0x7fff  │        │ saved_rsp = 0x8fff  │
    └─────────────────────┘        └─────────────────────┘

    Just numbers in memory!
```

### Running vs Not Running

```
PROCESS A RUNNING:

    CPU hardware numbers:        A's blob in memory:
    ┌──────────────────┐         ┌──────────────────┐
    │ calc = 5         │ ← LIVE  │ saved_calc = ?   │ (stale)
    │ position = 0x4000│         │ saved_pos = ?    │
    └──────────────────┘         └──────────────────┘


PROCESS A NOT RUNNING:

    CPU hardware numbers:        A's blob in memory:
    ┌──────────────────┐         ┌──────────────────┐
    │ (some other      │         │ saved_calc = 5   │ ← SAVED
    │  process's data) │         │ saved_pos = 0x4000│
    └──────────────────┘         └──────────────────┘
```

### The Switch

```
SWITCHING A → B:

1. Hardware kicks kernel (timer interrupt)

2. Kernel saves CPU numbers → A's blob
   ┌──────────────────┐         ┌──────────────────┐
   │ calc = 5         │ ──────► │ saved_calc = 5   │
   │ position = 0x4000│         │ saved_pos = 0x4000│
   └──────────────────┘         └──────────────────┘

3. Kernel loads B's blob → CPU numbers
   ┌──────────────────┐         ┌──────────────────┐
   │ calc = 99        │ ◄────── │ saved_calc = 99  │
   │ position = 0x7000│         │ saved_pos = 0x7000│
   └──────────────────┘         └──────────────────┘

4. Kernel changes map pointer to B's map

5. Return to unprivileged mode

6. B continues. Never knew it was paused.
```

**What gets saved/restored:**

| Saved | Why |
|-------|-----|
| Registers (rax, rbx, ...) | B's computation state |
| Instruction pointer | Where B was executing |
| Stack pointer | B's stack position |
| CR3 (page table) | B's memory view |
| FPU/SSE state | B's floating point |

**The insight:** Registers are just numbers. When running: in hardware. When not running: in memory. "Copy registers" = copy numbers from one place to another.

---

## Part 8: Process Interruption — What Actually Happens

### The Scary Question

Process is in the middle of `int x = a + b * c;` — timer fires. What happens?

### Answer: CPU Saves EVERYTHING

```
PROCESS RUNNING:

    Registers:
    ┌─────────────────┐
    │ rax = 5         │ ← mid-calculation
    │ rbx = 10        │
    │ rip = 0x4001234 │ ← instruction pointer
    │ rsp = 0x7fff100 │ ← stack pointer
    │ flags = ...     │
    └─────────────────┘

         │
         │  TIMER INTERRUPT
         ▼

    CPU AUTOMATICALLY (hardware):
    1. Finishes current instruction
    2. Saves rip, rsp, flags to special place
    3. Switches to ring 0
    4. Jumps to interrupt handler
```

**Key:** CPU finishes current instruction. Never stops mid-instruction.

### Each Process Has TWO Stacks

```
┌─────────────────────────────────────────┐
│              PROCESS A                  │
│                                         │
│   User Stack          Kernel Stack      │
│   (your code uses)    (kernel uses)     │
│                                         │
│   ┌──────────┐       ┌──────────┐      │
│   │ local    │       │ saved    │      │
│   │ vars     │       │ regs     │      │
│   │ ...      │       │ from     │      │
│   │          │       │ interrupt│      │
│   └──────────┘       └──────────┘      │
│                                         │
└─────────────────────────────────────────┘
```

When interrupted:
- CPU switches to kernel stack (automatically)
- Kernel pushes registers there
- User stack is untouched

---

## Part 9: Fork and Exec — How Processes Launch

```c
// Shell runs this when you type "./myprogram"
pid = fork();      // 1. Clone current process

if (pid == 0) {
    // Child process
    exec("./myprogram");  // 2. Replace with new program
}
```

### What fork() Does

```
FORK DOES (inside kernel):

    1. Allocate NEW task_struct in memory

    2. Copy parent's CURRENT register values into it:

       Child's task_struct (NEW):
       ┌──────────────────┐
       │ pid = 101        │  ← new PID
       │ saved_rax = 5    │  ← copied from physical regs
       │ saved_rbx = 10   │  ← copied from physical regs
       │ saved_rip = 0x4000│ ← same instruction!
       │ page_table = ... │  ← copy or share parent's
       │ files = ...      │  ← copy parent's fd table
       └──────────────────┘

    3. Put child on scheduler's run queue

    4. Return to parent (still running)
```

### Why Fork Returns Different Values

```c
pid = fork();
if (pid == 0) { /* child */ } else { /* parent */ }
```

How?

```
KERNEL DOES:

    Parent's task_struct:        Child's task_struct:
    ┌──────────────────┐         ┌──────────────────┐
    │ saved_rax = 101  │         │ saved_rax = 0    │
    │ (child's pid)    │         │ (zero!)          │
    └──────────────────┘         └──────────────────┘

    rax is where return value goes.
    Kernel puts different values in each task_struct.
    When each runs, they get their own return value.
```

### What exec() Does

1. Open the executable file
2. Parse ELF header (where's code, data, entry point?)
3. Wipe current memory mappings
4. Map new segments: .text, .data, stack, heap
5. Set instruction pointer to entry point
6. Return to user mode

---

## Part 10: File Descriptors — How I/O Works

```c
int fd = open("/tmp/foo", O_RDONLY);
```

What happened:

```
USER:   fd = 3 (just a number)
                │
KERNEL:         ▼
        ┌───────────────────────────┐
        │ Process's FD table        │
        │                           │
        │ 0 → stdin (terminal)      │
        │ 1 → stdout (terminal)     │
        │ 2 → stderr (terminal)     │
        │ 3 → struct file ──────────────┐
        └───────────────────────────┘   │
                                        ▼
                              ┌──────────────────┐
                              │  struct file     │
                              │  inode: ...      │
                              │  position: 0     │
                              │  ops: read/write │
                              └──────────────────┘
```

**fd is just an index.** Kernel holds the actual file struct. You can only refer to it by number. Kernel validates on every operation.

---

## Part 11: Multi-User — Just Numbers

```c
struct task_struct {
    uid_t uid;    // user id (just a number)
    gid_t gid;    // group id (just a number)
};

struct inode {
    uid_t uid;    // file owner
    gid_t gid;    // file group
    mode_t mode;  // permissions (rwxrwxrwx)
};
```

When you `open()`:

```c
// Kernel does:
if (process->uid == inode->uid) {
    // Check owner permissions
} else if (process->gid == inode->gid) {
    // Check group permissions
} else {
    // Check other permissions
}
```

**That's it.** Users are just numbers. Permissions are just bits. Kernel does if-statements. Hardware knows nothing about users.

---

## Part 12: The One-Page Linux Summary

```
┌─────────────────────────────────────────────────────────┐
│ HARDWARE ENFORCES:                                      │
│   - Ring 0 vs Ring 3 (CPU mode bit)                     │
│   - Page table translation (MMU on every access)        │
│   - Interrupts (timer forces kernel to get control)     │
└─────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────┐
│ KERNEL ENFORCES:                                        │
│   - Which process has which page table                  │
│   - Which process runs when (scheduler)                 │
│   - Which files you can access (uid/gid checks)         │
│   - Syscall validation (is this fd valid?)              │
└─────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────┐
│ PROCESS SEES:                                           │
│   - Illusion of own memory                              │
│   - Illusion of own CPU                                 │
│   - File descriptors (handles, not real access)         │
│   - Syscall API (request things, can't take them)       │
└─────────────────────────────────────────────────────────┘
```

---

# PART TWO: HYPERVISORS

---

## Part 13: Hypervisors — The Basics

A hypervisor is a program that pretends to be hardware.

```
┌─────────┐ ┌─────────┐ ┌─────────┐
│  VM 1   │ │  VM 2   │ │  VM 3   │
│ (Linux) │ │(Windows)│ │ (Linux) │
└────┬────┘ └────┬────┘ └────┬────┘
     │           │           │
     │    "I am a computer"  │
     │           │           │
┌────┴───────────┴───────────┴────┐
│          HYPERVISOR             │
│   (lies to everyone above)      │
└────────────────┬────────────────┘
                 │
         Real Hardware
         (actual CPU, RAM, NIC)
```

Each VM thinks it has: its own CPU, RAM, network card, disk. It doesn't. The hypervisor fakes all of it.

---

## Part 14: Same Patterns, One Level Deeper

```
LINUX:                          HYPERVISOR:

Ring 3 (user process)           Ring 3 (user process)
    │                               │
    │ syscall                       │ syscall
    ▼                               ▼
Ring 0 (kernel)                 Ring 0 (guest kernel)
    │                               │
    │ ← this is the bottom          │ VM exit
    ▼                               ▼
Hardware                        Ring -1 (hypervisor)
                                    │
                                    │ ← NOW this is the bottom
                                    ▼
                                Hardware
```

**Everything you learned about Linux applies. Just add one more layer.**

### The New Lie

```
Before hypervisor:
    Process thinks: "I have all memory"
    Kernel thinks:  "I control the hardware"

After hypervisor:
    Process thinks: "I have all memory"        (still a lie)
    Kernel thinks:  "I control the hardware"   (NOW ALSO A LIE)
```

---

## Part 15: Same Concepts, Renamed

| Linux | Hypervisor | Same Idea? |
|-------|------------|-----------|
| Ring 3 → Ring 0 | Ring 0 → Ring -1 | Yes, mode switch |
| Syscall | VM exit / hypercall | Yes, controlled entry |
| Page tables | Nested page tables | Yes, two levels now |
| Process context switch | VM context switch | Yes, save/restore state |
| Timer interrupt | VM preemption | Yes, forced scheduling |
| Process = task_struct | VM = VMCS | Yes, state in memory |

### What's Different

| Linux | Hypervisor |
|-------|------------|
| Virtualizes one program | Virtualizes entire OS + hardware |
| Fakes "I have all memory" | Fakes "I have CPU, RAM, NIC, disk" |
| Kernel trusts hardware | Hypervisor doesn't trust guest kernel |
| One level of page tables | Two levels (guest + host) |

---

## Part 16: VM State Blob (VMCS)

Same idea as process. Just more stuff.

```
PROCESS STATE BLOB:            VM STATE BLOB:
┌─────────────────────┐        ┌─────────────────────┐
│ saved CPU numbers   │        │ saved CPU numbers   │
│ map pointer         │        │ map pointer         │
│ handles             │        │ guest's map pointer │ ← extra!
│                     │        │ guest's mode bit    │ ← extra!
│                     │        │ virtual devices     │ ← extra!
└─────────────────────┘        └─────────────────────┘

VM blob is bigger because we're faking entire hardware,
not just "a process."
```

---

## Part 17: Two Levels of Maps (Nested Page Tables / EPT)

```
PROCESS (one map):

    Virtual → Physical
    0x1000  → 0x50000


VM (two maps):

    Guest Virtual → Guest Physical → Host Physical
    0x1000        → 0x2000         → 0x80000

    Guest thinks 0x1000 → 0x2000.
    But 0x2000 is ALSO fake!
    Real location is 0x80000.

    (officially: nested page tables / EPT)
```

### Hardware Does Both Translations

```
MEMORY ACCESS IN VM:

    Guest code: load [0x1000]

    1. Guest page table: 0x1000 → 0x2000 (guest physical)
    2. Nested page table: 0x2000 → 0x8000 (real physical)
    3. Actually read from 0x8000

    Hardware does both lookups!
    (officially: EPT - Extended Page Tables)
```

### Why It Matters

```
WITHOUT HARDWARE SUPPORT:
    Every memory access → VM exit → hypervisor translates
    Impossibly slow.

WITH HARDWARE SUPPORT (EPT):
    CPU does both translations automatically.
    No VM exit for normal memory access.
    This is why modern VMs are fast.
```

---

## Part 18: When Does Hypervisor Get Control?

Same as kernel. Two doors:

```
1. VM does something sensitive
   - Change its map pointer
   - Talk to "hardware" (which is fake)
   - Execute privileged instruction

   CPU says: "That's not real hardware. Let me ask hypervisor."
   (officially: VM exit)

2. Hardware kick (interrupt)
   - Timer
   - Real network packet
   - Hypervisor needs to do something
```

---

## Part 19: What Nitro Does Differently

### Traditional Hypervisor

```
┌───────────────────────────────────────┐
│           HYPERVISOR                  │
│                                       │
│  - CPU scheduling         (complex)  │
│  - Memory management      (complex)  │
│  - Fake network card      (slow)     │
│  - Fake disk              (slow)     │
│                                       │
└───────────────────────────────────────┘
```

### Nitro Splits It

```
┌───────────────────────────────────────┐
│           HYPERVISOR (tiny)           │
│                                       │
│  - CPU scheduling                     │
│  - Memory management                  │
│  - That's it                          │
│                                       │
└───────────────────────────────────────┘

┌───────────────────────────────────────┐
│           HARDWARE CARDS              │
│                                       │
│  - Network (Nitro card)               │
│  - Disk (Nitro card)                  │
│  - Real hardware, not emulated        │
│                                       │
└───────────────────────────────────────┘
```

**Hypervisor does less. Hardware does I/O. Faster + simpler.**

### Why It's Better

```
EMULATED NETWORK PACKET:
    VM sends packet
        → VM exit (expensive)
        → Hypervisor handles it (software)
        → Hypervisor talks to real NIC
        → Return to VM
    Every packet = VM exit = slow

NITRO NETWORK PACKET:
    VM sends packet
        → Goes directly to Nitro card (SR-IOV)
        → Nitro card handles it (hardware)
        → No VM exit!
    Packets bypass hypervisor = fast
```

---

## Part 20: The Whole Picture

```
┌─────────────────────────────────────────────────────────┐
│                    PROCESS                              │
│  Has: own memory view, CPU time, handles                │
│  Sees: "I'm alone, I have everything"                   │
└────────────────────────┬────────────────────────────────┘
                         │ thin API
┌────────────────────────▼────────────────────────────────┐
│                  GUEST KERNEL                           │
│  Has: all processes, scheduling, handles                │
│  Sees: "I control the hardware"                         │
│  Truth: it's all fake                                   │
└────────────────────────┬────────────────────────────────┘
                         │ VM exit (sensitive op)
┌────────────────────────▼────────────────────────────────┐
│                   HYPERVISOR                            │
│  Has: all VMs, real scheduling, real memory maps        │
│  Sees: actual hardware                                  │
│  Does: CPU + memory only (Nitro)                        │
└────────────────────────┬────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────┐
│                 NITRO CARDS                             │
│  Does: network, disk (real hardware)                    │
└────────────────────────┬────────────────────────────────┘
                         │
                     HARDWARE
```

---

## Part 21: Resource Isolation — Which Resources Are Hard?

| Resource | Dedicate? | Flush? | Encrypt? | Time-Slice? | Risk If Shared |
|----------|-----------|--------|----------|-------------|----------------|
| **CPU time** | ✓ pin cores | N/A | N/A | ✓ scheduler | ✗ noisy neighbor |
| **Cache** | ✓ CAT | ✗ slow | N/A | N/A | ✗ side channel |
| **RAM** | ✓ static partition | ✗ too slow | ✓ encrypt | N/A | ✗ data leak |
| **Network** | ✓ hardware queues | N/A | ✓ encrypt traffic | ✓ one at a time | ~ depends |
| **Disk I/O** | ✓ separate queues | N/A | ✓ encrypt blocks | ✓ one at a time | ~ depends |

**The Pattern:** Stateful + shared = danger. Cache is the hardest because:
- It's shared (cores share L3)
- It retains state (previous VM's data)
- Clearing is slow (flush penalties)
- Can't easily encrypt (it's internal)

---

## Part 22: The Nitro Architecture

```
┌─────────────────────────────────────────────────────┐
│                   EC2 Instance                       │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐   │
│  │  VM 1   │ │  VM 2   │ │  VM 3   │ │   ...   │   │
│  └────┬────┘ └────┬────┘ └────┬────┘ └────┬────┘   │
│       │           │           │           │         │
│  ┌────┴───────────┴───────────┴───────────┴────┐   │
│  │         Nitro Hypervisor (minimal)          │   │
│  │    - CPU/Memory allocation only             │   │
│  │    - "Firmware-like" — deliberately small   │   │
│  └─────────────────────┬───────────────────────┘   │
│                        │ PCIe                       │
│  ┌─────────────────────┴───────────────────────┐   │
│  │              Nitro Cards (hardware)          │   │
│  │  ┌─────────┐ ┌─────────┐ ┌─────────────┐    │   │
│  │  │VPC Card │ │EBS Card │ │Storage Card │    │   │
│  │  │(network)│ │(block)  │ │(local NVMe) │    │   │
│  │  └─────────┘ └─────────┘ └─────────────┘    │   │
│  └─────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────┘
```

**Key Insight:** Nitro offloads I/O to hardware so hypervisor only does CPU+memory. This is the "secret sauce."

---

## Part 23: Official Names Reference

| What I Said | Official (Terrible) Name |
|-------------|-------------------------|
| mode bit | ring / privilege level |
| privileged mode | ring 0 / kernel mode |
| unprivileged mode | ring 3 / user mode |
| ask kernel | syscall |
| hardware kick | interrupt |
| process state blob | task_struct |
| VM state blob | VMCS |
| map | page table |
| map pointer | CR3 |
| two-level map | nested page tables / EPT |
| VM sensitive operation | VM exit |
| return to VM | VM entry |
| hypervisor mode | VMX root / ring -1 |
| `one_level_deeper_privileged` | `ring -1` / `VMX root` |
| `guest_thinks_its_privileged` | `ring 0 in guest` |
| `guest_did_something_sensitive` | `VM exit` |
| `return_to_guest` | `VM entry` |
| `vm_state_blob` | `VMCS` |
| `two_level_address_map` | `nested page tables` / `EPT` |

---

## Part 24: How To Explain It

### "How does memory isolation work between VMs?"

> "Two levels of maps. Guest has its map — virtual to 'physical'. But guest physical is fake. Hypervisor has another map — guest physical to real physical. Hardware walks both. VM-A's guest physical 0x2000 maps to real 0x8000. VM-B's guest physical 0x2000 maps to different real address. They can't see each other. Hypervisor controls the second-level map."

### "Why does Nitro offload I/O to hardware cards?"

> "CPU is general-purpose but slow for specific tasks. Every emulated I/O operation causes a VM exit — that's expensive, like a syscall but worse. Dedicated hardware handles I/O directly. VM talks to card, not hypervisor. Hypervisor stays minimal — just CPU and memory. Smaller attack surface, faster I/O, better density."

### "How does VM isolation compare to process isolation?"

> "Linux already isolates processes with page tables and ring protection. But processes share the kernel — one kernel bug, everyone's exposed. VMs add another layer: each VM gets its own kernel, and the hypervisor sits where hardware sits. It's the same pattern — ring separation, page tables — just one level deeper."

### "What about cache/side-channel attacks?"

> "Shared resource with state = leak path. Cache is worst — shared, stateful, slow to clear. Options: dedicate cores so cache isn't shared, flush on switch, or hardware partitioning if available. Nitro probably dedicates cores for sensitive workloads. It's the cleanest — no sharing, no leak. Costs density."

---

## Part 25: Summary — Sketchy But Correct

| Question | Answer |
|----------|--------|
| What gets saved on interrupt? | All registers. CPU saves some automatically, kernel saves rest. |
| Where is it saved? | Each process has a kernel stack. State goes there. |
| Can process prevent interruption? | No. Timer is hardware. `cli` is privileged. |
| How is scheduling fair? | Kernel tracks runtime per process. Lowest runtime goes next. |
| How often does switching happen? | Timer every ~1-10ms. Kernel checks and maybe switches. |
| What about mid-calculation? | CPU finishes current instruction first. Then interrupts. |
| What's the difference between Linux and hypervisor? | Same patterns (rings, page tables, state blobs), one level deeper. |
| What makes Nitro special? | Minimal hypervisor + hardware offload for I/O. |
