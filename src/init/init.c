
int _syscall(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, int syscall_number);

int strlen(char* str) {
  int len = 0;
  while (*str) {
    len++;
    str++;
  }
  return len;
}

int main() {
  char *msg = "Welcome to Thaxtos!\n";
  int len = strlen(msg);

  _syscall((void*) 1, msg, (void*) (long) len, 0, 0, 0, 64);

  return 69;
}
