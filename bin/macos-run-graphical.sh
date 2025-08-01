/opt/homebrew/opt/socket_vmnet/bin/socket_vmnet_client /opt/homebrew/var/run/socket_vmnet \
  qemu-system-aarch64 \
    -machine virt,accel=hvf \
    -cpu host \
    -smp 8 \
    -m 1024m \
    -drive if=pflash,format=raw,file=boot/QEMU_EFI.fd,readonly=on \
    -drive if=pflash,format=raw,file=boot/QEMU_VARS.fd \
    -drive if=none,format=raw,file=disk.img,id=hd \
    -device virtio-blk-pci,drive=hd \
    -device virtio-net-pci,netdev=net0 -netdev socket,id=net0,fd=3 \
    -device nec-usb-xhci,id=xhci \
    -device usb-tablet,bus=xhci.0 \
    -device usb-kbd,bus=xhci.0 \
    -device ramfb \
    -serial mon:stdio \
    -nodefaults \
    $@

