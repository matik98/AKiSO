#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void convToNum(char* num) {
  while(*num != '\0') {
    if(*num == '.') {
      num++;
    }
    *num = *num - '0';
    num++;
  }
}


void size(char* number, int* first, int* second) {
  int counter = 0;
  while(*number != '\0') {
    if(*number == '.') {
      *first = counter;
      counter = 0;
      number++;
      break;
    };
    counter++;
    number++;
  }
  while(*number != '\0') {
    counter++;
    number++;
  }
  *second = counter;
}

bool div2(char* num, int lenght){
  bool rest = 0;
  char curNum;
  for(int i = 0; i < lenght; i++) {
    curNum = rest * 10 + *num;
    *num = curNum / 2;
    rest = curNum % 2;
    num++;
  }
  return rest;
}

void move(char* word) {
  char result [1024];
  int i = 0;
  while(word[i] != '\0') {
    result[i + 1] = word[i];
    i++;
  }
  result[i+1] = '\0';
  i = 1;
  while(result[i] != '\0') {
    word[i] = result[i];
    i++;
  }
  word[i] = '\0';

}

bool is0(char* number, int lenght) {
  for (int i = 0; i < lenght; i++) {
    if(*number != 0) {
      return false;
    }
    number++;
  }
  return true;
}


void conoz(char* num, int lenght) {
  char result[1024];
  result[0] = '\0';
  while(!is0(num, lenght)) {
    move(result);
    if(div2(num, lenght)) {
      result[0] = '1';
    } else {
      result[0] = '0';
    }
  }
  printf("%s", result);

}

bool mult2(char* num, int lenght) {
  bool carry = 0;
  for(int i = lenght -1; i >=0; i--) {
    num[i] = num[i] * 2 + carry;
    carry = num[i] > 9;
    if(carry) {
      num[i] = num[i] - 10;
    }
  }
  return carry;
}

void convuz(char* num, int lenght, int size) {
  for (int i = 0; i < size; i++){
    //printf("%d\n", num[0]);
    if(mult2(num, lenght)) {
      printf("1");
    } else {
      printf("0");
    }
  }
}





int main (int argc, char** argv) {
  if(argc != 3){
    printf("Składnia: <liczba> <liczba cyfr po przecinku>\n");
    return 0;
  }


  int size1;
  int size2;
  int size3 = atoi(argv[2]);
  convToNum(argv[1]);

  size(argv[1], &size1, &size2);

  char first[size1];
  char second [size2];
  for (int i = 0; i < size1; i++) {
      first[i] = argv[1][i];
  }

  for (int i = 0; i < size2; i++) {
      second[i] = argv[1][i + 1 + size1];
  }
  conoz(first, size1);
  printf(".");
  convuz(second, size2, size3);
  printf("\n");



}
