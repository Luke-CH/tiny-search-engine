# CS50 TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/cs50winter2022/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Querier.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use one main data structure: the index of counter sets
we are assisted by the use of cntrpairs and cntrnodes, which are passed to iterators

## Control flow

The Querier is implemented in one file `querier.c`, with four _primary_ functions.

### main

The `main` function simply calls `parseArgs` and `query_listen`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `isValidPageDir()`
* for `indexFilename`, ensure that it is a path to a readable file
* if any trouble is found, print an error to stderr and exit non-zero.

### query_listen

Listen for user inputs
Pseudocode:

	prompt
	while input is not EOF
        read input 
        initialize index
	    call handleQuery
		prompt
		delete allocated memory

### handleQuery

does the validating, parsing and tokenizing of the query
Pseudocode:

	validate that input isn't empty
    ensure all chars are spaces or letters
    initialize the word array
    call tokenizeQuery
    print normalized user input
    validate query syntax according to Requirements spec
    for each word in array
        if word is "or"
            unite andsequence and runningSum 
            increment i
        else if word is not and
            intersect word with runningProduct
    call printResult
    
    

## Other functions

### tokenizeQuery

Pseudocode for `tokenizeQuery`:

	slide word pointer to first char
    slide rest pointer to end of word
    insert word in word array
    set word = rest + 1;
    return upon end of query string


Pseudocode for `pagedir_save`:

	construct the pathname for the page file in pageDirectory
	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file

### Helper Methods
helper methods including iterators were particularly useful in this implementation but are pretty simple and do not need psuedocode explanations.

### Common

We utilize the common library for its data structures.

### Output

each match is printing in ranked order with the following expression:

```c
printf("score: %d docID: %d url: %s\n", node->score, node->key, url);
```


## Function prototypes

```c
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
```


## Error handling and recovery

Exit non-zero and print error message when invalid commandline arguments are passed or memory fails to allocate properly. 

## Testing plan

See Design Spec for more


### Integration/system testing

testing.sh outputs to testing.out, checks for memory leaks and tests some queries as described in Design Spec.