#ifndef NODE_FUNCTIONS_H
#define NODE_FUNCTIONS_H

struct Node;
struct Layer;
struct Model;
struct Prompt;
struct Output;
Prompt create_prompt(size_t length, double* tokens);
double calculate_output(Model used_model, Prompt prompt);

#endif