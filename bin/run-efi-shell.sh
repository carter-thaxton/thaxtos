qemu-system-aarch64 \
  -machine virt \
  -cpu cortex-a72 \
  -m 512m \
  -drive if=pflash,format=raw,file=boot/QEMU_EFI.fd,readonly=on \
  -drive if=pflash,format=raw,file=boot/QEMU_VARS.fd \
  -serial stdio \
  -nographic \
  -nodefaults
