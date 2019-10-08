#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include <fenwick.hpp>

using namespace std;
using namespace hft::fenwick;
using namespace std::chrono;

template <size_t N> using Fixed20Byte = Hybrid<FixedL, ByteF, N, 20>;
template <size_t N> using Fixed20Bit = Hybrid<FixedL, BitF, N, 20>;
template <size_t N> using Fixed26Byte = Hybrid<FixedL, ByteF, N, 26>;
template <size_t N> using Fixed26Bit = Hybrid<FixedL, BitF, N, 26>;

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
  Benchmark(string path, size_t size, size_t queries) : path(path), size(size), queries(queries) {}

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
    cumseqdist = uniform_int_distribution<uint64_t>(0, BOUND * size);

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
      for (uint64_t i = 0; i < queries; ++i) {
        u ^= tree.prefix(idxdist(mte) ^ (u & 1));
        // u ^= tree.prefix(idxdist(mte));
      }
      end = high_resolution_clock::now();
      prefix.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
    }
    std::sort(prefix.begin(), prefix.end());
    fprefix << to_string(prefix[MID] * c);

    cout << "find: " << flush;
    vector<chrono::nanoseconds::rep> find;
    for (int r = 0; r < REPS; r++) {
      cout << r << " " << flush;
      begin = high_resolution_clock::now();
      for (uint64_t i = 0; i < queries; ++i) {
        u ^= tree.find(cumseqdist(mte) ^ (u & 1));
        // u ^= tree.find(cumseqdist(mte));
      }
      end = high_resolution_clock::now();
      find.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
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
        int64_t val = seqdist(mte) ^ (u & 1);
        // tree.add(idx, i % 2 ? -val : val);
        tree.add(idx, sequence[idx] + val < BOUND ? val : -val);
        // size_t idx = idxdist(mte);
        // uint64_t val = seqdist(mte);
        // tree.add(idx, sequence[idx] + val < BOUND ? val : -val);
      }
      end = high_resolution_clock::now();
      add.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
    }
    std::sort(add.begin(), add.end());
    fadd << to_string(add[MID] * c);

    // the compiler can't erase the adds
    u ^= tree.prefix(idxdist(mte));

    cout << "bitspace... " << flush;
    fbitspace << to_string(tree.bitCount() / (size * 64.));
    cout << "done.  " << endl;

    const volatile uint64_t __attribute__((unused)) unused = u;
  }

private:
  bool is_empty(ifstream file) { return file.peek() == ifstream::traits_type::eof(); }

  void finit(ofstream &file, string filename, string text) {
    file.open(path + filename, ios::out | ios::app);

    if (is_empty(ifstream(path + filename))) {
      cout << "Initializing: " << path << filename << endl;
      file << text << endl;
    }

    file << size << ",";
  }
};

int main(int argc, char *argv[]) {
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
                  "fixed[20]byte,fixed[20]bit,fixed[26]byte,fixed[26]bit");

  cout << "size = " << size << ", queries = " << queries << " => fixed[F]:       ";
  bench.run<FixedF>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => fixed[l]:       ";
  bench.run<FixedL>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => byte[F]:        ";
  bench.run<ByteF>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => byte[l]:        ";
  bench.run<ByteL>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => bit[F]:         ";
  bench.run<BitF>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => bit[l]:         ";
  bench.run<BitL>();
  bench.separator();

  cout << "size = " << size << ", queries = " << queries << " => fixed[20]byte:  ";
  bench.run<Fixed20Byte>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => fixed[20]bit:   ";
  bench.run<Fixed20Bit>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => fixed[26]byte:  ";
  bench.run<Fixed26Byte>();
  bench.separator();
  cout << "size = " << size << ", queries = " << queries << " => fixed[26]bit:   ";
  bench.run<Fixed26Bit>();
  bench.separator("\n");

  return 0;
}
