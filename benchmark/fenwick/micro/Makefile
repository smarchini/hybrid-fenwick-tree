CC = g++ -g -std=c++17 -Wall -Wextra -O3 -march=native -fno-exceptions -fno-rtti -fno-omit-frame-pointer -I../../../include

dir:
	mkdir -p bin hugebin tranbin

# Note: TRASNSPARENT and HUGE are only used in ../microbench.h

bin: CC += -DHFT_DISABLE_TRANSHUGE
bin: BINDIR = ./bin
bin: all

tranbin: BINDIR = ./tranbin
tranbin: all

hugebin: CC += -DHFT_HUGETLBPAGE
hugebin: BINDIR = ./hugebin
hugebin: all


all: build prefix add find

build: build.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix: prefix_fixedf prefix_bytef prefix_bitf prefix_fixedl prefix_bytel prefix_bitl prefix_fixed20fixed prefix_fixed23byte prefix_fixed23bit prefix_byte23byte prefix_byte23bit prefix_bit23bit

add: add_fixedf add_bytef add_bitf add_fixedl add_bytel add_bitl add_fixed20fixed add_fixed23byte add_fixed23bit add_byte23byte add_byte23bit add_bit23bit

find: find_fixedf find_bytef find_bitf find_fixedl find_bytel find_bitl find_fixed20fixed find_fixed23byte find_fixed23bit find_byte23byte find_byte23bit find_bit23bit


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

prefix_fixed20fixed: prefix_fixed20fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed23byte: prefix_fixed23byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed23bit: prefix_fixed23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_byte23byte: prefix_byte23byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_byte23bit: prefix_byte23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bit23bit: prefix_bit23bit.cpp
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

add_fixed20fixed: add_fixed20fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed23byte: add_fixed23byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed23bit: add_fixed23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_byte23byte: add_byte23byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_byte23bit: add_byte23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bit23bit: add_bit23bit.cpp
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

find_fixed20fixed: find_fixed20fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed23byte: find_fixed23byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed23bit: find_fixed23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_byte23byte: find_byte23byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_byte23bit: find_byte23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bit23bit: find_bit23bit.cpp
	$(CC) -o $(BINDIR)/$@ $^


.PHONY: clean

clean:
	rm -rf bin hugebin tranbin
