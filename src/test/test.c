
#include <stdio.h>
#include <malloc.h>

#define DBG_INT(expr) printf("%s = %d\n", #expr, expr)

void print_sizes() {
  DBG_INT(sizeof(char));
  DBG_INT(sizeof(char));
  DBG_INT(sizeof(short));
  DBG_INT(sizeof(int));
  DBG_INT(sizeof(long));
  DBG_INT(sizeof(float));
  DBG_INT(sizeof(double));

  DBG_INT(sizeof(u8));
  DBG_INT(sizeof(u16));
  DBG_INT(sizeof(u32));
  DBG_INT(sizeof(u64));

  DBG_INT(sizeof(i8));
  DBG_INT(sizeof(i16));
  DBG_INT(sizeof(i32));
  DBG_INT(sizeof(i64));

  DBG_INT(sizeof(f32));
  DBG_INT(sizeof(f64));
}

void test_printf() {
  printf("Testing %%, sizeof(f32) = %d and more\n", sizeof(f32));

  char tstbuf[34];
  isize ret = snprintf(tstbuf, sizeof(tstbuf), "Testing write %d, and %d, and %s to string\n", 69, 420, "XXX");
  printf("Got %d chars\n", ret);
  tstbuf[sizeof(tstbuf)-1] = 0;
  printf("String was: %s\n", tstbuf);
}

void test_malloc() {
  usize sz = 17;
  char* buf;

  while (sz < 1000000) {
    buf = malloc(sz);
    snprintf(buf, sz, "Testing write to buffer of size %d", sz);
    printf("The string '%s' is at %p\n", buf, buf);
    free(buf);
    sz *= 3;
  }
}

void test() {
  test_printf();
  test_malloc();
}

int main() {
  test();
  return 0;
}
