/* devo creare delle funzioni capaci di creare e gestire una tabella di archi di un grafo di dimensioni pari al numero di nodi*/
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define VERBOSE 1


// Enumeration for matrix error codes
typedef enum {
    RETURN_TRUE = 1,                // No error; The called function returns true
    RETURN_FALSE = 0,                // No error; the called function returns false
    MATRIX_ERROR_NULL_POINTER = -1,    // Matrix pointer is NULL
    MATRIX_ERROR_INVALID_DIMENSIONS = -2, // Invalid dimensions (e.g., rows or columns <= 0)
    MATRIX_ERROR_ROW_NULL = -3,        // Specific row pointer is NULL
} MatrixError;

/**
 * @brief Creates a matrix of floats with specified rows and columns.
 *
 * This function dynamically allocates memory for a matrix represented as a
 * pointer to an array of pointers, each pointing to an array of floats.
 * The function creates the Matrix as a pointer to a vector of pointers, each pointer in said vector is a pointer to another vector which contains all the values.
 * to better explain: the matrix is a pointer, this pointer references a vector, this vector (columns) cotains j pointers, each of these pointers reference a vector (rows) wich contains the values of the matrix
 *
 * @param rows Number of rows in the matrix.
 * @param columns Number of columns in the matrix.
 * @return Pointer to the created matrix, or NULL if allocation fails.
 */
float** create_matrix_float(int rows, int columns){

    // Allocate mem for pointers to rows
    float** matrix = (float **)malloc(rows * sizeof(float*));
    if (matrix == NULL){
        printf("Error in mem allocation for rows\n");
        return(NULL);
    }

    // Allocate mem for row values
    for (int i = 0; i < rows; i++){
        matrix[i] = (float *)malloc(columns * sizeof(float));
        if (matrix[i] == NULL){
            printf("Error in mem allocation for column %d\n", i);
            return(NULL);
        }
    }
    return(matrix);
}
  

/**
 * @brief Initialize the given matrix cells all to a given FLOAT type variable
 * 
 * @param matrix_pointer 
 * @param rows 
 * @param columns 
 * @param value The value that is going to bewritten in every cell
 * @return float** if the matrix is initialized correctly, else return a NULL
 */
float** init_matrix_to_float_value(float** matrix_pointer, int rows, int columns, float value){
    if (matrix_pointer == NULL){
        printf("Error: Cannot initialize a NULL matrix.\n");
        return NULL;
    }

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
MatrixError check_if_all_elements_of_matrix_are_equal_to_value(float** matrix_pointer, int rows, int columns, float value){
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

void free_float_matrix(float** matrix_pointer, int rows){
    /* frees the matrix allocated memory*/

    for (int i = 0; i < rows; i++){
        free(matrix_pointer[i]);
        matrix_pointer[i] = NULL;
    }

}

    /* This function verifies if the matrix contains any null pointers
    :return: 0 if no null pointers are recognised, -1 if any exeptions are met. If the exeption is in the rows, then the row number is given by the equation (-1)*(row_number + 2), so that a negative value is given when an error is encountered, whilist mantaining -1 as the "error number" for matrix pointer == NULL*/
/**
 * @brief This function verifies if the matrix contains any null pointers
 * 
 * @param matrix_pointer 
 * @param rows 
 * @param columns 
 * @return MatrixError = {1, -1, -3...}. RETURN_TRUE = (1) if no null pointers are recognised; The number is negative if any exeptions are met -> If the exeption is in the rows, then the row number is given by the equation (-1)*(row_number + 3), so that a negative value is given when an error is encountered; MATRIX_ERROR_NULL_POINTER = -1 if the matrix is a NULL pointer
 */
MatrixError verify_matrix(float** matrix_pointer, int rows, int columns){
    
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

void print_matrix_float(float** matrix_pointer, int rows, int columns){
    printf("\n");
    if (matrix_pointer != NULL){
        for (int i = 0; i < rows; i++){
            if (matrix_pointer[i] == NULL){
                    printf("--- NULL ---"); // Prints null if a null pointer is found
            } else {
                for (int j = 0; j < columns; j++){
                    printf("%f ", matrix_pointer[i][j]);
                }
            }
        printf("\n");   // newline for column row
        }
    } else {
        printf("\n--- invalid matrix pointer ---\n");
    }
    printf("\n");
}

__attribute__((deprecated("Use verify_matrix followed by change_matrix_value instead")))
int change_value_matrix_secure(float** matrix_pointer, int row, int column, float value_to_change){
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

int change_value_matrix_matrix(float** matrix_pointer, int row, int column, float value_to_change){


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

void test(){
    int i = 2;
    int j = i;

    float** matrix = create_matrix_float(i, j);
    print_matrix_float(matrix, i, j);
    init_matrix_to_float_value(matrix, i, j, 0.1);
    print_matrix_float(matrix, i, j);

    change_value_matrix(matrix, 0, 0, 27);
    print_matrix_float(matrix, i, j);

    free_float_matrix(matrix, i);
    change_value_matrix(matrix, 0, 0, 27);
    matrix = NULL;
    
    change_value_matrix(matrix, 0, 0, 27);
    print_matrix_float(matrix, i, j);
}

int main(int argc, char **argv){
    return(0);
}