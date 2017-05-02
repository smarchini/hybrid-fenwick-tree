#include <iostream>
#include "utils.hpp"

int main()
{
    using namespace std;
    using namespace dyn;

    cout << "Elements,Simple,Typed,ITyped,Byte,Compact,Shrank\n";

    for (size_t size = 64; size < L2_CACHE_SIZE*1000; size <<= 1) {
        uint64_t *increments = random_array<uint64_t>(size);
        uint64_t *order = new uint64_t[size];
        for (size_t i = 0; i < size; i++)
            order[i] = i;
        random_shuffle(order, order+size);

        cout << size << ',';
        cout << bench_get<SimpleFenwickTree<7>> (size, increments, order) << ','
             << bench_get<TypedFenwickTree<7>>  (size, increments, order) << ','
             << bench_get<ITypedFenwickTree<7>> (size, increments, order) << ','
             << bench_get<ByteFenwickTree<7>>   (size, increments, order) << ','
             << bench_get<CompactFenwickTree<7>>(size, increments, order) << ','
             << bench_get<ShrankFenwickTree<7>> (size, increments, order) << '\n';

        delete[] increments;
        delete[] order;
    }

    return 0;
}
