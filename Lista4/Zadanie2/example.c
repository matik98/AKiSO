#include <stdio.h>
#include <stdlib.h>
int main()
{
  char* line = NULL;
  size_t bufforSize = 0;
  getline(&line, &bufforSize, stdin);
  printf("%s", line);
  return 0;
}
