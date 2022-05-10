#!/bin/bash
#testing.sh
#Luke Crawford Hovis
#2/14/2022
#tests indexer.c and indextest.c

mkdir testdir/

# ----- erroneous arguments ------ #
mkdir testdir/erroneous

# no args
./indexer

# one arg
./indexer ../../shared/tse/output/letters-2.index

# three args
./indexer ../../shared/tse/output/letters-2.index testdir/erroneous/testingErrors.out testdir/erroneous/testingErrors1.out

# invalid pageDirectory (non-existent path) 
./indexer ../../tse/output/letters-2.index testdir/erroneous/testingErrors.out

# invalid pageDirectory (not a crawler directory)
./indexer ../../shared/examples testdir/erroneous/testingErrors.out

# invalid indexFile (non-existent path) 
./indexer ../this/path/doesnt/exist/letters-2.index testdir/erroneous/testingErrors.out

# invallid indexFile (read-only directory)
mkdir testdir/erroneous/readonlydir

touch testdir/erroneous/readonlydir/coolFile

chmod 444 testdir/erroneous/readonlydir

./indexer ../../shared/tse/output/letters-2.index ../readonlydir/coolFile

chmod 777 testdir/erroneous/readonlydir

# invalid indexFile (read-only file) 
touch testdir/erroneous/readonly.txt

chmod 444 testdir/erroneous/readonly.txt

./indexer ../../shared/tse/output/letters-2.index testdir/erroneous/readonly.txt

chmod 777 testdir/erroneous/readonly.txt

# ----- valid test cases ------ #

# letters 2
./indexer ../../shared/tse/output/letters-2 testdir/let2

./indextest testdir/let2 testdir/let2.indextest

# sort the files to diff them

sort <testdir/let2 > testdir/let2.sorted

sort <testdir/let2.indextest > testdir/let2.indextestsorted

# diff the index files to ensure they are the same

diff testdir/let2.sorted testdir/let2.indextestsorted


# letters 10
./indexer ../../shared/tse/output/letters-10 testdir/let10

./indextest testdir/let10 testdir/let10.indextest

# sort the files to diff them

sort <testdir/let10 > testdir/let10.sorted

sort <testdir/let10.indextest > testdir/let10.indextestsorted

# diff the index files to ensure they are the same

diff testdir/let10.sorted testdir/let10.indextestsorted


# toscrape 1
./indexer ../../shared/tse/output/toscrape-1 testdir/scrape1

./indextest testdir/scrape1 testdir/scrape1.indextest

# sort the files to diff them

sort <testdir/scrape1 > testdir/scrape1.sorted

sort <testdir/scrape1.indextest > testdir/scrape1.indextestsorted

# diff the index files to ensure they are the same

diff testdir/scrape1.sorted testdir/scrape1.indextestsorted


# wikipedia 2
./indexer ../../shared/tse/output/wikipedia-2 testdir/wiki2

./indextest testdir/wiki2 testdir/wiki2.indextest

# sort the files to diff them

sort <testdir/wiki2 > testdir/wiki2.sorted

sort <testdir/wiki2.indextest > testdir/wiki2.indextestsorted

# diff the index files to ensure they are the same

diff testdir/wiki2.sorted testdir/wiki2.indextestsorted


# -------- Valgrind -------- #
mkdir testdir/valgrind 

valgrind --leak-check=full --show-leak-kinds=all ./indexer ../../shared/tse/output/toscrape-2 testdir/valgrind/scrape2

# Please Note: this valgrind call is valid. I call it on a provided index file and not one i produce (or more accurately, fail to produce)
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../../shared/tse/output/toscrape-2.index testdir/valgrind/scrape2.indextest

