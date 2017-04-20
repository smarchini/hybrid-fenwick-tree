CC = g++
PARAMS = -O3 -march=native -g
CFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)
LFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)

INCLUDES = include/broadword.hpp include/common.hpp include/darray.hpp include/fenwick/fenwick_tree.hpp include/fenwick/simple.hpp include/fenwick/typed.hpp include/fenwick/ityped.hpp include/fenwick/byte.hpp include/fenwick/compact.hpp include/fenwick/shrank.hpp include/rankselect/rank_select.hpp include/rankselect/word.hpp include/rankselect/line.hpp

L1_CACHE_SIZE=$(shell getconf LEVEL1_DCACHE_SIZE)
L2_CACHE_SIZE=$(shell getconf LEVEL2_CACHE_SIZE)


all: test benchmark

# https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
test: bin/test/test
	bin/test/test --gtest_color=yes

benchmark: bin/benchmark/trees bin/benchmark/get bin/benchmark/set bin/benchmark/find
	@echo
	bin/benchmark/trees 1048575
	@echo
	bin/benchmark/get
	@echo
	bin/benchmark/set
	@echo
	bin/benchmark/find


# bin
bin/test/test: $(INCLUDES) test/test.cpp test/broadword_test.hpp test/simple_fenwick_tree_test.hpp test/compact_fenwick_tree_test.hpp test/byte_fenwick_tree_test.hpp test/typed_fenwick_tree_test.hpp test/ityped_fenwick_tree_test.hpp test/shrank_fenwick_tree_test.hpp test/same_behavior_test.hpp test/word_rank_select_test.hpp test/line_rank_select_test.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) test/test.cpp -o bin/test/test -lgtest

bin/benchmark/trees: $(INCLUDES) benchmark/trees.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/trees.cpp -o bin/benchmark/trees -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)

bin/benchmark/get: $(INCLUDES) benchmark/get.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/get.cpp -o bin/benchmark/get -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)

bin/benchmark/set: $(INCLUDES) benchmark/set.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/set.cpp -o bin/benchmark/set -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)

bin/benchmark/find: $(INCLUDES) benchmark/find.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/find.cpp -o bin/benchmark/find -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)



# other
.PHONY: clean

clean:
	rm -rf obj/* bin/*
