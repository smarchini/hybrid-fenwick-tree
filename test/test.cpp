#include <gtest/gtest.h>

#include "broadword_test.hpp"

#include "simple_fenwick_tree_test.hpp"
#include "compact_fenwick_tree_test.hpp"
#include "byte_fenwick_tree_test.hpp"
#include "typed_fenwick_tree_test.hpp"

#include "same_behavior_test.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
