#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;
  int j = 0;
  for(i=1; i<1000; i++){
    j += j*i;
    printf(1, "run \n");
  }

  exit();
}
