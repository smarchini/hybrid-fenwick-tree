#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

#include <rankselect/rank_select.hpp>
#include <rankselect/word.hpp>
#include <rankselect/stride.hpp>

#include <fenwick/bitf.hpp>
#include <fenwick/bytef.hpp>
#include <fenwick/fenwick_tree.hpp>
#include <fenwick/bitl.hpp>
#include <fenwick/bytel.hpp>
#include <fenwick/fixedl.hpp>
#include <fenwick/typel.hpp>
#include <fenwick/hybrid.hpp>
#include <fenwick/fixedf.hpp>
#include <fenwick/typef.hpp>

#include <dynamic.hpp>

using namespace std;
using namespace hft;
using namespace hft::fenwick;
using namespace hft::ranking;
using namespace std::chrono;


// The size of a bottom tree is enough for a single 4kB memory page (1 word)
template <size_t N> using Fixed9Fixed = Hybrid<FixedL, FixedF, N, 9>;
template <size_t N> using Fixed12Byte = Hybrid<FixedL, ByteF, N, 12>;
template <size_t N> using Fixed12Bit = Hybrid<FixedL, BitF, N, 12>;
template <size_t N> using Byte12Byte = Hybrid<ByteL, ByteF, N, 12>;
template <size_t N> using Byte12Bit = Hybrid<ByteL, BitF, N, 12>;
template <size_t N> using Bit12Bit = Hybrid<BitL, BitF, N, 12>;

// The size of a bottom tree is enough for a single 4kB memory page (8, 16, 24 and 32 words)
// template <size_t N> using Fixed9Fixed = Hybrid<FixedL, FixedF, N, 9>; // (defined above)
// template <size_t N> using Fixed12Byte = Hybrid<FixedL, ByteF, N, 12>; // (defined above)
template <size_t N> using Fixed11Bit = Hybrid<FixedL, BitF, N, 11>;
// template <size_t N> using Byte12Byte = Hybrid<ByteL, ByteF, N, 12>;   // (defined above)
template <size_t N> using Byte11Bit = Hybrid<ByteL, BitF, N, 11>;
template <size_t N> using Bit11Bit = Hybrid<BitL, BitF, N, 11>;

// The size of a bottom tree is enough for the 32kB LL1 cache memory (1 word)
template <size_t N> using Fixed12Fixed = Hybrid<FixedL, FixedF, N, 12>;
template <size_t N> using Fixed15Byte = Hybrid<FixedL, ByteF, N, 15>;
template <size_t N> using Fixed15Bit = Hybrid<FixedL, BitF, N, 15>;
template <size_t N> using Byte15Byte = Hybrid<ByteL, ByteF, N, 15>;
template <size_t N> using Byte15Bit = Hybrid<ByteL, BitF, N, 15>;
template <size_t N> using Bit15Bit = Hybrid<BitL, BitF, N, 15>;

// The size of a bottom tree is enough for the 32kB LL1 cache memory (8, 16, 24 and 32 words)
// template <size_t N> using Fixed12Fixed = Hybrid<FixedL, FixedF, N, 12>; // (defined above)
// template <size_t N> using Fixed15Byte = Hybrid<FixedL, ByteF, N, 15>;   // (defined above)
template <size_t N> using Fixed14Bit = Hybrid<FixedL, BitF, N, 14>;
// template <size_t N> using Byte15Byte = Hybrid<ByteL, ByteF, N, 15>;     // (defined above)
template <size_t N> using Byte14Bit = Hybrid<ByteL, BitF, N, 14>;
template <size_t N> using Bit14Bit = Hybrid<BitL, BitF, N, 14>;


class Benchmark {
private:
    string path;
    size_t size, queries;

    uint64_t ones = 0, zeroes = 0;

    mt19937 mte;
    unique_ptr<uint64_t[]> bitvector = make_unique<uint64_t[]>(size);
    uniform_int_distribution<uint64_t> bvdist, sel0dist, sel1dist;
    uniform_int_distribution<size_t> idxdist, bitdist;

