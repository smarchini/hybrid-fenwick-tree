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
#include <fenwick/type.hpp>
#include <fenwick/ltype.hpp>
#include <fenwick/byte.hpp>
#include <fenwick/lbyte.hpp>
#include <fenwick/bit.hpp>
#include <fenwick/lbit.hpp>
#include <fenwick/mixed.hpp>

using namespace std;
using namespace dyn;
using namespace std::chrono;

class Benchmark {
private:
    string path;
    size_t size, queries;

    uint64_t ones = 0, zeroes = 0;

    unique_ptr<size_t[]> updateidx = make_unique<size_t[]>(queries),
        rank0 = make_unique<size_t[]>(queries),
        rank1 = make_unique<size_t[]>(queries);

    unique_ptr<uint64_t[]> bitvector = make_unique<uint64_t[]>(size),
        select0    = make_unique<uint64_t[]>(queries),
        select1    = make_unique<uint64_t[]>(queries),
        updateval  = make_unique<uint64_t[]>(queries);

    ofstream fbuild, frank0, frank1, fselect0, fselect1, fupdate;

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
    }

    void filesinit(string order) {
        finit(fbuild,   "build.csv",   "Elements," + order);
        finit(frank0,   "rank0.csv",   "Elements," + order);
        finit(frank1,   "rank1.csv",   "Elements," + order);
        finit(fselect0, "select0.csv", "Elements," + order);
        finit(fselect1, "select1.csv", "Elements," + order);
        finit(fupdate,  "update.csv",  "Elements," + order);
    }

    void separator(string sep = ",") {
        fbuild << sep;
        frank0 << sep;
        frank1 << sep;
        fselect0 << sep;
        fselect1 << sep;
        fupdate << sep;
    }

    void datainit(mt19937 &mte) {
        uniform_int_distribution<uint64_t> bvdist(0, UINT64_MAX);
        uniform_int_distribution<size_t> idxdist(0, size-1);

        for (size_t i = 0; i < size; i++) {
            bitvector[i] = bvdist(mte);
            ones += popcount(bitvector[i]);
        }

        zeroes = 64*size - ones;

        uniform_int_distribution<uint64_t> sel0dist(0, zeroes-1);
        uniform_int_distribution<uint64_t> sel1dist(0, ones-1);

        for (size_t i = 0; i < queries; i++) {
            rank0[i] = idxdist(mte);
            rank1[i] = idxdist(mte);
            select0[i] = sel0dist(mte);
            select1[i] = sel1dist(mte);
            updateval[i] = bvdist(mte);
            updateidx[i] = idxdist(mte);
        }
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
            u ^= bv.rankZero(rank0[i]);
        end = high_resolution_clock::now();
        auto rank0 = duration_cast<chrono::nanoseconds>(end-begin).count();
        frank0 << to_string(rank0 * c);

        cout << "select0... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.selectZero(select0[i]);
        end = high_resolution_clock::now();
        auto select0 = duration_cast<chrono::nanoseconds>(end-begin).count();
        fselect0 << to_string(select0 * c);

        cout << "rank1... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.rank(rank1[i]);
        end = high_resolution_clock::now();
        auto rank1 = duration_cast<chrono::nanoseconds>(end-begin).count();
        frank1 << to_string(rank1 * c);

        cout << "select1... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.select(select1[i]);
        end = high_resolution_clock::now();
        auto select1 = duration_cast<chrono::nanoseconds>(end-begin).count();
        fselect1 << to_string(select1 * c);

        cout << "update... " << flush;
        begin = high_resolution_clock::now();
        for(uint64_t i = 0; i < queries; ++i)
            u ^= bv.update(updateidx[i], updateval[i]);
        end = high_resolution_clock::now();
        auto update = duration_cast<chrono::nanoseconds>(end-begin).count();
        fupdate << to_string(update * c);
        cout << "done.  " << endl;

        const volatile uint64_t __attribute__((unused)) unused = u;
    }

    // TODO: run_dynamic

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
    bench.filesinit("Naive1,Type1,LType1,Byte1,LByte1,Bit1,LBit1");

    cout << "Naive1(" << size << ", " << queries << "): "; bench.run<WordRankSelect<NaiveFenwickTree>>(); bench.separator();
    cout << "Type1(" << size << ", " << queries << "):  "; bench.run<WordRankSelect<TypeFenwickTree>>();  bench.separator();
    cout << "LType1(" << size << ", " << queries << "): "; bench.run<WordRankSelect<LTypeFenwickTree>>(); bench.separator();
    cout << "Byte1(" << size << ", " << queries << "):  "; bench.run<WordRankSelect<ByteFenwickTree>>();  bench.separator();
    cout << "LByte1(" << size << ", " << queries << "): "; bench.run<WordRankSelect<LByteFenwickTree>>(); bench.separator();
    cout << "Bit1(" << size << ", " << queries << "):   "; bench.run<WordRankSelect<BitFenwickTree>>();   bench.separator();
    cout << "LBit1(" << size << ", " << queries << "):  "; bench.run<WordRankSelect<LBitFenwickTree>>();  bench.separator("\n");

    return 0;
}
