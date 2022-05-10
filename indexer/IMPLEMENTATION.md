# Implementation spec
## Luke Crawford Hovis, CS50, 22W

### indexer.c
Parses the arguements from the commandline in the `main` function 
Calls `indexBuild`
Finally writes the index to the indexFile pathname provided on the commandline and returns 0;

`indexBuild` allocates a new index and loops through files in a pageDirectory to build a representation of word frequencies in the directory by calling index page on each file

`indexPage` scans the page and adds words to the index if being seen for the first time or increases their count otherwise

### index.c
Implements the index

`index_new` 
Creates new index struct, which represents a hashtable of countersets, mapping each word to each docID it is in to the word's count

`index_insert`
adds new words

`index_wordincrement` 
Increments the count for a word already in the index

`index_read` 
Reads an index file and saves it into an index

`index_write`
Writes an index struct to an index file; has helper functions word_write and count_write

I utilize a couple of helper functions for `index_write` namely `words_write` and `counters_write` which iterate through the hashtable and each counter to write the output file with each line formatted as follows: `word docID count (docID count) (docID count) ...`

### word.c
word module provides function to normalize words

`normalizeWord`
Takes a word as input and lowercases each letter, returning the lowercased word. 
Assumes no non letter characters are being passed in each string. 

### pagedir.c
Added functionality to pagedir.c to provide functions that validate a pageDirectory, retrieve a crawled file, and get a webpage.

`isPageDirectory` 
Confrims that a given pathname is a directory of crawled pages

`getCrawledFile` 
Takes a page directory and a docID as inputs and returns a string in the format of `pageDirectory/docID`
Note that this function does not validate whether the returned string is a verifiable crawled file. That is the responsibility of the caller. 

`getWebpage` 
Reads a provided crawled file into a webpage struct, returning the webpage or NULL if unsuccessful. 

### Testing
`indextest.c` tests `index.c` while `testing.sh` tests the program in its entirty.

`testing.sh` tests a number of erroneous cases and then a variety of valid cases, finally running valgrind on both `indexer.c` and `indextest.c`