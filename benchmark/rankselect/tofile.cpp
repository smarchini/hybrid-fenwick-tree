#define HFT_USE_HUGETLB
//#define HFT_TRANSPARENT

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

template <size_t N> using LNaiveNaive16 = Hybrid<FixedL, FixedF, N, 16>;
template <size_t N> using LByteByte16 = Hybrid<ByteL, ByteF, N, 16>;
template <size_t N> using LBitBit16 = Hybrid<BitL, BitF, N, 16>;
template <size_t N> using LNaiveByte16 = Hybrid<FixedL, ByteF, N, 16>;
template <size_t N> using LNaiveBit16 = Hybrid<FixedL, BitF, N, 16>;
template <size_t N> using LByteBit16 = Hybrid<ByteL, BitF, N, 16>;


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

        cout << "toggle: " << flush;
        vector<chrono::nanoseconds::rep> update;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i)
                u ^= bv.toggle(bitdist(mte));
            end = high_resolution_clock::now();
            update.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
        }
        std::sort(update.begin(), update.end());
        fupdate << to_string(update[MID] * c);

        cout << "bitspace... " << flush;
        fbitspace << to_string(bv.bit_count() / (size * 64.));
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

        cout << "toggle: " << flush;
        vector<chrono::nanoseconds::rep> update;
        for (int r = 0; r < REPS; r++) {
            cout << r << " " << flush;
            begin = high_resolution_clock::now();
            for(uint64_t i = 0; i < queries; ++i) {
                size_t index = bitdist(mte);
                dynamic.set(index, !dynamic.at(index));
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

    bench.filesinit("fixed[F],fixed[$\\ell$],byte[F],byte[$\\ell$],bit[F],bit[$\\ell$],"
                    "fixed[$16$]fixed,byte[$16$]byte,bit[$16$]bit,fixed[$16$]byte,fixed[$16$]bit,byte[$16$]bit,"
                    "fixed[F]8,fixed[$\\ell$]8,byte[F]8,byte[$\\ell$]8,bit[F]8,bit[$\\ell$]8,"
                    "fixed[$16$]fixed8,byte[$16$]byte8,bit[$16$]bit8,fixed[$16$]byte8,fixed[$16$]bit8,byte[$16$]bit8,"
                    "fixed[F]16,fixed[$\\ell$]16,byte[F]16,byte[$\\ell$]16,bit[F]16,bit[$\\ell$]16,"
                    "fixed[$16$]fixed16,byte[$16$]byte16,bit[$16$]bit16,fixed[$16$]byte16,fixed[$16$]bit16,byte[$16$]bit16,"
                    "fixed[F]32,fixed[$\\ell$]32,byte[F]32,byte[$\\ell$]32,bit[F]32,bit[$\\ell$]32,"
                    "fixed[$16$]fixed32,byte[$16$]byte32,bit[$16$]bit32,fixed[$16$]byte32,fixed[$16$]bit32,byte[$16$]bit32,"
                    "fixed[F]64,fixed[$\\ell$]64,byte[F]64,byte[$\\ell$]64,bit[F]64,bit[$\\ell$]64,"
                    "fixed[$16$]fixed64,byte[$16$]byte64,bit[$16$]bit64,fixed[$16$]byte64,fixed[$16$]bit64,byte[$16$]bit64"); //,Prezza

    // bench.filesinit("Prezza");

    cout << "Fenw:            "; bench.run<Word<FixedF>>();              bench.separator();
    cout << "LFenw:           "; bench.run<Word<FixedL>>();              bench.separator();
    cout << "Byte:            "; bench.run<Word<ByteF>>();               bench.separator();
    cout << "LByte:           "; bench.run<Word<ByteL>>();               bench.separator();
    cout << "Bit:             "; bench.run<Word<BitF>>();                bench.separator();
    cout << "LBit:            "; bench.run<Word<BitL>>();                bench.separator();
    cout << "LFenw[16]Fenw:   "; bench.run<Word<LNaiveNaive16>>();       bench.separator();
    cout << "LByte[16]Byte:   "; bench.run<Word<LByteByte16>>();         bench.separator();
    cout << "LBit[16]Bit:     "; bench.run<Word<LBitBit16>>();           bench.separator();
    cout << "LFenw[16]Byte:   "; bench.run<Word<LNaiveByte16>>();        bench.separator();
    cout << "LFenw[16]Bit:    "; bench.run<Word<LNaiveBit16>>();         bench.separator();
    cout << "LByte[16]Bit:    "; bench.run<Word<LByteBit16>>();          bench.separator();

    cout << "Fenw8:           "; bench.run<Stride<FixedF, 8>>();         bench.separator();
    cout << "LFenw8:          "; bench.run<Stride<FixedL, 8>>();         bench.separator();
    cout << "Byte8:           "; bench.run<Stride<ByteF, 8>>();          bench.separator();
    cout << "LByte8:          "; bench.run<Stride<ByteL, 8>>();          bench.separator();
    cout << "Bit8:            "; bench.run<Stride<BitF, 8>>();           bench.separator();
    cout << "LBit8:           "; bench.run<Stride<BitL, 8>>();           bench.separator();
    cout << "LFenw[16]Fenw8:  "; bench.run<Stride<LNaiveNaive16, 8>>();  bench.separator();
    cout << "LByte[16]Byte8:  "; bench.run<Stride<LByteByte16, 8>>();    bench.separator();
    cout << "LBit[16]Bit8:    "; bench.run<Stride<LBitBit16, 8>>();      bench.separator();
    cout << "LFenw[16]Byte8:  "; bench.run<Stride<LNaiveByte16, 8>>();   bench.separator();
    cout << "LFenw[16]Bit8:   "; bench.run<Stride<LNaiveBit16, 8>>();    bench.separator();
    cout << "LByte[16]Bit8:   "; bench.run<Stride<LByteBit16, 8>>();     bench.separator();

    cout << "Fenw16:          "; bench.run<Stride<FixedF, 16>>();        bench.separator();
    cout << "LFenw16:         "; bench.run<Stride<FixedL, 16>>();        bench.separator();
    cout << "Byte16:          "; bench.run<Stride<ByteF, 16>>();         bench.separator();
    cout << "LByte16:         "; bench.run<Stride<ByteL, 16>>();         bench.separator();
    cout << "Bit16:           "; bench.run<Stride<BitF, 16>>();          bench.separator();
    cout << "LBit16:          "; bench.run<Stride<BitL, 16>>();          bench.separator();
    cout << "LFenw[16]Fenw16: "; bench.run<Stride<LNaiveNaive16, 16>>(); bench.separator();
    cout << "LByte[16]Byte16: "; bench.run<Stride<LByteByte16, 16>>();   bench.separator();
    cout << "LBit[16]Bit16:   "; bench.run<Stride<LBitBit16, 16>>();     bench.separator();
    cout << "LFenw[16]Byte16: "; bench.run<Stride<LNaiveByte16, 16>>();  bench.separator();
    cout << "LFenw[16]Bit16:  "; bench.run<Stride<LNaiveBit16, 16>>();   bench.separator();
    cout << "LByte[16]Bit16:  "; bench.run<Stride<LByteBit16, 16>>();    bench.separator();

    cout << "Fenw32:          "; bench.run<Stride<FixedF, 32>>();        bench.separator();
    cout << "LFenw32:         "; bench.run<Stride<FixedL, 32>>();        bench.separator();
    cout << "Byte32:          "; bench.run<Stride<ByteF, 32>>();         bench.separator();
    cout << "LByte32:         "; bench.run<Stride<ByteL, 32>>();         bench.separator();
    cout << "Bit32:           "; bench.run<Stride<BitF, 32>>();          bench.separator();
    cout << "LBit32:          "; bench.run<Stride<BitL, 32>>();          bench.separator();
    cout << "LFenw[16]Fenw32: "; bench.run<Stride<LNaiveNaive16, 32>>(); bench.separator();
    cout << "LByte[16]Byte32: "; bench.run<Stride<LByteByte16, 32>>();   bench.separator();
    cout << "LBit[16]Bit32:   "; bench.run<Stride<LBitBit16, 32>>();     bench.separator();
    cout << "LFenw[16]Byte32: "; bench.run<Stride<LNaiveByte16, 32>>();  bench.separator();
    cout << "LFenw[16]Bit32:  "; bench.run<Stride<LNaiveBit16, 32>>();   bench.separator();
    cout << "LByte[16]Bit32:  "; bench.run<Stride<LByteBit16, 32>>();    bench.separator();

    cout << "Fenw64:          "; bench.run<Stride<FixedF, 64>>();        bench.separator();
    cout << "LFenw64:         "; bench.run<Stride<FixedL, 64>>();        bench.separator();
    cout << "Byte64:          "; bench.run<Stride<ByteF, 64>>();         bench.separator();
    cout << "LByte64:         "; bench.run<Stride<ByteL, 64>>();         bench.separator();
    cout << "Bit64:           "; bench.run<Stride<BitF, 64>>();          bench.separator();
    cout << "LBit64:          "; bench.run<Stride<BitL, 64>>();          bench.separator();
    cout << "LFenw[16]Fenw64: "; bench.run<Stride<LNaiveNaive16, 64>>(); bench.separator();
    cout << "LByte[16]Byte64: "; bench.run<Stride<LByteByte16, 64>>();   bench.separator();
    cout << "LBit[16]Bit64:   "; bench.run<Stride<LBitBit16, 64>>();     bench.separator();
    cout << "LFenw[16]Byte64: "; bench.run<Stride<LNaiveByte16, 64>>();  bench.separator();
    cout << "LFenw[16]Bit64:  "; bench.run<Stride<LNaiveBit16, 64>>();   bench.separator();
    cout << "LByte[16]Bit64:  "; bench.run<Stride<LByteBit16, 64>>();    bench.separator("\n");

    // cout << "Prezza:          "; bench.run_dynamic();                    bench.separator("\n");


    return 0;
}
