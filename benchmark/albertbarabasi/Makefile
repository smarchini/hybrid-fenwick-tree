CC = g++ -std=c++17 -O3 -march=native -I../../include -I../../DYNAMIC

all:
	mkdir -p bin
	$(CC) ab.cpp -o bin/ab

.PHONY: clean

clean:
	rm -rf bin
