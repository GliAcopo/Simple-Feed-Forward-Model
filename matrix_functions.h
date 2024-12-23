#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

// Enumeration for matrix error codes
typedef enum {
    RETURN_FALSE = 1,                // No error; the called function returns false
    RETURN_TRUE = 0,                // No error; The called function returns true
    MATRIX_ERROR = -1,
    MATRIX_ERROR_NULL_POINTER = -2,    // Matrix pointer is NULL
    MATRIX_ERROR_INVALID_DIMENSIONS = -3, // Invalid dimensions (e.g., rows or columns <= 0)
    MATRIX_ERROR_ROW_NULL = -4,        // Specific row pointer is NULL
} MatrixError;

double** create_matrix_double(int rows, int columns);
double** init_matrix_to_double_value(double** matrix_pointer, int rows, int columns, double value);
MatrixError check_if_all_elements_of_matrix_are_equal_to_value(double** matrix_pointer, int rows, int columns, double value);
void free_double_matrix(double** matrix_pointer, int rows);
MatrixError verify_matrix(double** matrix_pointer, int rows);
void print_matrix_double(double** matrix_pointer, int rows, int columns);
int change_value_matrix_secure(double** matrix_pointer, int row, int column, double value_to_change);
MatrixError change_value_matrix(double** matrix_pointer, int row, int column, double value_to_change);

#endif // MATRIX_FUNCTIONS_H
