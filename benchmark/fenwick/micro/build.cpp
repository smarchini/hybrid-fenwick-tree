#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include "../microbench.h"

int main(int argc, char **argv) {
  using namespace std;
  using namespace hft::fenwick;

  if (argc < 2) {
    cerr << "Not enough parameters: <dir> <size>\n";
    return -1;
  }

  std::string name = argv[1];
  size_t size = std::stoul(argv[2]);
  uint64_t seed = argc == 3 ? std::stoul(argv[2]) : 0;

  mt19937 re(seed);

  fen_to_file<FixedF<64>>(name + "/" + "FixedF", size, re);
  fen_to_file<FixedL<64>>(name + "/" + "FixedL", size, re);
  fen_to_file<ByteF<64>>(name + "/" + "ByteF", size, re);
  fen_to_file<ByteL<64>>(name + "/" + "ByteF", size, re);
  fen_to_file<BitF<64>>(name + "/" + "BitF", size, re);
  fen_to_file<BitL<64>>(name + "/" + "BitL", size, re);

  return 0;
}
