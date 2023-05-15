#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void head(FILE* file, char* arg, char* numarg){
  //Read up to 10 lines
  int num = 10; //number of lines to print
  int counter = 0;

  char line[128];
  int numFromNumarg = atoi(numarg);


  //Deal with arguments
  if (strcmp(arg, "-n") == 0) {
    if (numarg[0] == '+'){
      while (numFromNumarg != 0){
        fgets(line, sizeof(line), file);
        numFromNumarg--;
      }
    } else {
      num = numFromNumarg;
    }
  } else if (strcmp(arg, "-c") == 0){
    if (numarg[0] == '+'){
      fread(line, 1, numFromNumarg, file);
    } else {
      int len = sizeof(line);
      fread(line, 1, numFromNumarg, file);
      printf("%s", line);
      exit(0);
    }
  }

  //Print out lines
  while (fgets(line, sizeof(line), file) != NULL){
    printf("%s", line);
    counter++;
    if (counter == num){
      exit(0);
    }
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  FILE *file;
  char* buf = malloc(1024);
  char* arg;
  char* numarg;
  int num;

  if (argc == 0) {exit(0);}
  else if (argc == 2) { // ./myhead input.txt
    file = fopen(argv[1], "r");
    head(file, "","");
  }
  else if (argc == 1) { // cat input.txt | ./myhead
    head(stdin, "", "");
  }
  else if (argc == 4) {// ./myhead [arg] [num] input.txt
    file = fopen(argv[3], "r");
    arg = argv[1];
    numarg = argv[2];
    head(file, arg, numarg);
  } else if (argc == 3){
    arg = argv[1];
    numarg = argv[2];
    head(stdin, arg, numarg);
  }
  fclose(file);
  exit(0);
}
