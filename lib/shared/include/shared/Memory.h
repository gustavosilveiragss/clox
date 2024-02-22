#pragma once

#include <shared/common.h>

/**
 * @brief Macro calculates a new capacity based on a given current capacity. Grows by a factor of 2.
 * @param capacity The size of the block to allocate.
 */
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

/**
 * @brief Macro to allocate a block of memory of the given type. Makes a call to reallocate().
 * @param type The type of the block to allocate.
 * @param pointer The pointer to the block to reallocate.
 * @param oldCount The old size of the block to reallocate.
 * @param newCount The new size of the block to reallocate.
 */
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

/**
 * @brief Macro to free a block of memory of the given type. Makes a call to reallocate().
 * @param type The type of the block to allocate.
 * @param pointer The pointer to the block to reallocate.
 * @param oldCount The old size of the block to reallocate.
 */
#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

/**
 * @brief Reallocate a block of memory, from a given old size to a given new size.
 * @details If the new size is 0, the block is freed.
 * @details If the old size is 0, the block is allocated.
 * @details If the new size is less than the old size, shrink existing allcation.
 * @details If the new size is greater than the old size, grow existing allocation.
 * @param pointer The pointer to the block to reallocate.
 * @param oldSize The old size of the block to reallocate.
 * @param newSize The new size of the block to reallocate.
 */
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