    ofstream fbuild, frank0, frank1, fselect0, fselect1, fupdate, fbitspace;

public:
    Benchmark(string path, size_t size, size_t queries) :
        path(path),
        size(size),
        queries(queries) {}

    void filesinit(string order) {
        finit(fbuild,    "build.csv",    "Elements," + order);
        finit(frank0,    "rank0.csv",    "Elements," + order);
        finit(frank1,    "rank1.csv",    "Elements," + order);
        finit(fselect0,  "select0.csv",  "Elements," + order);
        finit(fselect1,  "select1.csv",  "Elements," + order);
        finit(fupdate,   "update.csv",   "Elements," + order);
        finit(fbitspace, "bitspace.csv", "Elements," + order);
    }

    void separator(string sep = ",") {
        fbuild << sep;
        frank0 << sep;
        frank1 << sep;
        fselect0 << sep;
        fselect1 << sep;
        fupdate << sep;
        fbitspace << sep;
    }

    void datainit(mt19937 &engine) {
        mte = engine;
        bvdist = uniform_int_distribution<uint64_t>(0, UINT64_MAX);
        idxdist = uniform_int_distribution<size_t>(0, size-1);
        bitdist = uniform_int_distribution<size_t>(0, (size-1)*64);

        for (size_t i = 0; i < size; i++) {
            bitvector[i] = bvdist(mte);
            ones += popcount(bitvector[i]);
        }

        zeroes = 64*size - ones;

        sel0dist = uniform_int_distribution<uint64_t>(0, zeroes-1);
        sel1dist = uniform_int_distribution<uint64_t>(0, ones-1);
    }

    template<typename T>
    void run() {
        const double c = 1. / queries;
        high_resolution_clock::time_point begin, end;
        uint64_t u = 0;

        cout << "Constructor... " << flush;
        begin = high_resolution_clock::now();
        T bv(bitvector.get(), size);
        end = high_resolution_clock::now();
        auto build = duration_cast<chrono::nanoseconds>(end-begin).count();
        fbuild << to_string(build / (double)size);


        constexpr int REPS = 5;
        constexpr size_t MID = 2; // index of the median of a REPS elements sorted vector

        cout << "rank0: " << flush;
        vector<chrono::nanoseconds::rep> rank0;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= bv.rankZero(idxdist(mte));
            end = high_resolution_clock::now();
            rank0.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(rank0.begin(), rank0.end());
        frank0 << to_string(rank0[MID] * c);

        cout << "select0: " << flush;
        vector<chrono::nanoseconds::rep> select0;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= bv.selectZero(sel0dist(mte));
            end = high_resolution_clock::now();
            select0.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(select0.begin(), select0.end());
        fselect0 << to_string(select0[MID] * c);

        cout << "rank1: " << flush;
        vector<chrono::nanoseconds::rep> rank1;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= bv.rank(idxdist(mte));
            end = high_resolution_clock::now();
            rank1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(rank1.begin(), rank1.end());
        frank1 << to_string(rank1[MID] * c);

        cout << "select1: " << flush;
        vector<chrono::nanoseconds::rep> select1;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= bv.select(sel1dist(mte));
            end = high_resolution_clock::now();
            select1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(select1.begin(), select1.end());
        fselect1 << to_string(select1[MID] * c);

        // cout << "update: " << flush;
        // vector<chrono::nanoseconds::rep> update;
        // for (int r = 0; r < REPS; r++) {
        //     cout << r << " " << flush;
        //     begin = high_resolution_clock::now();
        //     for(uint64_t i = 0; i < queries; ++i) {
        //         if (i & 1) u ^= bv.set(bitdist(mte));
        //         else u ^= bv.clear(bitdist(mte));
        //     }
        //     end = high_resolution_clock::now();
        //     update.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        // }
        // std::sort(update.begin(), update.end());
        // fupdate << to_string(update[MID] * c);

        cout << "bitspace... " << flush;
        fbitspace << to_string(bv.bitCount() / (size * 64.));
        cout << "done.  " << endl;

        const volatile uint64_t __attribute__((unused)) unused = u;
    }

