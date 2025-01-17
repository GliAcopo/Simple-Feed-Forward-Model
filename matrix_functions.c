/* devo creare delle funzioni capaci di creare e gestire una tabella di archi di un grafo di dimensioni pari al numero di nodi*/
#include "matrix_functions.h"
#include <stdio.h>
#include <stdlib.h>

#define VERBOSE 1


// Enumeration for matrix error codes
// WARNING!!! The inmplemented (and maybe even updated) definition of this MatrixError enum is in the "matrix.h" header file
/*
typedef enum {
    RETURN_FALSE = 1,                // No error; the called function returns false
    RETURN_TRUE = 0,                // No error; The called function returns true
    MATRIX_ERROR_NULL_POINTER = -1,    // Matrix pointer is NULL
    MATRIX_ERROR_INVALID_DIMENSIONS = -2, // Invalid dimensions (e.g., rows or columns <= 0)
    MATRIX_ERROR_ROW_NULL = -3,        // Specific row pointer is NULL
} MatrixError;
*/

/**
 * @brief Creates a matrix of doubles with specified rows and columns.
 *
 * This function dynamically allocates memory for a matrix represented as a
 * pointer to an array of pointers, each pointing to an array of doubles.
 * The function creates the Matrix as a pointer to a vector of pointers, each pointer in said vector is a pointer to another vector which contains all the values.
 * to better explain: the matrix is a pointer, this pointer references a vector, this vector (columns) cotains j pointers, each of these pointers reference a vector (rows) wich contains the values of the matrix
 *
 * @param rows Number of rows in the matrix.
 * @param columns Number of columns in the matrix.
 * @return Pointer to the created matrix, or NULL if allocation fails.
 */
double** create_matrix_double(int rows, int columns){

    // Allocate mem for pointers to rows
    double** matrix = (double **)malloc(rows * sizeof(double*));
    if (matrix == NULL){
        printf("Error in mem allocation for rows\n");
        return(NULL);
    }

    // Allocate mem for row values
    for (int i = 0; i < rows; i++){
        matrix[i] = (double *)malloc(columns * sizeof(double));
        if (matrix[i] == NULL){
            printf("Error in mem allocation for column %d\n", i);
            return(NULL);
        }
    }
    return(matrix);
}
  

/**
 * @brief Initialize the given matrix cells all to a given double type variable
 * 
 * @param matrix_pointer 
 * @param rows 
 * @param columns 
 * @param value The value that is going to bewritten in every cell
 * @return double** if the matrix is initialized correctly, else return a NULL
 */
double** init_matrix_to_double_value(double** matrix_pointer, int rows, int columns, double value){
    /*
    if (matrix_pointer == NULL){
        printf("Error: Cannot initialize a NULL matrix.\n");
        return NULL;
    }
    */

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            matrix_pointer[i][j] = value;
        }
    }
    return matrix_pointer;
}


/**
 * @brief checks if all elements of the matrix are equal to the given value
 * 
 * @param matrix_pointer 
 * @param rows 
 * @param columns 
 * @param value 
 * @return MatrixError = 1 if all elements are equal to value (true), 0 if at least one element is different from value (false)
 */
MatrixError check_if_all_elements_of_matrix_are_equal_to_value(double** matrix_pointer, int rows, int columns, double value){
    if (matrix_pointer == NULL){
        return MATRIX_ERROR_NULL_POINTER;
    }

    for (int i = 0; i < rows; i++){
        if (matrix_pointer[i] == NULL){
            return MATRIX_ERROR_ROW_NULL - i; // Custom error for specific row
        }
        for (int j = 0; j < columns; j++){
            if (matrix_pointer[i][j] != value){
                return RETURN_FALSE; 
            }
        }
    }
    return RETURN_TRUE;
}

/**
 * @brief frees the matrix allocated memory
 * 
 * @param matrix_pointer 
 * @param rows 
 */
void free_double_matrix(double** matrix_pointer, int rows){

    for (int i = 0; i < rows; i++){
        free(matrix_pointer[i]);
        matrix_pointer[i] = NULL;
    }

}


/**
 * @brief This function verifies if the matrix contains any null pointers
 * 
 * @param matrix_pointer 
 * @param rows 
 * @param columns 
 * @return MatrixError = {1, -1, -3...}. RETURN_TRUE = (1) if no null pointers are recognised; The number is negative if any exeptions are met -> If the exeption is in the rows, then the row number is given by the equation (-1)*(row_number + 3), so that a negative value is given when an error is encountered; MATRIX_ERROR_NULL_POINTER = -1 if the matrix is a NULL pointer
 */
