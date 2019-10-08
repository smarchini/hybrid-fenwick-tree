#define __HFT_BENCHMARK_FUNCTION__                                                                 \
  find<Hybrid<FixedL, ByteF, 64, 23>>(name + "/" + "Fixed23Byte", queries, re);
#include "../micro.cpp"
