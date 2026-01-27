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
