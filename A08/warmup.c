/*
Name: Joselyne Malan
Date: 03/20/2023
This program deals with processes and forks. This system opens up 3 processors and then outputs each one with their PID in a speicfic order through the use of teh wait function
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
  pid_t pid;
  pid_t pid2;
  pid_t wait(int *child_s);
  int child_status;
  printf("%d : A0\n",getpid());
  pid = fork();
  if(pid == 0){
   printf("%d : B1\n",getpid());
  }
  else{
   printf("%d : B0\n",getpid());
   pid2 = fork();
      if(pid2 == 0){
        printf("%d : C1\n",getpid());
        }else{
        wait(&child_status);
        printf("%d : C0\n",getpid());
        }
   }
  printf("%d : Bye\n",getpid());
  return 0;
}
