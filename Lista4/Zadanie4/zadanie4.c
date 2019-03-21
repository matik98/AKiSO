#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>



#define OPEN_BACKGROUND_PROC_BUFFSIZE 128

void printuj(char **args); //DO USUNIECIA
void childHandler (void);

int findPipe(char** signsList);
char** beforPipe(char** args);
char** afterPipe(char** args);
void runPipe();

int findAmpersand(char** signsList);
bool cd(char** args);
bool exits(void);

bool run(char** args, int* runProcess);
bool runAmpersand(char** args);
bool execute(char** args, int* runProcess);
char** createArgs(char* line);
char* readLine(void);
void loop(int* runProcess);





int main (void)
{

  int runProcess=1;
  int* backgroundProcess = malloc(sizeof(int) * OPEN_BACKGROUND_PROC_BUFFSIZE);

  void ctrCHandler (int dummy)
  {
  	signal(2, ctrCHandler);
  	kill(runProcess, 2);
  }

  signal(2, ctrCHandler);

  if(backgroundProcess == NULL)
  {
    fprintf(stderr, "lsh: not enough memory\n");
    exit(EXIT_FAILURE);
  }
  backgroundProcess[0]=0;
  loop(&runProcess);
}

void childHandler (void)
{
  for(int i = 0; i<100; i++)
  {
    waitpid(-1, NULL, WNOHANG);
  }
}

void loop(int* runProcess)
{
  char* line;
  char** args;
  bool status;
  do {
    childHandler();
    printf("-->");
    line = readLine();
    args = createArgs(line);
    status = execute(args, runProcess);

    free(line);
    free(args);
  } while (status);
}

char* readLine(void)
{
  char* line = NULL;
  int bufsize = 0;
  if(getline(&line, &bufsize, stdin)==-1){
    exit(EXIT_FAILURE);
  }
  return line;
}


char** createArgs(char line[])
{
  const int ARGS_LIST_BUFSIZE = 64;
  const char* SEPARATE_SIGNS = " \t\r\n\a";


  int bufsize = ARGS_LIST_BUFSIZE;
  int iterator = 0;
  char** args = malloc(bufsize * sizeof(char*));
  char* argument;

  if(args==NULL)
  {
    fprintf(stderr, "lsh: not enough memory\n");
    exit(EXIT_FAILURE);
  }

  argument = strtok(line, SEPARATE_SIGNS);

  while(argument !=NULL) {
    args[iterator] = argument;
    iterator++;

    if(iterator >= bufsize) {
      bufsize = bufsize + ARGS_LIST_BUFSIZE;
      args = realloc(args, bufsize * sizeof(char*));
      if(args!=NULL)
      {
        fprintf(stderr, "lsh: not enough memory\n");
        exit(EXIT_FAILURE);
      }
    }
    argument = strtok(NULL, SEPARATE_SIGNS);

  }
  args[iterator]=NULL;
  return args;
}

int findAmpersand(char** signsList)
{
  //returns 1 if is located correctly
  //returns 0 if don't find
  //return -1 if will find in wrog place
  int iterator = 0;
  while (signsList[iterator] != NULL)
  {
    if(strcmp(signsList[iterator], "&")==0)
    {
      if(signsList[iterator+1]==NULL && iterator>0)
      {
        return 1;
      } else {
        return -1;
      }
    }
    iterator++;
  }
  return 0;
}


void printuj(char **args)
{
  int i = 0;
  while(args[i]!=NULL)
  {
    printf("%s\n", args[i]);
    i++;
  }
}

bool runAmpersand(char** args)
{
  int pid;
  pid = fork();
  if(pid == 0) {
    if(findPipe(args) >0)
    {
      runPipe(args);
    } else if (execvp(args[0], args) == -1)
    {
      perror("lsh");
    }
  } else if (pid < 0) {
    perror ("lsh");
  } else {
    return true;
  }
  return true;
}

bool run(char** args, int* runProcess)
{
  int pid;
  int status;
  pid = fork();
  if(pid == 0) {
    if(findPipe(args)>0)
    {
      runPipe(args);
      exit(0);
    }else if (execvp(args[0], args) == -1)
    {
      perror("lsh");
    }
  } else if (pid < 0) {
    perror ("lsh");
  } else {
    *runProcess = pid;
    do {
      waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return true;

}

bool cd(char** args)
{
  if( args[1] == NULL || args[2]!=NULL)
  {
    fprintf(stderr, "lsh: wrong argument to cd \n");
  } else {
    if ( chdir(args[1]) != 0 ) {
      perror("lsh");
    }
  }
  return true;
}

bool exits(void)
{
  return 0;
}

bool execute(char** args, int* runProcess)
{
  if (args[0] == NULL) {
    return true;
  } else if (strcmp(args[0], "cd") == 0) {
    return cd(args);
  } else if (strcmp(args[0], "exit") == 0) {
    return exits();
  } else if(findAmpersand(args) == 1) {
    char** updatedArgs = args;
    int iterator = 0;
    while(updatedArgs[iterator] != NULL)
    {
      if(updatedArgs[iterator+1] ==NULL)
      {
        updatedArgs[iterator] = NULL;
      }
      iterator++;
    }
    return runAmpersand(updatedArgs);
  }
  else{
    return run(args, runProcess);
  }
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
      return;
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
    return;
  }
}
