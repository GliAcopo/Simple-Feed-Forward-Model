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
