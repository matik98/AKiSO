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
  for(int i = 0; i<10; i++)
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

    if (execvp(args[0], args) == -1)
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
    if (execvp(args[0], args) == -1)
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
