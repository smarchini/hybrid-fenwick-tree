#ifndef __BROADWORD_H__
#define __BROADWORD_H__

#include <cstdint>
#include <x86intrin.h>

/**
 * drop_first_set - Set to 0 the least significant one in a word.
 * @word: Binary word.
 *
 * Return: @word with its least significant one setted to zero.
 */
inline std::uint64_t drop_first_set(std::uint64_t word)
{
    return word & (word - 1ULL);
}

/**
 * find_first_set - Find the position of the first bit set in a word.
 * @word: Binary word.
 *
 * This fucntion returns the position of the first (least significant) bit set
 * in @word. The least significant bit is position 1 and the most significant
 * position is 64. It returns 0 if @word is 0.
 *
 * Return: The position of the first set bit in @word or 0 if there isn't one.
 */
inline std::uint64_t find_first_set(std::uint64_t word)
{
    return __builtin_ffsll(word);
}

/**
 * find_last_set - Find the position of the last bit set in a word.
 * @word: Binary word.
 *
 * This fucntion returns the position of the last (most significant) bit set
 * in @word. The least significant bit is position 1 and the most significant
 * position is 64.
 *
 * Undefined behavior if @word is 0.
 *
 * Return: The position of the last set bit in @word if @word != 0.
 */
inline std::uint64_t find_last_set(std::uint64_t word)
{
    return 64ULL - __builtin_clzll(word);
}

/**
 * mask_first_set - Given a word, mask its least significant bit out.
 * @word: Binary word.
 *
 * Return: @word with anything but its least significant one setted to zero.
 * Same as 2^(find_first_set(word)-1) for any @word != 0.
 */
inline std::uint64_t mask_first_set(std::uint64_t word)
{
    return word & (-word);
}

/**
 * mask_last_set - Given a word, mask its most significant bit out.
 * @word: Binary word.
 *
 * Undefined behavior if @word is 0.
 *
 * Return: @word with anything but its most significant one setted to zero,
 * undefined behavior if @word is 0.
 */
inline std::uint64_t mask_last_set(std::uint64_t word)
{
    return 0x8000000000000000 >> __builtin_clzll(word);
}

/**
 * compact_bitmask - Generate a compact bitmask.
 * @count: Quantity of set bit.
 * @pos: Starting position.
 *
 * This fucntion returns a bitmask with @count set bits. The least significant
 * set start from @pos. If @pos == 0 you have @count one bits starting from the
 * least significant bit.
 *
 * Return: A bitmask with @count 1-bit starting from @pos
 */
inline std::uint64_t compact_bitmask(std::size_t count, std::size_t pos)
{
    return (-(count != 0ULL)) & (-1ULL >> (64ULL - count)) << pos;
}

#endif // __BROADWORD_H__
