.text

.global _start

_start:
    bl main

    // if we reach here, call exit with return value of main in x0
    mov     w8, #93     // exit syscall
    svc     #0
