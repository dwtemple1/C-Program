//-----------------------------------------------------------------------------
//   Dictionary.c
//   Dylan Temple 1609700
//   3.15.2018
//   CMPS-12M-02
//
//   Dictionary.c creates a dictionary
//   that holds a key and a value
//   which can be added, deleted, cleared
//   entirely, etc...
//
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"Dictionary.h"


// private types --------------------------------------------------------------

const int tableSize = 101;

// NodeObj
typedef struct NodeObj{
   char* key;
   char* value;
   struct NodeObj* next;
} NodeObj;

// Node
typedef NodeObj* Node;

// newNode()
// constructor of the Node type
Node newNode(char* x, char* y) {
   Node N = malloc(sizeof(NodeObj));
   assert(N!=NULL);
   N->key = x;
   N->value = y;
   N->next = NULL;
   return(N);
}

// freeNode()
// destructor for the Node type
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

// DictionaryObj
typedef struct DictionaryObj{
   //Node head;
   //Node tail;
   int numItems;
   Node* hashTable;
} DictionaryObj;

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
   int sizeInBits = 8*sizeof(unsigned int);
   shift = shift & (sizeInBits - 1);
   if ( shift == 0 )
      return value;
   return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input) {
   unsigned int result = 0xBAE86554;
   while (*input) {
      result ^= *input++;
      result = rotate_left(result, 5);
   }
   return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key){
   return pre_hash(key)%tableSize;
}

// public functions -----------------------------------------------------------

// newDictionary()
// constructor for the Dictionary type
Dictionary newDictionary(void){
   Dictionary D = malloc(sizeof(DictionaryObj));
   assert(D!=NULL);
   D->numItems = 0;
   D->hashTable = (Node *) calloc(tableSize, sizeof(NodeObj));
   return D;
}

// freeDictionary()
// destructor for the Dictionary type
void freeDictionary(Dictionary* pD){
   if( pD!=NULL && *pD!=NULL ){
      if( !isEmpty(*pD) ) makeEmpty(*pD);
      free(*pD);
      *pD = NULL;
   }
}

// isEmpty()
// returns 1 (true) if D is empty, 0 (false) otherwise
// pre: none
int isEmpty(Dictionary D){
   if( D==NULL ){
      fprintf(stderr, 
              "Dictionary Error: calling isEmpty() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   return(D->numItems==0);
}

// size()
// returns the number of (key, value) pairs in D
// pre: none
int size(Dictionary D){
   if( D==NULL ){
      fprintf(stderr, 
              "Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   return(D->numItems);
}

// insert()
// inserts new (key,value) pair into D
// pre: lookup(D, k)==NULL
void insert(Dictionary D, char* k, char* v){
   Node N;
   Node L;
   if( D==NULL ){
      fprintf(stderr, "Dictionary Error: calling push() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if( lookup(D, k)!=NULL ){
      fprintf(stderr, "Dictionary Error: attempting to insert currently existant key\n");
      exit(EXIT_FAILURE);
   }
   N = newNode(k, v);
   //place N at D[h(k)]
   int hashKey = hash(k);
   //printf("Hash value = %d\n", hashKey);
   if(D->hashTable[hashKey] == NULL){
      D->hashTable[hashKey] = N;
   }
   else{
      L = D->hashTable[hashKey];
      D->hashTable[hashKey] = N;
      N->next = L;
      /**
      while(L->next != NULL){
         L = L->next;
      }
      L->next = N;
      */
   }
   D->numItems++;
}

// delete()
// deletes pair with the key k
// pre: lookup(D, k)!=NULL
void delete(Dictionary D, char* k){
   Node N;
   Node L;
   if( D==NULL ){
      fprintf(stderr, "Dictionary Error: calling delete() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if( D->numItems==0 ){
      fprintf(stderr, "Dictionary Error: calling delete() on empty Dictionary\n");
      exit(EXIT_FAILURE);
   }
   if( lookup(D,k)==NULL ){
      fprintf(stderr, "Dictionary Error: calling delete() on a non-existant key reference\n");
      exit(EXIT_FAILURE);
   }
   int hashKey = hash(k);
   N = D->hashTable[hashKey];
   if( N->next == NULL ){
      D->hashTable[hashKey] = NULL;
   }
   else if( strcmp(N->key,k)==0 ){
      D->hashTable[hashKey] = N->next;
   }
   else{
      for(L=N; L->next!=NULL; L=L->next){
         if( strcmp(L->next->key,k)==0){
            N = L->next;
            if( L->next->next == NULL ){           
               L->next = NULL;
               break;
            }
            else{
               L->next = L->next->next;
            }
         }
      }
   }
   D->numItems--;
   freeNode(&N);
}

// lookup()
// returns the value v such that (k, v) is in D, or returns NULL if no 
// such value v exists.
// pre: none
char* lookup(Dictionary D, char* k){
   Node N;
   char* c;
   c = NULL;
   if( D==NULL ){
      fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   int hashKey = hash(k);
   for(N=D->hashTable[hashKey]; N!=NULL; N=N->next){
      if(strcmp(N->key,k)==0)
         c=N->value;
   }
   return c;
}

// makeEmpty()
// re-sets D to the empty state.
// pre: none
void makeEmpty(Dictionary D){
   Node N;
   Node L;
   if( D==NULL ){
      fprintf(stderr, 
              "Dictionary Error: calling makeEmpty() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if( D->numItems==0 ){
      fprintf(stderr, "Dictionary Error: calling makeEmpty() on empty Dictionary\n");
      exit(EXIT_FAILURE);
   }
   L = NULL;
   for( int i=0; i < tableSize; i++ ){
      for( N=D->hashTable[i]; N!=NULL; N=N->next){
         freeNode(&L);
         L=N;
      }
      freeNode(&L);
   }
   D->numItems = 0;
}

// printDictionary()
// prints a text representation of D to the file pointed to by out
// pre: none
void printDictionary(FILE* out, Dictionary D){
   Node N;
   if( D==NULL ){
      fprintf(stderr, 
              "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   for( int i=0; i < tableSize; i++){
      for(N=D->hashTable[i]; N!=NULL; N=N->next){
         fprintf(out, N->key);
         fprintf(out, " ");
         fprintf(out, N->value);
         fprintf(out, "\n");
      }
   }
}

