#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler (int sig)
{
	signal(sig, handler);
	printf ("Handled signal: %d\n", sig);
}

int main (void)
{
	for(int i = 1; i<=64; i++)
	{
		signal(i, handler);
	}

	for(int i = 0; i<100000; i++)
	{

		for(int j = 0; j<200000 ; j++);
	}
	return 0;
}
