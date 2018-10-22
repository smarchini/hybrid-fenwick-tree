#include <gtest/gtest.h>

#include "broadword.hpp"

#include "fenwick/fixedf.hpp"
// TODO: test approfondito per lnaive
#include "fenwick/bitf.hpp"
#include "fenwick/bitl.hpp"
#include "fenwick/bytef.hpp"
#include "fenwick/bytel.hpp"
#include "fenwick/typef.hpp"
#include "fenwick/typel.hpp"
#include "fenwick/hybrid.hpp"

#include "fenwicktree.hpp"
#include "rankselect.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
