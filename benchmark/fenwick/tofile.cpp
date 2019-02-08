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


// The size of a bottom tree is enough for a single 4kB memory page
template <size_t N> using Fixed9Fixed = Hybrid<FixedL, FixedF, N, 9>;
template <size_t N> using Fixed12Byte = Hybrid<FixedL, ByteF, N, 12>;
template <size_t N> using Fixed12Bit = Hybrid<FixedL, BitF, N, 12>;
template <size_t N> using Byte12Byte = Hybrid<ByteL, ByteF, N, 12>;
template <size_t N> using Byte12Bit = Hybrid<ByteL, BitF, N, 12>;
template <size_t N> using Bit12Bit = Hybrid<BitL, BitF, N, 12>;

// The size of a bottom tree is enough for a single 2MB huge memory page
template <size_t N> using Fixed18Fixed = Hybrid<FixedL, FixedF, N, 18>;
template <size_t N> using Fixed21Byte = Hybrid<FixedL, ByteF, N, 21>;
template <size_t N> using Fixed21Bit = Hybrid<FixedL, BitF, N, 21>;
template <size_t N> using Byte21Byte = Hybrid<ByteL, ByteF, N, 21>;
template <size_t N> using Byte21Bit = Hybrid<ByteL, BitF, N, 21>;
template <size_t N> using Bit21Bit = Hybrid<BitL, BitF, N, 21>;

// The size of a bottom tree is enough for the 32kB LL1 cache memory
template <size_t N> using Fixed12Fixed = Hybrid<FixedL, FixedF, N, 12>;
template <size_t N> using Fixed15Byte = Hybrid<FixedL, ByteF, N, 15>;
template <size_t N> using Fixed15Bit = Hybrid<FixedL, BitF, N, 15>;
template <size_t N> using Byte15Byte = Hybrid<ByteL, ByteF, N, 15>;
template <size_t N> using Byte15Bit = Hybrid<ByteL, BitF, N, 15>;
template <size_t N> using Bit15Bit = Hybrid<BitL, BitF, N, 15>;

// The size of a bottom tree is enough for the 256kB LL2 cache memory
template <size_t N> using Fixed15Fixed = Hybrid<FixedL, FixedF, N, 15>;
template <size_t N> using Fixed18Byte = Hybrid<FixedL, ByteF, N, 18>;
template <size_t N> using Fixed18Bit = Hybrid<FixedL, BitF, N, 18>;
template <size_t N> using Byte18Byte = Hybrid<ByteL, ByteF, N, 18>;
template <size_t N> using Byte18Bit = Hybrid<ByteL, BitF, N, 18>;
template <size_t N> using Bit18Bit = Hybrid<BitL, BitF, N, 18>;

// The size of a bottom tree is enough for the 8MB LL3 cache memory
template <size_t N> using Fixed20Fixed = Hybrid<FixedL, FixedF, N, 20>;
template <size_t N> using Fixed23Byte = Hybrid<FixedL, ByteF, N, 23>;
template <size_t N> using Fixed23Bit = Hybrid<FixedL, BitF, N, 23>;
template <size_t N> using Byte23Byte = Hybrid<ByteL, ByteF, N, 23>;
template <size_t N> using Byte23Bit = Hybrid<ByteL, BitF, N, 23>;
template <size_t N> using Bit23Bit = Hybrid<BitL, BitF, N, 23>;


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

    const size_t size = std::stoul(argv[2]);
    const size_t queries = std::stoul(argv[3]);

    constexpr size_t BOUND = 64;

    Benchmark<BOUND> bench(argv[1], size, queries);
    bench.datainit(mte);

    bench.filesinit("fixed[F],fixed[$\\ell$],byte[F],byte[$\\ell$],bit[F],bit[$\\ell$],"
                    "fixed[$9$]fixed,fixed[$12$]byte,fixed[$12$]bit,byte[$12$]byte,byte[$12$]bit,bit[$12$]bit,"
                    "fixed[$18$]fixed,fixed[$21$]byte,fixed[$21$]bit,byte[$21$]byte,byte[$21$]bit,bit[$21$]bit,"
                    "fixed[$12$]fixed,fixed[$15$]byte,fixed[$15$]bit,byte[$15$]byte,byte[$15$]bit,bit[$15$]bit,"
                    "fixed[$15$]fixed,fixed[$18$]byte,fixed[$18$]bit,byte[$18$]byte,byte[$18$]bit,bit[$18$]bit,"
                    "fixed[$20$]fixed,fixed[$23$]byte,fixed[$23$]bit,byte[$23$]byte,byte[$23$]bit,bit[$23$]bit");

    cout << "size = " << size << ", queries = " << queries << " => fixed[F]:       "; bench.run<FixedF>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[l]:       "; bench.run<FixedL>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[F]:        "; bench.run<ByteF>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[l]:        "; bench.run<ByteL>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[F]:         "; bench.run<BitF>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[l]:         "; bench.run<BitL>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[9]fixed:  ";  bench.run<Fixed9Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]byte:  ";  bench.run<Fixed12Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[12]bit:   ";  bench.run<Fixed12Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]byte:   ";  bench.run<Byte12Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[12]bit:    ";  bench.run<Byte12Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[12]bit:     ";  bench.run<Bit12Bit>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[18]fixed: ";  bench.run<Fixed18Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[21]byte:  ";  bench.run<Fixed21Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[21]bit:   ";  bench.run<Fixed21Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[21]byte:   ";  bench.run<Byte21Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[21]bit:    ";  bench.run<Byte21Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[21]bit:     ";  bench.run<Bit21Bit>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[12]fixed: ";  bench.run<Fixed12Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]byte:  ";  bench.run<Fixed15Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[15]bit:   ";  bench.run<Fixed15Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]byte:   ";  bench.run<Byte15Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[15]bit:    ";  bench.run<Byte15Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[15]bit:     ";  bench.run<Bit15Bit>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[15]fixed: ";  bench.run<Fixed15Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[18]byte:  ";  bench.run<Fixed18Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[18]bit:   ";  bench.run<Fixed18Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[18]byte:   ";  bench.run<Byte18Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[18]bit:    ";  bench.run<Byte18Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[18]bit:     ";  bench.run<Bit18Bit>(); bench.separator();

    cout << "size = " << size << ", queries = " << queries << " => fixed[20]fixed: ";  bench.run<Fixed20Fixed>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[23]byte:  ";  bench.run<Fixed23Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => fixed[23]bit:   ";  bench.run<Fixed23Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[23]byte:   ";  bench.run<Byte23Byte>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => byte[23]bit:    ";  bench.run<Byte23Bit>(); bench.separator();
    cout << "size = " << size << ", queries = " << queries << " => bit[23]bit:     ";  bench.run<Bit23Bit>(); bench.separator("\n");

    return 0;
}
