#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define MAXITEMS 5

int
main(int argc, char *argv[])
{
  int space, items; // semaphores
  int pid;

  space = semget(-1, MAXITEMS-3);
  items = semget(-1, 0);

  pid = fork();
  if (pid){
    space = semget(space, MAXITEMS);
    items = semget(items, 0);
    int i;
    for (i = 0; i < MAXITEMS; ++i){
      semdown(space);
      printf(1, "PROC %d says: We are producing\n", getpid());
      sleep(20);
      semup(items);
    }
  }
  else{
    int i;
    for (i = 0; i < MAXITEMS; ++i){
      semdown(items);
      printf(1, "PROC %d says: We are consuming\n", getpid());
      semup(space);
    }
  }

  semfree(items);
  semfree(space);

  exit();
}