##CC = clang++ -std=c++17 -Wall -Wextra -O3 -march=native -fno-exceptions -fno-rtti -fno-omit-frame-pointer -I../../../include $(CFLAGS)
CC = g++ -g -std=c++17 -Wall -Wextra -O3 -march=native -fno-exceptions -fno-rtti -fno-omit-frame-pointer -I../../../include $(CFLAGS)
BINDIR = ./

HUGETLB = -DHFT_USE_HUGETLB
TRANTLB = -DHFT_USE_HUGETLB -DHFT_TRANSPARENT

all: prefix add find
#all: add_bitf add_bytef add_fixedf

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


prefix: prefix_fixedf prefix_bytef prefix_bitf prefix_fixedl prefix_bytel prefix_bitl prefix_fixed18fixed prefix_fixed19byte prefix_fixed20bit prefix_byte19byte prefix_byte20bit prefix_bit20bit prefix_fixed20fixed prefix_fixed21byte prefix_fixed25bit prefix_byte21byte prefix_byte25bit prefix_bit25bit

add: add_fixedf add_bytef add_bitf add_fixedl add_bytel add_bitl add_fixed18fixed add_fixed19byte add_fixed20bit add_byte19byte add_byte20bit add_bit20bit add_fixed20fixed add_fixed21byte add_fixed25bit add_byte21byte add_byte25bit add_bit25bit

find: find_fixedf find_bytef find_bitf find_fixedl find_bytel find_bitl find_fixed18fixed find_fixed19byte find_fixed20bit find_byte19byte find_byte20bit find_bit20bit find_fixed20fixed find_fixed21byte find_fixed25bit find_byte21byte find_byte25bit find_bit25bit


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

prefix_fixed18fixed: prefix_fixed18fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed19byte: prefix_fixed19byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed20bit: prefix_fixed20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_byte19byte: prefix_byte19byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_byte20bit: prefix_byte20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bit20bit: add_bit20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed20fixed: prefix_fixed20fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed21byte: prefix_fixed21byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_fixed25bit: prefix_fixed25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_byte21byte: prefix_byte21byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_byte25bit: prefix_byte25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

prefix_bit25bit: prefix_bit25bit.cpp
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

add_fixed18fixed: add_fixed18fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed19byte: add_fixed19byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed20bit: add_fixed20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_byte19byte: add_byte19byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_byte20bit: add_byte20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bit20bit: add_bit20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed20fixed: add_fixed20fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed21byte: add_fixed21byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_fixed25bit: add_fixed25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_byte21byte: add_byte21byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_byte25bit: add_byte25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

add_bit25bit: add_bit25bit.cpp
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

find_fixed18fixed: find_fixed18fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed19byte: find_fixed19byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed20bit: find_fixed20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_byte19byte: find_byte19byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_byte20bit: find_byte20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bit20bit: find_bit20bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed20fixed: find_fixed20fixed.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed21byte: find_fixed21byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_fixed25bit: find_fixed25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_byte21byte: find_byte21byte.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_byte25bit: find_byte25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^

find_bit25bit: find_bit25bit.cpp
	$(CC) -o $(BINDIR)/$@ $^


.PHONY: clean

clean:
	rm -rf bin hugebin tranbin
