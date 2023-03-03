/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_array( char **array, int x){
    for (int i = 0; i<x; i++){
      free(array[i]);
    }
}

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  char bits_storage[64];
// todo: your code here
  unsigned long bitmask = 0x1ul<<63;
  for (int i = 0; i<64; i++){
     if (img & bitmask){
       bits_storage[i] = '@';
     }else{
      bits_storage[i] = ' ';
      }
     bitmask >>= 1;
  }

  char **d_array;
  d_array = malloc(sizeof(char *)* 8);
  for(int i =0;i<8;i++){
      d_array[i]=malloc(sizeof(char)*8);
  }

  int b = 0;
  for (int r =0; r<8 ;r++){
     for(int c=0;c<8 ;c++){
        d_array[r][c] = bits_storage[b++];
     }
  }

  for (int k = 0; k < 8; k++) {
      for (int j = 0; j < 8; j++) {
         printf("%c ", d_array[k][j]);
      }
      printf("\n");
  }
  printf("--------\n");
  free_array(d_array,8);
  free(d_array);
return 0;
}
