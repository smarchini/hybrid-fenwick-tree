//#define HFT_USE_HUGETLB

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "utils.hpp"

template <typename T>
void bench(const char *name, size_t size, uint64_t order[], uint64_t increments[],
           uint64_t add_updates[], uint64_t sequence[]);

int main(int argc, char **argv) {
  using namespace std;
  using namespace hft::fenwick;

  if (argc < 2) {
    cerr << "Not enough parameters\n";
    return -1;
  }

  istringstream iss(argv[1]);

  size_t size;
  if (!(iss >> size)) {
    cerr << "Invalid number " << argv[1] << '\n';
    return -1;
  }

  uint64_t *order = new uint64_t[size];
  for (size_t i = 0; i < size; i++)
    order[i] = i + 1;
  random_shuffle(order, order + size);

  uint64_t *increments = new uint64_t[size];
  uint64_t *add_updates = new uint64_t[size];

  fill_with_random_values(increments, size);
  fill_with_random_values(add_updates, size);
  for (size_t i = 0; i < size; i++) {
    int inc = add_updates[i] - increments[i];
    if (inc < 0)
      inc = -inc;
    add_updates[i] = (increments[i] + inc) < 64 ? inc : 0;
  }

  uint64_t *sequence = new uint64_t[size];
  inc_to_seq(increments, sequence, size);
  random_shuffle(sequence, sequence + size);

  bench<FixedF<64>>("FixedF", size, order, increments, add_updates, sequence);
  bench<FixedL<64>>("FixedL", size, order, increments, add_updates, sequence);
  bench<TypeF<64>>("TypeF", size, order, increments, add_updates, sequence);
  bench<TypeL<64>>("TypeL", size, order, increments, add_updates, sequence);
  bench<ByteF<64>>("ByteF", size, order, increments, add_updates, sequence);
  bench<ByteL<64>>("ByteL", size, order, increments, add_updates, sequence);
  bench<BitF<64>>("BitF", size, order, increments, add_updates, sequence);
  bench<BitL<64>>("BitL", size, order, increments, add_updates, sequence);

  delete[] increments;
  delete[] add_updates;
  delete[] order;
  delete[] sequence;

  return 0;
}

template <typename T>
void bench(const char *name, size_t size, uint64_t order[], uint64_t increments[],
           uint64_t add_updates[], uint64_t sequence[]) {
  using namespace std;
  chrono::high_resolution_clock::time_point begin, end;
  uint64_t u = 0;

  // constructor
  begin = chrono::high_resolution_clock::now();
  T tree(increments, size);
  end = chrono::high_resolution_clock::now();
  auto constructor = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

  // prefix
  // begin = chrono::high_resolution_clock::now();
  // for (size_t i = 0; i < size; i++)
  //     u ^= tree.prefix(order[i]);
  // end = chrono::high_resolution_clock::now();
  // auto prefix = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

  // find
  begin = chrono::high_resolution_clock::now();
  for (size_t i = 0; i < size; i++)
    u ^= tree.find(sequence[i]);
  end = chrono::high_resolution_clock::now();
  auto find = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

  // add
  // begin = chrono::high_resolution_clock::now();
  // for (size_t i = 0; i < size; i++)
  //     tree.add(order[i], add_updates[i]);
  // end = chrono::high_resolution_clock::now();
  // auto add = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

  // for (size_t i = 0; i < size; i++)
  //     tree.add(order[i], -add_updates[i]);

  // // find complement (after tree.add, so sequence[] is not cached)
  // begin = chrono::high_resolution_clock::now();
  // for (size_t i = 0; i < size; i++)
  //     u ^= tree.compFind(sequence[i]);
  // end = chrono::high_resolution_clock::now();
  // auto findc = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

  const volatile uint64_t __attribute__((unused)) unused = u;

  const double c = 1. / size;
  cout << "\n" << name << ": " << tree.bitCount() / (double)size << " b/item\n";
  cout << "build: " << fixed << setw(12) << constructor * c << " ns/item\n";
  // cout << "prefix:" << fixed << setw(12) << prefix * c << " ns/item\n";
  // cout << "add:   " << fixed << setw(12) << add * c << " ns/item\n";
  cout << "find:  " << fixed << setw(12) << find * c << " ns/item\n";
  // cout << "findc: " << fixed << setw(12) << findc * c << " ns/item\n";
}
