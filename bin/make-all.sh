set -e

# subdirectories of src that contain a Makefile, putting lib first
LIB_MAKEFILE_DIRS=$(find src/ -name 'Makefile' -printf "%h\n" | grep 'lib' | sort -u)
NOLIB_MAKEFILE_DIRS=$(find src/ -name 'Makefile' -printf "%h\n" | grep -v 'lib' | sort -u)
for dir in $LIB_MAKEFILE_DIRS $NOLIB_MAKEFILE_DIRS; do
  make -C $dir $@
done

# subdirectories of src that contain make.sh
MAKESH_DIRS=$(find src/ -name 'make.sh' -printf "%h\n" | sort -u)
for dir in $MAKESH_DIRS; do
  (cd $dir && ./make.sh $@)
done
