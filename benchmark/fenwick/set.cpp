#include <iostream>
#include "utils.hpp"

int main()
{
    using namespace std;
    using namespace dyn;

    cout << "Elements,Naive,LType,LByte,LBit,Bit\n";

    for (size_t size = 128; size < L2_CACHE_SIZE*4; size <<= 1) {
        uint64_t *increments = random_array<uint64_t>(size);
        uint64_t *order = new uint64_t[size];
        for (size_t i = 0; i < size; i++)
            order[i] = i;
        std::random_shuffle(order, order+size);

        int64_t *value = random_array<int64_t>(size);
        for (size_t i = 0; i < size; i++) {
            int inc = value[i] - value[i];
            if (inc < 0) inc = -inc;
            value[i] = (increments[i] + inc) < 64 ? inc : 0;
        }

        cout << size << ',';
        cout << bench_set<NaiveFenwickTree<64>> (size, increments, order, value) << ','
            // type aligned
             << bench_set<LTypeFenwickTree<64>>  (size, increments, order, value) << ','
             << bench_set<TypeFenwickTree<64>> (size, increments, order, value) << ','
            // byte aligned
             << bench_set<LByteFenwickTree<64>>   (size, increments, order, value) << ','
             << bench_set<ByteFenwickTree<64>>  (size, increments, order, value) << ','
            // bit aligned
             << bench_set<LBitFenwickTree<64>>(size, increments, order, value) << ','
             << bench_set<BitFenwickTree<64>> (size, increments, order, value) << '\n';

        delete[] increments;
        delete[] order;
        delete[] value;
    }

    return 0;
}
