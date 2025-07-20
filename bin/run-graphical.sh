qemu-system-aarch64 \
  -machine virt \
  -cpu cortex-a72 \
  -m 512m \
  -drive if=pflash,format=raw,file=boot/QEMU_EFI.fd,readonly=on \
  -drive if=pflash,format=raw,file=boot/QEMU_VARS.fd \
  -drive if=none,format=raw,file=disk.img,id=hd \
  -device virtio-blk-pci,drive=hd \
  -device nec-usb-xhci,id=xhci \
  -device usb-tablet,bus=xhci.0 \
  -device usb-kbd,bus=xhci.0 \
  -device ramfb \
  -serial mon:stdio \
  -nodefaults \
  $@
