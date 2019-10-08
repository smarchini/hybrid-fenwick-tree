#define __HFT_BENCHMARK_FUNCTION__                                                                 \
  add<Hybrid<ByteL, ByteF, 64, 23>>(name + "/" + "Byte23Byte", queries, re);
#include "../micro.cpp"
