
SRC=../../github/optimized-routines
OBJ="memcpy memset memchr memrchr memcmp strcpy stpcpy strcmp strchr strrchr strchrnul strlen strnlen strncmp"
# TODO: add math routines as they are needed

rm -f *.a *.so
rm -rf obj obj-all
mkdir -p obj obj-all

cp $SRC/build/lib/* .

ar -x --output obj-all libnetworking.a
ar -x --output obj-all libstringlib.a
ar -x --output obj-all libmathlib.a

for obj in $OBJ; do
  cp obj-all/$obj.o obj/
done
