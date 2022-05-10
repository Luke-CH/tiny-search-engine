# CS50 TSE Querier
## Design Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.
The knowledge unit noted that an [design spec](https://github.com/cs50winter2022/home/blob/main/knowledge/units/design.md#design-spec) may include many topics; not all are relevant to the TSE or the Query.
Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface

As described in the [Requirements Spec](REQUIREMENTS.md), the querier's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ querier pageDirectory indexFilename
```

For example, for a given query, the querier will read an index into memory from the indexFile and will search the webpages in the pageDirectory, returning all documents that match the query as defined in the [Requirements Spec](REQUIREMENTS.md).

``` bash
$ ./querier ../../shared/tse/output/crawler/pages-wikipedia-depth-3 ../../shared/tse/output/indexer/index-wikipedia-3
```

## Inputs and outputs

*Input:* there are no file inputs; there are command-line parameters described above.

*Output:* Per the requirements spec, Querier will display eaching matching document in stdout, printing the score, docID and url for the document.  

## Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and callse query listener
 2. *query_listen*, which loops over user-input from stdin and calls handleQuery
 3. *handleQuery*, validates input, tokenizes the query and validates it's syntax according to Rec Spec, and builds the result counter by calling intersectCounters and UniteCounters, calling printResult to print it
 4. *intersectCounters*, which performs an intersection ("and") on a runningProduct counterset and a new word; has helper method intersectIterate
 4. *uniteCounters*, which performs the union operation ("or") on a runningSum counterset and 

And some helper modules that provide data structures:

  1. *cntrpair* pair of countersets to pass to an iterator
  2. *cntrnode* holds docID and score pairs to be passed to an iterator

## Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, and listen for queries
    while input != EOF
        validate Input
        tokenize query (see knowledge unit)
        validate tokenized query
        loop over words
          while word is not or
            perform intersection on andsequence 
          perform union on andsequence upon or (initially just saves and sequence)
        print each matching document
        prompt and go back to listening


## Major data structures

Helper modules provide all the data structures we need:

- *index* of counter sets mapping docIDs to scores
- *tokenarray* of pointers to characters, which allowed for tokenization without allocation of new memory
- *counterset* of docIDs and scores
- *cntrpair* new struct containing two counters for ease of iterating
- *cntrnode* new struct containing docID and score pairings (very similar to countersnode_t in the counters.c program)

## Testing plan

Testing will utilize the provided output in cs50-dev/shared/tse/output. Since "known correct output" is hard to come by, testing will largely be evaluated by hand.

A sampling of tests that should be run:

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. a couple of simple queries of two or three words using letters pageDirectory
Verify that output created match expectations.

3. Test various combinations of intersections and unions

4. query larger pageDirectories but with uncommon words, ensureing output is correct.

5. When confident in the accuracy of the program, run on wikipedia depth 3 with a large query of unions

7. Valgrind a reaonable query