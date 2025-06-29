CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -mgeneral-regs-only -ffreestanding -nostdlib"
#LDFLAGS="-Wl,--subsystem,10 -e efi_main"
LDFLAGS="-e efi_main"

gcc $CFLAGS $LDFLAGS -o BOOTAA64.EFI efi_boot.c
