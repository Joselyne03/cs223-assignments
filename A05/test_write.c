/*----------------------------------------------
 * Author: Joselyne Malan
 * Date: 02.22.2023
 * Description: This program will read a pixel array and then rewrite these pixels into a ppm file.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
   write_ppm_2d("test.ppm",pixels,w,h);
    for(int f = 0; f<w;f++){
    free(pixels[f]);
 }
 free(pixels);
   pixels = read_ppm_2d("test.ppm", &w, &h);
   printf("Testing file test.ppm: %d %d \n", w, h);
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
