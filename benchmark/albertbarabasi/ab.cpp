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
    f.push(1); // NOTA: non dovrebbe essere d?

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

  return f.size();
}

template <size_t n, size_t d> void run_bench() {
  cout << n << "," << d << "," << flush;

  auto begin = high_resolution_clock::now();
  cout << "res=" << nostro<fenwick::FixedF<n * d>>(n, d) << ",";
  auto end = high_resolution_clock::now();
  auto elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << ",";

  begin = high_resolution_clock::now();
  cout << "res=" << nostro<fenwick::FixedL<n * d>>(n, d) << ",";
  end = high_resolution_clock::now();
  elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << ",";

  begin = high_resolution_clock::now();
  cout << "res=" << nostro<fenwick::ByteF<d * n>>(n, d) << ",";
  end = high_resolution_clock::now();
  elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << ",";

  begin = high_resolution_clock::now();
  cout << "res=" << nostro<fenwick::ByteL<d * n>>(n, d) << ",";
  end = high_resolution_clock::now();
  elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << ",";

  begin = high_resolution_clock::now();
  cout << "res=" << networkx(n, d) << ",";
  end = high_resolution_clock::now();
  elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed << endl;
}

int main(int argc, char *argv[]) {
  std::cout << "n,d,fixedf,fixedl,bytef,bytel,networkx\n";

  run_bench<1000000, 10>();
  run_bench<1000000, 20>();
  run_bench<1000000, 30>();
  run_bench<1000000, 40>();
  run_bench<1000000, 50>();
  run_bench<1000000, 60>();
  run_bench<1000000, 70>();
  run_bench<1000000, 80>();
  run_bench<1000000, 90>();

  run_bench<1000000, 100>();
  run_bench<1000000, 200>();
  run_bench<1000000, 300>();
  run_bench<1000000, 400>();
  run_bench<1000000, 500>();
  run_bench<1000000, 600>();
  run_bench<1000000, 700>();
  run_bench<1000000, 800>();
  run_bench<1000000, 900>();

  run_bench<1000000, 1000>();
  run_bench<1000000, 2000>();

  return 0;
}
