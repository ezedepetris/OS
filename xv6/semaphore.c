#include "semaphore.h"
#include <stddef.h>

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
  if (sem_id == -1){
    // want to create a new
    if (stable->first == NULL){
      // there aren't free semaphores
      return -3;
    } else{
      if (proc->smanager.scounter == MAXSEMPROC){
        // the process already got the max number of semaphores
        return -2;
      } else{
        semaphore sem = obtain();
        anex(sem_id);
        proc->smanager.scounter++;
        return sem_id;
      }
    }
  } else{
    if (stable[sem_id] == NULL){
      // sem_id is not in used
      return -1;
    } else{
      return stable[sem_id]->id
    }
  }
}

// Free some semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semfree(int sem_id)
{
  if (array[sem_id] == NULL)
    return -1;

  array[sem_id] = NULL
  return 0;
}

// Decrements semaphore's value and looks invoking process if the
// semaphore's value is negative.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semdown(int sem_id)
{
  if (array[sem_id] == NULL)
    return -1;

  array[sem_id]->sem_number--;
  return 0;
}

// Increases semaphore's value and wake up some process in the queue
// waiting for that semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semup(int sem_id)
{
  if (array[sem_id] == NULL)
    return -1;

  array[sem_id]->sem_number++;
  return 0;
}

int
obtain()
{
  struct free saux = stable.first;
  int id = saux->sem_id
  stable.first = stable.first->next;
  free(saux);
  return id;
}

void
annex(int sem_id)
{
  struct free ns = malloc(sizeof(struct free));
  ns->next = NULL;
  stable.last->next = ns;
}
