// #include "spinlock.h"

struct sem{
  int sid;                  // Semaphore identificator
  int value;                // Semaphore identificator
  int ref;                  // Process references counter
  struct spinlock lock;
};
