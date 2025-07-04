CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -mgeneral-regs-only -ffreestanding -nostdlib -shared"

#LDFLAGS="-Wl,--subsystem,10 -e efi_main"
LDFLAGS="-e efi_main -Wl,--defsym=EFI_SUBSYSTEM=10"

gcc $CFLAGS $LDFLAGS -o efi_boot.so efi_boot.c

objcopy -j .text -j .sdata -j .data -j .dynamic -j .rodata -j .rel \
        -j .rela -j .rel.* -j .rela.* -j .rel* -j .rela* \
        -j .areloc -j .reloc -O binary efi_boot.so efi_boot.efi
