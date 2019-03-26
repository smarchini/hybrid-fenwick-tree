#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "utils.hpp"

using namespace std;

template <template <size_t> class T>
void runall(const char *name, size_t size, size_t queries, mt19937 re);

// g++ -DHFT_DISABLE_TRANSHUGE -DHFT_NOHOLES -std=c++17 -O3 -march=native -I../../include runall.cpp
int main(int argc, char **argv) {
  using namespace hft::fenwick;

  if (argc < 3) {
    cerr << "Not enough parameters: <size> <queries>\n";
    return -1;
  }

  size_t size = stoul(argv[1]);
  size_t queries = stoul(argv[2]);
  uint64_t seed = argc >= 3 ? stoul(argv[2]) : 0;

  mt19937 re(seed);

  runall<FixedF>("FixedF (holes " STRINGIFY(HFT_HOLES) ")", size, queries, re);
  // runall<FixedL>("FixedL (holes " STRINGIFY(HFT_HOLES) ")", size, queries, re);
  runall<ByteF>("ByteF (holes " STRINGIFY(HFT_HOLES) ")", size, queries, re);
  // runall<ByteL>("ByteL (holes " STRINGIFY(HFT_HOLES) ")", size, queries, re);
  runall<BitF>("BitF (holes " STRINGIFY(HFT_HOLES) ")", size, queries, re);
  // runall<BitL>("BitL (holes " STRINGIFY(HFT_HOLES) ")", size, queries, re);

  return 0;
}

template <template <size_t> class T>
void runall(const char *name, size_t size, size_t queries, mt19937 re) {
  using namespace std::chrono;
  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t BOUND = 64, REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<uint64_t> seqdist(0, BOUND);
  uniform_int_distribution<size_t> idxdist(1, size);
  uniform_int_distribution<uint64_t> cumseqdist(0, BOUND * size);

  unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size);
  for (size_t i = 0; i < size; i++)
    sequence[i] = seqdist(re);

  begin = chrono::high_resolution_clock::now();
  T<BOUND> fenwick(sequence.get(), size);
  end = chrono::high_resolution_clock::now();
  auto ctor = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << name << ": " << fenwick.bitCount() / (double)size << " b/item\n";
  cout << "ctor: " << flush;
  cout << ctor / (double)size << setw(7) << " ns/item" << endl;
  sequence.reset(nullptr);

  cout << "prefix: " << flush;
  vector<chrono::nanoseconds::rep> prefix;
  for (size_t r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for (uint64_t i = 0; i < queries; ++i)
      u ^= fenwick.prefix(idxdist(re));
    end = high_resolution_clock::now();
    prefix.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  std::sort(prefix.begin(), prefix.end());
  cout << prefix[IDXMID] * c << setw(7) << " ns/item" << endl;

  cout << "find: " << flush;
  vector<chrono::nanoseconds::rep> find;
  for (size_t r = 0; r < REPS; r++) {
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
      u ^= fenwick.find(cumseqdist(re));
    end = chrono::high_resolution_clock::now();
    find.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  sort(find.begin(), find.end());
  cout << find[IDXMID] * c << setw(7) << " ns/item" << endl;

  cout << "add: " << flush;
  vector<chrono::nanoseconds::rep> add;
  for (size_t r = 0; r < REPS; r++) {
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++) {
      size_t idx = idxdist(re);
      fenwick.add(idx, i % 2 ? -seqdist(re) : seqdist(re));
    }
    end = chrono::high_resolution_clock::now();
    add.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  sort(add.begin(), add.end());
  cout << add[IDXMID] * c << setw(7) << " ns/item\n" << endl;

  // The add cannot be erased by the compiler
  u ^= fenwick.prefix(idxdist(re));
  const volatile uint64_t __attribute__((unused)) unused = u;
}
