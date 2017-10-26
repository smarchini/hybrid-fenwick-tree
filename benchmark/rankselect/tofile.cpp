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

#include <fenwick/fenwick_tree.hpp>
#include <fenwick/naive.hpp>
#include <fenwick/lnaive.hpp>
#include <fenwick/type.hpp>
#include <fenwick/ltype.hpp>
#include <fenwick/byte.hpp>
#include <fenwick/lbyte.hpp>
#include <fenwick/bit.hpp>
#include <fenwick/lbit.hpp>
#include <fenwick/mixed.hpp>#include <dynamic.hpp>

using namespace std;
using namespace dyn;
using namespace std::chrono;


template <size_t N> using LNaiveNaive16 = MixedFenwickTree<LNaiveFenwickTree, NaiveFenwickTree, N, 16>;
template <size_t N> using LByteByte16 = MixedFenwickTree<LByteFenwickTree, ByteFenwickTree, N, 16>;
template <size_t N> using LBitBit16 = MixedFenwickTree<LBitFenwickTree, BitFenwickTree, N, 16>;

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

    ~Benchmark() {
        fbuild.close();
        frank0.close();
        frank1.close();
        fselect0.close();
        fselect1.close();
        fupdate.close();
        fbitspace.close();
    }

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

        file << size << ",";
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

    //bench.filesinit("Naive32,Type32,LType32,Byte32,LByte32,Bit32,LBit32,LByteType32,LTypeNaive32");
    //cout << "Naive1(" << size << ", " << queries << "):      "; bench.run<LineRankSelect<NaiveFenwickTree, 32>>();      bench.separator();
    //cout << "Type1(" << size << ", " << queries << "):       "; bench.run<LineRankSelect<TypeFenwickTree,  32>>();      bench.separator();
    //cout << "LType1(" << size << ", " << queries << "):      "; bench.run<LineRankSelect<LTypeFenwickTree, 32>>();      bench.separator();
    //cout << "Byte1(" << size << ", " << queries << "):       "; bench.run<LineRankSelect<ByteFenwickTree,  32>>();      bench.separator();
    //cout << "LByte1(" << size << ", " << queries << "):      "; bench.run<LineRankSelect<LByteFenwickTree, 32>>();      bench.separator();
    //cout << "Bit1(" << size << ", " << queries << "):        "; bench.run<LineRankSelect<BitFenwickTree,   32>>();      bench.separator();
    //cout << "LBit1(" << size << ", " << queries << "):       "; bench.run<LineRankSelect<LBitFenwickTree,  32>>();      bench.separator();
    //cout << "LByteType1(" << size << ", " << queries << "):  "; bench.run<LineRankSelect<LByteTypeFenwickTree,  32>>(); bench.separator();
    //cout << "LTypeNaive1(" << size << ", " << queries << "): "; bench.run<LineRankSelect<LTypeNaiveFenwickTree, 32>>(); bench.separator("\n");


    bench.filesinit("Fenw,LFenw,Byte,LByte,Bit,LBit,LFenwFenw16,LByteByte16,LBitBit16,Fenw(16),LFenw(16),Byte(16),LByte(16),Bit(16),LBit(16),LFenwFenw16(16),LByteByte16(16),LBitBit16(16),Fenw(32),LFenw(32),Byte(32),LByte(32),Bit(32),LBit(32),LFenwFenw16(32),LByteByte16(32),LBitBit16(32),Fenw(64),LFenw(64),Byte(64),LByte(64),Bit(64),LBit(64),LFenwFenw16(64),LByteByte16(64),LBitBit16(64)");

    cout << "Fenw:            "; bench.run<WordRankSelect<NaiveFenwickTree>>();      bench.separator();
    cout << "LFenw:           "; bench.run<WordRankSelect<LNaiveFenwickTree>>();     bench.separator();
    cout << "Byte:            "; bench.run<WordRankSelect<ByteFenwickTree>>();       bench.separator();
    cout << "LByte:           "; bench.run<WordRankSelect<LByteFenwickTree>>();      bench.separator();
    cout << "Bit:             "; bench.run<WordRankSelect<BitFenwickTree>>();        bench.separator();
    cout << "LBit:            "; bench.run<WordRankSelect<LBitFenwickTree>>();       bench.separator();
    cout << "LFenwFenw16:     "; bench.run<WordRankSelect<LNaiveNaive16>>();         bench.separator();
    cout << "LByteByte16:     "; bench.run<WordRankSelect<LByteByte16>>();           bench.separator();
    cout << "LBitBit16:       "; bench.run<WordRankSelect<LBitBit16>>();             bench.separator();

    cout << "Fenw(16):        "; bench.run<LineRankSelect<NaiveFenwickTree,  16>>(); bench.separator();
    cout << "LFenw(16):       "; bench.run<LineRankSelect<LNaiveFenwickTree, 16>>(); bench.separator();
    cout << "Byte(16):        "; bench.run<LineRankSelect<ByteFenwickTree,   16>>(); bench.separator();
    cout << "LByte(16):       "; bench.run<LineRankSelect<LByteFenwickTree,  16>>(); bench.separator();
    cout << "Bit(16):         "; bench.run<LineRankSelect<BitFenwickTree,    16>>(); bench.separator();
    cout << "LBit(16):        "; bench.run<LineRankSelect<LBitFenwickTree,   16>>(); bench.separator();
    cout << "LFenwFenw16(16): "; bench.run<LineRankSelect<LNaiveNaive16,     16>>(); bench.separator();
    cout << "LByteByte16(16): "; bench.run<LineRankSelect<LByteByte16,       16>>(); bench.separator();
    cout << "LBitBit16(16):   "; bench.run<LineRankSelect<LBitBit16,         16>>(); bench.separator();

    cout << "Fenw(32):        "; bench.run<LineRankSelect<NaiveFenwickTree,  32>>(); bench.separator();
    cout << "LFenw(32):       "; bench.run<LineRankSelect<LNaiveFenwickTree, 32>>(); bench.separator();
    cout << "Byte(32):        "; bench.run<LineRankSelect<ByteFenwickTree,   32>>(); bench.separator();
    cout << "LByte(32):       "; bench.run<LineRankSelect<LByteFenwickTree,  32>>(); bench.separator();
    cout << "Bit(32):         "; bench.run<LineRankSelect<BitFenwickTree,    32>>(); bench.separator();
    cout << "LBit(32):        "; bench.run<LineRankSelect<LBitFenwickTree,   32>>(); bench.separator();
    cout << "LFenwFenw16(32): "; bench.run<LineRankSelect<LNaiveNaive16,     32>>(); bench.separator();
    cout << "LByteByte16(32): "; bench.run<LineRankSelect<LByteByte16,       32>>(); bench.separator();
    cout << "LBitBit16(32):   "; bench.run<LineRankSelect<LBitBit16,         32>>(); bench.separator();

    cout << "Fenw(64):        "; bench.run<LineRankSelect<NaiveFenwickTree,  64>>(); bench.separator();
    cout << "LFenw(64):       "; bench.run<LineRankSelect<LNaiveFenwickTree, 64>>(); bench.separator();
    cout << "Byte(64):        "; bench.run<LineRankSelect<ByteFenwickTree,   64>>(); bench.separator();
    cout << "LByte(64):       "; bench.run<LineRankSelect<LByteFenwickTree,  64>>(); bench.separator();
    cout << "Bit(64):         "; bench.run<LineRankSelect<BitFenwickTree,    64>>(); bench.separator();
    cout << "LBit(64):        "; bench.run<LineRankSelect<LBitFenwickTree,   64>>(); bench.separator();
    cout << "LFenwFenw16(64): "; bench.run<LineRankSelect<LNaiveNaive16,     64>>(); bench.separator();
    cout << "LByteByte16(64): "; bench.run<LineRankSelect<LByteByte16,       64>>(); bench.separator();
    cout << "LBitBit16(64):   "; bench.run<LineRankSelect<LBitBit16,         64>>(); bench.separator("\n");


    return 0;
}
