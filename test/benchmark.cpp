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

template<typename T>
void bench(const char* name, std::size_t size, std::uint64_t order[], std::uint64_t increments[], std::uint64_t set_updates[]);

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Not enough parameters\n";
        return -1;
    }

    std::istringstream iss(argv[1]);

    std::size_t size;
    if (!(iss >> size)) {
        std::cerr << "Invalid number " << argv[1] << '\n';
        return -1;
    }

    std::uint64_t *order = new std::uint64_t[size];
    for (std::size_t i = 0; i < size; i++)
        order[i] = i;
    std::random_shuffle(order, order+size);

    std::uint64_t *increments = new std::uint64_t[size];
    std::uint64_t *set_updates = new std::uint64_t[size];

    fill_with_random_values(increments, size);
    fill_with_random_values(set_updates, size);
    for (std::size_t i = 0; i < size; i++) {
        int inc = set_updates[i] - increments[i];
        if (inc < 0) inc = -inc;
        set_updates[i] = (increments[i] + inc) < 64 ? inc : 0;
    }

    bench<SimpleFenwickTree>("SimpleFenwickTree", size, order, increments, set_updates);
    bench<CompactFenwickTree>("CompactFenwickTree", size, order, increments, set_updates);
    bench<ByteFenwickTree>("ByteFenwickTree", size, order, increments, set_updates);
    bench<TypedFenwickTree>("TypedFenwickTree", size, order, increments, set_updates);
    bench<ShrankFenwickTree>("ShrankFenwickTree", size, order, increments, set_updates);

    delete[] increments;
    delete[] set_updates;
    delete[] order;

    return 0;
}

template<typename T>
void bench(const char* name, std::size_t size, std::uint64_t order[], std::uint64_t increments[], std::uint64_t set_updates[])
{
    std::chrono::high_resolution_clock::time_point begin, end;
    std::uint64_t *array = new std::uint64_t[size+1]; // in order to avoid compilers dead code elimination
    std::uint64_t *sequence = new std::uint64_t[size];
    increments_to_sequence(increments, sequence, size);

    // constructor
    begin = std::chrono::high_resolution_clock::now();
    T tree(increments, size);
    end = std::chrono::high_resolution_clock::now();
    auto constructor = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    // get
    begin = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i <= size; i++)
        array[i] = tree.get(order[i]);
    end = std::chrono::high_resolution_clock::now();
    auto get = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    // set
    begin = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < size; i++)
        tree.set(order[i], set_updates[i]);
    end = std::chrono::high_resolution_clock::now();
    auto set = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    // find
    begin = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < size; i++)
        array[i] = tree.find(sequence[i]);
    end = std::chrono::high_resolution_clock::now();
    auto find = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    std::cout << "\n" << name << " time:\n";
    std::cout << "biuld: " << std::right << std::setw(12) << constructor << " nsec\n";
    std::cout << "get:   " << std::right << std::setw(12) << get << " nsec\n";
    std::cout << "set:   " << std::right << std::setw(12) << set << " nsec\n";
    std::cout << "find:  " << std::right << std::setw(12) << find << " nsec\n";

    delete[] array;
    delete[] sequence;
}
