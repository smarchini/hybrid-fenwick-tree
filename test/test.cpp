#include <gtest/gtest.h>

#include "common.hpp"

#include "fenwick/fixedf.hpp"
// TODO: explicit tests for fixedl
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
