#define __HFT_BENCHMARK_FUNCTION__                                                                 \
  prefix<Hybrid<FixedL, FixedF, 64, 20>>(name + "/" + "Fixed20Fixed", queries, re);
#include "../micro.cpp"
