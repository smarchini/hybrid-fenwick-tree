#include <iostream>
#include "utils.hpp"

int main()
{
    using namespace std;
    using namespace dyn;

    cout << "Elements,Simple,Typed,Byte,Compact,Shrank\n";

    for (size_t size = 128; size < L2_CACHE_SIZE*4; size <<= 1) {
        uint64_t *increments = random_array<uint64_t>(size);
        uint64_t *sequence = new uint64_t[size];
        inc_to_seq(increments, sequence, size);
        random_shuffle(sequence, sequence+size);

        cout << size << ',';
        cout << bench_find<SimpleFenwickTree<7>> (size, increments, sequence) << ','
            // type aligned
             << bench_find<TypedFenwickTree<7>>  (size, increments, sequence) << ','
             << bench_find<ITypedFenwickTree<7>> (size, increments, sequence) << ','
            // byte aligned
             << bench_find<ByteFenwickTree<7>>   (size, increments, sequence) << ','
             << bench_find<IByteFenwickTree<7>>   (size, increments, sequence) << ','
            // bit aligned
             << bench_find<CompactFenwickTree<7>>(size, increments, sequence) << ','
             << bench_find<ShrankFenwickTree<7>> (size, increments, sequence) << '\n';

        delete[] increments;
        delete[] sequence;
    }

    return 0;
}
