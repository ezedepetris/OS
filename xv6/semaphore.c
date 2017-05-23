#include "semaphore.h"
#include <stddef.h>

struct {
  struct semaphore semaphore[MAXSEM];
  struct spinlock lock;
} stable;

// Create or get a descriptor of a semaphore.
// sem_id is semaphore identificator, or -1 if want to create a new.
// init_value only used when sem_id is -1.
int
semget(int sem_id, int init_value)
{
  if (sem_id == -1){ // want to create a new.
    if (stable.first == -1) // there aren't free semaphores.
      return -3;
    else{
      if (proc->smanager.scounter == MAXSEMPROC) // the process already got the max number of semaphores.
        return -2;
      else{
        int new_sem_id = obtainsem();
        anexsem(new_sem_id);
        proc->smanager.scounter++;
        stable.semaphore[new_sem_id] = malloc(sizeof(struct semaphore));
        stable.semaphore[new_sem_id].value = init_value;
        return new_sem_id;
      }
    }
  } else{
    if (stable.semaphore[sem_id] == -1) // sem_id is not in used.
      return -1;
    else
      return stable.semaphore[sem_id].sem_id;
  }
}

// Free some semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semfree(int sem_id)
{
  if (stable.semaphore[sem_id] == -1)
    return -1;

  if (stable.semaphore[sem_id].ref == 1)
    stable.semaphore[sem_id] = -1;
  else
    stable.semaphore[sem_id].ref--;

  return 0;
}

// Decrements semaphore's value and looks invoking process if the
// semaphore's value is negative.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semdown(int sem_id)
{
  if (stable.semaphore[sem_id] == -1)
    return -1;

  if (stable.semaphore[sem_id].value != 0)
    stable.semaphore[sem_id].value--;
  else
    sleep(proc, &ptable.lock);
  return 0;
}

// Increases semaphore's value and wake up some process in the queue
// waiting for that semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semup(int sem_id)
{
  struct proc *p;

  if (stable.semaphore[sem_id].ref == 0)
    return -1;

  stable.semaphore[sem_id].value++;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == SLEEPING)
      wakeup(p);
  }

  return 0;
}
