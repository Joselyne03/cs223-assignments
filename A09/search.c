#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
Name: Joselyne Malan
Date: 04 - 05 - 2023
This program uses multi-threads to find a value within a specific file name. Each thread is assigned 
to a range of a specific region in teh data set and will return the thread that finds the value. 
*/

struct list_info{
  int id;
  int *list;
  int start;
  int end;
  int range; 
  int num_thread;
  int key_value;
  int index;

};
void *findNum(void* ars){
  struct list_info *curr = (struct list_info*)ars;
  int *list_c = curr->list;
  int start = curr ->start;
  int end = curr->end;
  int value = curr->key_value;
  int index = -1;
  for (int a=start; a< end;a++){
    int v = list_c[a];
    if(v == value){
      index = a;
      break;
    }
  }
  curr->index = index;
    return NULL; 

}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  int N_thread = atoi(argv[1]);
  pthread_t *num_thread;
  char filename [1028];
  strcpy(filename,"data.bin");
  FILE* f = fopen(filename, "rb");
  if ( f == NULL ){
         printf("Error: Unable to open file\n");
      return 1;
  }
  int num;
  int value;
  int length;
  printf("Enter a value to search: ");
  scanf("%i",&value);//place a caution error statement
  fread(&length,sizeof(int),1,f);
  int *list = malloc(sizeof(int) * length);
  int r = 0;
  while (fread(&num,sizeof(int),1,f) == 1){
    list[r] = num;
    r++;
  }
  num_thread = malloc(N_thread *sizeof(pthread_t));
  struct list_info *thread_ids = malloc (N_thread*sizeof(struct list_info));
  int i = 0;
  for(i = 0; i<N_thread;i++){
    if(i == 0){
      thread_ids[i].start = 0;
      thread_ids[i].end = (int)length/N_thread;
    }else{
      int p_start = thread_ids[i-1].end;
      int p_range = thread_ids[i-1].range;
      thread_ids[i].start = thread_ids[i-1].end +1;
      thread_ids[i].end = p_range + p_start;
    }
    thread_ids[i].id = i +1;
    thread_ids[i].list = list;
    thread_ids[i].range = length / N_thread;
    thread_ids[i].key_value = value;
    thread_ids[i].num_thread = N_thread;
    pthread_create(&num_thread[i], NULL, findNum, &thread_ids[i]);
  }

  for(i = 0; i<N_thread;i++){
    pthread_join(num_thread[i], NULL);
  }
  int index = -1;
  int id = -1;
  for(int b = 0; b<N_thread; b++){
    if(thread_ids[b].index != -1){
      index = thread_ids[b].index;
      id = thread_ids[b].id;
      break;
    }
  }
  if(index != -1){
    printf("Thread %d found %d in index %d \n", id,value,index);
  }else{
    printf("Value not found!\n");
  }

  fclose(f);
  free(thread_ids);
  free(num_thread);
  free(list);
  return 0;
}
