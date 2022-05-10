/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Crawford Hovis, January 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "index.h"
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct index {
  hashtable_t* hashtable;
} index_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see index.h for any comments about exported functions */

/**************** local function prototypes ****************/
void word_write(void* arg, const char* key, void* item);
void count_write(void* arg, const int key, const int count);
void ctrs_delete(void* item);

/**************** index_new() ****************/
/* Allocate and initialize an index */
index_t* 
index_new(const int num_slots) 
{
  if (num_slots > 0) {
    index_t* index = mem_malloc(sizeof(index_t)); // allocate memory for hastable 
    if (index != NULL) { // check that memory was allocated properly
      index->hashtable = hashtable_new(num_slots); // set number of slots to provided argument
      return index;
    }
  }
  return NULL; // in the case of any error (memory allocation or num_slots <= 0)
}

/**************** index_insert() ****************/
/* see index.h for description */
bool 
index_insert(index_t* index, const char* indexKey, const int counterKey)
{
  // ensure valid parameters have been passed
  if (index != NULL && indexKey != NULL && counterKey > 0) {
    counters_t* count = counters_new(); // create counter set for new word in document
    counters_add(count, counterKey); // add the counterKey (docID in cs50 lab5) to counter set
    return hashtable_insert(index->hashtable, indexKey, count); // 
    
  }

  // return false in the case of null index, key, or counterKey
  return false;
}

/**************** index_wordincrement() ****************/
/* see index.h for description */
bool
index_wordincrement(index_t* index, char* indexKey, const int counterKey)
{
  // ensure valid parameters have been passed
  if (index != NULL && indexKey != NULL && counterKey > 0) {
    counters_t* counters = hashtable_find(index->hashtable, indexKey); // find the counter set for the given word
    return (counters_add(counters, counterKey) != 0); // add counterKey (docID) to counter set or increment count if it already exits
  }
  
  return false; // return false if parameters are invalid
}

/**************** index_find() ****************/
/* see index.h for description */
counters_t* 
index_find(index_t* index, const char* key)
{
  if (index != NULL && key != NULL) {
    return hashtable_find(index->hashtable, key); // return key pointer or null if key not found
  }
  return NULL; // in case that item is not found or in case of unexpected NULL
}

/**************** index_write() ****************/
/* see index.h for description */
void 
index_write(index_t* index, char* indexFilename)
{
  mem_assert(index, "NULL index passed to index_write\n");
  mem_assert(indexFilename, "NULL indexFilename passed to index_write\n");
  FILE* fp;
  // checks that indexFilename points to a readable file
  if ((fp = fopen(indexFilename, "w")) == NULL) {
    fprintf(stderr, "indexFilename failed to open in index_write in index.c\n");
    fclose(fp);  
    exit(2);
  }
  // iterate over each key of the hashtable using word_write function (defined below)
  hashtable_iterate(index->hashtable, fp, word_write);
  fclose(fp);
}

/**************** index_read() ****************/
/* see index.h for description */
index_t* index_read(char* indexFilename)
{
  FILE* fp; 
  // check if file is readable
  if((fp = fopen(indexFilename, "r")) == NULL) {
    fprintf(stderr, "falied to open indexFilename in index_read in index.c\n");
    fclose(fp);
    exit(2);
  }
  int num_slots = file_numLines(fp); // create index with equal number of slots as there are 
  index_t* index = index_new(num_slots); // lines in the file (one slot for each word)

  if (index == NULL) {
    fprintf(stderr, "index is NULL in index_read\n");
    exit(2);
  }

  // declare variables outside the loop
  char* currWord; 
  int docID; 
  int count; 
  counters_t* counterset;
  // loop over all words
  while((currWord = file_readWord(fp)) != NULL) {
    counterset = counters_new(); // new counter set for each word
    while (fscanf(fp, "%d %d", &docID, &count) == 2) { // read docID and count pairs until the end of the line
      counters_set(counterset, docID, count); // set docID and count pair (don't add becuase count is not controlable)
      hashtable_insert(index->hashtable, currWord, counterset); // add word and corresponding counterset to hashtable
    }
    free(currWord);
  }
  fclose(fp);
  return index;
}

/**************** hashtable_delete() ****************/
/* see hashtable.h for description */
void 
index_delete(index_t* index)
{
  hashtable_delete(index->hashtable, ctrs_delete);
  free(index);
}

/* ***** Local Functions ****** */
// helper function for index_write; respresents itemfunc in hashtable_iterate call
void word_write(void* arg, const char* key, void* item)
{
  FILE* fp = arg;
  fprintf(fp, "%s ", key); // print the word
  // iterate over counters to print 
  counters_iterate(item, fp, count_write);
  fprintf(fp, "\n"); // return a line to print next word
}

// helper function to word_write and index_write; represents intemfunc in counters_iterate call
void count_write(void* arg, const int key, const int count)
{
  FILE* fp = arg;
  fprintf(fp, "%d %d ", key, count); // print docID count pairs for the word
}

// helper function for delete as the counters_delete function is not of proper type void (*)(void *)
void ctrs_delete(void* item) 
{
  counters_delete(item);
}