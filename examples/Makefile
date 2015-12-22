# Makefile for cp2110-c example programs

CC       = gcc
CFLAGS   = -Wall -g
LIBS     = -ludev -lhidapi-libusb
INCLUDES = -I../include/
CP2110   = ../src/cp2110.c
BIN_DIR = bin

all: test

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

cp2110.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $(CP2110) -o cp2110.o


test: test.o cp2110.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN_DIR)/test $^ $(LIBS) 

clean:
	rm -f *.o bin/test