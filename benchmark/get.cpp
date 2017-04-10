#include <iostream>
#include "bench_utils.hpp"

int main()
{
    using namespace std;

    cout << "Elements,Simple,Typed,Byte,Compact,Shrank\n";

    for (size_t size = 64; size < L2_CACHE_SIZE*1000; size <<= 1) {
        uint64_t *increments = random_array<uint64_t>(size);
        uint64_t *order = new uint64_t[size];
        for (size_t i = 0; i < size; i++)
            order[i] = i;
        random_shuffle(order, order+size);

        cout << size << ',';
        cout << bench_get<SimpleFenwickTree> (size, increments, order) << ','
             << bench_get<TypedFenwickTree>  (size, increments, order) << ','
             << bench_get<ByteFenwickTree>   (size, increments, order) << ','
             << bench_get<CompactFenwickTree>(size, increments, order) << ','
             << bench_get<ShrankFenwickTree> (size, increments, order) << '\n';

        delete[] increments;
        delete[] order;
    }

    return 0;
}
