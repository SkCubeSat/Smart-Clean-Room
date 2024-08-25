CC = g++
CFLAGS = -Wall -pedantic -g -pthread -lpigpio -lrt

.PHONY: all clean

all: test

clean:
	rm -rf test

test: source/main.cpp
	$(CC) $(CFLAGS) source/main.cpp -o test