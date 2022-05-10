#!/bin/bash
#testing.sh
#Luke Crawford Hovis
#2/14/2022
#tests querier.c

mkdir testdir/

# Though the requirements spec says we needn't extensively error 
# check the inputs, I test some basic erroneous querier calls

# ----- erroneous arguments ------ #
mkdir testdir/erroneous

# no args
./querier

# one arg
./querier ../../shared/tse/output/letters-2.index

# three args
./querier ../../shared/tse/output/letters-2.index testdir/erroneous/testingErrors.out testdir/erroneous/testingErrors1.out

# invalid pageDirectory (non-existent path) 
./querier ../../tse/output/letters-2.index testdir/erroneous/testingErrors.out

# invalid pageDirectory (not a crawler directory)
./querier ../../shared/examples testdir/erroneous/testingErrors.out

# invalid indexFile (non-existent path) 
./querier ../this/path/doesnt/exist/letters-2.index testdir/erroneous/testingErrors.out

# -------- invalid syntax ---------- #
echo ' and page or playground or the for ' | ./querier ../../shared/tse/output/crawler/pages-letters-depth-10 ../../shared/tse/output/indexer/index-letters-10

echo 'page or and playground or the for ' | ./querier ../../shared/tse/output/crawler/pages-letters-depth-10 ../../shared/tse/output/indexer/index-letters-10

echo 'and or or and or' | ./querier ../../shared/tse/output/crawler/pages-letters-depth-10 ../../shared/tse/output/indexer/index-letters-10


# -------- valid test cases -------- #
# letters, easily verifiable, should be 2 docs
echo 'page or playground or the for ' | ./querier ../../shared/tse/output/crawler/pages-letters-depth-10 ../../shared/tse/output/indexer/index-letters-10

# toscrape, rare words, easy to check, should be 10 docs
echo '  ead or freedmen or rockpile or angel and herald' | ./querier ../../shared/tse/output/crawler/pages-wikipedia-depth-2 ../../shared/tse/output/indexer/index-wikipedia-2

# wiki, small test, should return no documents
echo 'humbly arisbe rinpoche' | ./querier ../../shared/tse/output/crawler/pages-wikipedia-depth-2 ../../shared/tse/output/indexer/index-wikipedia-2

# wiki, small test, should return three docs
echo 'humbly or arisbe or rinpoche' | ./querier ../../shared/tse/output/crawler/pages-wikipedia-depth-2 ../../shared/tse/output/indexer/index-wikipedia-2

# huge test on wiki, not verifiable
echo 'page or playground or the for' | ./querier ../../shared/tse/output/crawler/pages-wikipedia-depth-3 ../../shared/tse/output/indexer/index-wikipedia-3

# -------- Valgrind -------- #

echo 'page or playground or the for' | valgrind --leak-check=full --show-leak-kinds=all ./querier ../../shared/tse/output/crawler/pages-letters-depth-10 ../../shared/tse/output/indexer/index-letters-10

echo 'page or playground or the for' | valgrind --leak-check=full --show-leak-kinds=all ./querier ../../shared/tse/output/crawler/pages-wikipedia-depth-3 ../../shared/tse/output/indexer/index-wikipedia-3
