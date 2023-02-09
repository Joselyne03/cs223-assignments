/*----------------------------------------------
 * Author:Joselyne Malan
 * Date:02.08.23
 * Description: This program takes a CSV song list and organizes them based on the stats given within the data
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void print_out(struct song p[],int length){
    printf("Welcome to Joselyne Struct's Song List.\n");
    for (int i=0; i<length;i++){
        printf("%i) %s \tArtist: %s \tDuration: %i:%i \tD: %.3f \tE: %.3f \tT: %.3f \tV: %.3f\n",i, p[i].title, p[i].artist,p[i].m,p[i].s,p[i].dancability,p[i].energy,p[i].tempo,p[i].valence);
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
int main(){
    char listSize[200];
    struct song* listS;
    char copyS[2000];
    FILE *infile;
    infile = fopen("songlist.csv","r");
    if(infile == NULL){
        printf("Error: unable to open file %s\n", "songlist.csv");
        exit(1);
    }
    fgets(listSize,200,infile);
    char* token = strtok(listSize,",");
    int lenSong = atoi(token);
    fgets(copyS,2000,infile);
    listS =  malloc(sizeof(struct song )*lenSong);
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
    print_out(listS,lenSong);
    free(listS);
    fclose(infile);
    return 0;
    }
