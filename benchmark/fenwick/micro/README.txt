You might want to do something like this: 

make dir
make CFLAGS=-DMAGIC=0 tranbin && ./tranbin/add_bitf 10000000 100 
                    ^    ^           ^        ^         ^     ^
                    |    |           |        |         |     |
change these:     --+  --+         --+      --+       --+   --+

MAGIC is the initial offset

NOTE: at the moment 4096 is hardcoded in the fenwicks .cpp files so the
generated .txt files might not give you what you want, i.e. 2MB pages)
