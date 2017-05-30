#include "semaphore.h"
// #include "spinlock.h"

// extern int getindex(struct proc *p);

struct {
  struct sem sem[MAXSEM];
  struct spinlock semlock;
} stable;

int nextsid = 1;

// Init stable setting -1 in all positions
void
sinit(void)
{
  initlock(&stable.semlock, "stable");
  acquire(&stable.semlock);
  struct sem *s;
  for(s = stable.sem; s < stable.sem + MAXSEM; s++){
    s->sid = -1;
    s->ref = 0;
  }
  release(&stable.semlock);
}

// Create or get a descriptor of a semaphore.
// sem_id is semaphore identificator, or -1 if want to create a new.
// init_value only used when sem_id is -1.
int
semget(int sem_id, int init_value)
{
  struct sem *s;
  int index;

  acquire(&stable.semlock);
  if (sem_id == -1){ // want to create a new.
    for(s = stable.sem; s < stable.sem + MAXSEM; s++)
      if(s->ref == 0){
        if (proc->scounter == MAXSEMPROC){ // the process already got the max number of semaphores.
          release(&stable.semlock);
          return -2;
        }
        s->ref = 1;
        s->value = init_value;
        s->sid = nextsid;
        nextsid++;
        index = getindex(proc);
        proc->smanager[index] = s;
        release(&stable.semlock);
        return s->sid;
      }
     // there aren't free semaphores.
    release(&stable.semlock);
    return -3;
    }
    else{
    if (stable.sem[sem_id].sid == -1){ // sem_id is not in used.
      release(&stable.semlock);
      return -1;
    }
    else{
      if (proc->scounter == MAXSEMPROC){ // the process already got the max number of semaphores.
        release(&stable.semlock);
        return -2;
      }
      else{
        release(&stable.semlock);
        stable.sem[sem_id].ref++;
        return stable.sem[sem_id].sid;
      }
    }
  }
  return 0;
}

// Free some semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semfree(int sem_id)
{
  if (stable.sem[sem_id].sid == -1)
    return -1;

  if (stable.sem[sem_id].ref == 1)
    stable.sem[sem_id].sid = -1;

  stable.sem[sem_id].ref--;
  proc->scounter--;

  return 0;
}

// Decrements semaphore's value and looks invoking process if the
// semaphore's value is negative.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semdown(int sem_id)
{
  if (stable.sem[sem_id].sid == -1)
    return -1;

  acquire(&stable.sem[sem_id].lock);
  while (stable.sem[sem_id].value == 0)
    sleep(proc, &stable.sem[sem_id].lock);

  stable.sem[sem_id].value--;
  release(&stable.sem[sem_id].lock);

  return 0;
}

// Increases semaphore's value and wake up some process in the queue
// waiting for that semaphore.
// sem_id is semaphore identificator.
// Return -1 in error case, 0 in othewise.
int
semup(int sem_id)
{
  if (stable.sem[sem_id].ref == 0)
    return -1;

  stable.sem[sem_id].value++;

  struct sem *saux;
  saux = &stable.sem[sem_id];
  wakeup(saux);

  return 0;
}
