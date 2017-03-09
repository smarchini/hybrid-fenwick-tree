OBJS = obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o obj/typed_fenwick_tree.o
CC = g++
DEBUG = -O3 -march=native -g
CFLAGS = -std=c++14 -Wall -Wextra -c $(DEBUG)
LFLAGS = -std=c++14 -Wall -Wextra $(DEBUG)

all: test benchmark

# https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
test: bin/test
	bin/test --gtest_color=yes

#134217727
#67108863
#33554431
#1048575
benchmark: bin/benchmark
	bin/benchmark 33554431
	bin/benchmark 1048575

bin/test: $(OBJS) obj/test.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/test.o -lgtest -o bin/test

bin/benchmark: $(OBJS) obj/benchmark.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/benchmark.o -o bin/benchmark

obj/simple_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/simple_fenwick_tree.hpp src/simple_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/simple_fenwick_tree.cpp

obj/compact_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/compact_fenwick_tree.hpp src/compact_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/compact_fenwick_tree.cpp

obj/byte_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/byte_fenwick_tree.hpp src/byte_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/byte_fenwick_tree.cpp

obj/typed_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/typed_fenwick_tree.hpp src/typed_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/typed_fenwick_tree.cpp

obj/test.o: obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o test/test_utils.hpp test/broadword_test.hpp test/simple_fenwick_tree_test.hpp test/compact_fenwick_tree_test.hpp test/byte_fenwick_tree_test.hpp test/typed_fenwick_tree_test.hpp test/same_behavior_test.hpp test/test.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ test/test.cpp

obj/benchmark.o: obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o test/benchmark.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ test/benchmark.cpp


.PHONY: clean

clean:
	rm -f obj/* bin/*
