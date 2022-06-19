# If you want to use this on your own machine, change CLASSDIR and/or INCLUDE and LIBS so
# that they point to the correct directories/library.

CLASSDIR = libfdr
INCLUDE = -I$(CLASSDIR)/include
LIBS = $(CLASSDIR)/lib/libfdr.a
CC = gcc

EX  =  bin/ceviri \

all: $(EX) 

clean: 
	rm -f bin/*

bin/ceviri: src/ceviri.c $(LIBS)
	$(CC) $(INCLUDE) -o bin/ceviri src/ceviri.c $(LIBS)