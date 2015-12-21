

CC       = gcc
CFLAGS   = -Wall -g
LIBS     = -ludev -lhidapi-libusb
INCLUDES = -I./include/

all: test

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#hid.o:
#	$(CC) $(CFLAGS) $(INCLUDES) -c ../hidapi/linux/hid.c -o hid.o

cp2110.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c src/cp2110.c -o cp2110.o


test: test.o cp2110.o
	$(CC) $(CFLAGS) $(INCLUDES) -o test test.o cp2110.o $(LIBS) 

clean:
	rm -f *.o test