//#define HFT_USE_HUGETLB
//#define HFT_TRANSPARENT

#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

#include <fenwick.hpp>

using namespace std;
using namespace hft::fenwick;
using namespace std::chrono;


// The size of a bottom tree is enough for a single (4kB tlb) memory page
template <size_t N> using Fixed9Fixed = Hybrid<FixedL, FixedF, N, 9>;
template <size_t N> using Fixed11Byte = Hybrid<FixedL, ByteF, N, 11>;
template <size_t N> using Fixed12Bit = Hybrid<FixedL, BitF, N, 12>;
template <size_t N> using Byte11Byte = Hybrid<ByteL, ByteF, N, 11>;
template <size_t N> using Byte12Bit = Hybrid<ByteL, BitF, N, 12>;
template <size_t N> using Bit12Bit = Hybrid<BitL, BitF, N, 12>;

// The size of a bottom tree is enough for a single (2MB hugetlb) memory page
template <size_t N> using Fixed18Fixed = Hybrid<FixedL, FixedF, N, 18>;
template <size_t N> using Fixed19Byte = Hybrid<FixedL, ByteF, N, 19>;
template <size_t N> using Fixed20Bit = Hybrid<FixedL, BitF, N, 20>;
template <size_t N> using Byte19Byte = Hybrid<ByteL, ByteF, N, 19>;
template <size_t N> using Byte20Bit = Hybrid<ByteL, BitF, N, 20>;
template <size_t N> using Bit20Bit = Hybrid<BitL, BitF, N, 20>;

// The size of a bottom tree is enough for the (8MB = 4 * 2MB huge pages) cache
template <size_t N> using Fixed20Fixed = Hybrid<FixedL, FixedF, N, 20>;
template <size_t N> using Fixed21Byte = Hybrid<FixedL, ByteF, N, 21>;
template <size_t N> using Fixed25Bit = Hybrid<FixedL, BitF, N, 25>;
template <size_t N> using Byte21Byte = Hybrid<ByteL, ByteF, N, 21>;
template <size_t N> using Byte25Bit = Hybrid<ByteL, BitF, N, 25>;
template <size_t N> using Bit25Bit = Hybrid<BitL, BitF, N, 25>;


template <size_t BOUND> class Benchmark {
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
      seqdist = uniform_int_distribution<uint64_t>(0, BOUND);
      idxdist = uniform_int_distribution<size_t>(1, size);
      cumseqdist = uniform_int_distribution<uint64_t>(0, BOUND*size);

      for (size_t i = 0; i < size; i++)
        sequence[i] = seqdist(mte);
    }

    template <template <size_t> class T> void run() {
      const double c = 1. / queries;
      high_resolution_clock::time_point begin, end;
      uint64_t u = 0;

      cout << "Constructor... " << flush;
      begin = high_resolution_clock::now();
      T<BOUND> tree(sequence.get(), size);
      end = high_resolution_clock::now();
      auto build = duration_cast<chrono::nanoseconds>(end - begin).count();
      fbuild << to_string(build / (double)size);

      constexpr int REPS = 5;
      constexpr size_t MID = 2; // index of the median of a REPS elements sorted vector

      cout << "prefix: " << flush;
      vector<chrono::nanoseconds::rep> prefix;
      for (int r = 0; r < REPS; r++) {
          cout << r << " " << flush;
          begin = high_resolution_clock::now();
          for (uint64_t i = 0; i < queries; ++i)
              u ^= tree.prefix(idxdist(mte));
          end = high_resolution_clock::now();
          prefix.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
      }
      std::sort(prefix.begin(), prefix.end());
      fprefix << to_string(prefix[MID] * c);

      cout << "find: " << flush;
      vector<chrono::nanoseconds::rep> find;
      for (int r = 0; r < REPS; r++) {
          cout << r << " " << flush;
          begin = high_resolution_clock::now();
          for (uint64_t i = 0; i < queries; ++i)
              u ^= tree.find(cumseqdist(mte));
          end = high_resolution_clock::now();
          find.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
      }
      std::sort(find.begin(), find.end());
      ffind << to_string(find[MID] * c);

      cout << "add: " << flush;
      vector<chrono::nanoseconds::rep> add;
      for (int r = 0; r < REPS; r++) {
          cout << r << " " << flush;
          begin = high_resolution_clock::now();
          for (uint64_t i = 0; i < queries; ++i) {
              size_t idx = idxdist(mte);
              uint64_t val = seqdist(mte);
              tree.add(idx, sequence[idx] + val < BOUND ? val : -val);
          }
          end = high_resolution_clock::now();
          add.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
      }
      std::sort(add.begin(), add.end());
      fadd << to_string(add[MID] * c);

      // the optimizer can't erase the adds
      u ^= tree.prefix(idxdist(mte));

      // cout << "findc: " << flush;
      // vector<chrono::nanoseconds::rep> findc;
      // for (int r = 0; r < REPS; r++) {
      //     cout << r << " " << flush;
      //     begin = high_resolution_clock::now();
      //     for (uint64_t i = 0; i < queries; ++i)
      //         u ^= tree.compFind(cumseqdist(mte));
      //     end = high_resolution_clock::now();
      //     findc.push_back(duration_cast<chrono::nanoseconds>(end-begin).count());
      // }
      // std::sort(findc.begin(), findc.end());
      // ffindc << to_string(findc[MID] * c);

      cout << "bitspace... " << flush;
      fbitspace << to_string(tree.bitCount() / (size * 64.));
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

        file << size << ",";
    }
};


