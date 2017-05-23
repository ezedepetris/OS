#include "spinlock.h"

struct semaphore{
  int id;                     // Semaphore identificator
  int sem_number;            // Semaphore identificator
  int reference_counter;    // Process references counter
  struct spinlock lock;
};
