#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "utils.hpp"

using namespace std;

template<typename T> void microbench(const char* name, size_t size, uint64_t *increments);
uint64_t* random_array(size_t size);

int main()
{
    using namespace hft;

    for (size_t i = 2; i < L2_CACHE_SIZE; i <<= 1) {
        uint64_t *increments = random_array(i-1);

        cout << i << " Elements\n";
        microbench<fenwick::FixedF>("FixedF", i-1, increments);
        microbench<fenwick::TypeL>("TypeL", i-1, increments);
        microbench<fenwick::TypeF>("TypeF", i-1, increments);
        microbench<fenwick::ByteL>("ByteL", i-1, increments);
        microbench<fenwick::ByteF>("ByteF", i-1, increments);
        microbench<fenwick::BitL>("BitL", i-1, increments);
        microbench<fenwick::BitF>("BitF", i-1, increments);
        cout << "------------------------------------------------------------\n\n";

        delete[] increments;
    }

    return 0;
}

uint64_t* random_array(size_t size)
{
    static random_device rd;
    static mt19937 mte(rd());

    uniform_int_distribution<uint64_t> dist(0, 64);

    uint64_t *array = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        array[i] = dist(mte);

    return array;
}

// TODO: rivedere, i numeri non tornano
template<typename T>
void microbench(const char* name, size_t size, uint64_t *increments)
{
    constexpr size_t multiplier = 100000;
    uint64_t u = 0;
    chrono::high_resolution_clock::time_point begin, end;

    static random_device rd;
    static mt19937 mte(rd());
    uniform_int_distribution<size_t> index(0, size-1);
    uniform_int_distribution<uint64_t> value(0, 64*size);

    size_t prefix_idx = index(mte), add_idx = index(mte);
    uint64_t find_val = value(mte), add_val = value(mte);

    // constructor
    begin = chrono::high_resolution_clock::now();
    T tree(increments, size);
    end = chrono::high_resolution_clock::now();
    auto constructor = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    // prefix
    begin = chrono::high_resolution_clock::now();
    for (size_t i = multiplier; i != 0; i--)
        u ^= tree.prefix(prefix_idx);
    end = chrono::high_resolution_clock::now();
    auto prefix = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    // find
    begin = chrono::high_resolution_clock::now();
    for (size_t i = multiplier; i != 0; i--)
        u ^= tree.find(find_val);
    end = chrono::high_resolution_clock::now();
    auto find = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    // add
    begin = chrono::high_resolution_clock::now();
    for (size_t i = multiplier; i != 0; i--)
        tree.add(add_idx, add_val * (i % 2? 1 : -1));
    end = chrono::high_resolution_clock::now();
    auto add = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / multiplier; // size;
    cout << "\n" << name << ": " << tree.bitCount() / (double)size << " b/item\n";
    cout << "build: " << fixed << setw(12) << constructor * c << " ns/item\n";
    cout << "prefix:   " << fixed << setw(12) << prefix * c << " ns/item\n";
    cout << "add:   " << fixed << setw(12) << add * c << " ns/item\n";
    cout << "find:  " << fixed << setw(12) << find * c << " ns/item\n";
}
