#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

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

    unique_ptr<size_t[]> setidx = make_unique<size_t[]>(queries),
        getidx = make_unique<size_t[]>(queries);

    unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size),
        setval   = make_unique<uint64_t[]>(queries),
        findval  = make_unique<uint64_t[]>(queries),
        findcval = make_unique<uint64_t[]>(queries);

    ofstream fbuild, fset, fget, ffind, ffindc;

public:
    Benchmark(string path, size_t size, size_t queries) :
        path(path),
        size(size),
        queries(queries) {}

    ~Benchmark() {
        fbuild.close();
        fset.close();
        fget.close();
        ffind.close();
        ffindc.close();
    }

    void filesinit(string order) {
        finit(fbuild, "build.csv", "Elements," + order);
        finit(fset,   "set.csv",   "Elements," + order);
        finit(fget,   "get.csv",   "Elements," + order);
        finit(ffind,  "find.csv",  "Elements," + order);
        finit(ffindc, "findc.csv", "Elements," + order);
    }

    void separator(string sep = ",") {
        fbuild << sep;
        fset << sep;
        fget << sep;
        ffind << sep;
        ffindc << sep;
    }

    void datainit(mt19937 &mte) {
        uniform_int_distribution<uint64_t> distval(0, 64);
        uniform_int_distribution<size_t> distidx(0, size-1);

        for (size_t i = 0; i < size; i++)
            sequence[i] = distval(mte);

        for (size_t i = 0; i < queries; i++) {
            setidx[i] = distidx(mte);
            setval[i] = distval(mte);
            getidx[i] = distidx(mte);

            findval[0] = sequence[0];
            for (size_t j = distidx(mte); j != 0; j--)
                findval[i] += sequence[j];

            findcval[0] = sequence[0];
            for (size_t j = distidx(mte); j != 0; j--)
                findcval[i] += sequence[j];
        }
    }

    template<typename T>
    void run() {
        const double c = 1. / queries;
        high_resolution_clock::time_point begin, end;
        uint64_t u = 0;

        cout << "Constructor... " << flush;
        begin = high_resolution_clock::now();
        T tree(sequence.get(), size);
        end = high_resolution_clock::now();
        auto build = duration_cast<nanoseconds>(end-begin).count();
        fbuild << to_string(build / (double)size);

        cout << "get... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < queries; i++)
            u ^= tree.get(getidx[i]);
        end = high_resolution_clock::now();
        auto get = duration_cast<nanoseconds>(end-begin).count();
        fget << to_string(get * c);

        cout << "find... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < queries; i++)
            u ^= tree.find(findval[i]);
        end = high_resolution_clock::now();
        auto find = duration_cast<nanoseconds>(end-begin).count();
        ffind << to_string(find * c);

        cout << "set... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < queries; i++)
            tree.set(setidx[i], setval[i]);
        end = high_resolution_clock::now();
        auto set = duration_cast<nanoseconds>(end-begin).count();
        fset << to_string(set * c);

        cout << "find_complement... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < queries; i++)
            u ^= tree.find_complement(findcval[i]);
        end = high_resolution_clock::now();
        auto findc = duration_cast<nanoseconds>(end-begin).count();
        ffindc << to_string(findc * c);
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

    size_t size, queries;
    if (argc < 3 || !(istringstream(argv[2]) >> size) || !(istringstream(argv[3]) >> queries)) {
        cerr << "Invalid parameters" << endl;
        return -1;
    }

    Benchmark bench(argv[1], size, queries);

    bench.datainit(mte);
    bench.filesinit("Naive,Type,LType,Byte,LByte,Bit,LBit");

    cout << "Naive(" << size << ", " << queries << "): "; bench.run<NaiveFenwickTree<64>>(); bench.separator();
    cout << "Type(" << size << ", " << queries << "):  "; bench.run<TypeFenwickTree<64>>();  bench.separator();
    cout << "LType(" << size << ", " << queries << "): "; bench.run<LTypeFenwickTree<64>>(); bench.separator();
    cout << "Byte(" << size << ", " << queries << "):  "; bench.run<ByteFenwickTree<64>>();  bench.separator();
    cout << "LByte(" << size << ", " << queries << "): "; bench.run<LByteFenwickTree<64>>(); bench.separator();
    cout << "Bit(" << size << ", " << queries << "):   "; bench.run<BitFenwickTree<64>>();   bench.separator();
    cout << "LBit(" << size << ", " << queries << "):  "; bench.run<LBitFenwickTree<64>>();  bench.separator("\n");

    return 0;
}
