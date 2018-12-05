#define HFT_USE_HUGETLB
//#define HFT_TRANSPARENT

#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

#include <fenwick/fenwick_tree.hpp>
#include <fenwick/fixedf.hpp>
#include <fenwick/fixedl.hpp>
#include <fenwick/typef.hpp>
#include <fenwick/typel.hpp>
#include <fenwick/bytef.hpp>
#include <fenwick/bytel.hpp>
#include <fenwick/bitf.hpp>
#include <fenwick/bitl.hpp>
#include <fenwick/hybrid.hpp>

using namespace std;
using namespace hft::fenwick;
using namespace std::chrono;

template <size_t N> using LNaiveNaive12 = Hybrid<FixedL, FixedF, N, 12>;
template <size_t N> using LByteByte12 = Hybrid<ByteL, ByteF, N, 12>;
template <size_t N> using LBitBit12 = Hybrid<BitL, BitF, N, 12>;
template <size_t N> using LNaiveByte12 = Hybrid<FixedL, ByteF, N, 12>;
template <size_t N> using LNaiveBit12 = Hybrid<FixedL, BitF, N, 12>;
template <size_t N> using LByteBit12 = Hybrid<ByteL, BitF, N, 12>;

template <size_t N> using LNaiveNaive14 = Hybrid<FixedL, FixedF, N, 14>;
template <size_t N> using LByteByte14 = Hybrid<ByteL, ByteF, N, 14>;
template <size_t N> using LBitBit14 = Hybrid<BitL, BitF, N, 14>;
template <size_t N> using LNaiveByte14 = Hybrid<FixedL, ByteF, N, 14>;
template <size_t N> using LNaiveBit14 = Hybrid<FixedL, BitF, N, 14>;
template <size_t N> using LByteBit14 = Hybrid<ByteL, BitF, N, 14>;

template <size_t N> using LNaiveNaive16 = Hybrid<FixedL, FixedF, N, 16>;
template <size_t N> using LByteByte16 = Hybrid<ByteL, ByteF, N, 16>;
template <size_t N> using LBitBit16 = Hybrid<BitL, BitF, N, 16>;
template <size_t N> using LNaiveByte16 = Hybrid<FixedL, ByteF, N, 16>;
template <size_t N> using LNaiveBit16 = Hybrid<FixedL, BitF, N, 16>;
template <size_t N> using LByteBit16 = Hybrid<ByteL, BitF, N, 16>;

template <size_t N> using LNaiveNaive18 = Hybrid<FixedL, FixedF, N, 18>;
template <size_t N> using LByteByte18 = Hybrid<ByteL, ByteF, N, 18>;
template <size_t N> using LBitBit18 = Hybrid<BitL, BitF, N, 18>;
template <size_t N> using LNaiveByte18 = Hybrid<FixedL, ByteF, N, 18>;
template <size_t N> using LNaiveBit18 = Hybrid<FixedL, BitF, N, 18>;
template <size_t N> using LByteBit18 = Hybrid<ByteL, BitF, N, 18>;

template <size_t N> using LNaiveNaive20 = Hybrid<FixedL, FixedF, N, 20>;
template <size_t N> using LByteByte20 = Hybrid<ByteL, ByteF, N, 20>;
template <size_t N> using LBitBit20 = Hybrid<BitL, BitF, N, 20>;
template <size_t N> using LNaiveByte20 = Hybrid<FixedL, ByteF, N, 20>;
template <size_t N> using LNaiveBit20 = Hybrid<FixedL, BitF, N, 20>;
template <size_t N> using LByteBit20 = Hybrid<ByteL, BitF, N, 20>;


