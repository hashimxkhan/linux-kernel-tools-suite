## mini-ps

A minimal process inspection tool that enumerates active processes by
reading kernel-generated metadata from `/proc/<pid>/stat`.

### Kernel Interface
- `/proc/<pid>/stat` — exposes per-process state maintained by the kernel

### Concepts Demonstrated
- Process lifecycles
- Kernel ↔ userspace data exposure
- Race conditions during process termination

### Syscalls Used
- open / read / close (via libc)
- getdents (via readdir)

