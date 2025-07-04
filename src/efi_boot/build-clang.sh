INCLUDE="-I/usr/include -I/usr/include/aarch64-linux-gnu/"
CFLAGS="-ffreestanding -MMD -mno-red-zone -std=c11 -target aarch64-unknown-windows -Wall -Werror -pedantic $INCLUDE"
LDFLAGS="-flavor link -subsystem:efi_application -entry:efi_main"

clang $CFLAGS -c efi_boot.c -o efi_boot.o

lld $LDFLAGS efi_boot.o -out:efi_boot.efi
