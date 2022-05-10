# Makefile for 'querier' module
#
# adapted from indexer Makefile
#
# Luke Crawford Hovis, February 2022
C = ../common
L = ../libcs50
PROGS = querier
LIBS =
LLIBS = $C/common.a $L/libcs50.a
# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
# (and run `make clean; make` whenever you change this)
FLAGS = # -DAPPTEST # -DMEMTEST
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$C -I$L
CC = gcc
MAKE = make
.PHONY: all test clean
########### default ##################
all: $(PROGS)
########### querier ##################
# querier source dependencies
querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
querier.o: $C/index.h $C/pagedir.h $C/word.h
querier.o: $L/file.h $L/mem.h $L/counters.h $L/hashtable.h
../common/common.a: 
	make -C ../common
../libcs50/libcs50.a:
	make -C ../libcs50
########### test ##################
test: querier testing.sh
	bash -v testing.sh &> testing.out
clean:
	-rm -f core
	-rm -rf *~ *.o *.dSYM 
	-rm -f $(PROGS)
	-rm -r testdir