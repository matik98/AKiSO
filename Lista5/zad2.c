#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int size;
int iterator = 0;
int** A;
int** B;
int** result;
pthread_mutex_t mutex;

void fill(int** table) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      table[i][j] = rand()%2;
    }
  }
}

void count(int x, int y) {
  for(int i = 0; i < size; i++) {
    if((A[x][i] * B[i][y]) == 1) {
      result[x][y] = 1;
      return;
    }
  }
  result[x][y]=0;
}

void* count_line(void *arg) {
  while(iterator < size) {
    pthread_mutex_lock(&mutex);
    int line = iterator;
    iterator++;
    pthread_mutex_unlock(&mutex);
    if(line >= size){
      break;
    }
    for(int i = 0; i < size; i++) {
      count(line, i);
    }
  }
  return NULL;
}

void printuj() {
  for( int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%d", A[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  for( int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%d", B[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  for( int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%d", result[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {
  size = atoi(argv[1]);
  int threads = atoi(argv[2]);
  A = malloc(size * sizeof(int*));
  B = malloc(size * sizeof(int*));
  result = malloc(size * sizeof(int*));
  for(int i = 0; i < size ; i++){
    A[i] = malloc(size * sizeof(int));
    B[i] = malloc(size * sizeof(int));
    result[i] = malloc(size * sizeof(int));
  }
  fill(A);
  fill(B);

  pthread_t tids[threads];
  for (int i = 0; i < threads; i++) {
    pthread_create(tids + i, NULL, count_line, NULL);
  }
  for (int i = 0; i < threads; i++) {
    pthread_join(tids[i], NULL);
  }


}
