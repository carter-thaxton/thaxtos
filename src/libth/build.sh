gcc -nostdinc -I. -c syscalls.c
ar rs libth.a syscalls.o
