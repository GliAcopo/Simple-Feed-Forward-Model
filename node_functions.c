#include "settings.h"
#include "node_functions.h"
#include <stdlib.h>
#include <stdio.h>

/*  Since every node needs an activation function ad a threshold funtion I'll use a pointer to function in every node to point to said functions.
    */
typedef double (*activation_function)(double x);
typedef double (*threshold_function)(double x);

/**
 * @brief A struct representing each node of the model
 * 
 * @param index(int): the index is the identifier of the node, the "label" that represents the node numerically. It is advisable to not use the same label for more than one Node since there is't a system to verify uniquity. 
 * @param output(double): The output value that is given by the node, it is a double value by default.
 * @param bias(double): -> "a bias value allows you to shift the activation function to the left or right" -> https://stackoverflow.com/questions/2480650/what-is-the-role-of-the-bias-in-neural-networks
 * @param activation(activation_function): Pointer to the node function
 * @param threshold(threshold_function): Pointer to Node function. Facultative function for the activation of the node
 */
typedef struct Node{
    int index;                      // Index of the node (es: node 1, node 2)
    double output;                  // The output that the node calculated
    double bias;                    // Bias -> "a bias value allows you to shift the activation function to the left or right" -> https://stackoverflow.com/questions/2480650/what-is-the-role-of-the-bias-in-neural-networks

    activation_function activation; // Pointer to the node function
    threshold_function threshold;   // Facultative function for the activation of the node

    // Backpropagation value for the training of the model (not entirely sure how to use it tho...)
    double delta;                   // the "error"

} Node;
/*USE CASE:
double mySigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
Node n;
n.activation = &mySigmoid;
or 
n.activation = mySigmoid; ???
*/

/**
 * @brief A layer is a series of nodes separated by ONE GAP in wich there are the edges. It contains the array of nodes in the layer (top to bottom, left to right).
 * 
 * @param layer_number(int): The identifier of the layer, starts from 0 ATTENTION: If MORE macro is not active then this doesn't exist
 * @param layer_array_of_nodes(Node): An array of the nodes contained in the layer; the array goes top to bottom, left to right
 * @param rows_adj_matrix(__uint64_t): The number of rows the adj matrix has (The number of nodes at the left)
 * @param columns_adj_matrix(__uint64_t): The number of rows the adj matrix has (The number of nodes at the right)
 * @param adj_matrix(double**): The adj matrix of the layer
 */
typedef struct Layer
{
    #if MORE        // The layer number isn't really necessary since we already have an ordered array of Layers in the Model struct
    int layer_number;        // The identifier of the layer, starts from 0.
    #endif
    Node* layer_array_of_nodes;
    __uint64_t rows_of_adj_matrix;
    __uint64_t columns_of_adj_matrix;
    #if MORE        // It is probably reduntant since we already have all the matrices in the Model struct
    double** adj_matrix;
    #endif
}Layer;

/**
 * @brief This structs encapsulates every aspect of the model, everything can be accessed from here.
 * 
 * @param model_name(char*): The name of the model, acts as a dynamically allocated array of characters.
 * @param number_of_layers_in_the_model(__uint64_t): is the number of layers the model possesses.
 * @param model_layers(Layer): An ordered array containing the layers of the model, the first layer is the INPUT the last layer the OUTPUT while everything else the SECRET LAYER
 * @param model_weights(double***): An ordered array containing the pointer to the weights matrices. 
 */
typedef struct Model{
    char* model_name;
    __uint64_t number_of_layers_in_the_model;
    Layer* model_layers;
    double*** model_weights;

}Model;

/**
 * @brief A first try for a function to Create a model object (not complete, I'm not so sure I want it to be a pointer)
 * 
 * @param name 
 * @return Model* 
 */
Model* create_model(const char* name) {
    Model* model = malloc(sizeof(Model));
    if (!model) {
        fprintf(stderr, "Allocation error\n");
        return NULL;
    }

    // Initialize fields, for example:
    model->number_of_layers_in_the_model = 0;
    model->model_layers = NULL;
    model->model_weights = NULL;

    // Allocate space for the name and copy it
    model->model_name = malloc(strlen(name) + 1);
    if (!model->model_name) {
        fprintf(stderr, "Allocation error\n");
        free(model);  // Clean up partially allocated model
        return NULL;
    }
    strcpy(model->model_name, name);

    return model;
}


Layer init_layer(int layer_number, Node* array_of_nodes_present_in_the_layer, double*** vector_containing_the_matrices){

}

/**
 * @brief A simple struct to incapsulate the prompt array and the lenght of the array
 * 
 * @param data(double*): The array of tokens
 * @param lenght(size_t): The positive integer number, how many elements are in the prompt array
 */
