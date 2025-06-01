as syscall.s -o syscall.o
gcc -nostdinc -I. -c syscalls.c
gcc -nostdinc -I. -c string.c
ar rs libth.a syscall.o syscalls.o string.o
