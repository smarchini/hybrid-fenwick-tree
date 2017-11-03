#!/bin/env bash

mkdir -p ./images

for file in $1/*; do
    echo "Generating image: " $file
    python pdfplot.py $file
done;

mv images "$2"
