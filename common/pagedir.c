/* 
 * pagedir.c 
 *
 * provides functions to initialize the page directory and save pages to the directory
 * 
 * Crawford Hovis, CS50 Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** pagedir_init() ****************/
/* see pagedir.h for details */
bool pagedir_init(const char* pageDirectory) 
{
  mem_assert(&pageDirectory, "pageDirectory is NULL in pagedir_init\n"); // exit non-zero if privided directory is NULL
  // construct the pathname for the .crawler file in pageDirectory
  char* pathname = mem_malloc_assert(sizeof(char) * (strlen(pageDirectory) + strlen("/.crawler") + 1), "failed to create pathname in pagedir_init\n"); 
  sprintf(pathname, "%s/.crawler", pageDirectory);
  // ensure the file can be written to; taken from parseArgs.c in course examples
  FILE* fp;
  if ( (fp = fopen(pathname, "w")) != NULL) {
    fclose(fp);  
    free(pathname);
    return true;
  }
  free(pathname);
  return false;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for details */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) 
{ 
  mem_assert(&pageDirectory, "pageDirectory is NULL in pagedir_save\n"); // exit non-zero if privided directory is NULL
  mem_assert(&page, "page is NULL in pagedir_save\n"); // exit non-zero if privided page is NULL
  // construct the pathname for the page in pageDirectory
  char* pathname = mem_malloc_assert((strlen(pageDirectory) + 12) , "failed to create pathname in pagedir_save\n"); 
  sprintf(pathname, "%s/%d", pageDirectory,  docID);

  // ensure the file can be written to; taken from parseArgs.c in course examples
  FILE* fp;
  if ( (fp = fopen(pathname, "w")) != NULL) {
    fprintf(fp, "%s\n", webpage_getURL(page)); // print the URL
    fprintf(fp, "%d\n", webpage_getDepth(page)); // print the depth
    fprintf(fp, "%s\n", webpage_getHTML(page)); // print the contents of the webpage
    fclose(fp); 
    free(pathname);
  } else {
    fprintf(stderr, "Unable to create or write to a file of form pageDirectory/id\n");  
    free(pathname);
    exit(2);
  }
}

/**************** isValidPageDir() ****************/
/* see pagedir.h for details */
bool isValidPageDir(const char* pageDirectory)
{
  // make path to file to test for valid pagedir
  char* dir = mem_malloc_assert((strlen(pageDirectory) + 10) , "failed to allocate memory to test for validity of pageDirectory\n");
  sprintf(dir, "%s/.crawler", pageDirectory);

  FILE* fp; 
  if ( (fp = fopen(pageDirectory, "r")) == NULL) {
    fprintf(stderr, "provided pageDirectory is invalid\n"); 
    free(dir);
    return false;
  }
  free(dir);
  fclose(fp);
  return true;
}

/**************** getCrawledFile() ****************/
/* see pagedir.h for details */
char* getCrawledFile(const char* pageDirectory, const int docID)
{
  mem_assert(&pageDirectory, "pageDirectory is NULL in getCrawledFile\n"); // exit non-zero if privided directory is NULL
  char* pathname = mem_malloc_assert((strlen(pageDirectory) + 12) , "failed to create pathname in getCrawledFile\n"); 
  sprintf(pathname, "%s/%d", pageDirectory,  docID); // combine path to directory with docID

  return pathname;
}

/**************** getWebpage() ****************/
/* see pagedir.h for details */
webpage_t* getWebpage(FILE* fp)
{
  if (fp == NULL) {
    printf("fp is NULL\n");
    return NULL;
  } 
  // read url, depth, and html
  char* url = file_readLine(fp);
  char* strDepth = file_readLine(fp);
  if (url != NULL && strDepth != NULL) {
    int depth = atoi(strDepth); // convert depth to an integer
    free(strDepth); // free the string representation of the depth
   char* html = file_readFile(fp);
    webpage_t* page = webpage_new(url, depth, html);
    return page;
  }
  return NULL;
}