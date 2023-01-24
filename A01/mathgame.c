/***************************************************
 * mathgame.c
 * Author:Joselyne Malan  
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// ask  how many questions 
//use printf and scanf 
//store teh number of correct q's
//generate random number 
//print welcome message 
//print completion message 

int main() {
  time_t t;
  srand((unsigned) time(&t));
  int num_q;
  int answer;
  int real_ans;
  int check;
  int tracker = 0;
  printf("Welcome! To Math Game!\n How many rounds do you want to play? ");
  scanf("%d",&num_q);
  for (int i=0;i<num_q; i++){
   int n1 = rand() % 9;
   int n2 = rand() % 9;
   real_ans = n1 + n2;
   printf( "%d + %d = ? ",n1,n2);
   scanf("%d", &answer);
   if (answer == real_ans){
    tracker = tracker + 1;
    printf("Correct!\n");
   }else {
    printf("Incorrect!\n");
   }

  }
  printf("You answered %d/%d correctly.\n", tracker, num_q);
  return 0;
}
