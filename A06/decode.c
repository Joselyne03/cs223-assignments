/*---------------------------------------------
 * Author: Josleyne Malan
 * Date: 03/01/2023
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  char file_name [1028];
  strcpy(file_name,argv[1]);

  // read the file
  int w;
  int h;
  struct ppm_pixel** pixels = read_ppm_2d(file_name, &w, &h);
  printf("Reading %s with width %d and height %d \n", file_name, w, h);

  // Determines wheather the number of each RGB pixel ends in 0 or 1
  //insert these pixels into an unsigned char
   int size_txt =( w * h ) * 3;
   unsigned char* bit_storage = malloc(sizeof(unsigned char)*w*h*3);
   int bits ;
   int index_storage = 0;
   for (int r = 0; r < w; r++){
      for(int c =0; c< h; c++){
        if(pixels[r][c].red % 2 == 0){
          bits =  0;
        }else{
           bits = 1;
          }
         bit_storage[index_storage] = bits;
         index_storage ++;
         if(pixels[r][c].green % 2 == 0){
          bits =  0;
        }else{
           bits = 1;
          }
         bit_storage[index_storage] = bits;
         index_storage ++;
         if(pixels[r][c].blue % 2 == 0){
          bits =  0;
        }else{
           bits = 1;
          }
         bit_storage[index_storage] = bits;
         index_storage ++;
      }
  }
  int index = 0;
  int bit_index = 0;
  int max_size = size_txt / 8;
  char message[1028];
  for(int a = 0; a<max_size;a++){
    unsigned char store_here = 0;
    for(int j = 0; j < 8; j++){
        store_here = store_here << 1 | bit_storage[bit_index];
        bit_index++;
    }
    message[index] = (char)store_here;
    index++;
  }
  printf("Max number of characters in the image: %d\n",max_size);
  printf("%s\n", message);
  //free the 2_d array
   for(int r = 0; r<w;r++){
      free(pixels[r]);
   }
   free(pixels);
   free(bit_storage);
  return 0;
}
