#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

#include <rankselect/rank_select.hpp>
#include <rankselect/word.hpp>
#include <rankselect/line.hpp>

#include <fenwick/fenwick_tree.hpp>
#include <fenwick/simple.hpp>
#include <fenwick/typed.hpp>
#include <fenwick/ityped.hpp>
#include <fenwick/byte.hpp>
#include <fenwick/ibyte.hpp>
#include <fenwick/compact.hpp>
#include <fenwick/shrank.hpp>

#include <dynamic.hpp>

using namespace std;
using namespace dyn;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

template <typename T>
void internal(const char *name, uint64_t *bitvector, uint64_t *rank, uint64_t *select0, uint64_t *select1, size_t size);
void dynamic(const char *name, uint64_t *bitvector, uint64_t *rank, uint64_t *select0, uint64_t *select1, size_t size);


int main(int argc, char *argv[])
{
    mt19937 mte;

    if (argc < 2) {
        std::cerr << "Not enough parameters\n";
        return -1;
    }

    std::istringstream iss(argv[1]);
    size_t size;
    if (!(iss >> size)) {
        std::cerr << "Invalid number " << argv[1] << '\n';
        return -1;
    }

    uniform_int_distribution<uint64_t> dist(0, -1ULL);
    uint64_t *bitvector = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        bitvector[i] = dist(mte);

    uint64_t ones = 0;
    for (size_t i = 0; i < size; i++)
        ones += popcount(bitvector[i]);

    uint64_t zeroes = 64*size - ones;

    uniform_int_distribution<uint64_t> rankdist(0, 64*size);
    uint64_t *rank = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        rank[i] = rankdist(mte);

    uniform_int_distribution<uint64_t> select0dist(0, zeroes-1);
    uint64_t *select0 = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        select0[i] = select0dist(mte);

    uniform_int_distribution<uint64_t> select1dist(0, ones-1);
    uint64_t *select1 = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        select1[i] = select1dist(mte);


    dynamic("DYNAMIC", bitvector, rank, select0, select1, size);

    internal<WordRankSelect<SimpleFenwickTree>> ("WordRankSelect<SimpleFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<WordRankSelect<TypedFenwickTree>>  ("WordRankSelect<TypedFenwickTree>",   bitvector, rank, select0, select1, size);
    internal<WordRankSelect<ITypedFenwickTree>> ("WordRankSelect<ITypedFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<WordRankSelect<ByteFenwickTree>>   ("WordRankSelect<ByteFenwickTree>",    bitvector, rank, select0, select1, size);
    internal<WordRankSelect<IByteFenwickTree>>  ("WordRankSelect<IByteFenwickTree>",   bitvector, rank, select0, select1, size);
    internal<WordRankSelect<CompactFenwickTree>>("WordRankSelect<CompactFenwickTree>", bitvector, rank, select0, select1, size);
    internal<WordRankSelect<ShrankFenwickTree>> ("WordRankSelect<ShrankFenwickTree>",  bitvector, rank, select0, select1, size);

    internal<LineRankSelect<SimpleFenwickTree,  8>>("LineRankSelect<SimpleFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<TypedFenwickTree,   8>>("LineRankSelect<TypedFenwickTree>",   bitvector, rank, select0, select1, size);
    internal<LineRankSelect<ITypedFenwickTree,  8>>("LineRankSelect<ITypedFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<ByteFenwickTree,    8>>("LineRankSelect<ByteFenwickTree>",    bitvector, rank, select0, select1, size);
    internal<LineRankSelect<IByteFenwickTree,   8>>("LineRankSelect<IByteFenwickTree>",   bitvector, rank, select0, select1, size);
    internal<LineRankSelect<CompactFenwickTree, 8>>("LineRankSelect<CompactFenwickTree>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<ShrankFenwickTree,  8>>("LineRankSelect<ShrankFenwickTree>",  bitvector, rank, select0, select1, size);

    internal<LineRankSelect<ByteFenwickTree,   32>>("LineRankSelect<ByteFenwickTree>",    bitvector, rank, select0, select1, size);
    internal<LineRankSelect<IByteFenwickTree,  32>>("LineRankSelect<ByteFenwickTree>",    bitvector, rank, select0, select1, size);

    delete[] bitvector;
    delete[] rank;
    delete[] select0;
    delete[] select1;

    return 0;
}


template <typename T>
void internal(const char *name, uint64_t *bitvector, uint64_t *rank, uint64_t *select0, uint64_t *select1, size_t size)
{
    high_resolution_clock::time_point begin, end;
    uint64_t u = 0;

	begin = high_resolution_clock::now();
	T bv(bitvector, size);
	end = high_resolution_clock::now();
	auto insert = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rankZero(rank[i]);
	end = high_resolution_clock::now();
    auto rank0 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rank(rank[i], 1);
	end = high_resolution_clock::now();
    auto rank1 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.selectZero(select0[i]);
	end = high_resolution_clock::now();
    auto sel0 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.select(select1[i]);
	end = high_resolution_clock::now();
    auto sel1 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / size;
    cout << "\n" << name << ": " << bv.bit_count() / (double)size << " b/item\n";
	cout << "build: " << fixed << setw(12) << insert * c << " ns/item\n";
	cout << "rank0: " << fixed << setw(12) << rank0  * c << " ns/item\n";
	cout << "rank1: " << fixed << setw(12) << rank1  * c << " ns/item\n";
	cout << "sel0:  " << fixed << setw(12) << sel0   * c << " ns/item\n";
	cout << "sel1:  " << fixed << setw(12) << sel1   * c << " ns/item\n";
}


void dynamic(const char *name, uint64_t *bitvector, uint64_t *rank, uint64_t *select0, uint64_t *select1, size_t size)
{
    high_resolution_clock::time_point begin, end;
    uint64_t u = 0;

	begin = high_resolution_clock::now();
	suc_bv bv;
	for (uint64_t i = 0; i < size; ++i) {
        for (uint64_t j = 0; j < 64; ++j)
            bv.insert(64*i + j, bitvector[i] & (1ULL << j));
    }
	end = high_resolution_clock::now();
	auto insert = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rank(rank[i]);
	end = high_resolution_clock::now();
    auto rank0 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rank(rank[i], 1);
	end = high_resolution_clock::now();
    auto rank1 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.select(select0[i], 0);
	end = high_resolution_clock::now();
    auto sel0 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.select(select1[i], 1);
	end = high_resolution_clock::now();
    auto sel1 = duration_cast<std::chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / size;
    cout << "\n" << name << ": " << bv.bit_size() / (double)size << " b/item\n";
	cout << "build: " << fixed << setw(12) << insert * c << " ns/item\n";
	cout << "rank0: " << fixed << setw(12) << rank0  * c << " ns/item\n";
	cout << "rank1: " << fixed << setw(12) << rank1  * c << " ns/item\n";
	cout << "sel0:  " << fixed << setw(12) << sel0   * c << " ns/item\n";
	cout << "sel1:  " << fixed << setw(12) << sel1   * c << " ns/item\n";
}
