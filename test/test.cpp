#include <gtest/gtest.h>

#include "common.hpp"

// TODO: explicit level-order tests does not compile anymore because of Tree.get() to string
// TODO: explicit TypeF and TypeL tests does not compile anymore because of push and pop
#include "fenwick/fixedf.hpp"
// explicit tests for fixedl
#include "fenwick/bitf.hpp"
// #include "fenwick/bitl.hpp"
#include "fenwick/bytef.hpp"
//#include "fenwick/bytel.hpp"
// #include "fenwick/typef.hpp"
// #include "fenwick/typel.hpp"
#include "fenwick/hybrid.hpp"

#include "fenwicktree.hpp"
#include "rankselect.hpp"

#include "expansion.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
