/*----------------------------------------------
 * Author: Joselyne Malan
 * Date: 02.21.2023
 * Description: This program reads a ppm file and inputs it.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
int main() {
  //Read header
int w;
int h;
struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
  if( pixels == NULL){
   printf("No memory can be allocated for the image data!");
   exit(1);
  }
  // todo: print out the grid of pixels
 printf("Testing file feep-raw.ppm: %d %d \n", w, h);
 for (int p = 0; p<h;p++){
      printf("\n");
      for(int a = 0;a<w;a++){
        printf("(%d , %d , %d)",pixels[p][a].red,pixels[p][a].green,pixels[p][a].blue);
      }
  }
  printf("\n");
 for(int f = 0; f<w;f++){
    free(pixels[f]);
 }
 free(pixels);
  return 0;
}

