/*---------------------------------------------
 * Author:Joselyne Malan
 * Date:02.06.2023
 * Description:This program reads an input and then rewrites the input with a specified numbers of dots in between.
   This program uses linked lists(nodes,insertion,creation of nodes)
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int length;
    int index = 0;
    char comment[32];
    printf("Pause length: ");
    scanf("%d", &length);
    printf("Text: ");
    scanf("%31s", comment);
    char* updateList =(char*)malloc(strlen(comment)+(strlen(comment)*length)+1);
     for(int i=0;i<strlen(comment);i++){
        updateList[index] =comment[i];
        index ++;
        for(int j=0;j<length;j++){
            updateList[index]='.';
        index++;
        }
    }
    updateList[index] ='\0';
    printf("%s\n",updateList);
    free(updateList);
    return 0;
}

