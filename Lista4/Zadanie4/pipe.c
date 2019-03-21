#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>


int findPipe(char** signsList);
char** beforPipe(char** args);
char** afterPipe(char** args);
void runPipe();

int main(void)
{
  char linia[] = " echo ala | tr 'a' 'c' | tr 'l' 'k' | tr 'c' 's'" ;

  char** args = createArgs(linia);
  runPipe(args);
  return 0;
}

int findPipe(char** signsList)
{
  //returns number of pipes if is located correctly
  //returns 0 if don't find
  //return -1 if will find in wrog place
  int iterator = 0;
  int count = 0;

  while (signsList[iterator] != NULL)
  {
    if(strcmp(signsList[iterator], "|")==0)
    {
      if(signsList[iterator+1]==NULL || iterator==0 ||
        strcmp(signsList[iterator - 1], "|")==0 ||
        strcmp(signsList[iterator + 1], "|")==0)
      {
        return -1;
      } else {
        count ++;
      }
    }
    iterator++;
  }
  return count;
}


char** beforPipe(char** args)
{
  char** editedArgs = malloc(64 * sizeof(char*));
  int location = 0;
  int iterator = 0;
  while(args[iterator] != NULL)
  {
    if(strcmp(args[iterator], "|")==0)
    {
      location = iterator;
    }
    iterator++;
  }
  for(int i =0 ; i<location; i++)
  {
    editedArgs[i] = args[i];
  }
  editedArgs[location] = NULL;
  return editedArgs;
}

char** afterPipe(char** args)
{
  char** editedArgs = malloc(64 * sizeof(char*));
  int location = 0;
  int iterator = 0;
  int i = 0;
  while(args[iterator] != NULL)
  {
    if(strcmp(args[iterator], "|")==0)
    {
      location = iterator;
    }
    iterator++;

  }
  while(args[location + i] != NULL)
  {
    editedArgs[i] = args[i+location + 1];
    i++;
  }
  editedArgs[i] = NULL;
  return editedArgs;
}

void runPipe(char** args)
{
  int t2, t1, i;
  int pipefd[2];
  char** firstPart = beforPipe(args);
  char** secondPart = afterPipe(args);

  i = pipe(pipefd);
  if(i<0) {
    perror ("lsh: pipe");
    exit(1);
  }

  if(fork() == 0)
  {
    if(dup2(pipefd[1], 1)!= 1)
    {
      perror("lsh");
      exit(1);
    }

    close(pipefd[1]);
    close(pipefd[0]);

    if(findPipe(firstPart)>0)
    {
      runPipe(firstPart);
    } else {
      execvp(firstPart[0], firstPart);
      perror("lsh: blad");
      exit(1);
    }
  } else if (fork() == 0)
  {
    if ( dup2(pipefd[0], 0) != 0)
    {
      perror("lsh");
      exit(1);
    }
    close(pipefd[1]);
    close(pipefd[0]);
    execvp(secondPart[0], secondPart);
    perror("lsh");
    exit(1);
  } else
  {
    close(pipefd[1]);
    close(pipefd[0]);
    wait(&t1);
    wait(&t2);
    if (WEXITSTATUS(t1) || WEXITSTATUS(t2))
    {
      fprintf(stderr, "Blad. Wyjscie.\n");
    }
  }
}
