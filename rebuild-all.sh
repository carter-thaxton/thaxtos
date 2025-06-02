set -e

./make-all.sh clean
./make-all.sh
sudo ./make-all.sh install

echo "Copying root to disk"
./copy-to-disk.sh

echo "DONE"
