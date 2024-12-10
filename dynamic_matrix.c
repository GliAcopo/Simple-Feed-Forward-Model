#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define VERBOSE 1

/* devo creare delle funzioni capaci di creare e gestire una tabella di archi di un grafo di dimensioni pari al numero di nodi*/

float** create_matrix_float(int rows, int columns){
    /* float values
    The funtion uses malloc dynamic allocation.
        The function creates the Matrix as a pointer to a vector of pointers, each pointer in said vector is a pointer to another vector which contains all the values.
        to better explain: the matrix is a pointer, this pointer references a vector, this vector (columns) cotains j pointers, each of these pointers reference a vector (rows) wich contains the values of the matrix
    :return: the pointer to said table*/

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

int init_matrix_to_float_value(float** matrix_pointer, int rows, int columns, float value){
    /* initialize the given matrix to a given float value
    :return: 0 if everything is correct, -1 if something went wrong*/

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            matrix_pointer[i][j] = value;
        }
    }
    return(0);
}

void free_float_matrix(float** matrix_pointer, int rows){
    /* frees the matrix allocated memory*/

    for (int i = 0; i < rows; i++){
        free(matrix_pointer[i]);
        matrix_pointer[i] = NULL;
    }

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

int change_value(float** matrix_pointer, int row, int column, float value_to_change){
    /* This function aims to change a value in the matrix in the indicated column and row, with the given value.
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

int main(int argc, char **argv){
    int i = 2;
    int j = i;

    float** matrix = create_matrix_float(i, j);
    print_matrix_float(matrix, i, j);
    init_matrix_to_float_value(matrix, i, j, 0.1);
    print_matrix_float(matrix, i, j);

    change_value(matrix, 0, 0, 27);
    print_matrix_float(matrix, i, j);

    free_float_matrix(matrix, i);
    change_value(matrix, 0, 0, 27);
    matrix = NULL;
    
    change_value(matrix, 0, 0, 27);
    print_matrix_float(matrix, i, j);

    return(0);
}