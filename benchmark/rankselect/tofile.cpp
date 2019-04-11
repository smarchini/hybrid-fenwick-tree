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

template <size_t N> using Fixed26Byte = Hybrid<FixedL, ByteF, N, 26>;
template <size_t N> using Fixed26Bit = Hybrid<FixedL, BitF, N, 26>;
template <size_t N> using Fixed32Byte = Hybrid<FixedL, ByteF, N, 32>;
template <size_t N> using Fixed32Bit = Hybrid<FixedL, BitF, N, 32>;

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


    bench.filesinit("fixed[F]1,fixed[$\\ell$]1,byte[F]1,byte[$\\ell$]1,bit[F]1,bit[$\\ell$]1,fixed[26]byte1,fixed[26]bit1,fixed[32]byte1,fixed[32]bit1,"
                    "fixed[F]8,fixed[$\\ell$]8,byte[F]8,byte[$\\ell$]8,bit[F]8,bit[$\\ell$]8,fixed[26]byte8,fixed[26]bit8,fixed[32]byte8,fixed[32]bit8,"
                    "fixed[F]16,fixed[$\\ell$]16,byte[F]16,byte[$\\ell$]16,bit[F]16,bit[$\\ell$]16,fixed[26]byte16,fixed[26]bit16,fixed[32]byte16,fixed[32]bit16,"
                    "fixed[F]24,fixed[$\\ell$]24,byte[F]24,byte[$\\ell$]24,bit[F]24,bit[$\\ell$]24,fixed[26]byte24,fixed[26]bit24,fixed[32]byte24,fixed[32]bit24,"
                    "fixed[F]32,fixed[$\\ell$]32,byte[F]32,byte[$\\ell$]32,bit[F]32,bit[$\\ell$]32,fixed[26]byte32,fixed[26]bit32,fixed[32]byte32,fixed[32]bit32");
    // Prezza

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]1:       "; bench.run<Word<FixedF>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]1:       "; bench.run<Word<FixedL>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]1:        "; bench.run<Word<ByteF>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]1:        "; bench.run<Word<ByteL>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]1:         "; bench.run<Word<BitF>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]1:         "; bench.run<Word<BitL>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]byte1:  "; bench.run<Word<Fixed26Byte>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]bit1:   "; bench.run<Word<Fixed26Bit>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]byte1:  "; bench.run<Word<Fixed32Byte>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]bit1:   "; bench.run<Word<Fixed32Bit>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]8:       "; bench.run<Stride<FixedF, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]8:       "; bench.run<Stride<FixedL, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]8:        "; bench.run<Stride<ByteF, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]8:        "; bench.run<Stride<ByteL, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]8:         "; bench.run<Stride<BitF, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]8:         "; bench.run<Stride<BitL, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]byte8:  "; bench.run<Stride<Fixed26Byte, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]bit8:   "; bench.run<Stride<Fixed26Bit, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]byte8:  "; bench.run<Stride<Fixed32Byte, 8>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]bit8:   "; bench.run<Stride<Fixed32Bit, 8>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]16:      "; bench.run<Stride<FixedF, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]16:      "; bench.run<Stride<FixedL, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]16:       "; bench.run<Stride<ByteF, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]16:       "; bench.run<Stride<ByteL, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]16:        "; bench.run<Stride<BitF, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]16:        "; bench.run<Stride<BitL, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]byte16: "; bench.run<Stride<Fixed26Byte, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]bit16:  "; bench.run<Stride<Fixed26Bit, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]byte16: "; bench.run<Stride<Fixed32Byte, 16>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]bit16:  "; bench.run<Stride<Fixed32Bit, 16>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]24:      "; bench.run<Stride<FixedF, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]24:      "; bench.run<Stride<FixedL, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]24:       "; bench.run<Stride<ByteF, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]24:       "; bench.run<Stride<ByteL, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]24:        "; bench.run<Stride<BitF, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]24:        "; bench.run<Stride<BitL, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]byte24: "; bench.run<Stride<Fixed26Byte, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]bit24:  "; bench.run<Stride<Fixed26Bit, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]byte24: "; bench.run<Stride<Fixed32Byte, 24>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]bit24:  "; bench.run<Stride<Fixed32Bit, 24>>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]32:      "; bench.run<Stride<FixedF, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]32:      "; bench.run<Stride<FixedL, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]32:       "; bench.run<Stride<ByteF, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]32:       "; bench.run<Stride<ByteL, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]32:        "; bench.run<Stride<BitF, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]32:        "; bench.run<Stride<BitL, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]byte32: "; bench.run<Stride<Fixed26Byte, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[26]bit32:  "; bench.run<Stride<Fixed26Bit, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]byte32: "; bench.run<Stride<Fixed32Byte, 32>>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[32]bit32:  "; bench.run<Stride<Fixed32Bit, 32>>(); bench.separator("\n");

    // cout << "Prezza:          "; bench.run_dynamic();                    bench.separator("\n");

    return 0;
}
