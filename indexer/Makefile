# Makefile for 'indexer' module
#
# provided by Songyun
#
# Luke Crawford Hovis, February 2022
C = ../common
L = ../libcs50
PROGS = indexer indextest #indexcmp
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
########### indexer ##################
# indexer source dependencies
indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
indexer.o: $C/index.h $C/pagedir.h $C/word.h
indexer.o: $L/webpage.h $L/file.h $L/mem.h $L/counters.h $L/hashtable.h
../common/common.a: 
	make -C ../common
../libcs50/libcs50.a:
	make -C ../libcs50
########### indextest ##################
indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
# indextest source dependencies
indextest.o: $C/index.h
indextest.o: $L/hashtable.h
# ########### indexcmp ##################
# indexcmp: indexcmp.o $(LLIBS)
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
# # indexcmp source dependencies
# indexcmp.o: $C/index.h
# indexcmp.o: $L/hashtable.h
########### test ##################
test: indexer indextest testing.sh
	bash -v testing.sh &> testing.out
clean:
	-rm -f core
	-rm -rf *~ *.o *.dSYM 
	-rm -f $(PROGS)
	-rm -r testdir