#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
Author: Joselyne Malan
Date: 02/01/2023
This program shortens phrases
*/
void short_cut(char* s){
  char s2 [1024];
  int n = strlen(s);
  int num_char;
  int j=0;
  for (int i=0; i<n;i++){
     if(s[i] > 64 && s[i] < 91){
       s2[j] = s[i];
       j++;
     }
  }
  s2[j] = '\0';
  strcpy(s,s2);
 }
int main() {
  char phrase[1024];
  printf("Enter a phrase: \n ");
  scanf("%[^\n]%*c", phrase);
  short_cut(phrase);
  printf("Your acronym is %s\n", phrase);
  return 0;
}
