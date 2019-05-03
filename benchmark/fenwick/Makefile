CC = g++ -g -std=c++17 -Wall -Wextra -O3 -march=native -fno-exceptions -fno-rtti -fno-omit-frame-pointer -I../../include

all:
	mkdir -p bin
	$(CC) runall.cpp -o bin/runall

.PHONY: clean

clean:
	rm -rf bin
