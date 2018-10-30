#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>


inline int lambda(uint64_t word) { return 63 - __builtin_clzll(word); }
inline int rho(uint64_t word) { return __builtin_ctzll(word); }
inline uint64_t mask_rho(uint64_t word) { return word & (-word); }
inline uint64_t mask_lambda(uint64_t word) { return 0x8000000000000000 >> __builtin_clzll(word); }
inline uint64_t clear_rho(uint64_t word) { return word & (word - 1ULL); }


class Fenwick
{
public:
    static constexpr size_t LEAF_MAXVAL = 64; // Leaves store values up to 64
    static constexpr size_t LEAF_BITSIZE = 9; // log2(65);

protected:
    std::unique_ptr<uint64_t[]> tree;
    std::unique_ptr<size_t[]> level;

public:
    Fenwick(uint64_t sequence[], size_t size):
        tree(std::make_unique<size_t[]>(size)),
        level(std::make_unique<uint64_t[]>(lambda(size) + 2))
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

    // top-down: from the "root" to idx
    uint64_t fast_prefix(size_t idx) const
    {
        uint64_t sum = 0;
        size_t index = 0;

        while (idx != index) {
            index += mask_lambda(idx ^ index);

            const int height = rho(index);
            const size_t level_idx = index >> (1 + height);
            sum += tree[level[height] + level_idx];
        }

        return sum;
    }

    // bottom-up: classical parent relationship and conversion
    uint64_t fenwick_prefix(size_t idx) const
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

    // bottom-up: level-order parent formulae
    uint64_t level_prefix(size_t idx) const
    {
        uint64_t sum = 0;
        int height = rho(idx);
        size_t level_idx = idx >> (1 + height);

        while (level_idx < level[height+1]) {
            sum += tree[level[height] + level_idx];

            int next = rho(level_idx) + 1;
            height += next;
            level_idx >>= next;
        }

        return sum;
    }

};

int main()
{
    using namespace std::chrono;

    constexpr size_t SIZE = 100000000;
    constexpr double C = 1. / SIZE;

    auto sequence = std::make_unique<uint64_t[]>(SIZE);
    auto indices = std::make_unique<size_t[]>(SIZE);

    std::mt19937 mte;
    std::uniform_int_distribution<size_t> dist(0, SIZE);

    for (size_t i = 0; i < SIZE; ++i) {
        sequence[i] = i; // keep it simple (it does not affect the performance)
        indices[i] = dist(mte); // random indices in the rage [0, SIZE]
    }

    Fenwick fenwick(sequence.get(), SIZE);

    uint64_t tmp = 0;
    high_resolution_clock::time_point begin, end;

    std::cout << "Running level_prefix... " << std::flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < SIZE; ++i)
        tmp ^= fenwick.level_prefix(indices[i]);
    end = high_resolution_clock::now();
    auto level_prefix = duration_cast<nanoseconds>(end-begin).count();
    std::cout << level_prefix * C << " ns\n";

    std::cout << "Running fast_prefix... " << std::flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < SIZE; ++i)
        tmp ^= fenwick.fast_prefix(indices[i]);
    end = high_resolution_clock::now();
    auto fast_prefix = duration_cast<nanoseconds>(end-begin).count();
    std::cout << fast_prefix * C << " ns\n";

    std::cout << "Running fenwick_prefix... " << std::flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < SIZE; ++i)
        tmp ^= fenwick.fenwick_prefix(indices[i]);
    end = high_resolution_clock::now();
    auto fenwick_prefix = duration_cast<nanoseconds>(end-begin).count();
    std::cout << fenwick_prefix * C << " ns\n";

    const volatile uint64_t __attribute__((unused)) unused = tmp;

    // for (size_t i = 0; i < SIZE; ++i) {
    //     auto a = fenwick.fast_prefix(i);
    //     auto b = fenwick.fenwick_prefix(i);
    //     auto c = fenwick.level_prefix(i);
    //     auto msg = a != b || a != c || b != c ? " ERROR " : "";
    //     std::cout << "i = " << i << " => " << a << " " << b << " " << c << msg << "\n";
    // }

    return 0;
}
