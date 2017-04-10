OBJS = obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o obj/typed_fenwick_tree.o obj/shrank_fenwick_tree.o obj/dynamic_rank_select.o
CC = g++
OPTIMIZATIONS = -O3 -march=native -g
CFLAGS = -std=c++14 -Wall -Wextra -c $(OPTIMIZATIONS)
LFLAGS = -std=c++14 -Wall -Wextra $(OPTIMIZATIONS)

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
bin/test/test: $(OBJS) obj/test/test.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/test/test.o -lgtest -o bin/test/test

bin/benchmark/trees: $(OBJS) obj/benchmark/trees.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/benchmark/trees.o -o bin/benchmark/trees

bin/benchmark/get: $(OBJS) obj/benchmark/get.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/benchmark/get.o -o bin/benchmark/get

bin/benchmark/set: $(OBJS) obj/benchmark/set.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/benchmark/set.o -o bin/benchmark/set

bin/benchmark/find: $(OBJS) obj/benchmark/find.o
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) $(OBJS) obj/benchmark/find.o -o bin/benchmark/find


# lib objects
obj/simple_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/darray.hpp include/simple_fenwick_tree.hpp src/simple_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/simple_fenwick_tree.cpp

obj/typed_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/darray.hpp include/typed_fenwick_tree.hpp src/typed_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/typed_fenwick_tree.cpp

obj/byte_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/darray.hpp include/byte_fenwick_tree.hpp src/byte_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/byte_fenwick_tree.cpp

obj/compact_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/darray.hpp include/compact_fenwick_tree.hpp src/compact_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/compact_fenwick_tree.cpp

obj/shrank_fenwick_tree.o: include/broadword.hpp include/fenwick_tree.hpp include/darray.hpp include/shrank_fenwick_tree.hpp src/shrank_fenwick_tree.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/shrank_fenwick_tree.cpp

obj/dynamic_rank_select.o: include/broadword.hpp include/darray.hpp include/dynamic_rank_select.hpp src/dynamic_rank_select.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ src/dynamic_rank_select.cpp


# bin objects
obj/test/test.o: obj/simple_fenwick_tree.o obj/compact_fenwick_tree.o obj/byte_fenwick_tree.o test/test_utils.hpp test/broadword_test.hpp test/simple_fenwick_tree_test.hpp test/compact_fenwick_tree_test.hpp test/byte_fenwick_tree_test.hpp test/typed_fenwick_tree_test.hpp test/shrank_fenwick_tree_test.hpp test/same_behavior_test.hpp test/dynamic_rank_select_test.hpp test/test.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ test/test.cpp

obj/benchmark/trees.o: $(OBJS) benchmark/trees.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ benchmark/trees.cpp

obj/benchmark/get.o: $(OBJS) benchmark/bench_utils.hpp benchmark/get.cpp 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ benchmark/get.cpp  -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)

obj/benchmark/set.o: $(OBJS) benchmark/bench_utils.hpp benchmark/set.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ benchmark/set.cpp  -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)

obj/benchmark/find.o: $(OBJS) benchmark/bench_utils.hpp benchmark/find.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ benchmark/find.cpp  -DL1_CACHE_SIZE=$(L1_CACHE_SIZE) -DL2_CACHE_SIZE=$(L2_CACHE_SIZE)


# other
.PHONY: clean

clean:
	rm -rf obj/* bin/*
