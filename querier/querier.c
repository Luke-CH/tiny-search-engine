/* 
 * querier.c - Implements Lab 6 for CS50
 *
 * The querier takes as command-line arguments a pathname to a  
 * page directory and a pathname to a file produced by the indexer.
 *
 * The querier takes 'queries' from stdin and returns a set of documents
 * with matches to the search query ranked by score. 
 * 
 * Luke Crawford Hovis, February 2022, Winter Term
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../common/pagedir.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* cntrpair holds two counters to be intersected or united */
typedef struct cntrpair {
  counters_t* cntr1;
  counters_t* cntr2;
} cntrpair_t;

/* cntrnode holds key */
typedef struct cntrnode {
  int key;                    // search key for this counter
  int score;                  // value of this counter, or score of the doc
} cntrnode_t;

/* --- Functions --- */
int fileno(FILE *stream);
cntrpair_t* cntrpair_new(void);
cntrnode_t* cntrnode_new(int key, int score);
void parse_args(const int argc, char* argv[]);
bool query_listen(char* pageDirectory, char* indexFilename);
void handleQuery(char* inputLine, index_t* index, char* pageDirectory);
counters_t* intersectCounters(counters_t* runningProduct, counters_t* c2);
void intersectIterate(void* arg, const int key, const int count1);
counters_t* uniteCounters(counters_t* runningSum, counters_t* runningProduct);
void uniteIterate(void* arg, const int key, const int count1);
bool isInputBlankLine(char* inputLine);
bool isValidCharacters(char* inputLine);
int tokenizeQuery(char* inputLine, char** words);
bool isValidQuery(char** query, int numWords);
bool isOperator(char* word);
static void prompt(void);
void countMatches(void* arg, const int key, const int count);
void printResult(counters_t* result, char* pageDirectory, int matches);
void printResultLine(cntrnode_t* node, char* pageDirectory);
void maxIterator(void* arg, const int key, const int count);

/**************** cntrpair_new() ****************/
/* 
 * allocates memory for new cntrpair struct
 * initializes contents of struct to NULL 
 */
cntrpair_t*
cntrpair_new(void)
{
  cntrpair_t* cntrpair = mem_malloc_assert(sizeof(cntrpair_t), "Error allocating memory for cntrpair\n");
  // initialize contents of cntrpair structure
  cntrpair->cntr1 = NULL;
  cntrpair->cntr2 = NULL;
  return cntrpair;
}

/**************** cntrnode_new() ****************/
/* 
 * allocates memory for new cntrnode and initiates key and count to zero
 */
cntrnode_t*
cntrnode_new(int key, int score)
{
  cntrnode_t* node = mem_malloc_assert(sizeof(cntrnode_t), "Error allocating memory for cntrnode\n");
  // initialize contents of cntrnode structure
  node->key = key;
  node->score = score;
  return node;
}

/* Main function for Querier, calls functions and exits 
zero upon successful completion, non-zero upon error. */
int main(const int argc, char* argv[]) 
{
  parse_args(argc, argv);
  
  char* pageDirectory = argv[1];
  char* indexFilename = argv[2];
  
  // listen for queries 
  if (!query_listen(pageDirectory, indexFilename)) {
    exit(2); 
  }
  exit(0);
}

/* Function to parse command-line arguments; returns nothing. 
Exits non-zero and prints an error message if args are improper */
void
parse_args(const int argc, char* argv[])
{
  // ensure the proper number of arguments have been provided 
  if (argc != 3) {
    fprintf(stderr, "Unexpected number of arguments provided.\n\nUse Format: ./querier pageDirectory indexFilename\n");
    exit(2);
  }
  // check that provided parameters are not NULL
  if (argv[1] == NULL || argv[2] == NULL) {
    fprintf(stderr, "One or more parameters provided are invalid\n");
    exit(2);
  }
  // check that argv[1] represents a valid pageDirectory (includes a .crawler file)
  if (!isValidPageDir(argv[1])) {
    exit(2);
  }
  // ensure that argv[2] represents a pathname to a readable file
  FILE* fp;
  if ((fp = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "Unable to open indexFilename\n");
    exit(2);
  }
  fclose(fp);
}

