#include <gtest/gtest.h>
#include "broadword_test.hpp"
#include "simple_fenwick_tree_test.hpp"
#include "compact_fenwick_tree_test.hpp"
#include "byte_fenwick_tree_test.hpp"

// TODO: aggiungere test meno scrupolosi, ma con sequenze random grandi e
// verificare che tutti i FenwickTree si comportino in modo equivalente.
// 58 livelli => sequenza di 2^58 - 1 = 288230376151711743 elementi.

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
