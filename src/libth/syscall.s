.text

.global _syscall

_syscall:
    // Thankfully, Linux syscall and C ABI conventions mostly line up on aarch64,
    // so we don't need to move most arguments or return values to different registers.
    // The only exception is x8, for the syscall number, which we'll take from x6.
    // https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm64-64_bit
    mov x8, x6
    svc #0
    ret
