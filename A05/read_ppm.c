/*----------------------------------------------
 * Author: Joselyne Malan
 * Date: 02.21.2023
 * Description This program updates an array with pixels from a ppm file. (has no main)
  ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"
// Choose *one* to implement (do not remove the other one!)
/*struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}*/

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  FILE* pixels = fopen(filename, "rb");
  if( pixels == NULL){
    printf("Error: Unable to open file\n");
  }
  int max_size;
  char header[3];
  char comments [1028];
  unsigned char pix[3];
  //Read header
   fscanf(pixels,"%2s\n %*s", header);
  //Ignore the comments until the last one.
   fscanf(pixels,"%*s %*s %*s %*s %*s %*s %s \n", comments);
  //reads the hieght and width and pix size
   fscanf(pixels,"  %i %i %i ", w,h, &max_size);
  //create the 2D array
  struct ppm_pixel** p = malloc (3* sizeof(struct ppm_pixel *)* (*w));
  for(int j = 0;j<*w;j++){
    p[j] = malloc(sizeof(struct ppm_pixel)* (*h));
  }
  // Fills up the array with the struct pixels
  for(int r=0;r<*w;r++){
     for(int k=0;k<*h;k++){
       fread(pix, sizeof(unsigned char), 3, pixels);
       if(pix == NULL){
         return NULL;
       }
       p[r][k].red = pix[0];
       p[r][k].green = pix[1];
       p[r][k].blue = pix[2];
     }
  }
  fclose(pixels);
   return p;
}