bool
query_listen(char* pageDirectory, char* indexFilename)
{
  char* currLine;
  // listen for inputs and pass them to the handleQuery
  prompt(); // promt for query from user
  while ((currLine = file_readLine(stdin)) != NULL) {
    if (currLine == NULL) {
      fprintf(stderr, "Error reading input query or allocating memory in query_listen\n");
      return false;
    }
    // read the index from indexFilename into an internal data structure (resets counters that have been set to zero with each query)
    index_t* index = index_read(indexFilename);

    handleQuery(currLine, index, pageDirectory); // handle user inputs

    prompt(); // promt for query from user

    // clean up
    free(currLine);
    index_delete(index);
  }
  return true;
}

/**************** handleQuery() ****************/
/* 
 * primary query handling function, calls other functions to tokenize query and build output
 *
 * takes input line, empty query array, and index as input. 
 */
void
handleQuery(char* inputLine, index_t* index, char* pageDirectory) 
{
  // in input line is blank, go back to listen for input.
  if (isInputBlankLine(inputLine)) {
    return;
  }
  // checks if all characters are letters or spaces, returns to listener otherwise
  if (!isValidCharacters(inputLine)) {
    return;
  }
                                      // input length divided by 2 is largest
  char* words[(int)(strlen(inputLine)/2)]; // possible number of words, which would be
                                      // a string of space-seperated characters
  int numWords = tokenizeQuery(inputLine, words);
  // print query 
  printf("Your Query: ");
  for (int i = 0; i < numWords; i++) {
    printf("%s ", words[i]);
  }
  printf("\n");
  if(!isValidQuery(words, numWords)) {
    return;
  }
  // build result counter
  counters_t* runningProduct = index_find(index, words[0]);
  counters_t* runningSum = counters_new();
  int i = 1; 
  // loop over sequence of 'or' seperated 'andsequences'
  while (i < numWords) {
    if (strcmp(words[i], "or") == 0) {
      runningSum = uniteCounters(runningSum, runningProduct); // unite each andsequence
      i++;
      runningProduct = index_find(index, words[i]); // reset runningProduct with each intersection
    } else {
      if (strcmp(words[i], "and") != 0) {
        runningProduct = intersectCounters(runningProduct, index_find(index, words[i])); // intersect words within and sequence
      }
    }
    i++;
  } 
  runningSum = uniteCounters(runningSum, runningProduct); // runningSum with final andsequence

                                                          // line will simply set runningSum equal to runningProduct
  // check for case of no matching documents
  int matches = 0;
  counters_iterate(runningSum, &matches, countMatches); // count matches
  if (matches == 0) {
    printf("No documents match.\n");
    counters_delete(runningSum);
    return;
  }
  printResult(runningSum, pageDirectory, matches);
  counters_delete(runningSum);
}


/**************** intersectCounters() ****************/
/* 
 * iterate over counters in the runningProduct to set their counts 
 * by calling intersectIterate()
 * 
 * return intersected counter set
 */
counters_t* 
intersectCounters(counters_t* runningProduct, counters_t* c2)
{
  cntrpair_t* cntrpair = cntrpair_new();
  cntrpair->cntr1 = runningProduct;
  cntrpair->cntr2 = c2;

  counters_iterate(runningProduct, cntrpair, intersectIterate);
  runningProduct = cntrpair->cntr1;
  free(cntrpair);
  return runningProduct;
}

/**************** intersectIterate() ****************/
/* 
 * helper function for intersectCounters()
 *
 * check key in second counter set, if it doesn't exist, count is zero
 * if it does exist, count in minimum between count of first counter set
 * and second counter set
 */
