// Compile: g++ -std=c++17 -O3 -march=native -I../../include -I../../DYNAMIC -I../..DYNAMIC/algorithms -I../..DYNAMIC/internal -o rank_select rank_select.cpp
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>

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
#include <fenwick/hybrid.hpp>
#include <fenwick/typef.hpp>
#include <fenwick/typel.hpp>

#include <dynamic.hpp>

using namespace hft;
using namespace hft::fenwick;
using namespace hft::ranking;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

void dynamic(const char *name, uint64_t *bitvector, uniform_int_distribution<uint64_t> rank,
             uniform_int_distribution<uint64_t> sel0, uniform_int_distribution<uint64_t> sel1,
             uniform_int_distribution<uint64_t> bit, size_t size, size_t queries, mt19937 re);

template <typename T>
void internal(const char *name, uint64_t *bitvector, uniform_int_distribution<uint64_t> rank,
              uniform_int_distribution<uint64_t> sel0, uniform_int_distribution<uint64_t> sel1,
              uniform_int_distribution<uint64_t> bit, size_t size, size_t queries, mt19937 re);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Not enough parameters\n";
    return -1;
  }

  size_t size = stoul(argv[1]);
  size_t queries = stoul(argv[2]);
  uint64_t seed = argc >= 3 ? stoul(argv[2]) : 0;

  mt19937 re(seed);

  uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
  uint64_t *bv = new uint64_t[size];
  for (size_t i = 0; i < size; i++)
    bv[i] = dist(re);

  uint64_t ones = 0;
  for (size_t i = 0; i < size; i++)
    ones += popcount(bv[i]);

  uint64_t zeroes = 64 * size - ones;

  uniform_int_distribution<uint64_t> bit(0, (size-1)*64);
  uniform_int_distribution<uint64_t> rank(0, 64 * size);
  uniform_int_distribution<uint64_t> select0(0, zeroes - 1);
  uniform_int_distribution<uint64_t> select1(0, ones - 1);

  cout << "Bitvector with " << ones << " ones and " << zeroes << " zeroes" << endl;
  dynamic("DYNAMIC", bv, rank, select0, select1, bit, size, queries, re);
  cout << "\n------------------------------\n";
  internal<Word<FixedF>>("Fixed[F]1", bv, rank, select0, select1, bit, size, queries, re);
  internal<Word<FixedL>>("Fixed[L]1", bv, rank, select0, select1, bit, size, queries, re);
  internal<Word<ByteF>>("Byte[F]1", bv, rank, select0, select1, bit, size, queries, re);
  internal<Word<ByteL>>("Byte[L]1", bv, rank, select0, select1, bit, size, queries, re);
  internal<Word<BitF>>("Bit[F]1", bv, rank, select0, select1, bit, size, queries, re);
  internal<Word<BitL>>("Bit[L]1", bv, rank, select0, select1, bit, size, queries, re);
  cout << "\n------------------------------\n";
  internal<Stride<FixedF, 8>>("Fixed[F]8", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<FixedL, 8>>("Fixed[L]8", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<ByteL, 8>>("Byte[L]8", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<ByteF, 8>>("Byte[F]8", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<BitL, 8>>("Bit[L]8", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<BitF, 8>>("Bit[F]8", bv, rank, select0, select1, bit, size, queries, re);
  cout << "\n------------------------------\n";
  internal<Stride<FixedF, 16>>("Fixed[F]16", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<FixedL, 16>>("Fixed[L]16", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<ByteL, 16>>("Byte[L]16", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<ByteF, 16>>("Byte[F]16", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<BitL, 16>>("Bit[L]16", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<BitF, 16>>("Bit[F]16", bv, rank, select0, select1, bit, size, queries, re);
  cout << "\n------------------------------\n";
  internal<Stride<FixedF, 32>>("Fixed[F]32", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<FixedL, 32>>("Fixed[L]32", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<ByteL, 32>>("Byte[L]32", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<ByteF, 32>>("Byte[F]32", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<BitL, 32>>("Bit[L]32", bv, rank, select0, select1, bit, size, queries, re);
  internal<Stride<BitF, 32>>("Bit[F]32", bv, rank, select0, select1, bit, size, queries, re);

  delete[] bv;

  return 0;
}

void dynamic(const char *name, uint64_t *bitvector, uniform_int_distribution<uint64_t> rank,
             uniform_int_distribution<uint64_t> sel0, uniform_int_distribution<uint64_t> sel1,
             uniform_int_distribution<uint64_t> bit, size_t size, size_t queries, mt19937 re) {
  high_resolution_clock::time_point begin, end;
  const double c = 1. / queries;
  uint64_t u = 0;

  dyn::suc_bv dynamic;
  begin = high_resolution_clock::now();
  for (uint64_t i = 0; i < size; ++i) {
    for (uint64_t j = 0; j < 64; ++j)
      dynamic.insert(64*i + j, bitvector[i] & (1ULL << j));
  }
  end = high_resolution_clock::now();
  auto build = duration_cast<chrono::nanoseconds>(end-begin).count();
  cout << "\n" << name << ": " << dynamic.bit_size() / (size * 64.) << " b/item\n";
  cout << "ctor: " << build / (double)size << " ns/item\n" << flush;

  constexpr int REPS = 5;
  constexpr size_t MID = 2; // index of the median of a REPS elements sorted vector

  cout << "rank1: " << flush;
  vector<chrono::nanoseconds::rep> rank1;
  for (int r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for(uint64_t i = 0; i < queries; ++i)
      u ^= dynamic.rank1(rank(re) ^ (u & 1));
    end = high_resolution_clock::now();
    rank1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
  }
  std::sort(rank1.begin(), rank1.end());
  cout << rank1[MID] * c << " ns/item\n";

  cout << "select1: " << flush;
  vector<chrono::nanoseconds::rep> select1;
  for (int r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for(uint64_t i = 0; i < queries; ++i)
      u ^= dynamic.select1(sel1(re) ^ (u & 1));
    end = high_resolution_clock::now();
    select1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
  }
  std::sort(select1.begin(), select1.end());
  cout << select1[MID] * c << " ns/item\n";

  cout << "update: " << flush;
  vector<chrono::nanoseconds::rep> update;
  for (int r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for (uint64_t i = 0; i < queries; ++i) {
      if (i & 1) dynamic.set(bit(re) ^ (u & 1), true);
      else dynamic.set(bit(re) ^ (u & 1), false);
    }
    end = high_resolution_clock::now();
    update.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  std::sort(update.begin(), update.end());
  cout << update[MID] * c << " ns/item\n";

  const volatile uint64_t __attribute__((unused)) unused = u;
}

template <typename T>
void internal(const char *name, uint64_t *bitvector, uniform_int_distribution<uint64_t> rank,
              uniform_int_distribution<uint64_t> sel0, uniform_int_distribution<uint64_t> sel1,
              uniform_int_distribution<uint64_t> bit, size_t size, size_t queries, mt19937 re) {
  high_resolution_clock::time_point begin, end;
  const double c = 1. / queries;
  uint64_t u = 0;

  begin = high_resolution_clock::now();
  T bv(bitvector, size);
  end = high_resolution_clock::now();
  auto build = duration_cast<chrono::nanoseconds>(end-begin).count();
  cout << "\n" << name << ": " << bv.bitCount() / (size * 64.) << " b/item\n";
  cout << "ctor: " << build / (double)size << " ns/item\n" << flush;

  constexpr int REPS = 5;
  constexpr size_t MID = 2; // index of the median of a REPS elements sorted vector

  cout << "rank1: " << flush;
  vector<chrono::nanoseconds::rep> rank1;
  for (int r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for(uint64_t i = 0; i < queries; ++i)
      u ^= bv.rank(rank(re) ^ (u & 1));
    end = high_resolution_clock::now();
    rank1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
  }
  std::sort(rank1.begin(), rank1.end());
  cout << rank1[MID] * c << " ns/item\n";

  cout << "select1: " << flush;
  vector<chrono::nanoseconds::rep> select1;
  for (int r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for(uint64_t i = 0; i < queries; ++i)
      u ^= bv.select(sel1(re) ^ (u & 1));
    end = high_resolution_clock::now();
    select1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
  }
  std::sort(select1.begin(), select1.end());
  cout << select1[MID] * c << " ns/item\n";

  cout << "update: " << flush;
  vector<chrono::nanoseconds::rep> update;
  for (int r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for(uint64_t i = 0; i < queries; ++i) {
      if (i & 1) bv.set(bit(re)  ^ (u & 1));
      else bv.clear(bit(re) ^ (u & 1));
    }
    end = high_resolution_clock::now();
    update.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
  }
  std::sort(update.begin(), update.end());
  cout << update[MID] * c << " ns/item\n";

  const volatile uint64_t __attribute__((unused)) unused = u;
}
