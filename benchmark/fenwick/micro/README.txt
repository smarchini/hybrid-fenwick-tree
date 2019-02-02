mkdir bin && make -j8 CFLAGS=-DHUGE all && ./bin/add_bitf 10000000 100 
                                ^                 ^   ^       ^     ^
                                |                 |   |       |     |
                            page_type            op  fenw   size  queries

page_type: (nothing), HUGE, TRANSPARENT
op: prefix, add, find
fenw: fixedf, fixedl, bytef, bytel, bitf, bitl
