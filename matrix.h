#ifndef MATRIX_H
#define MATRIX_H

float** create_matrix_float(int rows, int columns);
int init_matrix_to_float_value(float** matrix_pointer, int rows, int columns, float value);
void free_float_matrix(float** matrix_pointer, int rows);
void print_matrix_float(float** matrix_pointer, int rows, int columns);
int change_value(float** matrix_pointer, int row, int column, float value_to_change);


#endif // MATRIX_H
