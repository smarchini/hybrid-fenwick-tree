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
#include <fenwick/hybrid.hpp>
#include <fenwick/typef.hpp>
#include <fenwick/typel.hpp>

#include <dynamic.hpp>

#define CSV 0

#ifdef CSV
#else
static constexpr string BITS = " bits";
static constexpr string SEP = " \t";
static constexpr string TIME = " ns/item";
#endif

using namespace std;
using namespace hft;
using namespace std::chrono;

inline uint64_t rotl(uint64_t x, size_t k) { return ((x << 8) & (1ULL << k) - 1) | (x >> k - 8); }

template <typename dynbv> size_t nostro(const uint64_t m, const uint64_t k, const uint64_t bitlen) {
  const size_t mask = (1ULL << bitlen) - 1;
  const double c = 1. / mask;
  size_t d = 0, p = 0;

  dynbv b(DArray<uint64_t>(mask / 64 + 1), mask / 64 + 1);
  cout << "," << b.bitCount() * c << "," << flush;

  auto begin = high_resolution_clock::now();
  for (size_t i = 0; i < mask; i++) {
    p = (p * m + k) & mask;                  // Actually p = sigmainv[rho[i]]; (rho is the identity)
    p = (rotl(p, bitlen) * 0x9E377B) & mask; // Mitigate the power of 2 LCG problems on lower bits
    d += b.rankZero(p);
    b.set(p);
  }
  auto end = high_resolution_clock::now();
  auto elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed * c << flush;

  return d;
}

size_t prezza(const uint64_t m, const uint64_t k, const uint64_t bitlen) {
  const size_t mask = (1ULL << bitlen) - 1;
  const double c = 1. / mask;
  size_t d = 0, p = 0;

  dyn::suc_bv b;
  for (size_t i = 0; i < mask; i++)
    b.push_back(0);
  cout << "," << b.bit_size() * c << "," << flush;

  auto begin = high_resolution_clock::now();
  for (size_t i = 0; i < bitlen; i++) {
    p = (p * m + k) & mask;                  // Actually p = sigmainv[rho[i]]; (rho is the identity)
    p = (rotl(p, bitlen) * 0x9E377B) & mask; // Mitigate the power of 2 LCG problems on lower bits
    d += b.rank0(p);
    b.set(p);
  }
  auto end = high_resolution_clock::now();
  auto elapsed = duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << elapsed * c << flush;

  return d;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Invalid parameters: filepath" << endl;
    return -1;
  }

  ifstream file(argv[1]);
  if (!file) {
    cerr << "Invalid parameters: can't open the file" << endl;
    return -1;
  }

  uint64_t bitlen, m, k;
  cout << "Length,fixed[F]1S,fixed[F]1,fixed[F]2S,fixed[F]2,fixed[F]4S,fixed[F]4,fixed[F]8S,fixed["
          "F]8,fixed[F]16S,fixed[F]16,prezzaS,prezza\n";

  while (file >> bitlen >> hex >> m >> k) {
    cout << bitlen << flush;

    cout << (uint64_t) nostro<ranking::Word<fenwick::FixedF>>(m, k, bitlen) << ",";
    cout << (uint64_t) nostro<ranking::Stride<fenwick::FixedF, 2>>(m, k, bitlen) << ",";
    cout << (uint64_t) nostro<ranking::Stride<fenwick::FixedF, 4>>(m, k, bitlen) << ",";
    cout << (uint64_t) nostro<ranking::Stride<fenwick::FixedF, 8>>(m, k, bitlen) << ",";
    cout << (uint64_t) nostro<ranking::Stride<fenwick::FixedF, 16>>(m, k, bitlen) << ",";
    cout << (uint64_t) prezza(m, k, bitlen) << ",";

    cout << endl;
  }

  return 0;
}
