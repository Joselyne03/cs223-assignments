/*----------------------------------------------
 * Author: Joselyne Malan 
 * Date: 02.22.23
 * Description: A crossword program
 ---------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void populate_array(char **array,int r, int c){
    for (int p = 0; p<r;p++){
      for(int a = 0;a<c;a++){
        array[p][a] = '.';
      }
    }

}
void update_c(char **array, int x, int y, int len_x, int len_y, char word2[],char word1[]){
  int l = 0;
  int s = 0;
  int row =0;
  int col = y;
  while (row < len_x && col < len_y) {
    array[row][col] = word1[l];
    l++;
    row++;
  }
  row = x;
  col = 0;
  while (row < len_x && col < len_y) {
    array[row][col] = word2[s];
    s++;
    col++;
  }
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }
  // todo: your code here
  int first_len = strlen(argv[1]);
  int second_len = strlen(argv[2]);
  char word1[first_len];
  strcpy(word1,argv[1]);
  char word2[second_len];
  strcpy(word2,argv[2]);


  //compares strings 
  char shared_l= '0';
  int x;
  int y;
  for(int j=0;j<first_len;j++){
    char letter = word1[j];
    for(int k=0;k<second_len;k++){
     if(letter == word2[k]){
          shared_l = word1[j];
           x = j;
           y = k;

           break;
      }
    }
    if(shared_l != '0'){
      break;
    }
  }
  if(shared_l == '0'){
    printf("There are no common letters\n");
    exit(0);
  }

//creation of the array
  char **d_array;
  d_array = malloc(sizeof(char *)* first_len);
  for(int i =0;i<first_len;i++){
    d_array[i]=malloc(sizeof(char)*second_len);
  }
  populate_array(d_array, first_len, second_len);
  update_c(d_array,x,y,first_len,second_len, word2,word1);
   for (int i = 0; i < first_len; i++) {
        for (int j = 0; j < second_len; j++) {
            printf("%c ", d_array[i][j]);
        }
        printf("\n");
    }
  free(d_array);
  d_array = NULL;
  return 0;
}





