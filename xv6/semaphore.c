#include "semaphore.h"

struct free {
  int sem_id;
  struct free *next;
};

struct {
  struct semaphore semaphore[MAXSEM];
  struct free *first;
  struct free *last;
} stable;

// Create or get a descriptor of a semaphore.
// sem_id is semaphore identificator, or -1 if want to create a new.
// init_value only used when sem_id is -1.
int
semget(int sem_id, int init_value)
{
  return 0;
}

// Free some semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semfree(int sem_id)
{
  return 0;
}

// Decrements semaphore's value and looks invoking process if the
// semaphore's value is negative.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semdown(int sem_id)
{
  return 0;
}

// Increases semaphore's value and wake up some process in the queue
// waiting for that semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semup(int sem_id)
{
  return 0;
}
