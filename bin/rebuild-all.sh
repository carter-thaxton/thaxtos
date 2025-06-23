set -e

bin/make-all.sh clean
bin/make-all.sh
sudo bin/make-all.sh install

echo "Copying root to disk"
bin/copy-to-disk.sh

echo "DONE"
