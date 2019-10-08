#define __HFT_BENCHMARK_FUNCTION__                                                                 \
  add<Hybrid<BitL, BitF, 64, 23>>(name + "/" + "Bit23Bit", queries, re);
#include "../micro.cpp"
