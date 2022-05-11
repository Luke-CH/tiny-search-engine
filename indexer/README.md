# CS50 TSE 
## Luke Crawford Hovis (Luke-CH)

### indexer

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, *indextest*, loads an index file produced by the indexer and saves it to another file.

See [IMPLEMENTATION](IMPLEMENTATION.md) for details regarding `indextest`.

### Indexer Usage

The indexer's only interface with the user is on the command-line; it must always receive two arguments.

```
./indexer pageDirectory indexFilename
```