void
intersectIterate(void* arg, const int key, const int count1)
{
  cntrpair_t* cntrpair = arg;
  int count2 = counters_get(cntrpair->cntr2, key);
  if (count2 == 0) {
    counters_set(cntrpair->cntr1, key, 0); // if key isn't shared, set it's count to zero
  } else {
    int count;
    // find minimum between the two counts
    if (count1 < count2) {
      count = count1;
    } else {
      count = count2;
    }
    counters_set(cntrpair->cntr1, key, count);
  }
}

/**************** uniteCounters() ****************/
/* 
 * iterate over counters in the runningSum to set their counts 
 * by calling uniteIterate() 
 * 
 * return union of two counter sets
 */
counters_t*
uniteCounters(counters_t* runningSum, counters_t* runningProduct)
{
  // iterate over second counterset, updating the first (runningSum)
  counters_iterate(runningProduct, runningSum, uniteIterate);
  return runningSum;
}

/**************** uniteIterate() ****************/
/* 
 * helper function for uniteCounters()
 *
 * check key in second counterset (runningProduct), if it exists, sum the counts, if it 
 * doesn't exist, leave count in first counterset (runningSum) as is.
 */
void
uniteIterate(void* arg, const int key, const int count1) {
  counters_t* runningSum = arg;
  int count2 = counters_get(runningSum, key); // get count for given key in runningSum
  counters_set(runningSum, key, (count2 + count1)); // set new count
}

/**************** isInputBlankLine() ****************/
/* 
 * checks if input is a blank line (either a newline at the start or only spaces)  
 *
 * in case of blank line: print error statement indicating blank line, return true
 * in case of non blank line: return false
 */
bool
isInputBlankLine(char* inputLine)
{
  if (inputLine == NULL) {
    fprintf(stderr, "inputLine passed to isInputBlankLine is NULL\n");
    exit(2);
  }
  // if the input starts with a newline character, return true
  if (inputLine[0] == '\n') {
    return true;
  }
  // loop over each character and increment character count if a character is not a space
  int charCount = 0;
  for (int i = 0; i < strlen(inputLine); i++) {
    if (!isspace(inputLine[i])) {
      charCount++;
    }
  }
  // if there are no non-space characters, return true
  if (charCount == 0) {
    return true;
  }
  return false;
}

/**************** isValidCharacters() ****************/
/* 
 * check that all input characters are either letters or spaces.   
 *
 * print error statement with invalid character if one is passed and return false
 * print nothing and return true if all characters are letters or spaces
 */
bool
isValidCharacters(char* inputLine)
{
  if (inputLine == NULL) {
    fprintf(stderr, "inputLine passed to isValidCharacters is NULL\n");
    exit(2);
  }
  for (int i = 0; i < strlen(inputLine); i++) {
    if (!(isalpha(inputLine[i])) && !(isspace(inputLine[i]))) {
      fprintf(stderr, "invalid input character: %c\n", inputLine[i]);
      return false;
    }
  }
  return true;
}

/**************** tokenizeQuery() ****************/
/* 
 * tokenize the input, sliding word pointer to start of each word and 
 * saving that in character array, then sliding rest until 
 *
 * return array of pointers to words
 * 
 * see https://github.com/cs50winter2022/home/blob/main/knowledge/units/querier-chop.md
 */
int
tokenizeQuery(char* inputLine, char** words)
{
  mem_assert(inputLine, "inputLine passed to tokenizeQuery is NULL\n");
  mem_assert(words, "words array passed to tokenizeQuery is NULL\n");
  char* word = inputLine; // word points to the start of word
  char* rest; // rest steps through the rest of the string until a 
  // loop over characters until null terminator is encountered
  int wordNum = 0;
  while (*word != '\0') {

    // slide word pointer until letter is found
    while (isspace(*word)) {
      word++;
    }
    rest = word; // rest equals string after word pointer

    // slide rest pointer until space or null terminator is found
    while (isalpha(*rest)) {
      rest++;
    }
    // if null terminator was encountered, add word and return word count
    if ((*rest) == '\0') {
      normalizeWord(word);
      words[wordNum] = word; // add normalized word to words array
      wordNum++; 
      return wordNum;
    } else { // if a space was encountered, null terminate the word
      *rest = '\0'; // null terminate the string
      rest++; // increment past null terminator
      normalizeWord(word);
      words[wordNum] = word; // add normalized word to words array
      wordNum++; 
      word = rest; // set word equal to rest of string
    }
  }
  return wordNum;
}

