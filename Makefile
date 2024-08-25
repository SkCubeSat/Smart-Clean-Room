CC = g++
CFLAGS = -Wall -pedantic -pthread

.PHONY: all clean

all: test

clean:
	rm -rf test

test: source/main.cpp
	$(CC) $(CFLAGS) -o test source/main.cpp -lpigpio -lrt