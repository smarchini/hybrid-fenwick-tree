#ifndef __FENWICK_TREE_H__
#define __FENWICK_TREE_H__

#include <cstdint>

class FenwickTree
{
public:
    virtual ~FenwickTree() = default;

    /**
     * get() - Retrieve the (@idx+1)th element of the cumulative frequency.
     * @idx: Index of the element, starting from 0.
     *
     * This method returns the (@idx+1)th element of the cumulative frequency
     * mantained by the current instance.
     *
     * Return: The (@idx+1)th element of the cumulative frequency
     */
    virtual std::uint64_t get(std::size_t idx) const = 0;

    /**
     * set() - Increment one element of the cumulative frequency.
     * @idx: The index (starting from 0) of the element you want to update.
     * @inc: Increment relative to the previous current value.
     *
     * This method update the current instance, it picks the (@idx+1)th element
     * of the cumulative frequency and change its value to @val+@inc.
     */
    virtual void set(std::size_t idx, std::uint64_t inc) = 0;

    /**
     * find() - Find the closest element less or equal then a given one.
     * @val: Value to search.
     *
     * This method find the maximum value less or equal then @val mantained by
     * the cumulative frequency.
     *
     * TODO: nota sul comportamento con val=0
     * TODO: nota sul comportamento in caso size != 2^k-1 per qualche k
     */
    virtual std::uint64_t find(std::uint64_t val) const = 0;
};

#endif // __FENWICK_TREE_H__
