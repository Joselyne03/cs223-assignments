/*
Name: Joselyne Malan
Date: 03/29/2023
This program uses multiple processes in order to create this mandelbrot faster compared to the single_mandelbrot file. .
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  pid_t pid;
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

  // generate pallet
  struct timeval tstart, tend;
  double timer;
  srand(time(0));
  gettimeofday(&tstart,NULL);

  struct ppm_pixel** ima = malloc (sizeof(struct ppm_pixel *)* (size));
  for(int a = 0;a<size;a++){
    ima[a] = malloc(sizeof(struct ppm_pixel)* (size));
  }
  struct ppm_pixel* palette;
  int shm1;
  shm1 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * maxIterations, 0644 | IPC_CREAT);
  if(shm1 == -1){
    perror("Error: cannot initalize memory!");
  }
  palette = shmat(shm1,NULL,0);
  if(palette == (void*)-1){
   perror( "Error: cannot access shared memory\n");
    exit(1);
  }

  for(int i = 0;i<maxIterations;i++){
       palette[i].red = rand() % 255;
       palette[i].green = rand() % 255;
       palette[i].blue = rand() % 255;
  }
  struct ppm_pixel* image;
  int shm2;
  shm2 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size *size, 0644 | IPC_CREAT);
  if(shm2 == -1){
    perror("Error: cannot initalize memory!");
  }
  image = shmat(shm2,NULL,0);
  if(image == (void*)-1){
    perror( "Error: cannot access shared memory\n");
    exit(1);
  }
  
  //here we set the Mandelbrot Set 
  int startCol;
  int endCol;
  int startRow;
  int endRow;

  for(int i =0; i<numProcesses;i++){
    pid = fork();
    if(pid ==-1){
      perror("Error: not forked properly\n");
      exit(2);
    }else if(pid == 0){
      if(i == 0){
        startCol = 0;
        endCol = 480;
        startRow = 0;
        endRow = 480;
        printf("%d) Sub-image block: col (%d,%d) to rows (%d,%d)",getpid(),startCol,endCol,startRow,endRow);
      }else if(i == 1){
        startCol = 480;
        endCol = 240;
        startRow = 0;
        endRow = 480;
        printf("%d) Sub-image block: col (%d,%d) to rows (%d,%d)",getpid(),startCol,endCol,startRow,endRow);
      }else if(i==2){
        startCol = 0;
        endCol = 480;
        startRow = 480;
        endRow = 240;
        printf("%d)Sub-image block: col (%d,%d) to rows (%d,%d)",getpid(),startCol,endCol,startRow,endRow);
      }else if(i==3){
        startCol = 480;
        endCol = 240;
        startRow = 480;
        endRow = 240;
        printf("%d)Sub-image block: col (%d,%d) to rows (%d,%d) ",getpid(),startCol,endCol,startRow,endRow);
      }
      for (int r = startRow; r <endRow; r++){
        for(int c=startCol; c<endCol; c++){
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
            image[(r*size) + c].blue = palette[iter].blue;
            image[(r*size) + c].red = palette[iter].red;
            image[(r*size) + c].green = palette[iter].green;
            }
          else{
            image[(r*size) + c].blue = 0;
            image[(r*size) + c].red = 0;
            image[(r*size) + c].green = 0;
          }
        }
      }//end of for-loop
      exit(0);
      }//child processor   
  }
  for (int i = 0; i < 4; i++) {
    int status;
    pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }
  // compute image
  for (int r = 0; r<size; r++){
    for(int c = 0; c<size; c++){
     ima[r][c]= image[r*size + c];
    }
  }
  gettimeofday(&tend,0);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  char filename[1028];
  snprintf(filename, 1028, "mandelbrot-%d-%ld.ppm", size,time(0));
  write_ppm_2d(filename,ima,size,size);
  if (shmdt(image) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }
  if (shmctl(shm2, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }
  if (shmdt(palette) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }
  if (shmctl(shm1, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  } 
  return 0;
}
