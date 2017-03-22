#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "test_utils.hpp"

#include "../include/fenwick_tree.hpp"
#include "../include/simple_fenwick_tree.hpp"
#include "../include/byte_fenwick_tree.hpp"
#include "../include/compact_fenwick_tree.hpp"
#include "../include/typed_fenwick_tree.hpp"


using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

template<typename T>
void bench(const char* name, size_t size, uint64_t order[], uint64_t increments[], uint64_t set_updates[], uint64_t sequence[]);

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Not enough parameters\n";
        return -1;
    }

    std::istringstream iss(argv[1]);

    size_t size;
    if (!(iss >> size)) {
        std::cerr << "Invalid number " << argv[1] << '\n';
        return -1;
    }

    uint64_t *order = new uint64_t[size];
    for (size_t i = 0; i < size; i++)
        order[i] = i;
    std::random_shuffle(order, order+size);

    uint64_t *increments = new uint64_t[size];
    uint64_t *set_updates = new uint64_t[size];

    fill_with_random_values(increments, size);
    fill_with_random_values(set_updates, size);
    for (size_t i = 0; i < size; i++) {
        int inc = set_updates[i] - increments[i];
        if (inc < 0) inc = -inc;
        set_updates[i] = (increments[i] + inc) < 64 ? inc : 0;
    }

    uint64_t *sequence = new uint64_t[size];
    increments_to_sequence(increments, sequence, size);
    std::random_shuffle(sequence, sequence+size);

    bench<SimpleFenwickTree>("SimpleFenwickTree", size, order, increments, set_updates, sequence);
    bench<TypedFenwickTree>("TypedFenwickTree", size, order, increments, set_updates, sequence);
    bench<ByteFenwickTree>("ByteFenwickTree", size, order, increments, set_updates, sequence);
    bench<CompactFenwickTree>("CompactFenwickTree", size, order, increments, set_updates, sequence);
    bench<ShrankFenwickTree>("ShrankFenwickTree", size, order, increments, set_updates, sequence);

    delete[] increments;
    delete[] set_updates;
    delete[] order;
    delete[] sequence;

    return 0;
}

template<typename T>
void bench(const char* name, size_t size, uint64_t order[], uint64_t increments[], uint64_t set_updates[], uint64_t sequence[])
{
    std::chrono::high_resolution_clock::time_point begin, end;
    uint64_t u = 0;

    // constructor
    begin = std::chrono::high_resolution_clock::now();
    T tree(increments, size);
    end = std::chrono::high_resolution_clock::now();
    auto constructor = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    // get
    begin = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++)
        u ^= tree.get(order[i]);
    end = std::chrono::high_resolution_clock::now();
    auto get = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    // find
    begin = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++)
        u ^= tree.find(sequence[i]);
    end = std::chrono::high_resolution_clock::now();
    auto find = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    // set
    begin = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++)
        tree.set(order[i], set_updates[i]);
    end = std::chrono::high_resolution_clock::now();
    auto set = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / size;
    std::cout << "\n" << name << ": " << tree.bit_count() / (double)size << " b/item\n";
    std::cout << "build: " << std::fixed << std::setw(12) << constructor * c << " ns/item\n";
    std::cout << "get:   " << std::fixed << std::setw(12) << get * c << " ns/item\n";
    std::cout << "set:   " << std::fixed << std::setw(12) << set * c << " ns/item\n";
    std::cout << "find:  " << std::fixed << std::setw(12) << find * c << " ns/item\n";
}
