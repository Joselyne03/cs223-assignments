#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main() {
  int length;
  int num;
  int index = -1;
  int halfL;
  pid_t pid;
  pid_t wait(int *child_s);
  int child_status;
  char filename [1028];
  strcpy(filename,"data.bin");
  FILE* f = fopen(filename, "rb");
  if ( f == NULL ){
         printf("Error: Unable to open file\n");
      return 1;
  }

  int value;
  printf("Enter a value to search: ");
  scanf("%i",&value);//place a caution error statement
  fread(&length,sizeof(int),1,f);
  printf("%i \n",length);
  int *list =malloc(sizeof(int) * length);
  int i = 0;
  while (fread(&num,sizeof(int),1,f) == 1){
    list[i] = num;
    i++;
  }
  halfL = length/2;
  pid = fork();
  if(pid == 0){
    for(int b=halfL; b< length; b++ ){
       if(list[b] == value){
         index = b;
       }
    }
    if (index != -1){
      printf("Child found %i at index %i \n",value,index);
    }else{
      printf("Not found! \n");
    } 
  }else { 
    wait(&child_status);
    for(int a=0; a< halfL; a++ ){
       if(list[a] == value){
         index = a;
       }
    }
    if (index != -1){
      printf("Parent found %i at index %i \n",value,index);
    }else{
      printf("Not found! \n");
     }
  }

  free(list);
  fclose(f);
  return 0;
}
