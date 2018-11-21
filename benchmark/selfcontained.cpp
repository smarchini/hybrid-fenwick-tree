/*
 * Self contained file (i.e. it does only #include the STL) to check the
 * differences between top-down and bottom-up (update and interrogation) tree
 * traversal.
 *
 * For the level ordered tree (FenwickL) we have:
 *     topdown_function(j): from the "root" to the j (convert every time);
 *     bottomup_function(j): from j to the "root" (convert every time);
 *     level_function(j): from j to the "root" (direct formulae).
 *
 * For the classical tree (FenwickF) we only have topdown_function and
 * fenwick_function.
 *
 * Compile with: g++ -O3 -march=native -o selfcontained selfcontained.cpp
 *
 */

#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <climits>

// common.hpp functions
inline int lambda(uint64_t word) { return 63 ^ __builtin_clzll(word); }
inline int rho(uint64_t word) { return __builtin_ctzll(word); }
inline uint64_t mask_rho(uint64_t word) { return word & (-word); }
inline uint64_t mask_lambda(uint64_t word) { return 0x8000000000000000 >> __builtin_clzll(word); }
inline uint64_t clear_rho(uint64_t word) { return word & (word - 1ULL); }
inline size_t updroot(size_t j, size_t n) { return n & (-1ULL << lambda((j ^ n) | mask_rho(j))); }


class FenwickL
{
public:
    static constexpr size_t LEAF_MAXVAL = 64; // Leaves store values up to 64
    static constexpr size_t LEAF_BITSIZE = 9; // log2(65);

protected:
    std::unique_ptr<uint64_t[]> tree;
    std::unique_ptr<size_t[]> level;
    size_t size, levels;

public:
    FenwickL(uint64_t sequence[], size_t size):
        tree(std::make_unique<size_t[]>(size)),
        level(std::make_unique<uint64_t[]>(lambda(size) + 2)),
        size(size),
        levels(lambda(size)+1)
    {
        size_t level_size = lambda(size) + 2;

        level[0] = 0;
        for (size_t i = 1; i < level_size; i++)
            level[i] = ((size + (1<<(i-1))) / (1<<i)) + level[i-1];

        const size_t levels = level_size - 1;
        for (size_t l = 0; l < levels; l++) {
            for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
                size_t sequence_idx = node-1;
                uint64_t value = sequence[sequence_idx];
                for (size_t j = 0; j < l; j++) {
                    sequence_idx >>= 1;
                    value += tree[level[j] + sequence_idx];
                }

                tree[level[l] + (node >> (l+1))] = value;
            }
        }
    }

    uint64_t topdown_prefix(size_t idx) const
    {
        uint64_t sum = 0;
        size_t index = 0;

        while (idx != index) {
            index ^= mask_lambda(idx ^ index);

            const int height = rho(index);
            const size_t level_idx = index >> (1 + height);
            sum += tree[level[height] + level_idx];
        }

        return sum;
    }

    uint64_t bottomup_prefix(size_t idx) const
    {
        uint64_t sum = 0;

        while (idx != 0) {
            const int height = rho(idx);
            const size_t level_idx = idx >> (1 + height);
            sum += tree[level[height] + level_idx];

            idx = clear_rho(idx);
        }

        return sum;
    }

    uint64_t level_prefix(size_t idx) const
    {
        if (idx == 0) return 0; // ugly

        size_t height = rho(idx); // undefined behavior in zero
        size_t level_idx = idx >> (1 + height);
        uint64_t sum = tree[level[height] + level_idx];

        while (0 < level_idx && level_idx < level[height+1]) {
            const int next = rho(level_idx) + 1;
            height += next;
            level_idx >>= next;

            sum += tree[level[height] + level_idx];
        }

        return sum;
    }

    void bottomup_add(size_t idx, int64_t inc)
    {
        while (idx <= size) {
            const int height = rho(idx);
            const size_t level_idx = idx >> (1 + height);
            tree[level[height] + level_idx] += inc;
            idx += mask_rho(idx);
        }
    }

    void topdown_add(size_t idx, int64_t inc)
    {
        size_t negindex = -updroot(idx, size);
        const size_t negidx = -idx;

        while (negindex != negidx) {
            const int height = rho(-negindex);
            const size_t level_idx = (-negindex) >> (1 + height);
            tree[level[height] + level_idx] += inc;
            negindex ^= 1ULL << lambda(negindex ^ negidx);
        }

        const int height = rho(idx);
        const size_t level_idx = idx >> (1 + height);
        tree[level[height] + level_idx] += inc;
    }

};


