#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

#include <rankselect/rank_select.hpp>
#include <rankselect/word.hpp>
#include <rankselect/line.hpp>

#include <fenwick/fenwick_tree.hpp>
#include <fenwick/naive.hpp>
#include <fenwick/lnaive.hpp>
#include <fenwick/type.hpp>
#include <fenwick/ltype.hpp>
#include <fenwick/byte.hpp>
#include <fenwick/lbyte.hpp>
#include <fenwick/bit.hpp>
#include <fenwick/lbit.hpp>
#include <fenwick/mixed.hpp>

#include <dynamic.hpp>

using namespace std;
using namespace hft;
using namespace std::chrono;


// [0..n) vectors is assumed
size_t inverse(uint64_t *sigma, uint64_t idx)
{
    size_t i = 0;
    while (sigma[i] != idx ) i++;
    return i;
}

size_t diligent_bubblesort(uint64_t *sigma, uint64_t *rho, size_t len)
{
    size_t d = 0;

    for (size_t i = 0; i < len; i++)
    {
        size_t p = inverse(sigma, rho[i]);

        for (size_t j = p; j >= i+1; j--)
            std::swap(sigma[j], sigma[j-1]);

        d += p-i;
    }

    return d;
}

template<typename dynbv>
size_t vigna_ktsl(uint64_t *sigmainv, uint64_t *rho, size_t len)
{
    size_t d = 0;

    size_t wordlen = len/64 + (len%64 != 0);
    dynbv b(DArray<uint64_t>(wordlen), wordlen);
    cout << setw(15) << b.bit_count() << " bits\t";

    for (size_t i = 0; i < len; i++)
    {
        size_t p = sigmainv[ rho[i] ];
        d += b.rankZero(p);
        b.add(p);
    }

    return d;
}

size_t vigna_ktsl_prezza(uint64_t *sigmainv, uint64_t *rho, size_t len)
{
    size_t d = 0;

    suc_bv b;
    for (size_t i = 0; i < len; i++)
        b.push_back(0);

    cout << setw(15) << b.bit_size() << " bits\t";

    for (size_t i = 0; i < len; i++)
    {
        size_t p = sigmainv[ rho[i] ];
        d += b.rank0(p);
        b.set(p);
    }

    return d;
}

size_t vigna_ktsl_prezza(suc_bv &b, uint64_t *sigmainv, uint64_t *rho, size_t len)
{
    size_t d = 0;

    cout << setw(15) << b.bit_size() << " bits\t";

    for (size_t i = 0; i < len; i++)
    {
        size_t p = sigmainv[ rho[i] ];
        d += b.rank0(p);
        b.set(p);
    }

    return d;
}

size_t linear_rankZero(uint64_t *bv, size_t pos)
{
    size_t i = 0, rank = 0;

    for (i = 0; i < pos/64; i++)
        rank += popcount(~bv[i]);

    return rank + popcount((~bv[i] & compact_bitmask(pos%64, 0)));
}

size_t vigna_linearsearch(uint64_t *sigmainv, uint64_t *rho, size_t len) {
    size_t d = 0;

    size_t wordlen = len / 64 + (len % 64 != 0);
    unique_ptr<uint64_t[]> bv = make_unique<uint64_t[]>(wordlen);

    cout << setw(15) << wordlen * 64 << " bits\t";

    for (size_t i = 0; i < len; i++)
    {
        size_t p = sigmainv[ rho[i] ];
        d += linear_rankZero(bv.prefix(), p);
        bv[p / 64] |= (1ULL << (p%64));
    }

    return d;
}

int main(int argc, char *argv[])
{
    size_t len;
    if (argc < 2 || !(istringstream(argv[1]) >> len)) {
        cerr << "Invalid parameters" << endl;
        return -1;
    }

    unique_ptr<uint64_t[]> sigma = make_unique<uint64_t[]>(len),
        rho = make_unique<uint64_t[]>(len),
        sigmainv = make_unique<uint64_t[]>(len);

    for (size_t i = 0; i < len; i++)
        sigma[i] = rho[i] = i;

    default_random_engine engine;
    engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

    shuffle(&sigma[0], &sigma[len], engine);
    shuffle(&rho[0], &rho[len], engine);

    high_resolution_clock::time_point begin, end;

    cout << "number of elements in each vector: " << len << "\n";
    cout << "precomputation of the inverse function: " << flush;
    begin = high_resolution_clock::now(); for (size_t i = 0; i < len; i++) { sigmainv[ sigma[i] ] = i; } end = high_resolution_clock::now();
    cout << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n\n";


    begin = high_resolution_clock::now();
    cout << setw(30) << "fixed[1]: " << flush << vigna_ktsl<Word<FixedF>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "fixed[2]: " << flush << vigna_ktsl<Stride<FixedF, 2>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "fixed[4]: " << flush << vigna_ktsl<Stride<FixedF, 4>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "fixed[8]: " << flush << vigna_ktsl<Stride<FixedF, 8>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "fixed[12]: " << flush << vigna_ktsl<Stride<FixedF, 12>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "linear: " << flush << vigna_linearsearch(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "bubble: " << setw(15) << 0 << " bits\t" << flush << diligent_bubblesort(sigma.prefix(), rho.prefix(), len) ;
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n" << endl;

/*
    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (word naive): " << flush << vigna_ktsl<Word<FixedF>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (word byte): " << flush << vigna_ktsl<Word<ByteF>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (line naive 8): " << flush << vigna_ktsl<Stride<FixedF, 8>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (line byte 8): " << flush << vigna_ktsl<Stride<ByteF, 8>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (line naive 16): " << flush << vigna_ktsl<Stride<FixedF, 16>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (line byte 16): " << flush << vigna_ktsl<Stride<ByteF, 16>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (line naive 32): " << flush << vigna_ktsl<Stride<FixedF, 32>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (line byte 32): " << flush << vigna_ktsl<Stride<ByteF, 32>>(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    suc_bv prezzabv;
    for (size_t i = 0; i < len; i++) prezzabv.push_back(0);
    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (prezza preinit): " << flush << vigna_ktsl_prezza(prezzabv, sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna ktsl (prezza): " << flush << vigna_ktsl_prezza(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "vigna linearsearch: " << flush << vigna_linearsearch(sigmainv.prefix(), rho.prefix(), len);
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n";

    begin = high_resolution_clock::now();
    cout << setw(30) << "diligent bubblesort: " << setw(15) << 0 << " bits\t" << flush << diligent_bubblesort(sigma.prefix(), rho.prefix(), len) ;
    end = high_resolution_clock::now();
    cout << setw(15) << duration_cast<chrono::nanoseconds>(end-begin).count() << " nsec\n" << endl;
*/

    return 0;
}
