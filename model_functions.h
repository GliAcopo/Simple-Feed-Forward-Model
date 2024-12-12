#ifndef MODEL_FUNCTIONS_H
#define MODEL_FUNCTIONS_H

float** create_adj_matrix_float_square(int layers, int nodes_per_layer);
float** create_FF_model_matrices(int layers, int nodes_per_layer);
void print_matrix_vector_float(float** matrices_vector, int layers, int nodes_per_layer);

#endif