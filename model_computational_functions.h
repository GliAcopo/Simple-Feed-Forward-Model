#ifndef MODEL_COMPUTATIONAL_FUNCTIONS_H
#define MODEL_COMPUTATIONAL_FUNCTIONS_H
#include <stddef.h> // for size_t
#include "data_structs_and_methods_definitions.h" // for Output Prompt and Model structs definitions

double** create_adj_matrix_double_square(int layers, int nodes_per_layer);
double*** create_FF_model_matrices(size_t layers, size_t nodes_per_layer);
void print_matrix_vector_double(double*** matrices_vector, int layers, int nodes_per_layer);

/* -+-+-+-+-+-+-+-+-+-+-+- INFERENCE -+-+-+-+-+-+-+-+-+-+-+- */
Output calculate_output(Prompt *prompt, Model *model);

#endif