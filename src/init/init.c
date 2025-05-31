
int _syscall(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, int syscall_number);

int strlen(char* str) {
  int len = 0;
  while (*str) {
    len++;
    str++;
  }
  return len;
}

void print(char *msg) {
  int len = strlen(msg);
  _syscall((void*) 1, msg, (void*) (long) len, 0, 0, 0, 64);
}

void delay(long ticks) {
  for (long i=0; i < ticks; i++) {
    // for now - just spin
    ;
  }
}

int main() {
  print("Welcome to Thaxtos!\n");

  while (1) {
    delay(100000000);
    print("TICK\n");
  }

  return 69;
}
