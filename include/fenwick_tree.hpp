#ifndef __FENWICK_TREE_H__
#define __FENWICK_TREE_H__

#include <cstdint>
#include <memory>

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
     * find() - Find the closest element less or equal than a given one.
     * @val: Value to search.
     *
     * This method find the maximum value less or equal then @val mantained by
     * the cumulative frequency, -1 if the fisrt element of the sequence is
     * greter than @val.
     *
     * Return: The closest element less or equal than @val or -1 if it doesn't
     * exists.
     *
     * TODO: aggiungere significato complement nei commenti
     */
    virtual std::size_t find(std::uint64_t val, bool complement=false) const = 0;

    /**
     * bit_count() - Returns an estimation of the number of bits used by the
     * tree.
     */
    virtual std::size_t bit_count() const = 0;
};

#endif // __FENWICK_TREE_H__