    void run_dynamic() {
        dyn::suc_bv dynamic;

        const double c = 1. / queries;
        high_resolution_clock::time_point begin, end;
        uint64_t u = 0;

        cout << "Constructor... " << flush;
        begin = high_resolution_clock::now();
        for (uint64_t i = 0; i < size; ++i) {
            for (uint64_t j = 0; j < 64; ++j)
                dynamic.insert(64*i + j, bitvector.get()[i] & (1ULL << j));
        }
        end = high_resolution_clock::now();
        auto build = duration_cast<chrono::nanoseconds>(end-begin).count();
        fbuild << to_string(build / (double)size);

        constexpr int REPS = 5;
        constexpr size_t MID = 2; // index of the median of a REPS elements sorted vector

        cout << "rank0: " << flush;
        vector<chrono::nanoseconds::rep> rank0;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= dynamic.rank0(idxdist(mte));
            end = high_resolution_clock::now();
            rank0.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(rank0.begin(), rank0.end());
        frank0 << to_string(rank0[MID] * c);

        cout << "select0: " << flush;
        vector<chrono::nanoseconds::rep> select0;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= dynamic.select0(sel0dist(mte));
            end = high_resolution_clock::now();
            select0.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort (select0.begin(), select0.end());
        fselect0 << to_string(select0[MID] * c);

        cout << "rank1: " << flush;
        vector<chrono::nanoseconds::rep> rank1;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= dynamic.rank1(sel0dist(mte));
            end = high_resolution_clock::now();
            rank1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(rank1.begin(), rank1.end());
        frank1 << to_string(rank1[MID] * c);

        cout << "select1: " << flush;
        vector<chrono::nanoseconds::rep> select1;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= dynamic.select1(sel1dist(mte));
            end = high_resolution_clock::now();
            select1.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(select1.begin(), select1.end());
        fselect1 << to_string(select1[MID] * c);

        cout << "update: " << flush;
        vector<chrono::nanoseconds::rep> update;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i) {
                if (i & 1) dynamic.set(bitdist(mte), true);
                else dynamic.set(bitdist(mte), false);
            }
            end = high_resolution_clock::now();
            update.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(update.begin(), update.end());
        fupdate << to_string(update[MID] * c);

        cout << "bitspace... " << flush;
        fbitspace << to_string(dynamic.bit_size() / (size * 64.));
        cout << "done.  " << endl;

        const volatile uint64_t __attribute__((unused)) unused = u;
    }

private:
    bool is_empty(ifstream file) {
        return file.peek() == ifstream::traits_type::eof();
    }

    void finit(ofstream &file, string filename, string text) {
        file.open(path + filename, ios::out | ios::app);

        if (is_empty(ifstream(path + filename))) {
            cout << "Initializing: " << path << filename << endl;
            file << text << endl;
        }

        file << size*64 << ",";
    }
};


