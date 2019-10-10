CC = g++ -std=c++17 -O3 -march=native -I../../include

all:
	mkdir -p bin
	$(CC) ab.cpp -o bin/ab
	$(CC) tryab.cpp -o bin/tryab

.PHONY: clean

clean:
	rm -rf bin
