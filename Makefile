OBJS = obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o obj/test.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++14 -Wall -Wextra -c $(DEBUG)
LFLAGS = -std=c++14 -Wall -Wextra $(DEBUG)

all: test

test: bin/test
	# https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
	bin/test --gtest_color=yes

bin/test: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -lgtest -o bin/test

obj/simple_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/simple_fenwick_tree.hpp src/simple_fenwick_tree.cpp
	$(CC) $(CFLAGS) -o $@ src/simple_fenwick_tree.cpp

obj/compact_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/compact_fenwick_tree.hpp src/compact_fenwick_tree.cpp
	$(CC) $(CFLAGS) -o $@ src/compact_fenwick_tree.cpp

obj/byte_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/byte_fenwick_tree.hpp src/byte_fenwick_tree.cpp
	$(CC) $(CFLAGS) -o $@ src/byte_fenwick_tree.cpp

obj/test.o: obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o test/test_utils.hpp test/broadword_test.hpp test/simple_fenwick_tree_test.hpp test/compact_fenwick_tree_test.hpp test/byte_fenwick_tree_test.hpp test/test.cpp
	$(CC) $(CFLAGS) -o $@ test/test.cpp

.PHONY: clean

clean:
	rm -f obj/* bin/*
