gcc -nostdinc -nostdlib -I../libth -c init.c
gcc -nostdlib -ffreestanding -no-pie -L../libth -I../libth init.o ../libth/start.o -lth -o init
