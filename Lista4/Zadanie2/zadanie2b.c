#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main (void)
{
	for(int i =1; i<=64; i++)
	{
		if(kill(1, i) == 0){
			printf("Succesfull sended signal : %d \n", i);
			sleep(1);
		} else {
			printf("Error: signal %d\n", i);
		}
	}
	return 0;
}
