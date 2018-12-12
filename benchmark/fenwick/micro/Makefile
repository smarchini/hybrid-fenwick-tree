CC = g++ -std=c++17 -Wall -Wextra -O3 -march=native -I../../../include

all: prefix add find


prefix: prefix_fixedf prefix_bytef prefix_bitf prefix_fixedl prefix_bytel prefix_bitl

add: add_fixedf add_bytef add_bitf add_fixedl add_bytel add_bitl

find: find_fixedf find_bytef find_bitf find_fixedl find_bytel find_bitl


prefix_fixedf: prefix_fixedf.cpp
	$(CC) -o $@ $^

prefix_bytef: prefix_bytef.cpp
	$(CC) -o $@ $^

prefix_bitf: prefix_bitf.cpp
	$(CC) -o $@ $^

prefix_fixedl: prefix_fixedl.cpp
	$(CC) -o $@ $^

prefix_bytel: prefix_bytel.cpp
	$(CC) -o $@ $^

prefix_bitl: prefix_bitl.cpp
	$(CC) -o $@ $^


add_fixedf: add_fixedf.cpp
	$(CC) -o $@ $^

add_bytef: add_bytef.cpp
	$(CC) -o $@ $^

add_bitf: add_bitf.cpp
	$(CC) -o $@ $^

add_fixedl: add_fixedl.cpp
	$(CC) -o $@ $^

add_bytel: add_bytel.cpp
	$(CC) -o $@ $^

add_bitl: add_bitl.cpp
	$(CC) -o $@ $^


find_fixedf: find_fixedf.cpp
	$(CC) -o $@ $^

find_bytef: find_bytef.cpp
	$(CC) -o $@ $^

find_bitf: find_bitf.cpp
	$(CC) -o $@ $^

find_fixedl: find_fixedl.cpp
	$(CC) -o $@ $^

find_bytel: find_bytel.cpp
	$(CC) -o $@ $^

find_bitl: find_bitl.cpp
	$(CC) -o $@ $^


.PHONY: clean

clean:
	rm prefix_fixedf prefix_bytef prefix_bitf prefix_fixedl prefix_bytel prefix_bitl
	rm add_fixedf add_bytef add_bitf add_fixedl add_bytel add_bitl
	rm find_fixedf find_bytef find_bitf find_fixedl find_bytel find_bitl
