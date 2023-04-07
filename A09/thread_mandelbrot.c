/*
Name: Joselyne Malan
Date: 04.05.2023
This program uses multiple threads to compute a mandelbrot ppm file in a efficient manner. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct element_info{
  struct ppm_pixel* palette;
  struct ppm_pixel** image;
  int max_iter;
  int startCol;
  int startRow;
  int endCol;
  int endRow;
  int size;

  float xmin;
  float xmax;
  float ymin;
  float ymax;

};

void *compute(void* args){
  struct element_info *c_pix = (struct element_info*)args;
  int startC = c_pix->startCol;
  int startR = c_pix->startRow;
  int endC = c_pix -> endCol;
  int endR = c_pix -> endRow;
  int size = c_pix ->size;
  int maxIterations = c_pix->max_iter;
  float xmin = c_pix->xmin;
  float xmax = c_pix->xmax;
  float ymin = c_pix->ymin;
  float ymax = c_pix->ymax;
  

  struct ppm_pixel* palette = c_pix->palette;
  struct ppm_pixel** ima = c_pix->image;
  struct ppm_pixel color;
  for (int r = startR; r<endR; r++){
     for(int c = startC; c<endC; c++){
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
        }
       c_pix->image[r][c] = color;
      }
   }

  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  struct timeval tstart, tend;
  double timer;
  srand(time(0));
  gettimeofday(&tstart,NULL);

  pthread_t *threads = malloc (numProcesses*sizeof(pthread_t));
  struct element_info *t_coord = malloc (numProcesses*sizeof(struct element_info));
  // generate pallet
  struct ppm_pixel* palette = malloc (sizeof(struct ppm_pixel)*maxIterations);
  struct ppm_pixel** image = malloc(sizeof(struct ppm_pixel *)*size);
  for(int a = 0;a<size;a++){
    image[a] = malloc(sizeof(struct ppm_pixel)* (size));
  }
  for(int i =0;i<maxIterations;i++){
    palette[i].red = rand()% 255;
    palette[i].blue = rand()% 255;
    palette[i].green = rand()% 255;
  }
  // compute image
  int i =0;
  for(i=0;i<numProcesses;i++){
    long id = threads[i];
    if(i == 0){
      t_coord[i].startCol = 0;
      t_coord[i].endCol = 240;
      t_coord[i].startRow = 0;
      t_coord[i].endRow =240;
      printf("Thread %ld) sub-image block: cols (%d,%d) to rows (%d,%d)\n",id,t_coord[i].startCol,t_coord[i].endCol,t_coord[i].startRow,t_coord[i].endRow  );
    }else if(i==1){
      t_coord[i].startCol = 240;
      t_coord[i].endCol = 480;
      t_coord[i].startRow = 0;
      t_coord[i].endRow =240;
      printf("Thread %ld) sub-image block: cols (%d,%d) to rows (%d,%d)\n",id,t_coord[i].startCol,t_coord[i].endCol,t_coord[i].startRow,t_coord[i].endRow  );
    }else if(i==2){
      t_coord[i].startCol = 0;
      t_coord[i].endCol = 240;
      t_coord[i].startRow = 240;
      t_coord[i].endRow =480;
      printf("Thread %ld) sub-image block: cols (%d,%d) to rows (%d,%d)\n",id,t_coord[i].startCol,t_coord[i].endCol,t_coord[i].startRow,t_coord[i].endRow  );
    }else {
      t_coord[i].startCol = 240;
      t_coord[i].endCol = 480;
      t_coord[i].startRow = 240;
      t_coord[i].endRow =480;
      printf("Thread %ld) sub-image block: cols (%d,%d) to rows (%d,%d)\n",id,t_coord[i].startCol,t_coord[i].endCol,t_coord[i].startRow,t_coord[i].endRow  );
    }
    t_coord[i].palette = palette;
    t_coord[i].image = image;
    t_coord[i].max_iter = maxIterations;
    t_coord[i].size = size;
    t_coord[i].xmax = xmax;
    t_coord[i].xmin = xmin;
    t_coord[i].ymax = ymax;
    t_coord[i].ymin = ymin;

    pthread_create(&threads[i], NULL, compute, &t_coord[i]);
  }
  for(i=0; i<numProcesses;i++){
    pthread_join(threads[i], NULL);
    printf("Thread %ld) finished\n",threads[i]);
  }
  image = t_coord->image;
  gettimeofday(&tend,0);
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
  free(t_coord);
  free(threads);
  return 0;
}
