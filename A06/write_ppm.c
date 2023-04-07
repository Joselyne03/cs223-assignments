/*----------------------------------------------
 * Author:Josleyne Malan
 * Date: Feb.28.2022
 * Description: This fucntion writes the ppm file. 
 ---------------------------------------------*/

// implement *one* (but do not remove the other one)!

/*void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
   unsigned char pix[3];
  FILE* fp = fopen(filename, "wb");
  char comment [1028] = "Created by NANO Version 2.10.24 PNM plug-in";
  fprintf(fp, "P6 \n # %s \n %d %d 225\n",comment,w,h);
    for(int r=0;r<w;r++){
     for(int c=0;c<h;c++){
       pix[0] = pixels[r][c].red;
       pix[1] = pixels[r][c].green;
       pix[2] = pixels[r][c].blue;
       fwrite(pix, sizeof(unsigned char), 3, fp);
     }
    }
   printf("Writing file %s\n",filename);
   fclose(fp);

}*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

// void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
//  struct ppm_pixel pix[3];
//   FILE* fp = fopen(filename, "wb");
//   char comment [1028] = "Created by NANO Version 2.10.24 PNM plug-in";
//   fprintf(fp, "P6 \n # %s \n %d %d 225\n",comment,w,h);
//     for(int r=0;r<w;r++){
//      for(int c=0;c<h;c++){
//        pix[0].red = pxs[r][c].red;
//        pix[1].green= pxs[r][c].green;
//        pix[2].blue= pxs[r][c].blue;
//        fwrite(pix, sizeof(struct ppm_pixel), 3, fp);
//      }
//     }
//    printf("Writing file %s\n",filename);
//    fclose(fp);

// }
void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
     struct ppm_pixel pix;
  FILE* fp = fopen(filename, "wb");
  char comment [1028] = "Created by NANO Version 2.10.24 PNM plug-in";
  fprintf(fp, "P6 \n # %s \n %d %d 255\n",comment,w,h);
    for(int r=0;r<h;r++){
     for(int c=0;c<w ;c++){
       pix = pxs[r][c];
       fwrite(&pix, sizeof(struct ppm_pixel), 1, fp);
     }
    }
   printf("Writing file %s\n",filename);
   fclose(fp);
}