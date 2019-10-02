CC = g++ -std=c++17 -O3 -march=native -I../../include -I../../DYNAMIC

all:
	mkdir -p bin
	$(CC) transposition.cpp -o bin/transposition

.PHONY: clean

clean:
	rm -rf bin
