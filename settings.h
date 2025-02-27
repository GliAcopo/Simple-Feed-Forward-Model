/**
 * @file settings.h
 * @author your name (you@domain.com)
 * @brief This header file purpose is to share compiler macros, types, variable definitions... Between all the code files of this model
 * @version 0.2
 * @date 2024-12-23
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#define DEBUG    1

#include <stdio.h>

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        fprintf(stderr, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)
#endif


#define VERBOSE  FALSE       // This variable will trigger *way more* debug messages if activated 
#define autoMode TRUE      // This variable is in boolean logic, and is used for the choice between auto training and standard training.
#define MORE     FALSE      // This variable basically implements extra code that is deemed redundant. But may come in handy.

/**
 * @brief just a simple enum to create a "bool" convention in this project
 * @return TRUE = 1 / FALSE = 0
 */
typedef enum Bool{
    TRUE = 1,
    FALSE = 0,
} Bool;

#endif