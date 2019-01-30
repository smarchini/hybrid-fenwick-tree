You might want to do something like this: 

make dir
make CFLAGS=-DMAGIC=0 tranbin && ./tranbin/add_bitf 10000000 100 
                    ^    ^           ^        ^         ^     ^
                    |    |           |        |         |     |
change these:     --+  --+         --+      --+       --+   --+

MAGIC is the initial offset
