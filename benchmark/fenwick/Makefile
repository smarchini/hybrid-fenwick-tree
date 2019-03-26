CC = g++ -g -std=c++17 -Wall -Wextra -O3 -march=native -fno-exceptions -fno-rtti -fno-omit-frame-pointer -I../../include

all:
	mkdir -p bin
	$(CC) -DHFT_HOLES=0 runall.cpp -o bin/runall0  # default
	$(CC) -DHFT_HOLES=1 runall.cpp -o bin/runall1  # no holes
	$(CC) -DHFT_HOLES=2 runall.cpp -o bin/runall2
	$(CC) -DHFT_HOLES=3 runall.cpp -o bin/runall3

.PHONY: clean

clean:
	rm -rf bin
