CC = g++
PARAMS = -O3 -march=native -g
CFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)
LFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)

INCLUDES = $(shell find include -name '*.hpp')
TEST_INCLUDES = $(shell find test -name '*.hpp')
INCLUDE_DYNAMIC = -Ibenchmark/rankselect/DYNAMIC -Ibenchmark/rankselect/DYNAMIC/algorithms -Ibenchmark/rankselect/DYNAMIC/internal

MACRO_CACHESIZE = -DL1_CACHE_SIZE=$(shell getconf LEVEL1_DCACHE_SIZE) -DL2_CACHE_SIZE=$(shell getconf LEVEL2_CACHE_SIZE) -DL3_CACHE_SIZE=$(shell getconf LEVEL3_CACHE_SIZE)


all: test benchmark

# https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
test: bin/test/test
	bin/test/test --gtest_color=yes

benchmark: benchmark/fenwick benchmark/rankselect

benchmark/fenwick: bin/benchmark/fenwick/trees bin/benchmark/fenwick/get bin/benchmark/fenwick/set bin/benchmark/fenwick/find

benchmark/rankselect: bin/benchmark/rankselect/rankselect


# bin
bin/test/test: $(INCLUDES) $(TEST_INCLUDES) test/test.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) test/test.cpp -o bin/test/test -lgtest

bin/benchmark/fenwick/trees: $(INCLUDES) benchmark/trees.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MACRO_CACHESIZE) benchmark/trees.cpp -o bin/benchmark/fenwick/trees 

bin/benchmark/fenwick/get: $(INCLUDES) benchmark/get.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MACRO_CACHESIZE) benchmark/get.cpp -o bin/benchmark/fenwick/get

bin/benchmark/fenwick/set: $(INCLUDES) benchmark/set.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MACRO_CACHESIZE) benchmark/set.cpp -o bin/benchmark/fenwick/set

bin/benchmark/fenwick/find: $(INCLUDES) benchmark/find.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MACRO_CACHESIZE) benchmark/find.cpp -o bin/benchmark/fenwick/find

bin/benchmark/rankselect/rankselect: $(INCLUDES) benchmark/rankselect/rank_select.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_DYNAMIC) benchmark/rankselect/rank_select.cpp -o bin/benchmark/rankselect/rankselect


# other
.PHONY: clean

clean:
	rm -rf obj/* bin/*
