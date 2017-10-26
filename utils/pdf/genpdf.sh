#!/bin/env bash

mkdir -p ./images

for file in $1/*; do
    echo "Generating image: " $file
    python pgfplot.py $file
done;

pdflatex template.tex
rm -r template.out template.aux template.log images
mv template.pdf images.pdf
