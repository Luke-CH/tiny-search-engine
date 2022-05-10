#!/bin/bash
#testing.sh
#Luke Crawford Hovis
#2/7/2022
#tests crawler.c including edge cases=

mkdir testdir/

----- erroneous arguments ------ #
mkdir testdir/case1

# improper number or arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/

# non-internal URL
./crawler http://www.google.com testdir/case1 1

# non-exitent directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters non_existent_directory/ 1

# invalid maxDepths
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testdir/case1 -1

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testdir/case1 11

# Valgrind over a moderate-sized test case
mkdir testdir/valgrind 
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ testdir/valgrind 1

# test with letters website at depths 0, 1, 2, and 10 
seedURL=http://cs50tse.cs.dartmouth.edu/tse/letters/

mkdir testdir/letters

mkdir testdir/letters/depth0
mkdir testdir/letters/depth1
mkdir testdir/letters/depth2
mkdir testdir/letters/depth10

./crawler $seedURL testdir/letters/depth0 0

./crawler $seedURL testdir/letters/depth1 1

./crawler $seedURL testdir/letters/depth2 2

./crawler $seedURL testdir/letters/depth10 10

# test with toscrape website at depths 0, 1, 2, and 3
seedURL=http://cs50tse.cs.dartmouth.edu/tse/toscrape/

mkdir testdir/toscrape

mkdir testdir/toscrape/depth0
mkdir testdir/toscrape/depth1
mkdir testdir/toscrape/depth2
mkdir testdir/toscrape/depth3

./crawler $seedURL testdir/toscrape/depth0 0

./crawler $seedURL testdir/toscrape/depth1 1

./crawler $seedURL testdir/toscrape/depth2 2

./crawler $seedURL testdir/toscrape/depth3 3

# test with wikipedia website at depths 0, 1, and 2
seedURL=http://cs50tse.cs.dartmouth.edu/tse/wikipedia/

mkdir testdir/wikipedia

mkdir testdir/wikipedia/depth0
mkdir testdir/wikipedia/depth1
mkdir testdir/wikipedia/depth2

./crawler $seedURL testdir/wikipedia/depth0 0

./crawler $seedURL testdir/wikipedia/depth1 1

./crawler $seedURL testdir/wikipedia/depth2 2