#ifdef __HFT_BENCHMARK_FUNCTION__

#include "microbench.h"

// g++ -std=c++17 -O3 -march=native bitf.cpp -o bitf
int main(int argc, char **argv)
{
    using namespace std;
    using namespace hft::fenwick;

    if (argc < 3) {
        cerr << "Not enough parameters: <size> <queries>\n";
        return -1;
    }

    size_t size = std::stoul(argv[1]);
    size_t queries = std::stoul(argv[2]);

	uint64_t seed = 0;
	if (argc == 4) seed = std::stoul(argv[3]);

    mt19937 re(seed);

    // run the benchmark
    __HFT_BENCHMARK_FUNCTION__

    return 0;
}

#endif
