#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
  int sid;
  sid = semget(-1,3);
  printf(1,"paso el semget\n");
  semdown(sid);
  semup(sid);
  semfree(sid);
  exit();
}
