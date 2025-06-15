# libth

`libth` is the equivalent of `libc` in ThaxtOS.
It is provided as both `libth.a` and `libth.so` in `/lib` of the installed OS.
Most other programs depend on `libth` in some way.

Note that there is currently no from-scratch implementation of `ld.so`, but shared libraries can still be used by copying `ld-linux-aarch64.so.1` from a host Linux distro, such as Debian.

`libth` provides many primitives, such as:
- all syscalls
- `fork`, `exeve`, etc.
- `malloc` and `free`, built on `brk` syscall
- `stdio`, including `fopen`, `fclose`, etc.
- `printf` and friends
- `strcmp`, `strlen`, etc.
- Basic locking
