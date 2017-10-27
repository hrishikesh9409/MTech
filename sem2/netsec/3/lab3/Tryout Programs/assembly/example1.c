#include <stdio.h>

int add() {
  return 0xbeef;
}

int main()
{
  add ();
  return 0xdead;
}
