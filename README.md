# Linux Kernel Tools Suite (C)

A small collection of systems utilities written in C to explore how
operating systems expose internal kernel state to user programs.

Each tool focuses on a single OS abstraction and interfaces **directly with
kernel-generated data structures** rather than relying on high-level libraries.

The goal is correctness, clarity, and understanding of kernel userspace
boundaries.


## mini-ps

A minimal process inspection tool that enumerates active processes by
reading kernel-generated metadata from `/proc/<pid>/stat`.

### Kernel Interface
- `/proc/<pid>/stat` — exposes per-process state maintained by the kernel

### Syscalls Used
- open / read / close (via libc)
- getdents (via readdir)

## mini-top
A minimal CPU accounting tool that reports per-process CPU usage by sampling
scheduler time deltas.

The implementation measures CPU usage by comparing changes in:
- total system CPU time (`/proc/stat`)
- per-process CPU time (`/proc/<pid>/stat`)

**Kernel interfaces**
- `/proc/stat`
- `/proc/<pid>/stat`




## mini-netstat
A minimal TCP socket inspection tool that reads the kernel-maintained
socket table from `/proc/net/tcp`.

The tool parses raw kernel socket entries and converts hexadecimal
addresses into human-readable IP/port pairs.

**Kernel interface**
- `/proc/net/tcp`
## proc_ancestry
A process lineage tracer that walks the parent chain from a given PID all the
way up to PID 1 (init), printing the full ancestry with process names and states.

Unlike `pstree` which shows the full tree top-down, this traces a single
process's lineage upward — showing exactly how it was spawned.

**Kernel interface**
- `/proc/<pid>/stat` (ppid field)

## syscall_snoop
A zero-overhead syscall peeker that reads `/proc/<pid>/syscall` to show what
system call a process is currently blocked in, along with its arguments, stack
pointer, and program counter.

Unlike `strace`, this does not attach via ptrace — it simply reads a kernel-
exported file, making it completely non-invasive.

**Kernel interface**
- `/proc/<pid>/syscall`

---

## Design Philosophy

- **Minimal**: each tool demonstrates one abstraction only
- **Transparent**: no hidden behavior behind libraries
- **Defensive**: handles real OS race conditions
- **Educational**: optimized for explainability, not features

---

## Tradeoffs & Non-Goals

- No command-line flags
- No sorting or filtering
- No curses or UI layers
- No PID → socket mapping
- No attempt at production readiness

These omissions are intentional to keep focus on OS abstractions rather than
application complexity.

---

## Build & Run

This project is developed and tested inside a Linux environment.

```bash
make
./mini_ps
./mini_top <pid>
./mini_netstat
./proc_ancestry <pid>
./syscall_snoop <pid>
