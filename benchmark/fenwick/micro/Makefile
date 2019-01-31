CC = g++ -std=c++17 -Wall -Wextra -O3 -march=native -fno-exceptions -fno-rtti -fno-omit-frame-pointer -I../../../include  $(CFLAGS)
BINDIR = ./

HUGETLB = -DHFT_USE_HUGETLB
TRANTLB = -DHFT_USE_HUGETLB -DHFT_TRANSPARENT

all: prefix add find

bin: BINDIR = ./bin
bin: all

hugebin: CC += -DHFT_USE_HUGETLB
hugebin: BINDIR = ./hugebin
hugebin: all

tranbin: CC += -DHFT_USE_HUGETLB -DHFT_TRANSPARENT
tranbin: BINDIR = ./tranbin
tranbin: all

dir:
	mkdir bin hugebin tranbin


prefix: prefix_fixedf prefix_bytef prefix_bitf prefix_fixedl prefix_bytel prefix_bitl 

add: add_fixedf add_bytef add_bitf add_fixedl add_bytel add_bitl 

find: find_fixedf find_bytef find_bitf find_fixedl find_bytel find_bitl 


prefix_fixedf: prefix_fixedf.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bytef: prefix_bytef.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bitf: prefix_bitf.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixedl: prefix_fixedl.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bytel: prefix_bytel.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bitl: prefix_bitl.cpp
	$(CC) -o $(BINDIR)/$@ $^


add_fixedf: add_fixedf.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bytef: add_bytef.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bitf: add_bitf.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixedl: add_fixedl.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bytel: add_bytel.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bitl: add_bitl.cpp
	$(CC) -o $(BINDIR)/$@ $^


find_fixedf: find_fixedf.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bytef: find_bytef.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bitf: find_bitf.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixedl: find_fixedl.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bytel: find_bytel.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bitl: find_bitl.cpp
	$(CC) -o $(BINDIR)/$@ $^


.PHONY: clean

clean:
	rm -rf bin hugebin tranbin
