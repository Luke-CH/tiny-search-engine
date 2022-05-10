/* 
 * index.h - header file for CS50 'index' module
 *
 * see index.h for more information.
 *
 * Crawford Hovis, January 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"

/**************** global types ****************/
typedef struct index index_t;  // opaque to users of the module

/**************** functions ****************/

/**************** index_new() ****************/
/* 
 * allocate memory for the index 
 *
 * Caller provides:
 *      number of slots for hashtable
 * 
 * we return:
 *      index made up of hashtable with num_slots slots
 * 
 */
index_t* index_new(const int num_slots);

/**************** index_insert() ****************/
/* 
 * insert new word into index
 *
 * Caller provides:
 *      index, word not already in index, and counter set key representing a docID
 * 
 * we return:
 *      true if insertion is successful
 *      false if hashtable_insert returns false
 * 
 */
bool index_insert(index_t* index, const char* indexKey, const int counterKey);

/**************** index_wordincrement() ****************/
/* 
 * increment the count of a word in index in a given doc
 *
 * Caller provides:
 *      index, word already in index, and counter set key representing the docID
 * 
 * we return:
 *      true if insertion is successful
 *      false if counters_add is not successful and returns 0
 * 
 */
bool index_wordincrement(index_t* index, char* indexKey, const int countersKey);

/**************** index_find() ****************/
/* 
 * find counter set for given word
 *
 * Caller provides:
 *      index, key (word) mapping to counter set
 * 
 * we return:
 *      counter set if successful
 *      NULL otherwise
 * 
 */
counters_t* index_find(index_t* index, const char* key);

/**************** index_write() ****************/
/* 
 * writes index to a file using according to specs (see REQUIREMENTS.md)
 *
 * Caller provides:
 *      index, indexFilename
 * 
 * Output written to indexFilename
 * 
 */
void index_write(index_t* index, char* indexFilename);

/**************** index_read() ****************/
/* 
 * reads index from a file formatted according to specs (see REQUIREMENTS.md)
 *
 * Caller provides:
 *      indexFilename
 * 
 * Input from indexFilename
 * 
 */
index_t* index_read(char* indexFilename);

/**************** index_delete() ****************/
/*
 * deletes counter sets and frees allocated memory
 * 
 * Caller provides:
 *      index to be deleted
 * 
 * Function calls counters_delete as its itemdelete function
 * 
 */ 
void index_delete(index_t* index);