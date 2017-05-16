#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

#include <rankselect/rank_select.hpp>
#include <rankselect/word.hpp>
#include <rankselect/line.hpp>

#include <fenwick/fenwick_tree.hpp>
#include <fenwick/naive.hpp>
#include <fenwick/ltype.hpp>
#include <fenwick/type.hpp>
#include <fenwick/lbyte.hpp>
#include <fenwick/byte.hpp>
#include <fenwick/lbit.hpp>
#include <fenwick/bit.hpp>
#include <fenwick/mixed.hpp>

#include <dynamic.hpp>

using namespace std;
using namespace dyn;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

template <typename T>
void internal(const char *name, uint64_t *bitvector, uint64_t *rank, uint64_t *select0, uint64_t *select1, size_t size);
void dynamic(const char *name, uint64_t *bitvector, uint64_t *rank, uint64_t *select0, uint64_t *select1, size_t size);


template <size_t N>
using MByteFenwickTree = MixedFenwickTree<LByteFenwickTree, ByteFenwickTree, N, 256>;


int main(int argc, char *argv[])
{
    mt19937 mte;

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

    cout << "Bitvector with " << ones << " ones and " << zeroes << " zeroes\n" << endl;
    //dynamic("DYNAMIC", bitvector, rank, select0, select1, size); // temporaneamente disabilitato per velocizzare i benchmark
    //cout << "\n------------------------------\n";
    internal<WordRankSelect<NaiveFenwickTree>>("WordRankSelect<NaiveFenwickTree>", bitvector, rank, select0, select1, size);
    internal<WordRankSelect<LTypeFenwickTree>>("WordRankSelect<LTypeFenwickTree>", bitvector, rank, select0, select1, size);
    internal<WordRankSelect<TypeFenwickTree>> ("WordRankSelect<TypeFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<WordRankSelect<LByteFenwickTree>>("WordRankSelect<LByteFenwickTree>", bitvector, rank, select0, select1, size);
    internal<WordRankSelect<ByteFenwickTree>> ("WordRankSelect<ByteFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<WordRankSelect<LBitFenwickTree>> ("WordRankSelect<LBitFenwickTree>",  bitvector, rank, select0, select1, size);
    internal<WordRankSelect<BitFenwickTree>>  ("WordRankSelect<BitFenwickTree>",   bitvector, rank, select0, select1, size);
    internal<WordRankSelect<MByteFenwickTree>>("WordRankSelect<MByteFenwickTree>",   bitvector, rank, select0, select1, size);
    cout << "\n------------------------------\n";
    internal<LineRankSelect<NaiveFenwickTree, 8>>("LineRankSelect<NaiveFenwickTree, 8>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LTypeFenwickTree, 8>>("LineRankSelect<LTypeFenwickTree, 8>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<TypeFenwickTree,  8>>("LineRankSelect<TypeFenwickTree, 8>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LByteFenwickTree, 8>>("LineRankSelect<LByteFenwickTree, 8>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<ByteFenwickTree,  8>>("LineRankSelect<ByteFenwickTree, 8>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LBitFenwickTree,  8>>("LineRankSelect<LBitFenwickTree, 8>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<BitFenwickTree,   8>>("LineRankSelect<BitFenwickTree, 8>",   bitvector, rank, select0, select1, size);
    cout << "\n------------------------------\n";
    internal<LineRankSelect<NaiveFenwickTree, 32>>("LineRankSelect<NaiveFenwickTree, 32>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LTypeFenwickTree, 32>>("LineRankSelect<LTypeFenwickTree, 32>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<TypeFenwickTree,  32>>("LineRankSelect<TypeFenwickTree, 32>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LByteFenwickTree, 32>>("LineRankSelect<LByteFenwickTree, 32>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<ByteFenwickTree,  32>>("LineRankSelect<ByteFenwickTree, 32>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LBitFenwickTree,  32>>("LineRankSelect<LBitFenwickTree, 32>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<BitFenwickTree,   32>>("LineRankSelect<BitFenwickTree, 32>",   bitvector, rank, select0, select1, size);
    cout << "\n------------------------------\n";
    internal<LineRankSelect<NaiveFenwickTree, 40>>("LineRankSelect<NaiveFenwickTree, 40>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LTypeFenwickTree, 40>>("LineRankSelect<LTypeFenwickTree, 40>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<TypeFenwickTree,  40>>("LineRankSelect<TypeFenwickTree, 40>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LByteFenwickTree, 40>>("LineRankSelect<LByteFenwickTree, 40>", bitvector, rank, select0, select1, size);
    internal<LineRankSelect<ByteFenwickTree,  40>>("LineRankSelect<ByteFenwickTree, 40>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<LBitFenwickTree,  40>>("LineRankSelect<LBitFenwickTree, 40>",  bitvector, rank, select0, select1, size);
    internal<LineRankSelect<BitFenwickTree,   40>>("LineRankSelect<BitFenwickTree, 40>",   bitvector, rank, select0, select1, size);

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
	auto insert = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rankZero(rank[i]);
	end = high_resolution_clock::now();
    auto rank0 = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rank(rank[i], 1);
	end = high_resolution_clock::now();
    auto rank1 = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.select(select1[i]);
	end = high_resolution_clock::now();
    auto sel1 = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.selectZero(select0[i]);
	end = high_resolution_clock::now();
    auto sel0 = duration_cast<chrono::nanoseconds>(end-begin).count();

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
	auto insert = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rank(rank[i]);
	end = high_resolution_clock::now();
    auto rank0 = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.rank(rank[i], 1);
	end = high_resolution_clock::now();
    auto rank1 = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.select(select1[i], 1);
	end = high_resolution_clock::now();
    auto sel1 = duration_cast<chrono::nanoseconds>(end-begin).count();

	begin = high_resolution_clock::now();
	for(uint64_t i = 0; i < size; ++i)
		u ^= bv.select(select0[i], 0);
	end = high_resolution_clock::now();
    auto sel0 = duration_cast<chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / size;
    cout << "\n" << name << ": " << bv.bit_size() / (double)size << " b/item\n";
	cout << "build: " << fixed << setw(12) << insert * c << " ns/item\n";
	cout << "rank0: " << fixed << setw(12) << rank0  * c << " ns/item\n";
	cout << "rank1: " << fixed << setw(12) << rank1  * c << " ns/item\n";
	cout << "sel0:  " << fixed << setw(12) << sel0   * c << " ns/item\n";
	cout << "sel1:  " << fixed << setw(12) << sel1   * c << " ns/item\n";
}
