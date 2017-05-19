#define MAXSEM = 3;

struct semaphore{
  int sem_id;               // Semaphore identificator
  int value;                // Semaphore looked or unlooked
  int reference_counter;    // Process references counter
};