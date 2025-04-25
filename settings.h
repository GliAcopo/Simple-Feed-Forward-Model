/**
 * @file settings.h
 * @author your name (you@domain.com)
 * @brief This header file purpose is to share compiler macros, types, variable definitions... Between all the code files of this model
 * @version 0.2
 * @date 2024-12-23
 */
#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @brief just a simple enum to create a "bool" convention in this project
 * @return TRUE = 1 / FALSE = 0
 */
typedef enum Bool
{
    TRUE = 1,
    FALSE = 0,
} Bool;

#define DEBUG 1       // Variables that enables all the debug messages. Setting the variable to 1 will automatically include stdio.h
#define VERBOSE FALSE // This variable will trigger *way more* debug messages if activated
#define autoMode TRUE // This variable is in boolean logic, and is used for the choice between auto training and standard training.
#define MORE FALSE    // This variable basically implements extra code that is deemed redundant. But may come in handy.

#ifdef DEBUG
#include <stdio.h>
#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "[%s:%s:%d] " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

/* ----------------------------------------------------------------------------- */
/*                                VARIABLE TYPE DEFINITIONS                      */

/** @note include or exclude the dependencies for the types you are going to use for the model here */
#include <stddef.h> // for size_t, ptrdiff_t

/**
 * @brief Variable type for all model parameters
 *        (weights of the arches and neuron biases)
 * @note  Doesn't have to necessarily support fractional values; but we use a floating‐point type here
 */
typedef double ffw_param_t;

/*–––––––––––––––––––––––––––––––––––––––––––––––––––
   Structure‐level counts (e.g. “how many layers”,
   “how many neurons in this layer”, etc.)
–––––––––––––––––––––––––––––––––––––––––––––––––––*/

/**
 * @brief Unsigned type for struct counts/sizes
 *        (e.g. number of layers, number of items)
 * @note  Always non‐negative; ideal for size fields in your structs
 */
typedef size_t ucount_t;

/**
 * @brief Signed type for struct counts/sizes
 * @note  Allows negative values if you ever need to represent
 *        “difference” or underflow checks, etc.
 */
typedef ptrdiff_t count_t;

/*–––––––––––––––––––––––––––––––––––––––––––––––––––
   Loop‐level indices (for iteration)
–––––––––––––––––––––––––––––––––––––––––––––––––––*/

/**
 * @brief Unsigned type for loop indices
 *        (zero‐based forward iteration)
 * @note  Use in for-loops where index is guaranteed ≥ 0
 */
typedef size_t uindex_t;

/**
 * @brief Signed type for loop indices
 * @note  Use when loops may go negative (reverse iteration,
 *        offset computations, sentinel values, etc.)
 */
typedef ptrdiff_t index_t;

#endif
