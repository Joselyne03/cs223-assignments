#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include "read_ppm.h"
#include "write_ppm.h"
pthread_barrier_t mybarrier;
pthread_mutex_t mutex;

struct element_info{
  struct ppm_pixel* palette;
  struct ppm_pixel** image;
  int max_iter;
  int startCol;
  int startRow;
  int endCol;
  int endRow;
  int size;
  bool **membership;
  int **counted;
  int max_count;

  float xmin;
  float xmax;
  float ymin;
  float ymax;

};

void *start (void *data){
  struct element_info *c_pix = (struct element_info*)data;
  int startC = c_pix -> startCol;
  int startR = c_pix -> startRow;
  int endC = c_pix -> endCol;
  int endR = c_pix -> endRow;
  int size = c_pix -> size;
  bool **membership  = c_pix ->membership;
  int **count = c_pix -> counted;
  int maxIterations = c_pix-> max_iter;
  float xmin = c_pix->xmin;
  float xmax = c_pix->xmax;
  float ymin = c_pix->ymin;
  float ymax = c_pix->ymax;
  struct ppm_pixel* palette = c_pix->palette;
  struct ppm_pixel color;
  //start of step 1
  for(int r = startR; r<endR;r++){
    for(int c = startC; c<endC;c++){
      float xfrac = (float)r/size;
      float yfrac = (float)c/size;
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
        membership[r][c] = false;
      }else{
        membership[r][c] = true;
      }
    }
  }//end of step 1 
  for(int r = startR;r<endR;r++){
    for(int c = startC;c<endC;c++){
      if(membership[r][c] == true){
        continue;
      }
        float xfrac = (float)r/size;
        float yfrac = (float)c/size;
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin);
        float x = 0;
        float y = 0;
        while ((x*x + y*y) < (2*2)){
          float xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;
          
          int yrow = round(size * (y-ymin)/(ymax -ymin));
          int xcol = round(size * (x-xmin)/(xmax - xmin));
          if(yrow < 0 || yrow >= size){
            continue;
          }
          if(xcol < 0 || xcol >= size){
            continue;
          }
         pthread_mutex_lock(&mutex);
          count[yrow][xcol] ++;
         if(count[yrow][xcol] > c_pix->max_count){
            c_pix->max_count = count[yrow][xcol];
          }
         pthread_mutex_unlock(&mutex);
        }
    }
  }//end of step 2
  pthread_barrier_wait(&mybarrier);
  float gamma = 0.681;
  float factor = 1.0/gamma;
  for(int r = startR; r<endR;r++){
    for(int c = startC; c<endC;c++){
      float value;
      if(count[r][c] > 0){
        value = log(count[r][c] / log(c_pix->max_count));
        value = pow(value,factor);
      }
      color.red = value * 255;
      color.green = value * 255;
      color.blue = value * 255;
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
  int max_count = 0;

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
  struct timeval tstart,tend;
  double timer;
  srand(time(0));
  gettimeofday(&tstart,NULL);
  
  pthread_barrier_init(&mybarrier,NULL,numProcesses);
  pthread_mutex_init(&mutex,NULL);
 
  struct element_info *t_coord = malloc (numProcesses*sizeof(struct element_info));
  pthread_t *num_thread = malloc (numProcesses*sizeof(pthread_t));
    bool **membership  = malloc (sizeof(bool*)*size);
  for(int a=0;a<size;a++){
     membership[a] = malloc(sizeof(bool)*size);
  }
  int **counted  = malloc (sizeof(int*)*size);
  for(int b=0;b<size;b++){
    counted[b] = malloc(sizeof(int)*size);
  }
  for(int a=0; a<size;a++){
    for(int b=0;b<size;b++){
      counted[a][b] = 0;
    }
  }
  
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  struct ppm_pixel** image  = malloc (sizeof(struct ppm_pixel *)*size);
  for(int a=0;a<size;a++){
    image[a] = malloc(sizeof(struct ppm_pixel)*size);
  }
  for(int i=0;i<maxIterations;i++){
    palette[i].red = rand()% 255;
    palette[i].blue = rand()% 255;
    palette[i].green = rand()% 255;  
  }

  // compute image
  for(int i=0;i<numProcesses;i++){
    if(i == 0){
      t_coord[i].startCol = 0;
      t_coord[i].endCol = 240;
      t_coord[i].startRow = 0;
      t_coord[i].endRow =240;
 
    }else if(i==1){
      t_coord[i].startCol = 240;
      t_coord[i].endCol = 480;
      t_coord[i].startRow = 0;
      t_coord[i].endRow =240;
    }else if(i==2){
      t_coord[i].startCol = 0;
      t_coord[i].endCol = 240;
      t_coord[i].startRow = 240;
      t_coord[i].endRow =480;
      
    }else {
      t_coord[i].startCol = 240;
      t_coord[i].endCol = 480;
      t_coord[i].startRow = 240;
      t_coord[i].endRow =480;
 
    }
    t_coord[i].palette = palette;
    t_coord[i].image = image;
    t_coord[i].max_iter = maxIterations;
    t_coord[i].size = size;
    t_coord[i].xmax = xmax;
    t_coord[i].xmin = xmin;
    t_coord[i].ymax = ymax;
    t_coord[i].ymin = ymin;
    t_coord[i].max_count = max_count;
    t_coord[i].counted = counted;
    t_coord[i].membership = membership;
    pthread_create(&num_thread[i], NULL, start, &t_coord[i]);
    long id = num_thread[i];
    printf("Thread %ld) sub-image block: cols (%d,%d) to rows (%d,%d)\n",id,t_coord[i].startCol,t_coord[i].endCol,t_coord[i].startRow,t_coord[i].endRow  );
  }
  for(int i=0; i<numProcesses;i++){
    pthread_join(num_thread[i], NULL);
    printf("Thread %ld) finished\n",num_thread[i]);
  }
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&mybarrier);

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
   for (int m = 0; m<size; m++){
     free(membership[m]);
  }
   for (int co = 0; co<size; co++){
     free(counted[co]);
  }
  free(counted);
  free(membership);
  free(image);
  free(palette);
  free(t_coord);
  free(num_thread);
  return 0;
}
