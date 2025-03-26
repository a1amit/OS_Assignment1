// filepath: /workspaces/xv6-riscv/user/testpipe.c
#include "kernel/types.h"
#include "user/user.h"

int main() {
  printf("This will go through the pipe\n");
  exit(0, "Test exit message");
}