/* 
 * indexer.c - implements lab 5
 *
 * standalone program that reads the document files produced 
 * by the TSE crawler, builds an index, and writes that index 
 * to a file.
 *
 * Usage: ./indexer pageDirectory indexFilename
 * 
 * Crawford Hovis, CS50 Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"

/* ***** Function Prototypes ***** */
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* ndecks, webpage_t* page, int docID);

/* validate and parse argmuments and call indexBuild */
int main(const int argc, char* argv[]) 
{
  // ensure the proper number of arguments have been provided  
  if (argc != 3) {
    fprintf(stderr, "Unexpected number of arguments; provide format: ./indexer pageDirectory indexFilename\n");
    exit(2);
  }
  // check that provided parameters are not NULL
  if (argv[1] == NULL || argv[2] == NULL) {
    fprintf(stderr, "One or more parameter provided is invalid\n");
    exit(2);
  }
  // check that argv[1] represents a valid pageDirectory (includes a .crawler file)
  if (!isValidPageDir(argv[1])) {
    fprintf(stderr, "directory is invalid\n");
    exit(2);
  }
  // ensure that argv[2] represents a pathname to a writeable file
  FILE* fp;
  if ((fp = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "unable to open indexFilename\n");
    exit(2);
  }
  fclose(fp);

  char* pageDirectory = argv[1];
  char* indexFilename = argv[2];
  // pass pageDirectory to indexBuild to build index
  index_t* index = indexBuild(pageDirectory);
  // write the index file
  index_write(index, indexFilename);
  // delete the index
  index_delete(index);

  exit(0);
}

/* build and return index */
static index_t* indexBuild(char* pageDirectory)
{
  index_t* ndecks = index_new(200); // construct new index of arbitrary size
  // check that memory was allocated properly
  mem_assert(ndecks, "index_new failed; error allocating memory for pagesToCrawl\n");
  FILE* fp;
  webpage_t* page;
  int docID = 1; // start on first document
  char* currFile = getCrawledFile(pageDirectory, docID);
  // loop over all files in pageDirectory
  while((fp = fopen(currFile, "r")) != NULL) {
    // load webpage from file
    if ((page = getWebpage(fp)) != NULL) { // if successful 
      indexPage(ndecks, page, docID);
    }
    // increment docID, update currFile, and clean up.
    docID++;
    free(currFile);
    currFile = getCrawledFile(pageDirectory, docID);
    fclose(fp);
    webpage_delete(page);
  }
  free(currFile);
  return ndecks;
}

/* scans a webpage document to add its words to the index */
static void indexPage(index_t* ndecks, webpage_t* page, int docID)
{
  if (ndecks != NULL && page != NULL && docID > 0) {
    int pos = 0; // position in webpage; starts at 0
    char* currWord = webpage_getNextWord(page, &pos);
    if (page == NULL || webpage_getHTML(page) == NULL || currWord == NULL) {
      fprintf(stderr, "NULL page, page html, or currWord\n");
      exit(2);
    }
    // step through each word of the webpage
    while (currWord != NULL) {
      if (strlen(currWord) >= 3) { // skip trivial words (less than length 3)
        normalizeWord(currWord); // convert to lowercase
        // the following follows the provided pseudo-code
        if (index_find(ndecks, currWord) == NULL) {
          index_insert(ndecks, currWord, docID); // add word to index if needed
        } 
        index_wordincrement(ndecks, currWord, docID); // increments word count in current docID
      }
      free(currWord);
      currWord = webpage_getNextWord(page, &pos);
    }
    free(currWord);
  } else {
    fprintf(stderr, "invalid parameters passed to indexPage function\n");
  }
}