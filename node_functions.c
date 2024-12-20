#include "settings.h"
#include "node_functions.h"
#include <stdlib.h>
#include <stdio.h>

/*  Since every node needs an activation function ad a threshold funtion I'll use a pointer to function in every node to point to said functions.
    */
typedef float (*activation_function)(float x);
typedef float (*threshold_function)(float x);

/**
 * @brief A struct representing each node of the model
 * 
 * @param index(int): the index is the identifier of the node, the "label" that represents the node numerically. It is advisable to not use the same label for more than one Node since there is't a system to verify uniquity. 
 * @param output(double): The output value that is given by the node, it is a float value by default.
 * @param bias(double): -> "a bias value allows you to shift the activation function to the left or right" -> https://stackoverflow.com/questions/2480650/what-is-the-role-of-the-bias-in-neural-networks
 * @param activation(activation_function): Pointer to the node function
 * @param threshold(threshold_function): Pointer to Node function. Facultative function for the activation of the node
 */
typedef struct {
    int index;                      // Index of the node (es: node 1, node 2)
    double output;                  // The output that the node calculated
    double bias;                    // Bias -> "a bias value allows you to shift the activation function to the left or right" -> https://stackoverflow.com/questions/2480650/what-is-the-role-of-the-bias-in-neural-networks

    activation_function activation; // Pointer to the node function
    threshold_function threshold;   // Facultative function for the activation of the node

    // Backpropagation value for the training of the model (not entirely sure how to use it tho...)
    double delta;                   // the "error"

} Node;

/**
 * @brief This is the layer of the model, it contains the array of nodes in the layer (top to bottom, left to right) and the relative adj matrix 
 * 
 * @param layer_number(int): The identifier of the layer, starts from 0
 * @param nodes(Node): An array of the nodes contained in the layer; the array goes top to bottom, left to right
 * @param rows_adj_matrix(int): The number of rows the adj matrix has
 * @param columns_adj_matrix(int): The number of rows the adj matrix has
 * @param adj_matrix(float**): The adj matrix of the layer
 */
typedef struct Layer
{
    int layer_number;        // The identifier of the layer, starts from 0.
    Node* nodes;
    int rows_adj_matrix;
    int columns_adj_matrix;
    float** adj_matrix;
}Layer;


Layer init_layer(int layer_number, Node* array_of_nodes_present_in_the_layer, float*** vector_containing_the_matrices){

}
