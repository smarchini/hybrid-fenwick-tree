#include <iostream>
#include "utils.hpp"

int main()
{
    using namespace std;
    using namespace dyn;

    cout << "Elements,Naive,LType,Type,LByte,LBit,Bit\n";

    for (size_t size = 64; size < L3_CACHE_SIZE*2; size <<= 1) {
        uint64_t *increments = random_array<uint64_t>(size);
        uint64_t *order = new uint64_t[size];
        for (size_t i = 0; i < size; i++)
            order[i] = i;
        random_shuffle(order, order+size);

        cout << size << ',';
        cout << bench_get<NaiveFenwickTree<7>> (size, increments, order) << ','
            // type aligned
             << bench_get<LTypeFenwickTree<7>>  (size, increments, order) << ','
             << bench_get<TypeFenwickTree<7>> (size, increments, order) << ','
            // byte aligned
             << bench_get<ByteFenwickTree<7>>   (size, increments, order) << ','
             << bench_get<ByteFenwickTree<7>>  (size, increments, order) << ','
            // bit aligned
             << bench_get<LBitFenwickTree<7>>(size, increments, order) << ','
             << bench_get<BitFenwickTree<7>> (size, increments, order) << '\n';

        delete[] increments;
        delete[] order;
    }

    return 0;
}
