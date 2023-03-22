#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
 unsigned char pix[3];
  FILE* fp = fopen(filename, "wb");
  char comment [1028] = "Created by NANO Version 2.10.24 PNM plug-in";
  fprintf(fp, "P6 \n # %s \n %d %d 225\n",comment,w,h);
    for(int r=0;r<w;r++){
     for(int c=0;c<h;c++){
       pix[0] = pxs[r][c].red;
       pix[1] = pxs[r][c].green;
       pix[2] = pxs[r][c].blue;
       fwrite(pix, sizeof(unsigned char), 3, fp);
     }
    }
   printf("Writing file %s\n",filename);
   fclose(fp);

}
