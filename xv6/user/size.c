#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
  int i;
  int j = 0;
  for(i=1; i<1000; i++){
    j += j*i;
    // printf(1,"The size of my address space is %d bytes\n", getmysize());
    getmysize();
  }

  exit();
}
