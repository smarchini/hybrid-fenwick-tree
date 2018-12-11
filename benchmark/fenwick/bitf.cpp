#include "microbench.h"

// g++ -std=c++17 -O3 -march=native bitf.cpp -o bitf
int main(int argc, char **argv)
{
    using namespace std;
    using namespace hft::fenwick;

    random_device rd;
    default_random_engine re(rd());

    if (argc < 3) {
        cerr << "Not enough parameters: <size> <queries>\n";
        return -1;
    }

    size_t size = std::stoi(argv[1]);
    size_t queries = std::stoi(argv[2]);

    bench<BitF<64>>("BitF", size, queries, re);

    return 0;
}
