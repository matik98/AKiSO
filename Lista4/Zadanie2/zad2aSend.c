#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main (int argc, char** argv)
{
	int pid = atoi (argv[1]);
	for(int i =1; i<=64; i++)
	{
		if(kill(pid, i) == 0){
			printf("Succesfull sended signal : %d \n", i);
			sleep(1);
		} else {
			printf("Error: signal %d\n", i);
		}
	}
	return 0;
}
