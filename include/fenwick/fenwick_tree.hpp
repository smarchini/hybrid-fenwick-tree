#ifndef __FENWICK_TREE_H__
#define __FENWICK_TREE_H__

#include "../common.hpp"

namespace dyn {

    class FenwickTree
    {
    public:
        virtual ~FenwickTree() = default;

        /**
         * get() - Retrieve the (@idx+1)th element of the cumulative frequency
         * @idx: Index of the element, starting from 0
         *
         * This method returns the (@idx+1)th element of the cumulative
         * frequency mantained by the current instance.
         *
         * Returns: The (@idx+1)th element of the cumulative frequency
         */
        virtual std::uint64_t get(std::size_t idx) const = 0;

        /**
         * set() - Increment one element of the cumulative frequency
         * @idx: The index (starting from 0) of the element you want to update
         * @inc: Increment relative to the previous current value
         *
         * This method update the current instance, it picks the (@idx+1)th
         * element of the cumulative frequency and change its value to
         * @val+@inc.
         */
        virtual void set(std::size_t idx, std::int64_t inc) = 0;

        /**
         * find() - Find the closest element less or equal than a given one
         * @val: Value to search
         * @complement: If true it comput
         *
         * This method finds the maximum index whose the values mantained by the
         * cumulative frequency is less or equal than @val.
         *
         * If @complement is true, it returns the index whose MAX_VAL minus the
         * values mantained by the cumulative frequency is less or equal than
         * @val. MAX_VAL being the maximum value storable in such element.
         *
         * It returns -1 if such an element doesn't exists.
         *
         * Returns: The closest index whose its element is less or equal than
         * @val, its complement if @complement is true or -1ULL if it doesn't
         * exists.
         */
        // TODO: dividere in due funzioni per evitare un if nel codice. Forse la
        // selectZero più veloce della select in LineRankSelect è dovuta a
        // questo if.
        virtual std::size_t find(std::uint64_t val, bool complement=false) const = 0;

        /**
         * bit_count() - An estimation of the number of bits used by the tree
         *
         * Returns: An estimation of the size of the tree.
         */
        virtual std::size_t bit_count() const = 0;
    };

}

#endif // __FENWICK_TREE_H__
