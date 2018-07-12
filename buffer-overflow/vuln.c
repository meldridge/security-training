#include <stdio.h>
#include <string.h>

const char junk[] = "\xFF\xE4";

int copy_buffer(char* input) {
  char buffer[800];
  strcpy(buffer, input);
  
  printf("Input was \"%s\"\n", buffer);

  return 0;  
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Must have an argument!\n");
    return 1;
  }
  
  copy_buffer(argv[1]);
  
  return 0;
}


