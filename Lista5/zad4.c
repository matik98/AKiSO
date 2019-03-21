#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>



int** A;
int** B;
int** C;
int** D;

int size;

void fill(int** tab) {
  for (int i =0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      tab[i][j] = rand();
    }
  }
}

void mn1() {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int sum = 0;
      for (int k = 0; k < size; k++) {
        sum = sum + A[i][k]*B[k][j];
      }
      C[i][j] = sum;
    }
  }
}

void transpoz(int** tab) {
  for (int i = 0; i < size; i++) {
    for ( int j = 0; j < size; j++) {
      tab[i][j] = B[j][i];
    }
  }
}

void mn2() {
  int** tab;
  tab = malloc(size*sizeof(int*));
  for (int i = 0; i < size; i++) {
    tab[i] = malloc(size*sizeof(int));
  }
  transpoz(tab);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int sum = 0;
      for (int k = 0; k < size; k++) {
        sum = sum + A[i][k]*tab[j][k];
      }
      D[i][j] = sum;
    }
  }
}

int main(int argc, char** argv) {
  struct timeval t1;
  struct timeval t2;
  struct timeval t3;
  struct timeval t4;
  size = atoi(argv[1]);
  A = malloc(size*sizeof(int*));
  B = malloc(size*sizeof(int*));
  C = malloc(size*sizeof(int*));
  D = malloc(size*sizeof(int*));

  for (int i = 0; i < size; i++) {
    A[i] = malloc(size*sizeof(int));
    B[i] = malloc(size*sizeof(int));
    C[i] = malloc(size*sizeof(int));
    D[i] = malloc(size*sizeof(int));
  }

  gettimeofday(&t1, NULL);
  mn1();
  gettimeofday(&t2, NULL);
  printf("Zajelo %d\n", (int)(t2.tv_usec - t1.tv_usec));

  gettimeofday(&t3, NULL);
  mn1();
  gettimeofday(&t4, NULL);
  printf("Zajelo %d\n", (int)(t4.tv_usec - t3.tv_usec));
}