int main(int argc, char *argv[])
{
    random_device rd;
    mt19937 mte(rd());

    size_t size, queries;
    if (argc < 3 || !(istringstream(argv[2]) >> size) || !(istringstream(argv[3]) >> queries)) {
        cerr << "Invalid parameters" << endl;
        return -1;
    }

    Benchmark bench(argv[1], size, queries);

    bench.datainit(mte);

    bench.filesinit("fixed[F]1,fixed[$\\ell$]1,byte[F]1,byte[$\\ell$]1,bit[F]1,bit[$\\ell$]1,"
                    "fixed[$9$]fixed1,fixed[$12$]byte1,fixed[$12$]bit1,byte[$12$]byte1,byte[$12$]bit1,bit[$12$]bit1,"
                    "fixed[$12$]fixed1,fixed[$15$]byte1,fixed[$15$]bit1,byte[$15$]byte1,byte[$15$]bit1,bit[$15$]bit1,"

                    "fixed[F]8,fixed[$\\ell$]8,byte[F]8,byte[$\\ell$]8,bit[F]8,bit[$\\ell$]8,"
                    "fixed[$9$]fixed8,fixed[$12$]byte8,fixed[$11$]bit8,byte[$12$]byte8,byte[$11$]bit8,bit[$11$]bit8,"
                    "fixed[$12$]fixed8,fixed[$15$]byte8,fixed[$14$]bit8,byte[$15$]byte8,byte[$14$]bit8,bit[$14$]bit8,"

                    "fixed[F]16,fixed[$\\ell$]16,byte[F]16,byte[$\\ell$]16,bit[F]16,bit[$\\ell$]16,"
                    "fixed[$9$]fixed16,fixed[$12$]byte16,fixed[$11$]bit16,byte[$12$]byte16,byte[$11$]bit16,bit[$11$]bit16,"
                    "fixed[$12$]fixed16,fixed[$15$]byte16,fixed[$14$]bit16,byte[$15$]byte16,byte[$14$]bit16,bit[$14$]bit16,"

                    "fixed[F]24,fixed[$\\ell$]24,byte[F]24,byte[$\\ell$]24,bit[F]24,bit[$\\ell$]24,"
                    "fixed[$9$]fixed24,fixed[$12$]byte24,fixed[$11$]bit24,byte[$12$]byte24,byte[$11$]bit24,bit[$11$]bit24,"
                    "fixed[$12$]fixed24,fixed[$15$]byte24,fixed[$14$]bit24,byte[$15$]byte24,byte[$14$]bit24,bit[$14$]bit24,"

                    "fixed[F]32,fixed[$\\ell$]32,byte[F]32,byte[$\\ell$]32,bit[F]32,bit[$\\ell$]32,"
                    "fixed[$9$]fixed32,fixed[$12$]byte32,fixed[$11$]bit32,byte[$12$]byte32,byte[$11$]bit32,bit[$11$]bit32,"
                    "fixed[$12$]fixed32,fixed[$15$]byte32,fixed[$14$]bit32,byte[$15$]byte32,byte[$14$]bit32,bit[$14$]bit32"); //,Prezza


    cout << "size = " << size << ", queries = " << queries << " => fixed[F]1:       "; bench.run<Word<FixedF>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]1:       "; bench.run<Word<FixedL>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]1:        "; bench.run<Word<ByteF>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]1:        "; bench.run<Word<ByteL>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]1:         "; bench.run<Word<BitF>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]1:         "; bench.run<Word<BitL>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed1:  "; bench.run<Word<Fixed9Fixed>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]byte1:  "; bench.run<Word<Fixed12Byte>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]bit1:   "; bench.run<Word<Fixed12Bit>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]byte1:   "; bench.run<Word<Byte12Byte>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]bit1:    "; bench.run<Word<Byte12Bit>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[12]bit1:     "; bench.run<Word<Bit12Bit>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]fixed1: "; bench.run<Word<Fixed12Fixed>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]byte1:  "; bench.run<Word<Fixed15Byte>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]bit1:   "; bench.run<Word<Fixed15Bit>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]byte1:   "; bench.run<Word<Byte15Byte>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]bit1:    "; bench.run<Word<Byte15Bit>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[15]bit1:     "; bench.run<Word<Bit15Bit>>(); bench.separator();


    cout << "size = " << size << ", queries = " << queries << " => fixed[F]8:       "; bench.run<Stride<FixedF, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]8:       "; bench.run<Stride<FixedL, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]8:        "; bench.run<Stride<ByteF, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]8:        "; bench.run<Stride<ByteL, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]8:         "; bench.run<Stride<BitF, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]8:         "; bench.run<Stride<BitL, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed8:  "; bench.run<Stride<Fixed9Fixed, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]byte8:  "; bench.run<Stride<Fixed12Byte, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[11]bit8:   "; bench.run<Stride<Fixed11Bit, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]byte8:   "; bench.run<Stride<Byte12Byte, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[11]bit8:    "; bench.run<Stride<Byte11Bit, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[11]bit8:     "; bench.run<Stride<Bit11Bit, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]fixed8: "; bench.run<Stride<Fixed12Fixed, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]byte8:  "; bench.run<Stride<Fixed15Byte, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[14]bit8:   "; bench.run<Stride<Fixed14Bit, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]byte8:   "; bench.run<Stride<Byte15Byte, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[14]bit8:    "; bench.run<Stride<Byte14Bit, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[14]bit8:     "; bench.run<Stride<Bit14Bit, 8>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]16:       "; bench.run<Stride<FixedF, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]16:       "; bench.run<Stride<FixedL, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]16:        "; bench.run<Stride<ByteF, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]16:        "; bench.run<Stride<ByteL, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]16:         "; bench.run<Stride<BitF, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]16:         "; bench.run<Stride<BitL, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed16:  "; bench.run<Stride<Fixed9Fixed, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]byte16:  "; bench.run<Stride<Fixed12Byte, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[11]bit16:   "; bench.run<Stride<Fixed11Bit, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]byte16:   "; bench.run<Stride<Byte12Byte, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[11]bit16:    "; bench.run<Stride<Byte11Bit, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[11]bit16:     "; bench.run<Stride<Bit11Bit, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]fixed16: "; bench.run<Stride<Fixed12Fixed, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]byte16:  "; bench.run<Stride<Fixed15Byte, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[14]bit16:   "; bench.run<Stride<Fixed14Bit, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]byte16:   "; bench.run<Stride<Byte15Byte, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[14]bit16:    "; bench.run<Stride<Byte14Bit, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[14]bit16:     "; bench.run<Stride<Bit14Bit, 16>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]24:       "; bench.run<Stride<FixedF, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]24:       "; bench.run<Stride<FixedL, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]24:        "; bench.run<Stride<ByteF, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]24:        "; bench.run<Stride<ByteL, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]24:         "; bench.run<Stride<BitF, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]24:         "; bench.run<Stride<BitL, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed24:  "; bench.run<Stride<Fixed9Fixed, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]byte24:  "; bench.run<Stride<Fixed12Byte, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[11]bit24:   "; bench.run<Stride<Fixed11Bit, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]byte24:   "; bench.run<Stride<Byte12Byte, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[11]bit24:    "; bench.run<Stride<Byte11Bit, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[11]bit24:     "; bench.run<Stride<Bit11Bit, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]fixed24: "; bench.run<Stride<Fixed12Fixed, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]byte24:  "; bench.run<Stride<Fixed15Byte, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[14]bit24:   "; bench.run<Stride<Fixed14Bit, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]byte24:   "; bench.run<Stride<Byte15Byte, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[14]bit24:    "; bench.run<Stride<Byte14Bit, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[14]bit24:     "; bench.run<Stride<Bit14Bit, 24>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]32:       "; bench.run<Stride<FixedF, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]32:       "; bench.run<Stride<FixedL, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]32:        "; bench.run<Stride<ByteF, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]32:        "; bench.run<Stride<ByteL, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]32:         "; bench.run<Stride<BitF, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]32:         "; bench.run<Stride<BitL, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed32:  "; bench.run<Stride<Fixed9Fixed, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]byte32:  "; bench.run<Stride<Fixed12Byte, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[11]bit32:   "; bench.run<Stride<Fixed11Bit, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]byte32:   "; bench.run<Stride<Byte12Byte, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[11]bit32:    "; bench.run<Stride<Byte11Bit, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[11]bit32:     "; bench.run<Stride<Bit11Bit, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]fixed32: "; bench.run<Stride<Fixed12Fixed, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]byte32:  "; bench.run<Stride<Fixed15Byte, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[14]bit32:   "; bench.run<Stride<Fixed14Bit, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]byte32:   "; bench.run<Stride<Byte15Byte, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[14]bit32:    "; bench.run<Stride<Byte14Bit, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[14]bit32:     "; bench.run<Stride<Bit14Bit, 32>>(); bench.separator("\n");

    // cout << "Prezza:          "; bench.run_dynamic();                    bench.separator("\n");

    return 0;
}
