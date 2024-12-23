#include "matrix_functions.h"
#include "model_functions.h"
#include "settings.h"
#include <stdio.h>
#include <stdlib.h>

[[deprecated("This function will simply create a layers x nodes_per_layer matrix, and init it to 0, this function is not really useful nor optimised for the FF model, use other functions intead")]]
double** create_adj_matrix_double_square(int layers, int nodes_per_layer){
    /*
    This function creates an ajacence matrix of a model composed of tot layers and tot nodes per layer, this model is extremely simple since it is exentially a square
    The matrix gets checked toroughly and initialized to 0; 
    :return: The pointer to the matrix
    */
    double** matrix_pointer = create_matrix_double(layers, nodes_per_layer);
    matrix_pointer = init_matrix_to_double_value(matrix_pointer, layers, nodes_per_layer, 0);

    int check = verify_matrix(matrix_pointer, layers);
    if (check < 0){ 
        printf("\n verify_matrix returned an error: %d\n", check);
        return(NULL);
    } else if (check_if_all_elements_of_matrix_are_equal_to_value(matrix_pointer, layers, nodes_per_layer, 0) != RETURN_TRUE){
        printf("\n check_if_all_elements_of_matrix_are_equal_to_value returned an error: %d \n", check_if_all_elements_of_matrix_are_equal_to_value(matrix_pointer, layers, nodes_per_layer, 0));
        return(NULL);
    }
    #if VERBOSE
        printf("Nothing went wrong, returning matrix pointer...\n");
    #endif
    
    return(matrix_pointer);
}

/**
 * @brief This function will create a vector of matrices, each of the matrices will contain the weights of the edges connected to the nodes.
 * Since this is a fully connected FeedForward neural network we will represent the edges as follows: 0 = no connection, positive = edge from left to right (going forward); negative = edge from right to left (backwards)
 * @return matrices_vector (double***). This is a vector, each element of this vector contains the matrices of the weights for each layer of dimension (nodes_per_layer x nodes_per_layer), (yes, this neural network is a square)
 */
double*** create_FF_model_matrices(int layers, int nodes_per_layer){
    double*** matrices_vector = (double ***)malloc(layers * sizeof(double**));                         // Creating the vector to store the matrices

    for (int i = 0; i < layers; i++){
        double** matrix = create_matrix_double(nodes_per_layer, nodes_per_layer);                     // creating the matrix of nodes x nodes
        matrix = init_matrix_to_double_value(matrix, nodes_per_layer, nodes_per_layer, (double)1);    // initiating the matrix to value 1 to symbolise the connections

        // security checks to identify problem in matrix creation{
            int check = verify_matrix(matrix, nodes_per_layer);                                     // verifiyng that the matrix is valid
            if (check < 0){ 
                printf("\n verify_matrix returned an error: %d\n", check);
                return(NULL);
            }
            check = check_if_all_elements_of_matrix_are_equal_to_value(matrix, nodes_per_layer, nodes_per_layer, (double)1);
            if (check != RETURN_TRUE){
                printf("\n check_if_all_elements_of_matrix_are_equal_to_value returned an error: %d \n", check);
                return(NULL);
            }
        //}

        matrices_vector[i] = matrix;
        // free_double_matrix(matrix, nodes_per_layer); THIS DOESN'T WORK: memory getting freed is memory containing the matrices
    }
    return(matrices_vector);
}

void print_matrix_vector_double(double*** matrices_vector, int layers, int nodes_per_layer){
    for (int layer = 0; layer < layers; layer++){
        print_matrix_double(matrices_vector[layer], nodes_per_layer, nodes_per_layer);
    }
}