/**************** isValidQuery() ****************/
/* 
 * check that operators ('and'/'or') are not at the start or end of the word 
 * and that they don't appear in succession
 *
 * print error statement at improper operator syntax and return false
 * print nothing and return true if query is of proper syntax
 */
bool
isValidQuery(char** query, int numWords) 
{
  // if first or last word is an operator, return false
  if (isOperator(query[0]) || isOperator(query[numWords - 1])) {
    fprintf(stderr, "Query may not start or end with an operator\n");
    return false;
  }
  // loop over each word to check for valid operators syntax
  for (int i = 1; i < numWords - 2; i++) {
    if (isOperator(query[i]) && isOperator(query[i+1])) {
      fprintf(stderr, "Query mustn't contain more than one operator in sequence\n");
      return false;
    }
  }
  return true;
}

/**************** isOperator() ****************/
/* 
 * checks if provided word is an operator ('and'/'or')
 *
 * return true if input is operator
 * return false otherwise
 */
bool
isOperator(char* word) 
{
  if (strcmp(word, "or") == 0 || strcmp(word, "and") == 0) {
    return true;
  }
  return false;
}

/* Funtion provided by Professor; see README.md */
static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** countMatches() ****************/
/* 
 * counts counterNodes with docID and count > 1
 */
void
countMatches(void* arg, const int key, const int count)
{
  int* matches = arg;
  if (count > 0) {
    (*matches)++;
  }
}

/**************** printResult() ****************/
/* 
 * prints the search result in decending order according to score
 */
void
printResult(counters_t* result, char* pageDirectory, int matches)
{
  printf("\n|---------------------------------------------------------|\n");
  printf("Matches %d documents (ranked):\n\n", matches);
  cntrnode_t* max = cntrnode_new(0,0); // create node to track max score
  counters_iterate(result, max, maxIterator); // iterate to find max score
  printResultLine(max, pageDirectory);
  counters_set(result, max->key, 0); // set max node to count 0 after it's been printed
  // loop until score is zero
  while (max->score != 0) {
    max->score = 0; // set max score to zero to find doc with next highest score
    counters_iterate(result, max, maxIterator);
    if (max->score != 0) { // as long as new max isn't zero
      printResultLine(max, pageDirectory); 
      counters_set(result, max->key, 0); // set max node to count 0 after it's been printed
    }
  }
  printf("\n|---------------------------------------------------------|\n\n");
  free(max);
}

/**************** printResultLine() ****************/
/* 
 * helper function for printResult; prints single line of result
 */
void
printResultLine(cntrnode_t* node, char* pageDirectory)
{
  if (node == NULL || pageDirectory == NULL)
  {
    fprintf(stderr, "NULL node or pageDirectory passed to printResultLine\n");
    return;
  }
  char* currFile = getCrawledFile(pageDirectory, node->key); 
  FILE* fp;
  if ((fp = fopen(currFile, "r")) == NULL) {
    fprintf(stderr, "failed to open %s\n", currFile);
    return;
  }
  char* url = file_readLine(fp); // read url from first line of currFile

  printf("score: %d docID: %d url: %s\n", node->score, node->key, url); // print result line
  
  // clean up
  fclose(fp);
  free(url);
  free(currFile);
}

/**************** maxIterator() ****************/
/* 
 * helper function for printResult; gets the next result to print
 * based on it's score
 */
void
maxIterator(void* arg, const int key, const int score)
{
  cntrnode_t* max = arg;
  if (score > max->score) {
    max->key = key;
    max->score = score;
  }
}