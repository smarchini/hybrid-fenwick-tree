#include <gtest/gtest.h>

#include "broadword.hpp"

#include "fenwick/naive.hpp"
#include "fenwick/type.hpp"
#include "fenwick/ltype.hpp"
#include "fenwick/byte.hpp"
#include "fenwick/lbyte.hpp"
#include "fenwick/bit.hpp"
#include "fenwick/lbit.hpp"

#include "fenwick/same_behavior.hpp"

#include "rankselect/word.hpp"
#include "rankselect/line.hpp"

#include "rankselect/same_behavior.hpp"
#include "rankselect/like_dynamic.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
