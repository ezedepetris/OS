// Memory test using factorial funtion

#include "types.h"
#include "stat.h"
#include "user.h"

int
factorial(int number)
{
  if (number == 1)
    return 1;
  else
    return number * factorial(number-1);
}

void
biglocalarray(int number)
{
  int array[number];
  printf(1, "%d\n", array[0]);
}

int
main(int argc, char const *argv[])
{
  int number;

  number = factorial(atoi(argv[1]));
  printf(1, "%d\n", number);
  // biglocalarray(atoi(argv[1]));

  exit();
}
