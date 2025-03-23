#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int initial_size;
  char *mem;
  int after_malloc_size;
  int final_size;
  
  // Get initial memory size
  initial_size = memsize();
  printf("Initial memory size: %d bytes\n", initial_size);
  
  // Allocate 20KB of memory
  mem = malloc(20 * 1024);
  if(mem == 0) {
    printf("malloc failed\n");
    exit(1,"");
  }
  
  // Use the memory to ensure it's actually allocated
  mem[0] = 'x';
  
  // Check memory size after allocation
  after_malloc_size = memsize();
  printf("Memory size after allocation: %d bytes\n", after_malloc_size);
  printf("Difference: %d bytes\n", after_malloc_size - initial_size);
  
  // Free the allocated memory
  free(mem);
  
  // Check final memory size
  final_size = memsize();
  printf("Memory size after free: %d bytes\n", final_size);
  printf("Difference from initial: %d bytes\n", final_size - initial_size);
  
  exit(0,"");
}