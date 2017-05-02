#include <gtest/gtest.h>

#include "broadword.hpp"

#include "fenwick/simple.hpp"
#include "fenwick/compact.hpp"
#include "fenwick/byte.hpp"
#include "fenwick/typed.hpp"
#include "fenwick/ityped.hpp"
#include "fenwick/shrank.hpp"

#include "fenwick/same_behavior.hpp"

#include "rankselect/word.hpp"
#include "rankselect/line.hpp"

#include "rankselect/same_behavior.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
