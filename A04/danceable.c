/*----------------------------------------------
 * Author:Josleyne Malan 
 * Date: 02/16/2022
 * Description: This program reads a file filled with songs and elements and then outputs the most danceable song
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
  struct song* val;
  struct node* next;
};

struct song{
    char title[1028];
    char artist[1028];
    int m;
    int s;
    float dancability;
    float energy;
    float tempo;
    float valence;
};

void print_out(struct song* p,int length){
    printf("Welcome to Joselyne Struct's Song List.\n");
    for (int i=0; i<length;i++){
        printf("%i) %s \tArtist: %s \tDuration: %i:%i \tD: %.3f \tE: %.3f \tT: %.3f \tV: %.3f\n",
        i, p[i].title, p[i].artist,p[i].m,p[i].s,p[i].dancability,p[i].energy,p[i].tempo,p[i].valence);
    }

}
int min_c(int ms ){
    int min = (ms / 1000)/60;
    return min;
}
int sec_c(int ms){
    int sec = ((ms / 1000)%60);
    return sec;
}

struct node* insert_front(struct song so, struct node* head) {
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  n-> val= malloc(sizeof(struct song));
  if( n->val == NULL){
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  strcpy(n->val->title, so.title);
  strcpy(n->val->artist,so.artist);
  n->val->dancability = so.dancability;
  n->val->valence = so.valence;
  n->val->energy=so.energy;
  n->val->m=so.m;
  n->val->s = so.s;
  n->val->tempo = so.tempo;
  n->next = head;
  
  return n;
}

struct node* remove_node(struct node* l, int position){
  struct node* temp = l;
  struct node* prev = NULL;
  int p = 1;
  while(temp != NULL && p < position) {
    prev = temp;
    temp = temp->next;
    p = p+1;
  }
  if((temp) == NULL){
     return l;
   }
  if((prev) == NULL){
     l = temp->next;
  }else{
     prev-> next = temp-> next;
  }
  free(temp);
  temp = NULL;
  return l;

}

struct song dance_node(struct node* li){
  struct song so;
  float most_d = 0;
  int p = 0;
   struct node* temp = li; 
  while ( temp != NULL){
     if( temp->val->dancability > most_d){
        most_d = temp->val->dancability;
        so = *(temp->val);
        }
     temp = temp->next;
     p = p+1;
   }
  
   return so;

}//return the Node with the highest dance record. 

int position_node (struct song s, struct node* l){
  struct node* temp = l;
  int p = 0;
  float dance;
  while(temp != NULL){
    if(temp->val->dancability == s.dancability){
        return p;
    }
    temp = temp->next;
    p = p+1;
  }
  return p;
}

void print_node(struct node* list) {
  struct node* n = list;
  int i = 0;
  while (n != NULL) {
   printf("%i) %s \tArtist: %s \tDuration: %i:%i \tD: %.3f \tE: %.3f \tT: %.3f \tV: %.3f\n",
   i, n->val->title, n->val->artist, n->val->m, n->val->s, n->val->dancability,n->val->energy,n->val->tempo,n->val->valence);
  n = n->next;
  i++;
  }
}
void freeNodes(struct node* n){

    while(n != NULL){
        struct node* toBeFree = n;
        n = n -> next ;
        free(toBeFree);
    }
    n=NULL;
}

int main() {
    char response;
    struct song* listS;
    char copyS[2000];
    int fileLength = 0;
    char* token;
    struct node* lSongs = NULL;
    FILE *infile; 
    infile = fopen("songlist.csv","r");
    if(infile == NULL){
        printf("Error: unable to open file %s\n", "songlist.csv");
        exit(1);
    }
    fgets(copyS,2000,infile);
    while (fgets(copyS,2000,infile) != NULL){
      fileLength = fileLength + 1;
    }
    listS =  malloc(sizeof(struct song )*fileLength);
    fseek(infile,0,SEEK_SET);

   fgets(copyS,2000,infile);
    int index = 0;
    while (fgets(copyS,2000,infile) != NULL){
        token = strtok(copyS,",");
        strcpy(listS[index].title,token);
         token = strtok(NULL,",");
        strcpy(listS[index].artist,token);
         token = strtok(NULL,",");
         int min = atoi(token);
         int secs = atoi(token);
        listS[index].m = min_c(min);
        listS[index].s = sec_c(secs);
         token = strtok(NULL,",");
        listS[index].dancability=atof(token);
         token = strtok(NULL,",");
        listS[index].energy=atof(token);
         token = strtok(NULL,",");
        listS[index].tempo=atof(token);
        token = strtok(NULL,",");
        listS[index].valence=atof(token);
        index++;
     }
     for(int i=0;i<fileLength;i++){
        struct song temp = listS[i];
        lSongs = insert_front(temp,lSongs);
      }

    struct song h_dance;
    int positions;
    print_node(lSongs);
    printf("\n Dataset contain %i songs \n",fileLength);
    printf("=======================\n");
    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    scanf(" %c", &response);
    while(response == 'd'){
      fileLength--;
    if(fileLength <= 0){
      printf("Dataset contains 0 songs\n");
      printf("=========================\n");
      printf("Press 'd' to show the most danceable song (any other key to quit): ");
      scanf(" %c", &response);
    }else{
    h_dance = dance_node(lSongs);
    positions = position_node(h_dance, lSongs);
    lSongs = remove_node(lSongs,positions);
    print_node(lSongs);
    printf("\n Dataset contain %i songs \n",fileLength);
    printf("=======================\n");
    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    scanf(" %c", &response);}
    }
    if(response != 'd'){
  freeNodes(lSongs);
  free(listS);
  fclose(infile);
      exit(0);
    }
  freeNodes(lSongs);
  free(listS);
  free(token);
  fclose(infile);
return 0;

}
