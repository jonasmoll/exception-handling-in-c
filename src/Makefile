###
# Makefile for the c exception hanling extension
###

# use clang compiler and tell it to suppress warnings
CC = cc -w

all: example

example:  example.o
	${CC} example.o -o example

example.o: example.c example.h cexcept_ext.h
	${CC} -c example.c
	
cexcept_ext.o: cexcept_ext.h
	${CC} -c cexcept_ext.h

run:
	./example
	
clean :
	rm example.o example
