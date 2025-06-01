gcc -nostdinc -nostdlib -I../libth -c init.c
gcc -nostdlib -ffreestanding -no-pie -o init -I../libth init.o ../libth/start.o ../libth/libth.a
# gcc -nostdlib -ffreestanding -no-pie -o init -I../libth init.o ../libth/start.o -L../libth -lth
