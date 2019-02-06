mkdir dir && make -j8 hugebin && ./hugebin/add_bitf 10000000 100 
                         ^            ^     ^   ^       ^     ^
                         |            |     |   |       |     |
                         +- pag_type -+    op fenw    size queries

pag_type: bin, hugebin, tranbin
op: prefix, add, find
fenw: fixedf, fixedl, bytef, bytel, bitf, bitl
