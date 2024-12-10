#ifndef MATRIX_H
#define MATRIX_H

// Enumeration for matrix error codes
typedef enum {
    RETURN_FALSE = 1,                // No error; the called function returns false
    RETURN_TRUE = 0,                // No error; The called function returns true
    MATRIX_ERROR = -1,
    MATRIX_ERROR_NULL_POINTER = -2,    // Matrix pointer is NULL
    MATRIX_ERROR_INVALID_DIMENSIONS = -3, // Invalid dimensions (e.g., rows or columns <= 0)
    MATRIX_ERROR_ROW_NULL = -4,        // Specific row pointer is NULL
} MatrixError;

float** create_matrix_float(int rows, int columns);
float** init_matrix_to_float_value(float** matrix_pointer, int rows, int columns, float value);
MatrixError check_if_all_elements_of_matrix_are_equal_to_value(float** matrix_pointer, int rows, int columns, float value);
void free_float_matrix(float** matrix_pointer, int rows);
MatrixError verify_matrix(float** matrix_pointer, int rows);
void print_matrix_float(float** matrix_pointer, int rows, int columns);
int change_value_matrix_secure(float** matrix_pointer, int row, int column, float value_to_change);
MatrixError change_value_matrix(float** matrix_pointer, int row, int column, float value_to_change);

#endif // MATRIX_H
