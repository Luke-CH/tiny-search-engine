/* 
 * indextest.c - implements lab 5; tests index.c
 *
 * tester program that will read an index file into
 * an index and then writes out to a new index file
 *
 * Usage: ./indextest oldIndexFile newIndexFile
 * 
 * Crawford Hovis, CS50 Winter 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common/index.h"

/* main function to test index.c */
int main(const int argc, char* argv[])
{
    // ensure the proper number of arguments have been provided  
  if (argc != 3) {
    fprintf(stderr, "Unexpected number of arguments; provide format: ./indextest oldIndexFile newIndexFile\n");
    exit(2);
  }
  // check that provided parameters are not NULL
  if (argv[1] == NULL || argv[2] == NULL) {
    fprintf(stderr, "One or more parameter provided is invalid\n");
    exit(2);
  }

  char* oldIndexFile = argv[1];
  char* newIndexFile = argv[2];

  // check for valid pathname
  FILE* fp;
  if ((fp = fopen(oldIndexFile, "r")) == NULL) {
    fprintf(stderr, "oldIndexFile is not readable\n");
    fclose(fp);
    exit(2);
  }
  fclose(fp);
  // read from oldIndexFile into index
  index_t* ndecks = index_read(oldIndexFile);

  // test that ndecks allocated properly
  if (ndecks == NULL) {
    fprintf(stderr, "index is NULL in indextest\n");
    exit(2);
  }

  // check for valid pathname
  if ((fp = fopen(newIndexFile, "w")) == NULL) {
    fprintf(stderr, "newIndexFile is not writeable\n");
    fclose(fp);
    exit(2);
  }
  fclose(fp);
  // write to new file
  index_write(ndecks, newIndexFile);

  // delete indecies
  index_delete(ndecks);

  return 0;
}
