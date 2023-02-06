#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
Author: Joselyne Malan
Date: 02/01/2023
This program tracks different songs and allows users to update the playlist. 
*/
struct song{
    char title[1028];
    char artist[1028];
    int minutes;
    int sec;
    float dance;

};

void print_out(struct song p[],int length){
    printf("Welcome to Joselyne Struct's Song List.\n");
    for (int i=0; i<length;i++){
        printf("%i) %s       artist: %s      Duration: %i:%i     danceability: %.2f \n",i, p[i].title, p[i].artist,p[i].minutes,p[i].sec,p[i].dance);
       
    }
   // printf("\n================================\n");
}
void update_song(struct song p[], char* edit,int id){
    char val[1028];
  //int min;
  //int sec;
  //float dancy;
    char a[] = "artist";
    char t[] = "title";
    char d[] = "duration";
    char dance[] = "danceability";
    if(strcmp(edit,a) == 0){
        printf("Enter artist\n");
        scanf(" %s", val);
        strcpy(p[id].artist, val);


    }else if(strcmp(edit,t)== 0){
        printf("Enter title\n");
        scanf(" %s", val);
        strcpy(p[id].title, val);
    

    }else if(strcmp(edit,d)==0){
         printf("Enter minutes\n");
      // scanf(" %i", min);
      // p[id].minutes = min;
      // printf("Enter seconds\n");
      // p[id].sec = sec;
        printf("have not been updated yet.");
    }else if(strcmp(edit,dance)==0){
         printf("Enter danceability\n");
       //scanf(" %d", dancy);
       //p[id].dance = dancy;
        printf("Have not updated yet.");

    }else {
        printf("Not the write response. Check Your spelling\n");
        exit(0);
    }


}

int id_check(int i){
    if (i == 0 || i == 1 || i == 2 ){
        return 1;
    }else{
        return 0;
    }
}

int main(){
    struct song playlist[3];
    int id;
    char choice[1028];
    strcpy(playlist[0].title, "Friday Night");
    strcpy(playlist[0].artist, "Katy Perry");
    playlist[0].minutes = 3;
    playlist[0].sec = 30;
    playlist[0].dance = 0.2;
    //
    strcpy(playlist[1].title, "WTFS");
    strcpy(playlist[1].artist, "Foxy");
    playlist[1].minutes = 2;
    playlist[1].sec = 30;
    playlist[1].dance = 0.9;
    //
    strcpy(playlist[2].title, "Panda");
    strcpy(playlist[2].artist, "XXX");
    playlist[2].minutes = 2;
    playlist[2].sec = 12;
    playlist[2].dance = 0.5;
    print_out(playlist,3);
    printf("Enter a song id to edit [0,1,2]: ");
    scanf("%d", &id );
    if (id_check(id)== 1){
       printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
       scanf("%s", choice);
    }else {
        printf("Invaild Choice!\n");
    }
    update_song(playlist,choice,id);
    print_out(playlist,3);
    return 0;

}