class FenwickF
{
public:
    static constexpr size_t LEAF_MAXVAL = 64; // Leaves store values up to 64
    static constexpr size_t LEAF_BITSIZE = 9; // log2(65);

protected:
    std::unique_ptr<uint64_t[]> tree;
    size_t size;

public:
    FenwickF(uint64_t sequence[], size_t size):
        tree(std::make_unique<size_t[]>(size)),
        size(size)
    {
        std::copy_n(sequence, size, tree.get());

        for (size_t m = 2; m <= size; m <<= 1) {
            for (size_t idx = m; idx <= size; idx += m)
                tree[idx - 1] += tree[idx - m/2 - 1];
        }
    }

    uint64_t topdown_prefix(size_t idx) const
    {
        uint64_t sum = 0;
        size_t index = 0;

        while (idx != index) {
            index ^= mask_lambda(idx ^ index);
            sum += tree[index - 1];
        }

        return sum;
    }

    uint64_t bottomup_prefix(size_t idx) const
    {
        uint64_t sum = 0;

        while (idx != 0) {
            sum += tree[idx - 1];
            idx = clear_rho(idx);
        }

        return sum;
    }


    void bottomup_add(size_t idx, int64_t inc)
    {
        while (idx <= size) {
            tree[idx-1] += inc;
            idx += mask_rho(idx);
        }
    }

    void topdown_add(size_t idx, int64_t inc)
    {
        size_t negindex = -updroot(idx, size);
        const size_t negidx = -idx;

        while (negindex != negidx) {
            tree[-negindex] += inc;
            negindex ^= 1ULL << lambda(negindex ^ negidx);
        }

        tree[idx] += inc;
    }

};



int main(int argc, char *argv[])
{
    using namespace std;
    using namespace std::chrono;

    size_t SIZE, QUERIES;
    istringstream(argv[1]) >> SIZE;
    istringstream(argv[2]) >> QUERIES;
    double C = 1. / QUERIES;


    random_device rd;
    mt19937 mte(rd());
    uniform_int_distribution<size_t> idxdist(1, SIZE);
    uniform_int_distribution<size_t> valdist(0, 64);

    auto sequence = make_unique<uint64_t[]>(SIZE);
    for (size_t i = 0; i < SIZE; ++i)
        sequence[i] = valdist(mte);

    FenwickF fenF(sequence.get(), SIZE);
    FenwickL fenL(sequence.get(), SIZE);

    uint64_t tmp = 0;
    high_resolution_clock::time_point begin, end;

    // repeat the benchmark few times to see if the results are consistent
    for (size_t i = 0; i < 3; i++) {
        cout << "fenF bottomup_add... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            fenF.bottomup_add(idxdist(mte), 0);
        end = high_resolution_clock::now();
        auto fenf_bottomup_add = duration_cast<nanoseconds>(end-begin).count();
        cout << fenf_bottomup_add * C << " ns\n";

        cout << "fenF topdown_add... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            fenF.topdown_add(idxdist(mte), 0);
        end = high_resolution_clock::now();
        auto fenf_topdown_add = duration_cast<nanoseconds>(end-begin).count();
        cout << fenf_topdown_add * C << " ns\n";

        cout << "fenL bottomup_add... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            fenL.bottomup_add(idxdist(mte), 0);
        end = high_resolution_clock::now();
        auto fenl_bottomup_add = duration_cast<nanoseconds>(end-begin).count();
        cout << fenl_bottomup_add * C << " ns\n";

        cout << "fenL topdown_add... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            fenL.topdown_add(idxdist(mte), 0);
        end = high_resolution_clock::now();
        auto fenl_topdown_add = duration_cast<nanoseconds>(end-begin).count();
        cout << fenl_topdown_add * C << " ns\n";


        cout << "fenF bottomup_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenF.bottomup_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenf_fenwick_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenf_fenwick_prefix * C << " ns\n";

        cout << "fenF topdown_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenF.topdown_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenf_reversed_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenf_reversed_prefix * C << " ns\n";

        cout << "fenL bottomup_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenL.bottomup_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenl_fenwick_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenl_fenwick_prefix * C << " ns\n";

        cout << "fenL topdown_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenL.topdown_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenl_reversed_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenl_reversed_prefix * C << " ns\n";

        cout << "fenL level_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenL.level_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenl_level_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenl_level_prefix * C << " ns\n";

        cout << "\n";
    }


    const volatile uint64_t __attribute__((unused)) unused = tmp;

    // Check if everything is working
    for (size_t i = 0; i < SIZE; ++i) {
        auto a = fenF.bottomup_prefix(i);
        auto b = fenF.topdown_prefix(i);
        auto c = fenL.bottomup_prefix(i);
        auto d = fenL.topdown_prefix(i);
        auto e = fenL.level_prefix(i);

        bool error = a != b || b != c || c != d || d != e;

        if (error)
            cout << "idx(" << i << ") => " << a << " " << b << " " << c << " " << d << " " << e << "\n";
    }

    return 0;
}