template <size_t LEAF_MAXVAL> class Benchmark {
private:
    string path;
    size_t size, queries;

    mt19937 mte;
    unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size);
    uniform_int_distribution<uint64_t> seqdist, cumseqdist;
    uniform_int_distribution<size_t> idxdist;

    ofstream fbuild, fprefix, fadd, ffind, ffindc, fbitspace;

  public:
    Benchmark(string path, size_t size, size_t queries) :
        path(path),
        size(size),
        queries(queries) {}

    void filesinit(string order) {
      finit(fbuild, "build.csv", "Elements," + order);
      finit(fprefix, "prefix.csv", "Elements," + order);
      finit(fadd, "add.csv", "Elements," + order);
      finit(ffind, "find.csv", "Elements," + order);
      finit(ffindc, "findc.csv", "Elements," + order);
      finit(fbitspace, "bitspace.csv", "Elements," + order);
    }

    void separator(string sep = ",") {
        fbuild << sep;
        fprefix << sep;
        fadd << sep;
        ffind << sep;
        ffindc << sep;
        fbitspace << sep;
    }

    void datainit(mt19937 &engine) {
      mte = engine;
      seqdist = uniform_int_distribution<uint64_t>(0, LEAF_MAXVAL);
      idxdist = uniform_int_distribution<size_t>(1, size);
      cumseqdist = uniform_int_distribution<uint64_t>(0, LEAF_MAXVAL*size);

      for (size_t i = 0; i < size; i++)
        sequence[i] = seqdist(mte);
    }

    template <template <size_t> class T> void run() {
      const double c = 1. / queries;
      high_resolution_clock::time_point begin, end;
      uint64_t u = 0;

      cout << "Constructor... " << flush;
      begin = high_resolution_clock::now();
      T<LEAF_MAXVAL> tree(sequence.get(), size);
      end = high_resolution_clock::now();
      auto build = duration_cast<chrono::nanoseconds>(end - begin).count();
      fbuild << to_string(build / (double)size);

      cout << "prefix... " << flush;
      begin = high_resolution_clock::now();
      for (uint64_t i = 0; i < queries; ++i)
        u ^= tree.prefix(idxdist(mte));
      end = high_resolution_clock::now();
      auto prefix = duration_cast<chrono::nanoseconds>(end - begin).count();
      fprefix << to_string(prefix * c);

      cout << "add... " << flush;
      begin = high_resolution_clock::now();
      for (uint64_t i = 0; i < queries; ++i) {
        size_t idx = idxdist(mte);
        tree.add(idx, (sequence[idx] + seqdist(mte)) % LEAF_MAXVAL);
      }
      end = high_resolution_clock::now();
      auto add = duration_cast<chrono::nanoseconds>(end - begin).count();
      fadd << to_string(add * c);

      cout << "find... " << flush;
      begin = high_resolution_clock::now();
      for (uint64_t i = 0; i < queries; ++i)
        u ^= tree.find(cumseqdist(mte));
      end = high_resolution_clock::now();
      auto find = duration_cast<chrono::nanoseconds>(end - begin).count();
      ffind << to_string(find * c);

      cout << "findc... " << flush;
      begin = high_resolution_clock::now();
      for (uint64_t i = 0; i < queries; ++i)
        u ^= tree.compfind(cumseqdist(mte));
      end = high_resolution_clock::now();
      auto findc = duration_cast<chrono::nanoseconds>(end - begin).count();
      ffindc << to_string(findc * c);

      cout << "bitspace... " << flush;
      fbitspace << to_string(tree.bit_count() / (size * 64.));
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

    constexpr size_t LEAF_MAXVAL = 64;

    Benchmark<LEAF_MAXVAL> bench(argv[1], size, queries);

    bench.datainit(mte);
    bench.filesinit("fixed[F],fixed[$\\ell$],byte[F],byte[$\\ell$],bit[F],bit[$\\ell$],"
                    "fixed[$12$]fixed,byte[$12$]byte,bit[$12$]bit,fixed[$12$]byte,fixed[$12$]bit,byte[$12$]bit,"
                    "fixed[$14$]fixed,byte[$14$]byte,bit[$14$]bit,fixed[$14$]byte,fixed[$14$]bit,byte[$14$]bit,"
                    "fixed[$16$]fixed,byte[$16$]byte,bit[$16$]bit,fixed[$16$]byte,fixed[$16$]bit,byte[$16$]bit,"
                    "fixed[$18$]fixed,byte[$18$]byte,bit[$18$]bit,fixed[$18$]byte,fixed[$18$]bit,byte[$18$]bit,"
                    "fixed[$20$]fixed,byte[$20$]byte,bit[$20$]bit,fixed[$20$]byte,fixed[$20$]bit,byte[$20$]bit");

    cout << "fixed[F](" << size << ", " << queries << "):       "; bench.run<FixedF>(); bench.separator();
    cout << "fixed[l](" << size << ", " << queries << "):       "; bench.run<FixedL>(); bench.separator();
    cout << "byte[F](" << size << ", " << queries << "):        "; bench.run<ByteF>(); bench.separator();
    cout << "byte[l](" << size << ", " << queries << "):        "; bench.run<ByteL>(); bench.separator();
    cout << "bit[F](" << size << ", " << queries << "):         "; bench.run<BitF>(); bench.separator();
    cout << "bit[l](" << size << ", " << queries << "):         "; bench.run<BitL>(); bench.separator();

    cout << "fixed[12]fixed(" << size << ", " << queries << "): "; bench.run<LNaiveNaive12>(); bench.separator();
    cout << "byte[12]byte(" << size << ", " << queries << "):   "; bench.run<LByteByte12>(); bench.separator();
    cout << "bit[12]bit(" << size << ", " << queries << "):     "; bench.run<LBitBit12>(); bench.separator();
    cout << "fixed[12]byte(" << size << ", " << queries << "):  "; bench.run<LNaiveByte12>(); bench.separator();
    cout << "fixed[12]bit(" << size << ", " << queries << "):   "; bench.run<LNaiveBit12>(); bench.separator();
    cout << "byte[12]bit(" << size << ", " << queries << "):    "; bench.run<LByteBit12>(); bench.separator();

    cout << "fixed[14]fixed(" << size << ", " << queries << "): "; bench.run<LNaiveNaive14>(); bench.separator();
    cout << "byte[14]byte(" << size << ", " << queries << "):   "; bench.run<LByteByte14>(); bench.separator();
    cout << "bit[14]bit(" << size << ", " << queries << "):     "; bench.run<LBitBit14>(); bench.separator();
    cout << "fixed[14]byte(" << size << ", " << queries << "):  "; bench.run<LNaiveByte14>(); bench.separator();
    cout << "fixed[14]bit(" << size << ", " << queries << "):   "; bench.run<LNaiveBit14>(); bench.separator();
    cout << "byte[14]bit(" << size << ", " << queries << "):    "; bench.run<LByteBit14>(); bench.separator();

    cout << "fixed[16]fixed(" << size << ", " << queries << "): "; bench.run<LNaiveNaive16>(); bench.separator();
    cout << "byte[16]byte(" << size << ", " << queries << "):   "; bench.run<LByteByte16>(); bench.separator();
    cout << "bit[16]bit(" << size << ", " << queries << "):     "; bench.run<LBitBit16>(); bench.separator();
    cout << "fixed[16]byte(" << size << ", " << queries << "):  "; bench.run<LNaiveByte16>(); bench.separator();
    cout << "fixed[16]bit(" << size << ", " << queries << "):   "; bench.run<LNaiveBit16>(); bench.separator();
    cout << "byte[16]bit(" << size << ", " << queries << "):    "; bench.run<LByteBit16>(); bench.separator();

    cout << "fixed[18]fixed(" << size << ", " << queries << "): "; bench.run<LNaiveNaive18>(); bench.separator();
    cout << "byte[18]byte(" << size << ", " << queries << "):   "; bench.run<LByteByte18>(); bench.separator();
    cout << "bit[18]bit(" << size << ", " << queries << "):     "; bench.run<LBitBit18>(); bench.separator();
    cout << "fixed[18]byte(" << size << ", " << queries << "):  "; bench.run<LNaiveByte18>(); bench.separator();
    cout << "fixed[18]bit(" << size << ", " << queries << "):   "; bench.run<LNaiveBit18>(); bench.separator();
    cout << "byte[18]bit(" << size << ", " << queries << "):    "; bench.run<LByteBit18>(); bench.separator();

    cout << "fixed[20]fixed(" << size << ", " << queries << "): "; bench.run<LNaiveNaive20>(); bench.separator();
    cout << "byte[20]byte(" << size << ", " << queries << "):   "; bench.run<LByteByte20>(); bench.separator();
    cout << "bit[20]bit(" << size << ", " << queries << "):     "; bench.run<LBitBit20>(); bench.separator();
    cout << "fixed[20]byte(" << size << ", " << queries << "):  "; bench.run<LNaiveByte20>(); bench.separator();
    cout << "fixed[20]bit(" << size << ", " << queries << "):   "; bench.run<LNaiveBit20>(); bench.separator();
    cout << "byte[20]bit(" << size << ", " << queries << "):    "; bench.run<LByteBit20>(); bench.separator("\n");

    return 0;
}
