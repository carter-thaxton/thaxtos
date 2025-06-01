set -e
set -o pipefail

(cd src/libth && ./build.sh)
(cd src/init && ./build.sh)

sudo cp src/libth/libth.a root/lib/
sudo cp src/libth/libth.so root/lib/
sudo cp src/init/init root/sbin/

./copy-to-disk.sh
