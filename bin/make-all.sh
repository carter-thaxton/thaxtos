
DIRS="libth init test"

for dir in $DIRS; do
  make -C src/$dir $@
done
