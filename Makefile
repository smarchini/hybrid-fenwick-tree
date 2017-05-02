CC = g++
PARAMS = -O3 -march=native -g
CFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)
LFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)

INCLUDES = $(shell find include -name '*.hpp')
TEST_INCLUDES = $(shell find test -name '*.hpp')

L1_CACHE_SIZE=$(shell getconf LEVEL1_DCACHE_SIZE)
L2_CACHE_SIZE=$(shell getconf LEVEL2_CACHE_SIZE)
L3_CACHE_SIZE=$(shell getconf LEVEL3_CACHE_SIZE)


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
bin/test/test: $(INCLUDES) $(TEST_INCLUDES) test/test.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) test/test.cpp -o bin/test/test -lgtest

bin/benchmark/trees: $(INCLUDES) benchmark/trees.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/trees.cpp -o bin/benchmark/trees -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE) -DL3_CACHE_SIZE=$(L3_CACHE_SIZE)

bin/benchmark/get: $(INCLUDES) benchmark/get.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/get.cpp -o bin/benchmark/get -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE) -DL3_CACHE_SIZE=$(L3_CACHE_SIZE)

bin/benchmark/set: $(INCLUDES) benchmark/set.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/set.cpp -o bin/benchmark/set -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE) -DL3_CACHE_SIZE=$(L3_CACHE_SIZE)

bin/benchmark/find: $(INCLUDES) benchmark/find.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) benchmark/find.cpp -o bin/benchmark/find -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE) -DL3_CACHE_SIZE=$(L3_CACHE_SIZE)



# other
.PHONY: clean

clean:
	rm -rf obj/* bin/*
