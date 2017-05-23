#include "spinlock.h"

struct semaphore{
  int sem_id;               // Semaphore identificator
  int value;                // Semaphore identificator
  int reference_counter;    // Process references counter
  struct spinlock lock;
};
