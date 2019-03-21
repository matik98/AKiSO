#include <unistd.h>
#include <stdlib.h>

void print_string(char s[]){
  int size = 0;
  while(s[size]!='\0')
  {
    size++;
  }
  write(1, s, size);
}

void print_char(char c){
  write(1, &c, 1);
}

void print_dint(int d){
  int num = d;
  if(num < 0) {
    num = num * -1;
  }
  char* print[15];
  int i = -1;
  do {
    i++;
    print[i] = '0' + num%10;
    num = num/10;
  } while (num!= 0);
  if (d < 0) {
    i++;
    print[i] = '-';
  }
  while(i > -1) {
    print_char(print[i]);
    i--;
  }
}

void print_xint(int x){
  int num = x;
  if(num < 0) {
    num = num * -1;
  }
  char* print[10];
  int i = -1;
  do {
    i++;
    if((num%16) > 9) {
      print[i] = 'A' - 10 + num%16;
    } else {
      print[i] = '0' + num%16;
    }
    num = num/16;
  } while (num!= 0);
  if (x < 0) {
    i++;
    print[i] = '-';
  }
  while(i > -1) {
    print_char(print[i]);
    i--;
  }
}

void print_bint(int b){
  int num = b;
  if(num < 0) {
    num = num * -1;
  }
  char* print[32];
  int i = -1;
  do {
    i++;
    print[i] = '0' + num%2;
    num = num/2;
  } while ((num)!= 0);
  if (b < 0) {
    i++;
    print[i] = '-';
  }
  while(i > -1) {
    print_char(print[i]);
    i--;
  }
}

void myprintf(char *fd, ...){

  int arg = 0;

  char c;
  int i;
  char*s;

  while (*fd) {
    if(*fd == '%') {
      fd++;
      arg++;

      if(*fd =='c') {
        c = *((int*) ((char*)&fd + arg * sizeof(void*)));
        print_char(c);
      } else if (*fd == 'd') {
        i = *((int*) ((char*)&fd + arg * sizeof(void*)));
        print_dint(i);
      } else if (*fd == 'x') {
        i = *((int*) ((char*)&fd + arg * sizeof(void*)));
        print_xint(i);
      } else if (*fd == 'b') {
        i = *((int*) ((char*)&fd + arg * sizeof(void*)));
        print_bint(i);
      } else if (*fd == 's') {
        s = *((int*) ((char*)&fd + arg * sizeof(void*)));
        print_string(s);
      }
    } else {
      print_char(*fd);
    }
    fd++;
  }
}

int strtoint(char* str, int base) {
  int result = 0;
  int minus = 0;
  if (*str == '-') {
    minus = 1;
  }

  while (*str != '\0') {
    if (*str >= '0' && *str <= '9') {
      result = result*base + (*str - '0');
    } else if (*str >= 'a' && *str <= 'f'){
      result = result*base + (*str - 'a' + 10);
    }
    str++;
  }

  if (minus == 1) {
    result = -result;
  }
  return result;

}

void myscanf(char* fd, ...) {
  char* argv = (char*)&fd + sizeof(char*);

  while(*fd) {
    if (*fd == '%') {
      char* scan = malloc(1024 * sizeof(char));

      int i = -1;
      char* sign = malloc(sizeof(char));

      do {
        i++;
        read(0, sign, sizeof(char));
        scan[i] = *sign;
      } while (*sign != '\n' && *sign != *(fd + 2));
      free(sign);

      scan[i]='\0';

      fd++;
      if (*fd == 'd') {
        int* result = (int*)(*(int*)argv);
        *result = strtoint( scan, 10);
        argv = argv + sizeof(int*);
      } else if (*fd == 'b') {
        int* result = (int*)(*(int*)argv);
        *result = strtoint( scan, 2);
        argv = argv + sizeof(int*);
      } else if (*fd == 'x') {
        int* result = (int*)(*(int*)argv);
        *result = strtoint( scan, 16);
        argv = argv + sizeof(int*);
      } else if (*fd == 's') {
        char* result = (char*)(*(int*)argv);
        int i = 0;
        while (scan[i] != '\0') {
          result[i] = scan[i];
          i++;
        }
        result[i] = '\0';
        argv = argv + sizeof(char*);
      }
    }
    fd++;
  }
}

int main(){
  int num;
  myscanf ("To %d", &num);
  myprintf("%s w dec: %d, w hex: %x, w bin: %b%c", "ala", num, 51, 51, '\n');
  return 0;
}
