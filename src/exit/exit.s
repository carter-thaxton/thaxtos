.text

.global _start

_start:
    mov     x0, #105    // status
    mov     w8, #93     // exit syscall is #93
    svc     #0          // invoke syscall
