
DIRS="libth init"

for dir in $DIRS; do
  make -C src/$dir $@
done