MatrixError verify_matrix(double** matrix_pointer, int rows){
    
    if (matrix_pointer == NULL){
        return MATRIX_ERROR_NULL_POINTER;
    }

    for (int i = 0; i < rows; i++){
        if (matrix_pointer[i] == NULL){
            return MATRIX_ERROR_ROW_NULL - i; // Custom error for specific row
        }
    }
    return RETURN_TRUE;
}

/**
 * @brief Prints all matrix values in an actual "matrix form", as they are actually placed in the matrix
 * 
 * @param matrix_pointer 
 * @param rows 
 * @param columns 
 */
void print_matrix_double(double** matrix_pointer, int rows, int columns){
    printf("\n");
    if (matrix_pointer != NULL){
        for (int i = 0; i < rows; i++){
            if (matrix_pointer[i] == NULL){
                    printf("--- NULL ---");         // Prints null if a null pointer is found
            } else {
                for (int j = 0; j < columns; j++){
                    printf("%f ", matrix_pointer[i][j]);
                }
            }
        printf("\n");                               // newline for column row
        }
    } else {
        printf("\n--- invalid matrix pointer ---\n");
    }
    printf("\n");
}

[[deprecated("Use verify_matrix followed by change_matrix_value instead")]]
int change_value_matrix_secure(double** matrix_pointer, int row, int column, double value_to_change){
    /* DEPRECATED --> it is much better to verify the maatrix first with verify_matrix function, as this function runs every test every time it is called on the same matrix. If you want to change the calue on a matrix you are going to access many times then it is better to run first verify_matrix to check the matrix once and then run change_matrix_value
    This function aims to change a value in the matrix in the indicated column and row, with the given value.
    this function will handle the errors related to types and unexpected behaviour in pointers 
    VERBOSE constant abilitates debug messages
    :return: -1 if unexpected behaviour is recognised, else it returns 0
    */

    if (matrix_pointer == NULL){
        printf("\n ERROR: matrix pointer is NULL \n");
        return(-1);
    } else {
        if (matrix_pointer[row] == NULL){
            printf("\n ERROR: matrix pointer to row %d is null \n", row);
            return(-1);
        } else {
            if (VERBOSE != 0){
                printf("Previous matrix element was %f --> ", matrix_pointer[row][column]);
                matrix_pointer[row][column] = value_to_change;
                printf("It was changed to %f\n", matrix_pointer[row][column]);
            } else {
                matrix_pointer[row][column] = value_to_change;
            }
            
            // This logic cheks if the value was actually changed correctly
            if (matrix_pointer[row][column] == value_to_change){
                return(0);
            } else {
                return(-1);
            }
        }
    }

}

/**
 * @brief Changes the value in the matrix specified by the given coordinates to another double value specified by input 
 * 
 * @param matrix_pointer 
 * @param row 
 * @param column 
 * @param value_to_change 
 * @return int 
 */
MatrixError change_value_matrix(double** matrix_pointer, int row, int column, double value_to_change){

    #if VERBOSE
        printf("Previous matrix element was %f --> ", matrix_pointer[row][column]);
    #endif 
        matrix_pointer[row][column] = value_to_change;
    #if VERBOSE
        printf("It was changed to %f\n", matrix_pointer[row][column]);
    #endif
    
            
    // This logic cheks if the value was actually changed correctly
    if (matrix_pointer[row][column] == value_to_change){
        return RETURN_TRUE;                 // Since the value got actually changed, we return a success with Error = false
    } else {
        return MATRIX_ERROR_NULL_POINTER;   // returns -1 as a general error if the value wasn't actually changed
    }

}

/**
 * @brief Get the value in the matrix at the specified coordinates
 * 
 * @param matrix_pointer 
 * @param row 
 * @param column 
 * @return double, the value
 */
double get_value_matrix(double** matrix_pointer, int row, int column){
    #if VERBOSE
        printf("%f\n", matrix_pointer[row][column]);
    #endif
    return(matrix_pointer[row][column]);
}

/**
 * @brief it's just a function I run in order to test if everything works correctly
 * 
 */
void testmat(){
    int i = 2;
    int j = i;

    double** matrix = create_matrix_double(i, j);
    print_matrix_double(matrix, i, j);
    init_matrix_to_double_value(matrix, i, j, 0.1);
    print_matrix_double(matrix, i, j);

    change_value_matrix(matrix, 0, 0, 27);
    print_matrix_double(matrix, i, j);

    free_double_matrix(matrix, i);
    change_value_matrix(matrix, 0, 0, 27);
    matrix = NULL;
    
    change_value_matrix(matrix, 0, 0, 27);
    print_matrix_double(matrix, i, j);
}

