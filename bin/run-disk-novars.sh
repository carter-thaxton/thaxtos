qemu-system-aarch64 \
  -machine virt \
  -cpu cortex-a72 \
  -m 512m \
  -drive if=pflash,format=raw,file=boot/QEMU_EFI.fd,readonly=on \
  -drive if=none,format=raw,file=disk.img,id=hd \
  -device virtio-blk-pci,drive=hd \
  -serial stdio \
  -nographic \
  -nodefaults \
  $@