int main(int argc, char *argv[])
{
    random_device rd;
    mt19937 mte(rd());

    if (argc < 3) {
        cerr << "Invalid parameters: <outdir> <size> <queries>" << endl;
        return -1;
    }

    const size_t size = std::stoi(argv[2]);
    const size_t queries = std::stoi(argv[3]);

    constexpr size_t BOUND = 64;

    Benchmark<BOUND> bench(argv[1], size, queries);
    bench.datainit(mte);

    bench.filesinit("fixed[F],fixed[$\\ell$],byte[F],byte[$\\ell$],bit[F],bit[$\\ell$],"
                    "fixed[$9$]fixed,fixed[$11$]byte,fixed[$12$]bit,byte[$11$]byte,byte[$12$]bit,byte[$12]bit,"
                    "fixed[$18$]fixed,fixed[$19$]byte,fixed[$20$]bit,byte[$19$]byte,byte[$20$]bit,byte[$20$]bit,"
                    "fixed[$20$]fixed,fixed[$21$]byte,fixed[$25$]bit,byte[$21$]byte,byte[$25$]bit,byte[$25$]bit");

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]:       "; bench.run<FixedF>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]:       "; bench.run<FixedL>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]:        "; bench.run<ByteF>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]:        "; bench.run<ByteL>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]:         "; bench.run<BitF>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]:         "; bench.run<BitL>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed:  ";  bench.run<Fixed9Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[11]byte:  ";  bench.run<Fixed11Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]bit:   ";  bench.run<Fixed12Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[11]byte:   ";  bench.run<Byte11Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]bit:    ";  bench.run<Byte12Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]bit:    ";  bench.run<Byte12Bit>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[18]fixed: ";  bench.run<Fixed18Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[19]byte:  ";  bench.run<Fixed19Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[20]bit:   ";  bench.run<Fixed20Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[19]byte:   ";  bench.run<Byte19Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[20]bit:    ";  bench.run<Byte20Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[20]bit:    ";  bench.run<Byte20Bit>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[20]fixed: ";  bench.run<Fixed20Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[21]byte:  ";  bench.run<Fixed21Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[25]bit:   ";  bench.run<Fixed25Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[21]byte:   ";  bench.run<Byte21Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[25]bit:    ";  bench.run<Byte25Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[25]bit:    ";  bench.run<Byte25Bit>(); bench.separator("\n");

    return 0;
}
