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

template <typename dynft> size_t nostro(const uint64_t n, const uint64_t d) {
	assert(n >= d);
	uint64_t *v= new uint64_t[d];
	for(int i = 0; i < d; i++) v[i] = d - 1;
	dynft f(v, n * d);
	uint64_t sum_degrees = d * (d - 1);

	for(uint64_t i = d; i < n; i++) {
		for(j = 0; j < d; j++) {
			uint64_t x = f.find(random() % sum_degrees) - 1; // New vertex to connect to
			assert(x >= 0);
			assert(x < i);

			sum_degrees++;
			f.add(x, 1);
		}
		f.push(i, d);
		sum_degrees += d;
	}
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

    nostro<ranking::Word<fenwick::FixedF>>(m, k, bitlen);
    nostro<ranking::Stride<fenwick::FixedF, 2>>(m, k, bitlen);
    nostro<ranking::Stride<fenwick::FixedF, 4>>(m, k, bitlen);
    nostro<ranking::Stride<fenwick::FixedF, 8>>(m, k, bitlen);
    nostro<ranking::Stride<fenwick::FixedF, 16>>(m, k, bitlen);
    prezza(m, k, bitlen);

    cout << endl;
  }

  return 0;
}
