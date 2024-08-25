CC = gpp
CFLAGS = -Wall -pedantic -g -pthread -lpigpio -lrt

.PHONY: all clean

all: test

clean:
	rm -rf test

test: main.cpp
	$(CC) $(CFLAGS) main.cpp -o test