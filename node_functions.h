#ifndef NODE_FUNCTIONS_H
#define NODE_FUNCTIONS_H

#include <stddef.h>

// Forward declarations with typedef
typedef struct Node Node;
typedef struct Layer Layer;
typedef struct Model Model;
typedef struct Prompt Prompt;
typedef struct Output Output;

// Function declarations without 'struct' keyword
Prompt create_prompt(size_t length, double* tokens);
Output calculate_output(Prompt* prompt, Model* model);
void test_calculate_output(void);

#endif // NODE_FUNCTIONS_H
