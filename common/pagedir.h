/* 
 * pagedir - utility functions for initializing and saving to a directory of webpages
 *
 * this module provides functions to initialize a pathname for a .crawler file in a given directory
 * and to write a page to the pageDirectory
 * 
 * additionally provides functions to validate a pageDirectory, retrieve a crawled file, 
 * and get a webpage
 * 
 * Crawford Hovis, CS50, Winter 2022
 *
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** pagedir_init ****************/
/* construct the pathname for the .crawler file in that directory
 *
 * Caller provides:
 *   page directory
 *
 * We return:
 *  false if attempt to open file produces error
 *  true if file opens properly
 *
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* construct the pathname for the page file in pageDirectory
 *
 * Caller provides:
 *   page, page directory, and docID
 *
 * We return:
 *  Nothing
 *
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** isValidPageDir ****************/
/* confirm that a provided pathname represents a page directory that includes a .crawler file
 *
 * Caller provides:
 *   page directory
 *
 * We return:
 *  True if pageDirectory is valid
 *  False otherwise
 *
 */
bool isValidPageDir(const char* pageDirectory);

/**************** getCrawledFile ****************/
/* return string representing file produced from crawler 
 *
 * Caller provides:
 *   directory path and docID
 *
 * We return:
 *  string representing pathname
 *
 */
char* getCrawledFile(const char* pageDirectory, const int docID);

/**************** getWebpage ****************/
/* return webpage object given html in file provided
 *
 * Caller provides:
 *   directory path
 *
 * We return:
 *  webpage from file
 *
 */
webpage_t* getWebpage(FILE* fp);

#endif