.text

.global _start

_start:
    ldr x0, [sp]        // argc
    add x1, sp, #8      // argv

    // envp = argv + argc + 1
    add x2, x1, w0, sxtw 3
    add x2, x2, #8

    bl main

    // if we reach here, call exit with return value of main in x0
    mov     w8, #93     // exit syscall
    svc     #0
