#ifndef MATRIX_H
#define MATRIX_H

float** create_matrix_float(int rows, int columns);
int init_matrix_to_float_value(float** matrix_pointer, int rows, int columns, float value);
int check_if_all_elements_of_matrix_are_equal_to_value(float** matrix_pointer, int rows, int columns, float value);
void free_float_matrix(float** matrix_pointer, int rows);
void print_matrix_float(float** matrix_pointer, int rows, int columns);
int change_value_matrix_secure(float** matrix_pointer, int row, int column, float value_to_change);
int change_value_matrix(float** matrix_pointer, int row, int column, float value_to_change);
int verify_matrix(float** matrix_pointer, int rows, int columns);

float** create_adj_matrix_float_square(int layers, int nodes_per_layer);

#endif // MATRIX_H
