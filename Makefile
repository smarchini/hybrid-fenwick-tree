CC = g++
RELEASE = -O3 -march=native
DEBUG = -g -O0 --coverage -fprofile-dir=coverage
CFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)
LFLAGS = -std=c++14 -Wall -Wextra $(PARAMS)

INCLUDES = $(shell find include -name '*.hpp')
TEST_INCLUDES = $(shell find test -name '*.hpp')

INCLUDE_DYNAMIC = -IDYNAMIC -IDYNAMIC/algorithms -IDYNAMIC/internal
INCLUDE_INTERNAL = -Iinclude -Iinclude/fenwick -Iinclude/rankselect

MACRO_CACHESIZE = -DL1_CACHE_SIZE=$(shell getconf LEVEL1_DCACHE_SIZE) -DL2_CACHE_SIZE=$(shell getconf LEVEL2_CACHE_SIZE) -DL3_CACHE_SIZE=$(shell getconf LEVEL3_CACHE_SIZE)

FENBENCH_PATH = benchout/fenwick/$(shell date +"%Y%m%d-%H%M%S")/
RANSELBENCH_PATH = benchout/rankselect/$(shell date +"%Y%m%d-%H%M%S")/
KENEMYBENCH_PATH = benchout/kenemy/$(shell date +"%Y%m%d-%H%M%S")/

FENBENCH_VALS = 10000 100000 1000000 10000000 10000000
RANSELBENCH_VALS = 10000 100000 1000000 10000000 10000000 100000000 1000000000 2000000000 3000000000 4000000000 5000000000 6000000000 7000000000 8000000000 9000000000 10000000000

all: test benchmark

test: bin/test/test
	bin/test/test --gtest_color=yes
	lcov --capture --directory ./coverage --output-file coverage/lcov.info
	genhtml coverage/lcov.info --output-directory coverage/html


# Run
fenbench: benchmark/fenwick
	@mkdir -p $(FENBENCH_PATH)
	for (( m = 2; m < 10; m++ )); do \
		for (( size = 10**m; size < 10**(m+1); size += (m-1)*10**(m-1) )); do \
			echo "bin/benchmark/fenwick/tofile $(FENBENCH_PATH) $$size 1000000"; \
			bin/benchmark/fenwick/tofile $(FENBENCH_PATH) $$size 1000000; \
		done; \
	done

ranselbench: benchmark/rankselect
	@mkdir -p $(RANSELBENCH_PATH)
	for (( m = 1; m < 10; m++ )); do \
		for (( size = 10**m; size < 10**(m+1); size += 10**m )); do \
			echo "bin/benchmark/rankselect/tofile $(RANSELBENCH_PATH) $$size 1000000"; \
			bin/benchmark/rankselect/tofile $(RANSELBENCH_PATH) $$size 1000000; \
		done; \
	done

kenemybench: benchmark/kenemy
	@mkdir -p $(KENEMYBENCH_PATH)
	for (( m = 4; m < 10; m++ )); do \
		for (( size = 10**m; size < 10**(m+1); size += (m-1)*10**(m-1) )); do \
			echo "bin/benchmark/kenemy/tofile $(KENEMYBENCH_PATH) $$size"; \
			bin/benchmark/kenemy/tofile $(KENEMYBENCH_PATH) $$size; \
		done; \
	done

# Benchmark
benchmark: benchmark/fenwick benchmark/rankselect

benchmark/fenwick: bin/benchmark/fenwick/trees bin/benchmark/fenwick/tofile

benchmark/rankselect: bin/benchmark/rankselect/rankselect bin/benchmark/rankselect/tofile

benchmark/kenemy: bin/benchmark/kenemy/kenemy bin/benchmark/kenemy/tofile

# Test
# https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#running-test-programs-advanced-options
bin/test/test: $(INCLUDES) $(TEST_INCLUDES) test/test.cpp
	@mkdir -p $(@D) coverage
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDE_DYNAMIC) test/test.cpp -o bin/test/test -lgtest
	@mv *.gcno coverage/


# Benchmark fenwick tree
bin/benchmark/fenwick/trees: $(INCLUDES) benchmark/fenwick/trees.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE) $(INCLUDE_INTERNAL) $(MACRO_CACHESIZE) benchmark/fenwick/trees.cpp -o bin/benchmark/fenwick/trees

bin/benchmark/fenwick/tofile: $(INCLUDES) benchmark/fenwick/tofile.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE) $(INCLUDE_INTERNAL) $(MACRO_CACHESIZE) benchmark/fenwick/tofile.cpp -o bin/benchmark/fenwick/tofile

# Benchmark rank select
bin/benchmark/rankselect/rankselect: $(INCLUDES) benchmark/rankselect/rank_select.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE) $(INCLUDE_INTERNAL) $(INCLUDE_DYNAMIC) benchmark/rankselect/rank_select.cpp -o bin/benchmark/rankselect/rankselect

bin/benchmark/rankselect/tofile: $(INCLUDES) benchmark/rankselect/tofile.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE) $(INCLUDE_INTERNAL) $(INCLUDE_DYNAMIC) $(MACRO_CACHESIZE) benchmark/rankselect/tofile.cpp -o bin/benchmark/rankselect/tofile

# Benchmark kenemy
bin/benchmark/kenemy/kenemy: $(INCLUDES) benchmark/kenemy/kenemy.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE) $(INCLUDE_INTERNAL) $(INCLUDE_DYNAMIC) benchmark/kenemy/kenemy.cpp -o bin/benchmark/kenemy/kenemy

bin/benchmark/kenemy/tofile: $(INCLUDES) benchmark/kenemy/tofile.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE) $(INCLUDE_INTERNAL) $(INCLUDE_DYNAMIC) benchmark/kenemy/tofile.cpp -o bin/benchmark/kenemy/tofile


# Other
.PHONY: clean

clean:
	rm -rf bin coverage benchout *.gcno
