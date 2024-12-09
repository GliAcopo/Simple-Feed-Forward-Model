#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

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
    for (int i = 0; i < columns; i++){
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

}

void free_float_matrix(float** matrix_pointer, int rows){
    /* frees the matrix allocated memory*/

    for (int i = 0; i < rows; i++){
        free(matrix_pointer[i]);
    }
}

