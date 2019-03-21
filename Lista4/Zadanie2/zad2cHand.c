#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler (int sig)
{
	signal(10, handler);
	printf ("Handled signal: %d\n", sig);
  //sleep(1);
}

int main (void)
{

	signal(10, handler);
  for(int i = 0; i<100000; i++)
	{

		for(int j = 0; j<100000 ; j++);
	}
	return 0;
}
