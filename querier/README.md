# CS50 TSE 
## Luke Crawford Hovis (Luke-CH)

### querier
The querier prints the set of documents that contain all the words in the query in decreasing order by score, where the score is the number of occurrences of a word in a document. 'and' and 'or' are treated as operators, returning the intersection or union of search terms respectively. 

The querier handles error cases where any combination of 'and' or 'or' are adjacent or if one of these operators starts or ends a query. 

### Usage

The querier must always receive two arguments.

```
./querier pageDirectory indexFilename
```