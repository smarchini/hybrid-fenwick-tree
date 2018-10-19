#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

#include <rankselect/rank_select.hpp>
#include <rankselect/word.hpp>
#include <rankselect/line.hpp>

#include <fenwick/fenwick_tree.hpp>
#include <fenwick/naive.hpp>
#include <fenwick/lnaive.hpp>
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
void internal(const char *name, uint64_t *bitvector, uint64_t *rank0, uint64_t *rank1, uint64_t *select0, uint64_t *select1, size_t size, size_t queries);
void dynamic(const char *name, uint64_t *bitvector, uint64_t *rank0, uint64_t *rank1, uint64_t *select0, uint64_t *select1, size_t size, size_t queries);

template <size_t N>
using MByteF = MixedFenwickTree<ByteL, TypeF, N, 12>;

int main(int argc, char *argv[])
{
    std::random_device rd;
    mt19937 mte(rd());

    if (argc < 2) {
        cerr << "Not enough parameters\n";
        return -1;
    }

    size_t size, queries;
    if (!(istringstream(argv[1]) >> size) || !(istringstream(argv[2]) >> queries)) {
        cerr << "Invalid parameters\n";
        return -1;
    }

    uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    uint64_t *bitvector = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        bitvector[i] = dist(mte);

    uint64_t ones = 0;
    for (size_t i = 0; i < size; i++)
        ones += popcount(bitvector[i]);

    uint64_t zeroes = 64*size - ones;

    uniform_int_distribution<uint64_t> rankdist(0, 64*size);
    uint64_t *rank0 = new uint64_t[queries], *rank1 = new uint64_t[queries];
    for (size_t i = 0; i < queries; i++) {
        rank0[i] = rankdist(mte);
        rank1[i] = rankdist(mte);
    }

    uniform_int_distribution<uint64_t> select0dist(0, zeroes-1);
    uint64_t *select0 = new uint64_t[queries];
    for (size_t i = 0; i < queries; i++)
        select0[i] = select0dist(mte);

    uniform_int_distribution<uint64_t> select1dist(0, ones-1);
    uint64_t *select1 = new uint64_t[queries];
    for (size_t i = 0; i < queries; i++)
        select1[i] = select1dist(mte);

    cout << "Bitvector with " << ones << " ones and " << zeroes << " zeroes\n" << endl;
    dynamic("DYNAMIC", bitvector, rank0, rank1, select0, select1, size, queries); // temporaneamente disabilitato per velocizzare i benchmark
    cout << "\n------------------------------\n";
    internal<WordRankSelect<FixedF>> ("WordRankSelect<FixedF>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<FixedL>>("WordRankSelect<FixedL>", bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<TypeL>> ("WordRankSelect<TypeL>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<TypeF>>  ("WordRankSelect<TypeF>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<ByteL>> ("WordRankSelect<ByteL>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<ByteF>>  ("WordRankSelect<ByteF>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<BitL>>  ("WordRankSelect<BitL>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<BitF>>   ("WordRankSelect<BitF>",    bitvector, rank0, rank1, select0, select1, size, queries);
    internal<WordRankSelect<MByteF>> ("WordRankSelect<MByteF>",  bitvector, rank0, rank1, select0, select1, size, queries);
    cout << "\n------------------------------\n";
    internal<LineRankSelect<FixedF,  8>>("LineRankSelect<FixedF, 8>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<FixedL, 8>>("LineRankSelect<FixedL, 8>", bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<TypeL,  8>>("LineRankSelect<TypeL, 8>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<TypeF,   8>>("LineRankSelect<TypeF, 8>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<ByteL,  8>>("LineRankSelect<ByteL, 8>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<ByteF,   8>>("LineRankSelect<ByteF, 8>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<BitL,   8>>("LineRankSelect<BitL, 8>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<BitF,    8>>("LineRankSelect<BitF, 8>",    bitvector, rank0, rank1, select0, select1, size, queries);
    cout << "\n------------------------------\n";
    internal<LineRankSelect<FixedF,  32>>("LineRankSelect<FixedF, 32>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<FixedL, 32>>("LineRankSelect<FixedL, 32>", bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<TypeL,  32>>("LineRankSelect<TypeL, 32>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<TypeF,   32>>("LineRankSelect<TypeF, 32>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<ByteL,  32>>("LineRankSelect<ByteL, 32>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<ByteF,   32>>("LineRankSelect<ByteF, 32>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<BitL,   32>>("LineRankSelect<BitL, 32>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<BitF,    32>>("LineRankSelect<BitF, 32>",    bitvector, rank0, rank1, select0, select1, size, queries);
    cout << "\n------------------------------\n";
    internal<LineRankSelect<FixedF,  40>>("LineRankSelect<FixedF, 40>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<FixedL, 40>>("LineRankSelect<FixedL, 40>", bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<TypeL,  40>>("LineRankSelect<TypeL, 40>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<TypeF,   40>>("LineRankSelect<TypeF, 40>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<ByteL,  40>>("LineRankSelect<ByteL, 40>",  bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<ByteF,   40>>("LineRankSelect<ByteF, 40>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<BitL,   40>>("LineRankSelect<BitL, 40>",   bitvector, rank0, rank1, select0, select1, size, queries);
    internal<LineRankSelect<BitF,    40>>("LineRankSelect<BitF, 40>",    bitvector, rank0, rank1, select0, select1, size, queries);

    delete[] bitvector;
    delete[] rank0;
    delete[] rank1;
    delete[] select0;
    delete[] select1;

    return 0;
}


template <typename T>
void internal(const char *name, uint64_t *bitvector, uint64_t *rank0, uint64_t *rank1, uint64_t *select0, uint64_t *select1, size_t size, size_t queries)
{
    high_resolution_clock::time_point begin, end;
    uint64_t u = 0;

    begin = high_resolution_clock::now();
    T bv(bitvector, size);
    end = high_resolution_clock::now();
    auto insert = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.rankZero(rank0[i]);
    // end = high_resolution_clock::now();
    // auto ran0 = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.selectZero(select0[i]);
    // end = high_resolution_clock::now();
    // auto sel0 = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.rank(rank1[i]);
    // end = high_resolution_clock::now();
    // auto ran1 = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.select(select1[i]);
    // end = high_resolution_clock::now();
    // auto sel1 = duration_cast<chrono::nanoseconds>(end-begin).count();


    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / queries;
    cout << "\n" << name << ": " << bv.bit_count() / (double)size << " b/item\n";
    // cout << "build: " << fixed << setw(12) << insert / (double)size << " ns/item\n";
    // cout << "rank0: " << fixed << setw(12) << ran0 * c << " ns/item\n";
    // cout << "rank1: " << fixed << setw(12) << ran1 * c << " ns/item\n";
    // cout << "sel0:  " << fixed << setw(12) << sel0 * c << " ns/item\n";
    // cout << "sel1:  " << fixed << setw(12) << sel1 * c << " ns/item\n";
}


void dynamic(const char *name, uint64_t *bitvector, uint64_t *rank0, uint64_t *rank1, uint64_t *select0, uint64_t *select1, size_t size, size_t queries)
{
    high_resolution_clock::time_point begin, end;
    uint64_t u = 0;

    cout << "Construction... " << flush;
    begin = high_resolution_clock::now();
    suc_bv bv;
    for (uint64_t i = 0; i < size; ++i) {
        for (uint64_t j = 0; j < 64; ++j)
            bv.insert(64*i + j, bitvector[i] & (1ULL << j));
    }
    end = high_resolution_clock::now();
    cout << "finished.\n" << flush;

    // auto insert = duration_cast<chrono::nanoseconds>(end-begin).count();
    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.rank(rank0[i], 0);
    // end = high_resolution_clock::now();
    // auto ran0 = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.select(select0[i], 0);
    // end = high_resolution_clock::now();
    // auto sel0 = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.rank(rank1[i], 1);
    // end = high_resolution_clock::now();
    // auto ran1 = duration_cast<chrono::nanoseconds>(end-begin).count();

    // begin = high_resolution_clock::now();
    // for(uint64_t i = 0; i < queries; ++i)
    //     u ^= bv.select(select1[i], 1);
    // end = high_resolution_clock::now();
    // auto sel1 = duration_cast<chrono::nanoseconds>(end-begin).count();


    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / queries;
    cout << "\n" << name << ": " << bv.bit_size() / (double)size << " b/item\n";
    // cout << "build: " << fixed << setw(12) << insert * c << " ns/item\n";
    // cout << "rank0: " << fixed << setw(12) << ran0 * c << " ns/item\n";
    // cout << "rank1: " << fixed << setw(12) << ran1 * c << " ns/item\n";
    // cout << "sel0:  " << fixed << setw(12) << sel0 * c << " ns/item\n";
    // cout << "sel1:  " << fixed << setw(12) << sel1 * c << " ns/item\n";
}
