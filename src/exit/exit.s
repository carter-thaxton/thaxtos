.text

.global _start

_start:
    mov     x0, #1      // fd (STDOUT)
    ldr     x1, =msg    // msg
    ldr     x2, =len    // len
    mov     w8, #64     // write syscall
    svc     #0

    mov     x0, #105    // status = 0x69
    mov     w8, #93     // exit syscall
    svc     #0

.data

msg:
    .ascii        "Welcome to thaxtos\n"
len = . - msg
