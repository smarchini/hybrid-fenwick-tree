#ifndef __BENCH_UTILS_H__
#define __BENCH_UTILS_H__

#ifndef L1_CACHE_SIZE
#error "L1_CACHE_SIZE is undefined"
#endif

#ifndef L2_CACHE_SIZE
#error "L2_CACHE_SIZE is undefined"
#endif

#include <chrono>
#include <algorithm>
#include <random>

#include "../include/fenwick/simple.hpp"
#include "../include/fenwick/typed.hpp"
#include "../include/fenwick/byte.hpp"
#include "../include/fenwick/compact.hpp"
#include "../include/fenwick/shrank.hpp"


template<typename T> T* random_array(size_t size);
void inc_to_seq(std::uint64_t *inc, std::uint64_t *seq, std::size_t size);
template<typename T> double bench_get(std::size_t size, std::uint64_t increments[], std::uint64_t order[]);
template<typename T> double bench_set(std::size_t size, std::uint64_t increments[], std::uint64_t order[], std::int64_t values[]);
template<typename T> double bench_find(std::size_t size, std::uint64_t increments[], std::uint64_t elements[]);


template<typename T>
T* random_array(size_t size)
{
    using namespace std;
    static random_device rd;
    static mt19937 mte(rd());

    uniform_int_distribution<T> dist(0, 64);

    T *array = new T[size];
    for (size_t i = 0; i < size; i++)
        array[i] = dist(mte);

    return array;
}

void inc_to_seq(std::uint64_t *inc, std::uint64_t *seq, std::size_t size)
{
    seq[0] = inc[0];
    for (std::size_t i = 1; i < size; i++)
        seq[i] = seq[i-1] + inc[i];
}


template<typename T>
double bench_get(std::size_t size, std::uint64_t increments[], std::uint64_t order[])
{
    using namespace std;
    uint64_t u = 0;

    T tree(increments, size);

    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++)
        u ^= tree.get(order[i]);
    auto end = chrono::high_resolution_clock::now();
    auto get = chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / size;
    return get * c;
}

template<typename T>
double bench_set(std::size_t size, std::uint64_t increments[], std::uint64_t order[], std::int64_t values[])
{
    using namespace std;

    T tree(increments, size);

    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++)
        tree.set(order[i], values[i]);
    auto end = chrono::high_resolution_clock::now();
    auto set = chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    const double c = 1. / size;
    return set * c;
}

template<typename T>
double bench_find(std::size_t size, std::uint64_t increments[], std::uint64_t elements[])
{
    using namespace std;
    uint64_t u = 0;

    T tree(increments, size);

    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++)
        u ^= tree.find(elements[i]);
    auto end = chrono::high_resolution_clock::now();
    auto find = chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    const volatile uint64_t __attribute__((unused)) unused = u;

    const double c = 1. / size;
    return find * c;
}


#endif // __BENCH_UTILS_H__
