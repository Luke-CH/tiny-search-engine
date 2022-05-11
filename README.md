# CS50 TSE, Winter 2022
## Luke Crawford Hovis (Luke-CH)

This repository contains code for Dartmouth CS50's Tiny Search Engine project, which allows its user to search from a given seed url and a given depth, returning the most relavant search results as calculated by the number of times a webpage matches the given search term(s). Please see each subdirectory's README for more information. 

Tiny Search Engine (TSE) is a Dartmouth CS50 course project. 
The project implements on the following components:
1. Crawler: Crawls through a website and retrieves webpages up to a given `depth`, extracting the embedded URLs, and writes them to a file in a given directory.
2. Indexer: Builds an `inverted index` from the `crawler produced directory`, mapping from words to document ID and count
3. Querier: Returns webpages based on the user's queries (supports 'and' and 'or' operators, which all look for unions or intersetions of words in given webpages)

### Compilation

Run `make` in the top level directory to compile. 

### Disclaimer

Please note that some of this code, specifically `webpage.c` and `mem.c` was provided by Professor Charles Palmer. The rest, however, is mine. 
