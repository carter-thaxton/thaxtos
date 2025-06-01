set -e
set -o pipefail

(cd src/libth && ./build.sh)
(cd src/init && ./build.sh)

sudo cp src/init/init root/sbin/init
./copy-to-disk.sh
