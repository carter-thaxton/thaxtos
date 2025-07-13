set -e

sudo modprobe nbd max_part=8
sudo qemu-nbd -c /dev/nbd1 -f raw disk.img
sleep 0.1
echo /dev/nbd1 connected
sudo mount -v /dev/nbd1p2 ./mnt && sudo mkdir -p ./mnt/boot
sudo mount -v /dev/nbd1p1 ./mnt/boot
