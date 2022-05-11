# CS50 TSE 
## Luke Crawford Hovis (Luke-CH)
### crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL.
It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limits its exploration to a given "depth". The crawler does not explore pages in any particular order. This crawler limits its crawl to a specific web server inside Dartmouth CS.

Each explored webpage is saved to a file, one file per page, using a unique document ID as the file name.

### Usage

The crawler's only interface with the user is on the command-line; it must always have three arguments.

```
./crawler seedURL pageDirectory maxDepth
```