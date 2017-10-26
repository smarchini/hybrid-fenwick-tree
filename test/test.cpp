#include <gtest/gtest.h>

#include "broadword.hpp"

#include "fenwick/naive.hpp"
// TODO: test approfondito per lnaive
#include "fenwick/bit.hpp"
#include "fenwick/lbit.hpp"
#include "fenwick/byte.hpp"
#include "fenwick/lbyte.hpp"
#include "fenwick/type.hpp"
#include "fenwick/ltype.hpp"
#include "fenwick/mixed.hpp"

#include "fenwicktree.hpp"
#include "rankselect.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
