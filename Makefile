CC = g++
PARAMS = -O3 -march=native -g
CFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)
LFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)

INCLUDES = $(shell find include -name '*.hpp')
TEST_INCLUDES = $(shell find test -name '*.hpp')

INCLUDE_DYNAMIC = -IDYNAMIC -IDYNAMIC/algorithms -IDYNAMIC/internal
INCLUDE_INTERNAL = -Iinclude -Iinclude/fenwick -Iinclude/rankselect

MACRO_CACHESIZE = -DL1_CACHE_SIZE=$(shell getconf LEVEL1_DCACHE_SIZE) -DL2_CACHE_SIZE=$(shell getconf LEVEL2_CACHE_SIZE) -DL3_CACHE_SIZE=$(shell getconf LEVEL3_CACHE_SIZE)


all: test benchmark

# https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
test: bin/test/test
	bin/test/test --gtest_color=yes

benchmark: benchmark/fenwick benchmark/rankselect

benchmark/fenwick: bin/benchmark/fenwick/trees bin/benchmark/fenwick/get bin/benchmark/fenwick/set bin/benchmark/fenwick/find

benchmark/rankselect: bin/benchmark/rankselect/rankselect


# Test
bin/test/test: $(INCLUDES) $(TEST_INCLUDES) test/test.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_DYNAMIC) test/test.cpp -o bin/test/test -lgtest


# Benchmark fenwick tree
bin/benchmark/fenwick/trees: $(INCLUDES) benchmark/fenwick/trees.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_INTERNAL) $(MACRO_CACHESIZE) benchmark/fenwick/trees.cpp -o bin/benchmark/fenwick/trees

bin/benchmark/fenwick/get: $(INCLUDES) benchmark/fenwick/get.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_INTERNAL) $(MACRO_CACHESIZE) benchmark/fenwick/get.cpp -o bin/benchmark/fenwick/get

bin/benchmark/fenwick/set: $(INCLUDES) benchmark/fenwick/set.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_INTERNAL) $(MACRO_CACHESIZE) benchmark/fenwick/set.cpp -o bin/benchmark/fenwick/set

bin/benchmark/fenwick/find: $(INCLUDES) benchmark/fenwick/find.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_INTERNAL) $(MACRO_CACHESIZE) benchmark/fenwick/find.cpp -o bin/benchmark/fenwick/find


# Benchmark rank select
bin/benchmark/rankselect/rankselect: $(INCLUDES) benchmark/rankselect/rank_select.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE_INTERNAL) $(INCLUDE_DYNAMIC) benchmark/rankselect/rank_select.cpp -o bin/benchmark/rankselect/rankselect


# Other
.PHONY: clean

clean:
	rm -rf obj/* bin/*
