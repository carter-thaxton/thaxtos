as start.s -o start.o
as syscall.s -o syscall.o
gcc -nostdinc -I. -c syscalls.c
gcc -nostdinc -I. -c string.c
ld -shared -nostdlib -o libth.so syscall.o syscalls.o string.o
ar rs libth.a syscall.o syscalls.o string.o
