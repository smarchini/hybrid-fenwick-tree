#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

#include <rankselect/rank_select.hpp>
#include <rankselect/word.hpp>
#include <rankselect/line.hpp>

#include <fenwick/bit.hpp>
#include <fenwick/byte.hpp>
#include <fenwick/fenwick_tree.hpp>
#include <fenwick/lbit.hpp>
#include <fenwick/lbyte.hpp>
#include <fenwick/lnaive.hpp>
#include <fenwick/ltype.hpp>
#include <fenwick/mixed.hpp>
#include <fenwick/naive.hpp>
#include <fenwick/type.hpp>

#include <dynamic.hpp>

using namespace std;
using namespace dyn;
using namespace std::chrono;

template <size_t N> using LNaiveNaive16 = MixedFenwickTree<FixedL, FixedF, N, 16>;
template <size_t N> using LByteByte16 = MixedFenwickTree<ByteL, ByteF, N, 16>;
template <size_t N> using LBitBit16 = MixedFenwickTree<BitL, BitF, N, 16>;
template <size_t N> using LNaiveByte16 = MixedFenwickTree<FixedL, ByteF, N, 16>;
template <size_t N> using LNaiveBit16 = MixedFenwickTree<FixedL, BitF, N, 16>;
template <size_t N> using LByteBit16 = MixedFenwickTree<ByteL, BitF, N, 16>;


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

        cout << "rank0... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.rankZero(idxdist(mte));
        end = high_resolution_clock::now();
        auto rank0 = duration_cast<chrono::nanoseconds>(end-begin).count();
        frank0 << to_string(rank0 * c);

        cout << "select0... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.selectZero(sel0dist(mte));
        end = high_resolution_clock::now();
        auto select0 = duration_cast<chrono::nanoseconds>(end-begin).count();
        fselect0 << to_string(select0 * c);

        cout << "rank1... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.rank(idxdist(mte));
        end = high_resolution_clock::now();
        auto rank1 = duration_cast<chrono::nanoseconds>(end-begin).count();
        frank1 << to_string(rank1 * c);

        cout << "select1... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.select(sel1dist(mte));
        end = high_resolution_clock::now();
        auto select1 = duration_cast<chrono::nanoseconds>(end-begin).count();
        fselect1 << to_string(select1 * c);

        cout << "update... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i) {
            size_t index = bitdist(mte) / 64;
            uint64_t value = bv.bitvector()[index] ^ (1 << (bitdist(mte) % 64));
            u ^= bv.update(index, value);
        }
        end = high_resolution_clock::now();
        auto update = duration_cast<chrono::nanoseconds>(end-begin).count();
        fupdate << to_string(update * c);

        cout << "bitspace... " << flush;
        fbitspace << to_string(bv.bit_count() / (size * 64.));
        cout << "done.  " << endl;

        const volatile uint64_t __attribute__((unused)) unused = u;
    }

