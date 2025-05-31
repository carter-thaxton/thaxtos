.text

.global _start
.global _syscall

_start:
    bl main

    // if we reach here, call exit with return value of main in x0
    mov     w8, #93     // exit syscall
    svc     #0

_syscall:
    // Thankfully, Linux syscall and C ABI conventions mostly line up on aarch64,
    // so we don't need to move most arguments or return values to different registers.
    // The only exception is x8, for the syscall number, which we'll take from x6.
    // https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm64-64_bit
    mov x8, x6
    svc #0
    ret
