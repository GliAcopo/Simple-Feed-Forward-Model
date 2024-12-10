#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "model_functions.h"

float** create_adj_matrix_float_square(int layers, int nodes_per_layer){
    /*
    This function creates an ajacence matrix of a model composed of tot layers and tot nodes per layer, this model is extremely simple since it is exentially a square
    The matrix gets checked toroughly and initialized to 0; 
    :return: The pointer to the matrix
    */
    float** matrix_pointer = create_matrix_float(layers, nodes_per_layer);
    matrix_pointer = init_matrix_to_float_value(matrix_pointer, layers, nodes_per_layer, 0);
    if (verify_matrix(matrix_pointer, layers, nodes_per_layer) == -1){ 
        printf("\n verify_matrix returned -1 \n");
            return(NULL);
    } else if (check_if_all_elements_of_matrix_are_equal_to_value(matrix_pointer, layers, nodes_per_layer, 0) == 0){
        printf("\n check_if_all_elements_of_matrix_are_equal_to_value returned 0 \n");
        return(NULL);
    }
    return(matrix_pointer);
}

int main(int argc, char** argv){
    return(0);
}