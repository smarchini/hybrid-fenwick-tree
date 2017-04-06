#ifndef L1_CACHE_SIZE
#error "L1_CACHE_SIZE is undefined"
#endif

#ifndef L2_CACHE_SIZE
#error "L2_CACHE_SIZE is undefined"
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "../test/test_utils.hpp"

#include "../include/fenwick_tree.hpp"
#include "../include/simple_fenwick_tree.hpp"
#include "../include/byte_fenwick_tree.hpp"
#include "../include/compact_fenwick_tree.hpp"
#include "../include/typed_fenwick_tree.hpp"

using namespace std;

template<typename T> void microbench(const char* name, size_t size, uint64_t *increments);
uint64_t* random_array(size_t size);

int main()
{
    // TODO: output comodo da parsare (generazione grafico automatica in python)
    for (size_t i = 2; i < L2_CACHE_SIZE; i <<= 1) {
        uint64_t *increments = random_array(i-1);

        cout << i << " Elements\n";
        microbench<SimpleFenwickTree>("SimpleFenwickTree", i-1, increments);
        microbench<TypedFenwickTree>("TypedFenwickTree", i-1, increments);
        microbench<ByteFenwickTree>("ByteFenwickTree", i-1, increments);
        microbench<CompactFenwickTree>("CompactFenwickTree", i-1, increments);
        microbench<ShrankFenwickTree>("ShrankFenwickTree", i-1, increments);
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

    size_t get_idx = index(mte), set_idx = index(mte);
    uint64_t find_val = value(mte), set_val = value(mte);

    // constructor
    begin = chrono::high_resolution_clock::now();
    T tree(increments, size);
    end = chrono::high_resolution_clock::now();
    auto constructor = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    // get
    begin = chrono::high_resolution_clock::now();
    for (size_t i = multiplier; i != 0; i--)
        u ^= tree.get(get_idx);
    end = chrono::high_resolution_clock::now();
    auto get = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    // find
    begin = chrono::high_resolution_clock::now();
    for (size_t i = multiplier; i != 0; i--)
        u ^= tree.find(find_val);
    end = chrono::high_resolution_clock::now();
    auto find = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    // set
    begin = chrono::high_resolution_clock::now();
    for (size_t i = multiplier; i != 0; i--)
        tree.set(set_idx, set_val * (i % 2? 1 : -1));
    end = chrono::high_resolution_clock::now();
    auto set = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / multiplier; // size;
    cout << "\n" << name << ": " << tree.bit_count() / (double)size << " b/item\n";
    cout << "build: " << fixed << setw(12) << constructor * c << " ns/item\n";
    cout << "get:   " << fixed << setw(12) << get * c << " ns/item\n";
    cout << "set:   " << fixed << setw(12) << set * c << " ns/item\n";
    cout << "find:  " << fixed << setw(12) << find * c << " ns/item\n";
}
