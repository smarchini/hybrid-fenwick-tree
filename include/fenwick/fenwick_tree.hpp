#ifndef __FENWICK_TREE_HPP__
#define __FENWICK_TREE_HPP__

#include "../common.hpp"
#include "../darray.hpp"

namespace hft {
    namespace fenwick {

        /**
         * FenwickTree - Fenwick Tree data structure interface.
         * @sequence: An integer vector.
         * @length: The length (in words) of the sequence.
         * @LEAF_MAXVAL: maximum value that @sequence can store.
         *
         * This data structure indices starts from 1 and ends in @length.
         *
         */
        class FenwickTree
        {
        public:
            virtual ~FenwickTree() = default;

            /**
             * prefix() - Compute the prefix sum.
             * @idx: Length of the prefix sum.
             *
             * Sum the elements in the range (0 .. @idx], returns zero when @idx
             * is zero.
             *
             */
            virtual std::uint64_t prefix(std::size_t idx) const = 0;

            /**
             * add() - Increment an element of the sequence (not the tree).
             * @idx: Index (starting from 1) of the element.
             * @inc: Value to sum.
             *
             * You are allowed to use negative values for the increment, but
             * keep in mind you should respect the structure boundaries.
             *
             */
            virtual void add(std::size_t idx, std::int64_t inc) = 0;

            /**
             * find() - Search the index of the closest (less or equal than) prefix.
             * @val: Prefix to search.
             *
             * If @val is an l-value reference its value will be changed with
             * the distance between the found and the searched prefix (i.e. the
             * difference between the prefix and @val).
             *
             * This method returns zero if such an element doesn't exists (i.e.
             * there are no prefixes that are greater or equal to @val).
             *
             */
            virtual std::size_t find(std::uint64_t *val) const = 0;
            std::size_t find(std::uint64_t val) const {
                return find(&val);
            }

            /**
             * compfind() - Complement find.
             * @val: Prefix to search.
             *
             * This method search the index whose its prefix its the closest to
             * MAXVAL-@val. MAXVAL is the maximum possibile value for such a
             * prefix (@sequence is therefore bounded).
             *
             * The same considerations made for FenwickTree::find() holds.
             *
             */
            virtual std::size_t compfind(std::uint64_t *val) const = 0;
            std::size_t compfind(std::uint64_t val) const {
                return compfind(&val);
            }

            /**
             * size() - Returns the length of the sequence.
             *
             */
            virtual std::size_t size() const = 0;

            /**
             * bit_count() - Estimation of the size (in bits) of this structure.
             *
             */
            virtual std::size_t bit_count() const = 0;
        };

    }
}

#endif // __FENWICK_TREE_HPP__
