#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>

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

struct Files {
    ofstream build, set, get, find, findc;

    Files(string path) :
        build(path + "build.csv", ofstream::out | ofstream::app),
        set(path + "set.csv",     ofstream::out | ofstream::app),
        get(path + "get.csv",     ofstream::out | ofstream::app),
        find(path + "find.csv",   ofstream::out | ofstream::app),
        findc(path + "findc.csv", ofstream::out | ofstream::app) {}
};

template<typename T>
void bench(Files &out, uint64_t *init, size_t *setidx, uint64_t *setval, size_t *getidx, uint64_t *findval, uint64_t *findcval, size_t size, size_t queries);
void write(Files &out, string str);

int main(int argc, char *argv[])
{
    random_device rd;
    mt19937 mte(rd());

    if (argc < 3) {
        cerr << "Not enough parameters\n";
        return -1;
    }

    size_t size, queries;
    if (!(istringstream(argv[2]) >> size) || !(istringstream(argv[3]) >> queries)) {
        cerr << "Invalid parameters\n";
        return -1;
    }

    uniform_int_distribution<uint64_t> distval(0, 64);
    uniform_int_distribution<size_t> distidx(0, size-1);

    uint64_t *init = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        init[i] = distval(mte);

    size_t *setidx = new size_t[queries];
    for (size_t i = 0; i < queries; i++)
        setidx[i] = distidx(mte);

    uint64_t *setval = new uint64_t[queries];
    for (size_t i = 0; i < queries; i++)
        setval[i] = distval(mte);

    size_t *getidx = new size_t[queries];
    for (size_t i = 0; i < queries; i++)
        getidx[i] = distidx(mte);

    uint64_t *findval = new uint64_t[queries];
    for (size_t i = 0; i < queries; i++) {
        for (size_t j = distidx(mte); j != -1ULL; j--)
            findval[i] += init[j];
    }

    uint64_t *findcval = new uint64_t[queries]();
    for (size_t i = 0; i < queries; i++) {
        for (size_t j = distidx(mte); j != -1ULL; j--)
            findcval[i] += init[j];
    }

    Files out(argv[1]);

    //write(out, "Elements,Naive,Type,LType,Byte,LByte,Bit,LBit"); write(out, "\n");
    write(out, to_string(size) + ",");
    bench<NaiveFenwickTree<64>>(out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, ",");
    bench<TypeFenwickTree<64>> (out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, ",");
    bench<LTypeFenwickTree<64>>(out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, ",");
    bench<ByteFenwickTree<64>> (out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, ",");
    bench<LByteFenwickTree<64>>(out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, ",");
    bench<BitFenwickTree<64>>  (out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, ",");
    bench<LBitFenwickTree<64>> (out, init, setidx, setval, getidx, findval, findcval, size, queries); write(out, "\n");

    delete[] init;
    delete[] setidx; delete[] setval;
    delete[] getidx;
    delete[] findval; delete[] findcval;

    return 0;
}


template<typename T>
void bench(Files &out, uint64_t *init, size_t *setidx, uint64_t *setval, size_t *getidx, uint64_t *findval, uint64_t *findcval, size_t size, size_t queries)
{
    const double c = 1. / queries;
    high_resolution_clock::time_point begin, end;
    uint64_t u = 0;

    cout << "Constructor... " << flush;
    begin = high_resolution_clock::now();
    T tree(init, size);
    end = high_resolution_clock::now();
    auto build = duration_cast<nanoseconds>(end-begin).count();
    out.build << to_string(build / (double)size);
    cout << "done." << endl;

    cout << "get... " << flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
        u ^= tree.get(getidx[i]);
    end = high_resolution_clock::now();
    auto get = duration_cast<nanoseconds>(end-begin).count();
    out.get << to_string(get * c);
    cout << "done." << endl;

    cout << "find... " << flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
        u ^= tree.find(findval[i]);
    end = high_resolution_clock::now();
    auto find = duration_cast<nanoseconds>(end-begin).count();
    out.find << to_string(find * c);
    cout << "done." << endl;

    cout << "set... " << flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
        tree.set(setidx[i], setval[i]);
    end = high_resolution_clock::now();
    auto set = duration_cast<nanoseconds>(end-begin).count();
    out.set << to_string(set * c);
    cout << "done." << endl;

    cout << "find_complement... " << flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
        u ^= tree.find_complement(findcval[i]);
    end = high_resolution_clock::now();
    auto findc = duration_cast<nanoseconds>(end-begin).count();
    out.findc << to_string(findc * c);
    cout << "done." << endl;

    const volatile uint64_t __attribute__((unused)) unused = u;
}

void write(Files &out, string str)
{
    out.build << str; out.get << str; out.set << str; out.find << str; out.findc << str;
}
