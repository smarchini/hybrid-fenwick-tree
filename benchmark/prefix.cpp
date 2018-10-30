#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
//#include <fenwick.hpp>


inline int lambda(uint64_t word) { return 63 - __builtin_clzll(word); }
inline int rho(uint64_t word) { return __builtin_ctzll(word); }
inline uint64_t mask_rho(uint64_t word) { return word & (-word); }
inline uint64_t mask_lambda(uint64_t word) { return 0x8000000000000000 >> __builtin_clzll(word); }
inline uint64_t clear_rho(uint64_t word) { return word & (word - 1ULL); }


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

    // top-down: from the "root" to idx
    uint64_t reversed_prefix(size_t idx) const
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

        while (level_idx < level[height+1] && height < levels) {
            sum += tree[level[height] + level_idx];

            const int next = rho(level_idx) + 1;
            height += next;
            level_idx >>= next;
        }

        return sum;
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

    // top-down: from the "root" to idx
    uint64_t reversed_prefix(size_t idx) const
    {
        uint64_t sum = 0;
        size_t index = 0;

        while (idx != index) {
            index += mask_lambda(idx ^ index);
            sum += tree[index - 1];
        }

        return sum;
    }

    // bottom-up: classical parent relationship and conversion
    uint64_t fenwick_prefix(size_t idx) const
    {
        uint64_t sum = 0;

        while (idx != 0) {
            sum += tree[idx - 1];
            idx = clear_rho(idx);
        }

        return sum;
    }

};



int main(int argc, char *argv[])
{
    using namespace hft;
    using namespace std;
    using namespace std::chrono;

    size_t SIZE, QUERIES;
    istringstream(argv[1]) >> SIZE;
    istringstream(argv[2]) >> QUERIES;
    double C = 1. / QUERIES;


    random_device rd;
    mt19937 mte(rd());
    uniform_int_distribution<size_t> idxdist(0, SIZE);
    uniform_int_distribution<size_t> valdist(0, 64);

    auto sequence = make_unique<uint64_t[]>(SIZE);
    for (size_t i = 0; i < SIZE; ++i)
        sequence[i] = valdist(mte);

    FenwickF fenF(sequence.get(), SIZE);
    FenwickL fenL(sequence.get(), SIZE);
    // hft::fenwick::FixedL<64> realL(sequence.get(), SIZE);
    // hft::fenwick::FixedF<64> realF(sequence.get(), SIZE);

    uint64_t tmp = 0;
    high_resolution_clock::time_point begin, end;

    // repeat the benchmark few times to see if the results are consistent
    for (size_t i = 0; i < 3; i++) {
        cout << "fenF fenwick_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenF.fenwick_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenf_fenwick_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenf_fenwick_prefix * C << " ns\n";

        cout << "fenF reversed_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenF.reversed_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenf_reversed_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenf_reversed_prefix * C << " ns\n";

        cout << "fenL fenwick_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenL.fenwick_prefix(idxdist(mte));
        end = high_resolution_clock::now();
        auto fenl_fenwick_prefix = duration_cast<nanoseconds>(end-begin).count();
        cout << fenl_fenwick_prefix * C << " ns\n";

        cout << "fenL reversed_prefix... " << flush;
        begin = high_resolution_clock::now();
        for (size_t i = 0; i < QUERIES; ++i)
            tmp ^= fenL.reversed_prefix(idxdist(mte));
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

        // cout << "realF prefix... " << flush;
        // begin = high_resolution_clock::now();
        // for (size_t i = 0; i < QUERIES; ++i)
        //     tmp ^= realF.prefix(idxdist(mte));
        // end = high_resolution_clock::now();
        // auto realF_prefix = duration_cast<nanoseconds>(end-begin).count();
        // cout << realF_prefix * C << " ns\n";

        // cout << "realL prefix... " << flush;
        // begin = high_resolution_clock::now();
        // for (size_t i = 0; i < QUERIES; ++i)
        //     tmp ^= realL.prefix(idxdist(mte));
        // end = high_resolution_clock::now();
        // auto realL_prefix = duration_cast<nanoseconds>(end-begin).count();
        // cout << realL_prefix * C << " ns\n";

        cout << "\n";
    }


    const volatile uint64_t __attribute__((unused)) unused = tmp;

    // Check if everything is working
    // for (size_t i = 0; i < SIZE; ++i) {
    //     auto a = fenF.fenwick_prefix(i);
    //     auto b = fenF.reversed_prefix(i);
    //     auto c = fenL.fenwick_prefix(i);
    //     auto d = fenL.reversed_prefix(i);
    //     auto e = fenL.level_prefix(i);
    //     auto f = realF.prefix(i);
    //     auto g = realL.prefix(i);

    //     bool error = a != b || b != c || c != d || d != e || e != f || f != g;

    //     if (error) {
    //         cout << "idx(" << i << ") => "
    //                   << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << "\n";

    //     }
    // }

    return 0;
}
