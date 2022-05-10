# Makefile for 'common' module
#
# provided by Songyun
#
# Crawford Hovis, February 2022

L = ../libcs50

# include index.o and word.o when tse is finished

OBJS = pagedir.o index.o word.o
LIB = common.a

FLAGS =

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

index.o: index.h $L/file.h $L/counters.h $L/hashtable.h 
pagedir.o: pagedir.h $L/webpage.h $L/file.h
word.o: word.h

clean:
	rm -f core
	rm -f $(LIB) *~ *.o