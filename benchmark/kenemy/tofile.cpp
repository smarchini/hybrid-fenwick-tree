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
using namespace dyn;
using namespace std::chrono;

bool is_fileempty(ifstream file) {
    return file.peek() == ifstream::traits_type::eof();
}

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

    for (size_t i = 0; i < len; i++)
    {
        size_t p = sigmainv[ rho[i] ];
        d += b.rankZero(p);
        b.set(p);
    }

    return d;
}

size_t vigna_ktsl_prezza(uint64_t *sigmainv, uint64_t *rho, size_t len)
{
    size_t d = 0;

    suc_bv b;
    for (size_t i = 0; i < len; i++)
        b.push_back(0);

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
    unique_ptr<uint64_t[]> bv = make_unique<uint64_t[]>(len / 64 + (len % 64 != 0));

    for (size_t i = 0; i < len; i++)
    {
        size_t p = sigmainv[ rho[i] ];
        d += linear_rankZero(bv.get(), p);
        bv[p / 64] |= (1ULL << (p%64));
    }

    return d;
}

int main(int argc, char *argv[])
{
    string order = "inverse,fixed[$1$],byte[$1$],bit[$1$],fixed[$2$],byte[$2$],bit[$2$],fixed[$4$],byte[$4$],bit[$4$],fixed[$8$],byte[$8#],bit[$8$],fixed[$12$],byte[$12$],bit[$12$],linear,bubble";

    size_t len;
    if (argc < 3 || !(istringstream(argv[2]) >> len)) {
        cerr << "Invalid parameters" << endl;
        return -1;
    }

    string filename = argv[1] + string("fkenemy.csv");
    ofstream file(filename, ios::out | ios::app);

    if (is_fileempty(ifstream(filename))) {
        cout << "Initializing: " << filename << endl;
        file << "Elements," + order << endl;
    }

    unique_ptr<uint64_t[]> sigma = make_unique<uint64_t[]>(len),
        rho = make_unique<uint64_t[]>(len),
        sigmainv = make_unique<uint64_t[]>(len);

    for (size_t i = 0; i < len; i++)
        sigma[i] = rho[i] = i;

    default_random_engine engine;
    engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

    high_resolution_clock::time_point begin, end;

    shuffle(&sigma[0], &sigma[len], engine);
    shuffle(&rho[0], &rho[len], engine);

    file << len << ",";

    cout << "inverse... " << flush;
    begin = high_resolution_clock::now();
    for (size_t i = 0; i < len; i++) sigmainv[ sigma[i] ] = i;
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "fixed[1]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<WordRankSelect<NaiveFenwickTree>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "byte[1]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<WordRankSelect<ByteFenwickTree>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "bit[1]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<WordRankSelect<BitFenwickTree>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "fixed[2]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<NaiveFenwickTree, 2>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "byte[2]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<ByteFenwickTree, 2>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "bit[2]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<BitFenwickTree, 2>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "fixed[4]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<NaiveFenwickTree, 4>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "byte[4]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<ByteFenwickTree, 4>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "bit[4]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<BitFenwickTree, 4>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "fixed[8]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<NaiveFenwickTree, 8>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "byte[8]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<ByteFenwickTree, 8>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "bit[8]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<BitFenwickTree, 8>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "fixed[12]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<NaiveFenwickTree, 12>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "byte[12]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<ByteFenwickTree, 12>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

    cout << "bit[12]... " << flush;
    begin = high_resolution_clock::now();
    vigna_ktsl<LineRankSelect<BitFenwickTree, 12>>(sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";

/*  if (len < 60000000) {
    cout << "prezza preinit... " << flush;
    suc_bv prezzabv;
    for (size_t i = 0; i < len; i++) prezzabv.push_back(0);
    begin = high_resolution_clock::now();
    vigna_ktsl_prezza(prezzabv, sigmainv.get(), rho.get(), len);
    end = high_resolution_clock::now();
    file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";
    } else {
        cout << "prezza preinit (skipped)... " << flush;
        file << "" << ",";
    }

    if (len < 40000000) {
        cout << "prezza... " << flush;
        begin = high_resolution_clock::now();
        vigna_ktsl_prezza(sigmainv.get(), rho.get(), len);
        end = high_resolution_clock::now();
        file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";
    } else {
        cout << "prezza (skipped)... " << flush;
        file << "" << ",";
    } */

    if (len < 10000000) {
        cout << "linear... " << flush;
        begin = high_resolution_clock::now();
        vigna_linearsearch(sigmainv.get(), rho.get(), len);
        end = high_resolution_clock::now();
        file << duration_cast<chrono::nanoseconds>(end-begin).count() << ",";
    } else {
        cout << "linear (skipped)... " << flush;
        file << "" << ",";
    }

    if (len < 800000) {
        cout << "bubble... " << flush;
        begin = high_resolution_clock::now();
        diligent_bubblesort(sigma.get(), rho.get(), len);
        end = high_resolution_clock::now();
        file << duration_cast<chrono::nanoseconds>(end-begin).count() << "\n";
    } else {
        cout << "bubble (skipped)... " << flush;
        file << "" << "\n";
    }
    cout << "Done." << endl;

    return 0;
}
