/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }
   char file_name [1028];
   strcpy(file_name,argv[1]);
//read the file
   int h ;
   int w ;
   struct ppm_pixel** pixels = read_ppm_2d(file_name,&w,&h);
   printf("Reading %s with width %d and height %d\n",file_name,w,h);
   int size_txt = (h*w)*3;
   int max_size = size_txt/8;
   printf("Max number of character in the image: %d\n",max_size);
//Ask for the word
   char* input = malloc(sizeof(char)*max_size);
   printf("Enter a phrase: " );
   scanf("%s", input);
   if(strlen(input)>max_size){
      printf("The word is too long! Please make sure the word is %d long! \n",max_size);
      exit(1);
   }
//convert the words into bits and store it
   int input_bit = 8 * (strlen(input));
   unsigned char* bits = malloc(sizeof(unsigned char)* input_bit);
   int bit_size = 0;
   for (int m = 0; m < strlen(input); m++){
     unsigned char c = input[m];
     for (int i = 7; i >= 0; i--) {
       bits[bit_size] = (c >> i) & 1;
       bit_size++;
     }
   }
//obtain and update the last bit in this file.
   int r =0;
   int c = 0;
   for(int j = 0; j< input_bit; j++){
     if( (pixels[r][c].blue == '\0') || (pixels[r][c].red == '\0') || (pixels[r][c].green == '\0')){ break;}
     else{
     unsigned char c_red = pixels[r][c].red;
     unsigned char c_green = pixels[r][c].green;
     unsigned char c_blue = pixels[r][c].blue;
      if ( bits[j] == 1){
        pixels[r][c].red = (c_red | 1);
        pixels[r][c].green =( c_green | 1);
        pixels[r][c].blue = (c_blue | 1);
      }
//else if it is 0
      else {
        pixels[r][c].red = (c_red & ~1);
        pixels[r][c].green =(c_green & ~1);
        pixels[r][c].blue = (c_blue & ~1);
       }
     c++;
         if(c == w){
          r++;
          c = 0;
         }
       }
    }
    write_ppm_2d("feep-raw-encoded.ppm",pixels,w,h);
    for(int f = 0; f<w;f++){
    free(pixels[f]);
    }
    free(pixels);
    free(bits);
    free(input);
  return 0;
}

