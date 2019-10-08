#include <gtest/gtest.h>

#include "common.hpp"

#include "fenwick/fixedf.hpp"
// TODO: explicit tests for fixedl
#include "fenwick/bitf.hpp"
// #include "fenwick/bitl.hpp" doesn't compile anymore (Tree.get() to string)
#include "fenwick/bytef.hpp"
//#include "fenwick/bytel.hpp" doesn't compile anymore (Tree.get() to string)
#include "fenwick/typef.hpp"
#include "fenwick/typel.hpp"
#include "fenwick/hybrid.hpp" doesn't work anymore (fails)

#include "fenwicktree.hpp"
#include "rankselect.hpp"

#include "expansion.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
