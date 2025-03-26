#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ARRAY_SIZE (1 << 16)  // 2^16 = 65536
#define NUM_CHILDREN 4

int array[ARRAY_SIZE];

int
main(void)
{
  int pids[NUM_CHILDREN];
  int n, statuses[64];  // NPROC = 64 by default
  int result;
  int i, start, end;
  int sum;
  long long total_sum = 0;  // Use long long to prevent overflow
  
  // Initialize array with consecutive numbers
  for (i = 0; i < ARRAY_SIZE; i++) {
    array[i] = i;
  }
  
  printf("Creating %d child processes...\n", NUM_CHILDREN);
  
  // Create child processes
  result = forkn(NUM_CHILDREN, pids);
  
  if (result < 0) {
    exit(1, "forkn failed");
  }
  
  if (result == 0) {
    // Parent process
    printf("Created %d child processes with PIDs: ", NUM_CHILDREN);
    for (i = 0; i < NUM_CHILDREN; i++) {
      printf("%d ", pids[i]);
    }
    printf("\n");
    
    // Wait for all children to finish
    if (waitall(&n, statuses) < 0) {
      exit(1, "waitall failed");
    }
    
    // Verify all children finished
    if (n != NUM_CHILDREN) {
      printf("Expected %d children, got %d\n", NUM_CHILDREN, n);
      exit(1, "wrong number of children");
    }
    
    // Print results from each child and calculate total sum
    total_sum = 0;
    for (i = 0; i < n; i++) {
      int child_num = i + 1;
      int chunk_size = ARRAY_SIZE / NUM_CHILDREN;
      int start = (child_num - 1) * chunk_size;
      int end = (child_num == NUM_CHILDREN) ? ARRAY_SIZE : child_num * chunk_size;
      
      printf("Child %d (PID %d): Sum of elements %d to %d is %d\n", 
             child_num, pids[i], start, end - 1, statuses[i]);
      
      total_sum += statuses[i];
    }
    
    printf("Total sum: %d\n", (int)total_sum);
    
    // Check if sum is correct
    if (total_sum == 2147450880) {
      exit(0, "Sum is correct!");
    } else {
      exit(1, "Sum is incorrect!");
    }
  } else {
    // Child process - only calculate, don't print
    int child_num = result;
    
    // Calculate start and end indices for this child
    int chunk_size = ARRAY_SIZE / NUM_CHILDREN;
    start = (child_num - 1) * chunk_size;
    end = (child_num == NUM_CHILDREN) ? ARRAY_SIZE : child_num * chunk_size;
    
    // Calculate sum for this chunk
    sum = 0;
    for (i = start; i < end; i++) {
      sum += array[i];
    }
    
    // Just return the sum as exit status without printing
    exit(sum, "");
  }
}