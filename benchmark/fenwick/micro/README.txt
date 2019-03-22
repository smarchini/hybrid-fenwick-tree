How to use:

  1) Create the directory structure
     - $make dir

  2) Edit ./build.cpp as you want

  3) Compile what you need with
     - $make -j8 bin
     - $make -j8 tranbin
     - $make -j8 hugebin

  4) Create the directory for the trees (you choose the name)
     - $mkdir trees

  5) Build the trees choosen in step 2 with the dirname choosen in step 4 (you choose the size)
     - $./bin/build trees 1000000

  6) Run the benchmark (you choose the Fenwick, the operation and number of quries)
     - $./bin/prefix_fixedf trees 100000


pagetype: bin, hugebin, tranbin
operation: prefix, add, find
fenwick: fixedf, fixedl, bytef, bytel, bitf, bitl and some hybrids
