# CS50 TSE, Winter 2022
## Luke Crawford Hovis (Luke-CH)

Tiny Search Engine (TSE) is a Dartmouth CS50 course project. 
The project implements on the following components:
1. Crawler: Crawls through a website and retrieves webpages up to a given `depth`, extracting the embedded URLs, and writes them to a file in a given directory.
2. Indexer: Builds an `inverted index` from the `crawler produced directory`, mapping from words to document ID and count
3. Querier: Returns webpages based on the user's queries (supports 'and' and 'or' operators, which all look for unions or intersetions of words in given webpages)

### Assumptions
I make no assumptions beyond those that were enumerated in the specs, which have since been removed from the course repository. 
