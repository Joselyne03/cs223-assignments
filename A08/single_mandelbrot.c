/*Name: Joselyne Malan
  Date: 03/22/2023
  This program creates a mandelbrot by creating a pixel file using write_ppm file 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct timeval tstart, tend;
  double timer;
  srand(time(0));

  struct ppm_pixel* palette = malloc (sizeof(struct ppm_pixel )* (maxIterations));
  for(int i = 0;i<maxIterations;i++){
       palette[i].red = rand() % 255;
       palette[i].green = rand() % 255;
       palette[i].blue = rand() % 225;
  }

  //here we set the Mandelbrot Set
  gettimeofday(&tstart,NULL);
  struct ppm_pixel color;
  struct ppm_pixel** image = malloc (sizeof(struct ppm_pixel *)* (size));
  for(int a = 0;a<size;a++){
    image[a] = malloc(sizeof(struct ppm_pixel)* (size));
  }

  for (int r = 0; r<size; r++){
     for(int c = 0; c<size; c++){
        float xfrac = (float)c/size;
        float yfrac = (float)r/size;
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin); 
        float x = 0;
        float y = 0;
  	    int iter = 0;
  	    while ((iter < maxIterations) && ((x*x) + (y*y) < 2*2)){
           float xtmp = (x*x)-(y*y)+ x0;
           y = (2*x*y) + y0;
    	     x = xtmp;
    	     iter++;
  	    }

      	if(iter < maxIterations){
           color = palette[iter];
        }else{
           color.red = 0;
           color.green = 0;
           color.blue = 0;
        }//end of if statement 
        image[r][c]= color;
      }
   }//end of for-loop
  gettimeofday(&tend,NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  char filename[1028];
  snprintf(filename, 1028, "mandelbrot-%d-%ld.ppm", size,time(0));
  write_ppm_2d(filename,image,size,size);
  for (int f = 0; f<size; f++){
     free(image[f]);
  }
  free(image);
  free(palette);
  return 0;
}
