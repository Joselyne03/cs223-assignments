#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
Author: Joselyne Malan
Date: 02/01/2023
Replaces words with symbols.
 */

void replaced(char* s, int len){
    int num_value = 0;
    for (int i=0; i<len;i++){
        num_value = s[i];
	if (num_value > 96){
	  s[i] = "@!#*"[rand() % 4];
	}
    }
}
int main(){
    time_t t;
    srand((unsigned) time(&t));
    char buff[32];
    printf("Enter a word: ");
    scanf("%s", buff);
    int n = strlen(buff);
    replaced(buff, n);
    printf("OMG!  %s\n",buff);

    }
