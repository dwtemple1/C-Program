#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 180

int main(int argc, char *argv[]) {
   Dictionary A = newDictionary();
   char* word1[] = {"one","two","three","four","five","six","seven"};
   char* word2[] = {"foo","bar","blah","galumph","happy","sad","blue"};
   int i;

   for(i=0; i<7; i++){
      insert(A, word1[i], word2[i]);
   }

   delete(A, "six");
   //delete(A, "three");
   //delete(A, "seven");
   //char* word = "I am dylan";
   //printf("Hello world %s \n", word);
   //insert(A, word1[0], word2[0]);
   //insert(A, word1[1], word2[1]);
   //insert(A, word1[2], word2[2]);

   //printf("%d\n", isEmpty(A));

   printDictionary(stdout, A);
   return(EXIT_SUCCESS);
}
