#!/usr/bin/bash
set -e

if [ "$1" = "clean" ]; then
  echo "Cleaning efi_boot"
  rm -rf *.o *.d *.so *.efi
elif [ "$1" = "install" ]; then
  echo "Installing efi_boot AS /EFI/BOOT/BOOTAA64.EFI"
  cp efi_boot.efi ../../root/boot/EFI/BOOT/BOOTAA64.EFI
else
  INCLUDE="-I/usr/include -I/usr/include/aarch64-linux-gnu/"
  CFLAGS="-ffreestanding -MMD -mno-red-zone -std=c17 -target aarch64-unknown-windows -Wall -Werror -pedantic $INCLUDE"
  LDFLAGS="-flavor link -subsystem:efi_application -entry:efi_main"

  clang $CFLAGS -c efi_boot.c -o efi_boot.o

  lld $LDFLAGS efi_boot.o -out:efi_boot.efi
  echo "Built efi_boot.efi"
fi