typedef struct Prompt{
    double* data;   // Pointer to the array of input values
    size_t length;  // Number of input values
} Prompt;

/**
 * @brief Create a prompt object with an allocated array of the given length.
 * 
 * @param length(size_t): The number of elements the array is composed of
 * @param tokens(double*): The pointer to the array containing the tokens for the model.
 * @return Prompt 
 */
Prompt create_prompt(size_t length, double* tokens) {
    Prompt prompt;
    prompt.data = (double*)malloc(length * sizeof(double));
    prompt.length = length;
    for (size_t i = 0; i < length; i++) {
        prompt.data[i] = (double)tokens[i];
    }
    return prompt;
}

/**
 * @brief A simple struct to incapsulate the output array and the lenght of the array
 * 
 * @param data(double*): The array of output tokens
 * @param lenght(size_t): The positive integer number, how many elements are in the prompt array
 */
typedef struct Output{
    double* data;   // Pointer to the array of input values
    size_t length;  // Number of input values
} Output;

/**
 * I had kind of an enlightment about the calculation of the output, I was alaways scared of how I could pass the output of 
 * the previous layers and the next ones, and I actually had no idea on how to extract every single weight of the matrix to 
 * then calculate the input for the next nodes.
 * But now I think I had a pretty solid idea! 
 * The idea is that I can transform the outputs of the nodes into a vertical vector! Then I multiply this vector to the 
 * adj.Matrix. This would be exactly like multipliyng the output for each node! Obtaining an input for the next nodes.
 * I actually knew that modern models used matrices to do their calculations, but now I understand why and how!
 */

/**
 * @brief Calculates the output using the given prompt and model. 
 * 
 * @param prompt (Prompt Object): The prompt that will be processed by the model
 * @param used_model (pointer to Model): The model struct used to calculate the output
 * @return (Output Object): The array of output values calculated by the model
 */
Output calculate_output(const Model* used_model, Prompt prompt) {
    Output output;

    size_t first_layer_size = used_model->model_layers[0].rows_of_adj_matrix;
    // Check the prompt length vs. input layer size
    if (prompt.length != first_layer_size) {
        printf("Invalid prompt length. "
               "Accepted length by model: %d; Given prompt length: %d.\n",
               used_model->model_layers[0].rows_of_adj_matrix,
               prompt.length);
        output.length = 0;
        output.data   = NULL;
        return output;
    }

    // Handle first layer: simply apply activation functions directly on prompt inputs
    double* layer_input = malloc(first_layer_size * sizeof(double));
    if (layer_input == NULL) {
        printf("Not enough memory in input layer result allocation.\n");
        output.length = 0;
        output.data   = NULL;
        return output;
    }

    for (size_t j = 0; j < first_layer_size; j++) {
        layer_input[j] = used_model->model_layers[0].layer_array_of_nodes[j].activation(prompt.data[j]);
    }

    // Process the layers after the input layer (i > 0)
    size_t i = 1;                                           // Since we use layer information even outside the loop, we need the variable to remain visible 
    for (i < used_model->number_of_layers_in_the_model; i++;) {
        // When we arrive at the end of the model we exit the loop
        if (used_model->model_weights[i] == NULL){
            break;
        }

        size_t output_size = used_model->model_layers[i].columns_of_adj_matrix;
        size_t input_size  = used_model->model_layers[i].rows_of_adj_matrix; // same as the size of layer_input

        double* layer_output = malloc(output_size * sizeof(double));
        if (layer_output == NULL) {
            free(layer_input);
            printf("Not enough memory in layer %zu result allocation.\n", i);
            output.length = 0;
            output.data   = NULL;
            return output;
        }


        // 1) Multiply the previous layer outputs by the adjacency matrix 
        // 2) Pass the result to the activation function of each node
        for (size_t col = 0; col < output_size; col++) {
        double sum = 0.0;
            for (size_t row = 0; row < input_size; row++) {
                sum += layer_input[row] * used_model->model_weights[i][row][col];
            }
            // If there's a bias term
            sum += used_model->model_layers[i].layer_array_of_nodes[col].bias;
            // Pass the sum through the layer's activation function
            layer_output[col] = used_model->model_layers[i].layer_array_of_nodes[col].activation(sum);
        }

        // Use the output of this layer as input for the next one:
        free(layer_input);
        layer_input = layer_output;
    }

    // Once finished, 'layer_input' should contain the final output.
    output.length = used_model->model_layers[i-1].columns_of_adj_matrix;
    output.data   = layer_input;
    free(layer_input);
    
    return output;
}
