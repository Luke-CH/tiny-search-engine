/* 
 * crawler.c - implements lab 4
 *
 * standalone program that crawls the web and retrieves 
 * webpages starting from a "seed" URL. 
 * 
 * It parses the seed webpage, extracts any embedded URLs, 
 * then retrieves each of those pages, recursively, 
 * but limiting its exploration to a given "depth".
 *
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * 
 * Crawford Hovis, CS50 Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../libcs50/mem.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"

/* ***** Function Prototypes ***** */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* ************* Private Global Variables ************** */
static char* UNIVERSAL_ITEM = ""; // arbitrary item to insert into hashtable

/* main function calls parseArgs and crawl, then exits zero if both execute successfully */
int main(const int argc, char* argv[])
{
  // declare variables in main so they are accessible in parseArgs and crawl
  char* seedURL = NULL;
  char* pageDirectory = NULL;
  int maxDepth = 0;

  // call parseArgs and crawl
  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
  crawl(seedURL, pageDirectory, maxDepth);
  
  exit(0);
}

// parses command line args; exits non-zero in case of error, otherwise returns void
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth) 
{
  // check for proper number of args
  if (argc != 4) {
    fprintf(stderr, "Unexpected number of arguments; provide format: ./crawler seedURL pageDirectory maxDepth\n");
    exit(2);
  }

  // check for NULL seedURL
  mem_assert(argv[1], "Argument provided for seedURL is NULL\n");

  // normalize url and validate as internal url
  *seedURL = normalizeURL(argv[1]);
  if (!isInternalURL(*seedURL)) {
    fprintf(stderr, "seedURL is not an internal URL; must begin with: http://cs50tse.cs.dartmouth.edu/tse/\n");
    free(*seedURL);
    exit(2);
  }

  // check for NULL pageDirectory
  mem_assert(argv[2], "Argument provided for pageDirectory is NULL\n");

  // initialize pageDirectory and exit on error
  *pageDirectory = argv[2];
  if (!pagedir_init(*pageDirectory)) {
    fprintf(stderr, "Failed to initialize pageDirectory\n");
    exit(2);
  }

  // ensure maxDepth is an integer and is between 0 and 10 inclusive
  *maxDepth = atoi(argv[3]);
  if (*maxDepth < 0 || *maxDepth > 10) { 
    fprintf(stderr, "maxDepth is not a proper integer; must be an integer from [0,10]\n");
    exit(2);
  }
}

// does the work of crawling from seedURL to maxDepth and saving pages in pageDirectory
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) 
{
  hashtable_t* pagesSeen = hashtable_new(200); // size of hashtable is impossible to determine in advance so we use 200
  mem_assert(pagesSeen, "hashtable_new failed; error allocating memory for pagesToCrawl\n"); // check that memory was allocated properly
  hashtable_insert(pagesSeen, seedURL, UNIVERSAL_ITEM);
  bag_t* pagesToCrawl = bag_new();
  // initialize the bag and add a webpage representing the seedURL at depth 0
  webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
  bag_insert(pagesToCrawl, seedPage);
  // loop each bag item until bag is empty 
  webpage_t* page;
  int docID = 1; // index documents
  while ((page = bag_extract(pagesToCrawl)) != NULL) {
    sleep(1); // pause for 1 second
    // fetch and save webpage HTML to pageDirectory
    if (webpage_fetch(page)) {  // if fetch is successful
      pagedir_save(page, pageDirectory, docID);  // save the webpage
      docID++; // increment docID for next page to be saved
      if (webpage_getDepth(page) < maxDepth) {  // if the webpage is not at maxDepth
        pageScan(page, pagesToCrawl, pagesSeen); // pageScan the current HTML
      }
    }
    webpage_delete(page); // delete the webpage
  }
  hashtable_delete(pagesSeen, NULL); // delete the hashtable 
                                        // (no need for an item delete function since our items are all an empty string)
  bag_delete(pagesToCrawl, webpage_delete); // delete the bag of webpages 
}

// implements pagescanner mentioned in the design. see DESIGN.md for details
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  // loop over every url in the page
  char* nextURL;
  int pos = 0; // position in webpage; starts at 0
  while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) {
    char* normalizedNextURL = normalizeURL(nextURL); // create normalized URL
    if (normalizedNextURL != NULL) {
      if (isInternalURL(normalizedNextURL)) { // if the URL is internal
        if (hashtable_insert(pagesSeen, normalizedNextURL, UNIVERSAL_ITEM)) { // and if the URL is successfully inserted into the pagesSeen hashtable 
          char* allocURL = mem_malloc_assert(sizeof(char) * (strlen(normalizedNextURL) + 1), "Error allocating memory for allocURL"); 
          strcpy(allocURL, normalizedNextURL); 
          webpage_t* newPage = webpage_new(allocURL, (webpage_getDepth(page) + 1), NULL); // create new webpage with depth of getDepth(page) + 1
          bag_insert(pagesToCrawl, newPage); // insert newPage into pagesToCrawl bag
        } 
      } 
    }
    free(normalizedNextURL);
    free(nextURL);
  }
}