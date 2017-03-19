#!/bin/env bash

DIRNAME=$(date +"%m%d-%H%M-")$(hostname)
mkdir -p stats/$DIRNAME
mkdir -p stats/$DIRNAME/disassembly
mkdir -p stats/$DIRNAME/benchmark

lscpu > ./stats/$DIRNAME/cpuinfo.txt

for file in obj/*.o; do
    filename=$(basename "$file" .o)
    objdump -d -S $file > stats/$DIRNAME/disassembly/$filename".asm"
done;

for val in 1048575 33554431 67108863 134217727; do
    echo "benchmark" $val
    ./bin/benchmark $val > stats/$DIRNAME/benchmark/$val.txt
done 
