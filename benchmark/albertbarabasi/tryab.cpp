#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <random>
#include <string>

#include <rankselect/rank_select.hpp>
#include <rankselect/stride.hpp>
#include <rankselect/word.hpp>

#include <fenwick/bitf.hpp>
#include <fenwick/bitl.hpp>
#include <fenwick/bytef.hpp>
#include <fenwick/bytel.hpp>
#include <fenwick/fenwick_tree.hpp>
#include <fenwick/fixedf.hpp>
#include <fenwick/fixedl.hpp>

#include "../xoroshiro128pp.hpp"

using namespace std;
using namespace hft;
using namespace std::chrono;

template <typename dynft> size_t nostro(const uint64_t n, const uint64_t d) {
  assert(n >= d);

  dynft f;
  for (int i = 0; i < d; i++)
    f.push(1);

  uint64_t sum_degrees = d;

  for (uint64_t i = d; i < n; i++) {
    for (uint64_t j = 0; j < d; j++) {
      uint64_t x = f.find(next() % sum_degrees); // New vertex to connect to
      assert(x >= 0);
      assert(x < i);
      sum_degrees++;
      f.add(x + 1, 1);
    }
    f.push(d);
    sum_degrees += d;
  }

  cout << "space = " << f.bitCount() << ", ";
  return sum_degrees;
}

size_t networkx(const uint64_t n, const uint64_t d) {
  std::vector<int32_t> f;

  for (int i = 0; i < d; i++)
    f.push_back(i);

  for (uint64_t i = d; i < n; i++) {
    for (uint64_t j = 0; j < d; j++) {
      f.push_back(f[next() % f.size()]);
    }

    for (uint64_t t = 0; t < d; t++)
      f.push_back(i);
  }

  cout << "space = " << f.capacity() * sizeof(uint64_t) << ", ";
  return f.size();
}

void run_bench(size_t n, size_t d) {
  cout << "n = " << n << "\n";
  cout << "d = " << d << endl;

  auto begin = high_resolution_clock::now();
  cout << "fixedf(" << nostro<fenwick::FixedF<1>>(n, d) << ") = ";
  auto end = high_resolution_clock::now();
  auto elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << " ns" << endl;

  // begin = high_resolution_clock::now();
  // cout << "res=" << nostro<fenwick::ByteL<d * n>>(n, d) << ",";
  // end = high_resolution_clock::now();
  // elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  // cout << elapsed << ",";

  begin = high_resolution_clock::now();
  cout << "Netw_X(" << networkx(n, d) << ") = ";
  end = high_resolution_clock::now();
  elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << " ns" << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage ./bin/tryab [n] [d]\n";
    return 1;
  }

  size_t n, d;
  istringstream(argv[1]) >> n;
  istringstream(argv[2]) >> d;

  run_bench(n, d);

  return 0;
}