//    void run_dynamic() {
//        suc_bv dynamic;
//
//        const double c = 1. / queries;
//        high_resolution_clock::time_point begin, end;
//        uint64_t u = 0;
//
//        cout << "Constructor... " << flush;
//        begin = high_resolution_clock::now();
//        for (uint64_t i = 0; i < size; ++i) {
//            for (uint64_t j = 0; j < 64; ++j)
//                dynamic.insert(64*i + j, bitvector.get()[i] & (1ULL << j));
//        }
//        end = high_resolution_clock::now();
//        auto build = duration_cast<chrono::nanoseconds>(end-begin).count();
//        fbuild << to_string(build / (double)size);
//
//        cout << "rank0... " << flush;
//        begin = high_resolution_clock::now();
//        for(uint64_t i = 0; i < queries; ++i)
//            u ^= dynamic.rank0(rank0[i]);
//        end = high_resolution_clock::now();
//        auto rank0 = duration_cast<chrono::nanoseconds>(end-begin).count();
//        frank0 << to_string(rank0 * c);
//
//        cout << "select0... " << flush;
//        begin = high_resolution_clock::now();
//        for(uint64_t i = 0; i < queries; ++i)
//            u ^= dynamic.select0(select0[i]);
//        end = high_resolution_clock::now();
//        auto select0 = duration_cast<chrono::nanoseconds>(end-begin).count();
//        fselect0 << to_string(select0 * c);
//
//        cout << "rank1... " << flush;
//        begin = high_resolution_clock::now();
//        for(uint64_t i = 0; i < queries; ++i)
//            u ^= dynamic.rank1(rank1[i]);
//        end = high_resolution_clock::now();
//        auto rank1 = duration_cast<chrono::nanoseconds>(end-begin).count();
//        frank1 << to_string(rank1 * c);
//
//        cout << "select1... " << flush;
//        begin = high_resolution_clock::now();
//        for(uint64_t i = 0; i < queries; ++i)
//            u ^= dynamic.select1(select1[i]);
//        end = high_resolution_clock::now();
//        auto select1 = duration_cast<chrono::nanoseconds>(end-begin).count();
//        fselect1 << to_string(select1 * c);
//
//        cout << "update... " << flush;
//        begin = high_resolution_clock::now();
//        for(uint64_t i = 0; i < queries; ++i)
//            dynamic.set(updateidx[i], !dynamic.at(updateidx[i]));
//        end = high_resolution_clock::now();
//        auto update = duration_cast<chrono::nanoseconds>(end-begin).count();
//        fupdate << to_string(update * c);
//
//        cout << "bitspace... " << flush;
//        fbitspace << to_string(dynamic.bit_size() / (size * 64.));
//        cout << "done.  " << endl;
//    }

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
    //bench.filesinit("Fenw,LFenw,Byte,LByte,Bit,LBit,LFenwFenw16,LByteByte16,LBitBit16,Fenw(16),LFenw(16),Byte(16),LByte(16),Bit(16),LBit(16),LFenwFenw16(16),LByteByte16(16),LBitBit16(16),Fenw(32),LFenw(32),Byte(32),LByte(32),Bit(32),LBit(32),LFenwFenw16(32),LByteByte16(32),LBitBit16(32),Fenw(64),LFenw(64),Byte(64),LByte(64),Bit(64),LBit(64),LFenwFenw16(64),LByteByte16(64),LBitBit16(64)");

     bench.filesinit("fixed[F],fixed[$\\ell$],byte[F],byte[$\\ell$],bit[F],bit[$\\ell$],"
                     "fixed[$16$]fixed,byte[$16$]byte,bit[$16$]bit,fixed[$16$]byte,fixed[$16$]bit,byte[$16$]bit,"
                     "fixed[$16$]fixed,byte[$16$]byte,bit[$16$]bit,fixed[$16$]byte,fixed[$16$]bit,byte[$16$]bit,"
                     "fixed[$20$]fixed,byte[$20$]byte,bit[$20$]bit,fixed[$20$]byte,fixed[$20$]bit,byte[$20$]bit,"
                     "fixed[F]_16,fixed[$\\ell$]_16,byte[F]_16,byte[$\\ell$]_16,bit[F]_16,bit[$\\ell$]_16,"
                     "fixed[$16$]fixed_16,byte[$16$]byte_16,bit[$16$]bit_16,fixed[$16$]byte_16,fixed[$16$]bit_16,byte[$16$]bit_16,"
                     "fixed[$16$]fixed_16,byte[$16$]byte_16,bit[$16$]bit_16,fixed[$16$]byte_16,fixed[$16$]bit_16,byte[$16$]bit_16,"
                     "fixed[$20$]fixed_16,byte[$20$]byte_16,bit[$20$]bit_16,fixed[$20$]byte_16,fixed[$20$]bit_16,byte[$20$]bit_16,"
                     "fixed[F]_32,fixed[$\\ell$]_32,byte[F]_32,byte[$\\ell$]_32,bit[F]_32,bit[$\\ell$]_32,"
                     "fixed[$16$]fixed_32,byte[$16$]byte_32,bit[$16$]bit_32,fixed[$16$]byte_32,fixed[$16$]bit_32,byte[$16$]bit_32,"
                     "fixed[$16$]fixed_32,byte[$16$]byte_32,bit[$16$]bit_32,fixed[$16$]byte_32,fixed[$16$]bit_32,byte[$16$]bit_32,"
                     "fixed[$20$]fixed_32,byte[$20$]byte_32,bit[$20$]bit_32,fixed[$20$]byte_32,fixed[$20$]bit_32,byte[$20$]bit_32,"
                     "fixed[F]_64,fixed[$\\ell$]_64,byte[F]_64,byte[$\\ell$]_64,bit[F]_64,bit[$\\ell$]_64,"
                     "fixed[$16$]fixed_64,byte[$16$]byte_64,bit[$16$]bit_64,fixed[$16$]byte_64,fixed[$16$]bit_64,byte[$16$]bit_64,"
                     "fixed[$16$]fixed_64,byte[$16$]byte_64,bit[$16$]bit_64,fixed[$16$]byte_64,fixed[$16$]bit_64,byte[$16$]bit_64,"
                     "fixed[$20$]fixed_64,byte[$20$]byte_64,bit[$20$]bit_64,fixed[$20$]byte_64,fixed[$20$]bit_64,byte[$20$]bit_64");

    cout << "Fenw:            "; bench.run<WordRankSelect<FixedF>>();      bench.separator();
    cout << "LFenw:           "; bench.run<WordRankSelect<FixedL>>();     bench.separator();
    cout << "Byte:            "; bench.run<WordRankSelect<ByteF>>();       bench.separator();
    cout << "LByte:           "; bench.run<WordRankSelect<ByteL>>();      bench.separator();
    cout << "Bit:             "; bench.run<WordRankSelect<BitF>>();        bench.separator();
    cout << "LBit:            "; bench.run<WordRankSelect<BitL>>();       bench.separator();
    cout << "LFenwFenw16:     "; bench.run<WordRankSelect<LNaiveNaive16>>();         bench.separator();
    cout << "LByteByte16:     "; bench.run<WordRankSelect<LByteByte16>>();           bench.separator();
    cout << "LBitBit16:       "; bench.run<WordRankSelect<LBitBit16>>();             bench.separator();
    cout << "LByteBit16:      "; bench.run<WordRankSelect<LByteBit16>>();            bench.separator();
    cout << "LFenwByte16:     "; bench.run<WordRankSelect<LNaiveByte16>>();          bench.separator();
    cout << "LFenwBit16:      "; bench.run<WordRankSelect<LNaiveBit16>>();           bench.separator();
    cout << "LByteBit16:      "; bench.run<WordRankSelect<LByteBit16>>();            bench.separator();

    cout << "Fenw_16:         "; bench.run<LineRankSelect<FixedF, 16>>();      bench.separator();
    cout << "LFenw_16:        "; bench.run<LineRankSelect<FixedL, 16>>();     bench.separator();
    cout << "Byte_16:         "; bench.run<LineRankSelect<ByteF, 16>>();       bench.separator();
    cout << "LByte_16:        "; bench.run<LineRankSelect<ByteL, 16>>();      bench.separator();
    cout << "Bit_16:          "; bench.run<LineRankSelect<BitF, 16>>();        bench.separator();
    cout << "LBit_16:         "; bench.run<LineRankSelect<BitL, 16>>();       bench.separator();
    cout << "LFenwFenw16_16:  "; bench.run<LineRankSelect<LNaiveNaive16, 16>>();         bench.separator();
    cout << "LByteByte16_16:  "; bench.run<LineRankSelect<LByteByte16, 16>>();           bench.separator();
    cout << "LBitBit16_16:    "; bench.run<LineRankSelect<LBitBit16, 16>>();             bench.separator();
    cout << "LByteBit16_16:   "; bench.run<LineRankSelect<LByteBit16, 16>>();            bench.separator();
    cout << "LFenwByte16_16:  "; bench.run<LineRankSelect<LNaiveByte16, 16>>();          bench.separator();
    cout << "LFenwBit16_16:   "; bench.run<LineRankSelect<LNaiveBit16, 16>>();           bench.separator();
    cout << "LByteBit16_16:   "; bench.run<LineRankSelect<LByteBit16, 16>>();            bench.separator();

    cout << "Fenw_32:         "; bench.run<LineRankSelect<FixedF, 32>>();      bench.separator();
    cout << "LFenw_32:        "; bench.run<LineRankSelect<FixedL, 32>>();     bench.separator();
    cout << "Byte_32:         "; bench.run<LineRankSelect<ByteF, 32>>();       bench.separator();
    cout << "LByte_32:        "; bench.run<LineRankSelect<ByteL, 32>>();      bench.separator();
    cout << "Bit_32:          "; bench.run<LineRankSelect<BitF, 32>>();        bench.separator();
    cout << "LBit_32:         "; bench.run<LineRankSelect<BitL, 32>>();       bench.separator();
    cout << "LFenwFenw16_32:  "; bench.run<LineRankSelect<LNaiveNaive16, 32>>();         bench.separator();
    cout << "LByteByte16_32:  "; bench.run<LineRankSelect<LByteByte16, 32>>();           bench.separator();
    cout << "LBitBit16_32:    "; bench.run<LineRankSelect<LBitBit16, 32>>();             bench.separator();
    cout << "LByteBit16_32:   "; bench.run<LineRankSelect<LByteBit16, 32>>();            bench.separator();
    cout << "LFenwByte16_32:  "; bench.run<LineRankSelect<LNaiveByte16, 32>>();          bench.separator();
    cout << "LFenwBit16_32:   "; bench.run<LineRankSelect<LNaiveBit16, 32>>();           bench.separator();
    cout << "LByteBit16_32:   "; bench.run<LineRankSelect<LByteBit16, 32>>();            bench.separator();

    cout << "Fenw_64:         "; bench.run<LineRankSelect<FixedF, 64>>();      bench.separator();
    cout << "LFenw_64:        "; bench.run<LineRankSelect<FixedL, 64>>();     bench.separator();
    cout << "Byte_64:         "; bench.run<LineRankSelect<ByteF, 64>>();       bench.separator();
    cout << "LByte_64:        "; bench.run<LineRankSelect<ByteL, 64>>();      bench.separator();
    cout << "Bit_64:          "; bench.run<LineRankSelect<BitF, 64>>();        bench.separator();
    cout << "LBit_64:         "; bench.run<LineRankSelect<BitL, 64>>();       bench.separator();
    cout << "LFenwFenw16_64:  "; bench.run<LineRankSelect<LNaiveNaive16, 64>>();         bench.separator();
    cout << "LByteByte16_64:  "; bench.run<LineRankSelect<LByteByte16, 64>>();           bench.separator();
    cout << "LBitBit16_64:    "; bench.run<LineRankSelect<LBitBit16, 64>>();             bench.separator();
    cout << "LByteBit16_64:   "; bench.run<LineRankSelect<LByteBit16, 64>>();            bench.separator();
    cout << "LFenwByte16_64:  "; bench.run<LineRankSelect<LNaiveByte16, 64>>();          bench.separator();
    cout << "LFenwBit16_64:   "; bench.run<LineRankSelect<LNaiveBit16, 64>>();           bench.separator();
    cout << "LByteBit16_64:   "; bench.run<LineRankSelect<LByteBit16, 64>>();            bench.separator("\n");

    return 0;
}
