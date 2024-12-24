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
typedef struct {
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
 * @brief This is the layer of the model, it contains the array of nodes in the layer (top to bottom, left to right) and the relative adj matrix 
 * 
 * @param layer_number(int): The identifier of the layer, starts from 0
 * @param layer_array_of_nodes(Node): An array of the nodes contained in the layer; the array goes top to bottom, left to right
 * @param rows_adj_matrix(__uint64_t): The number of rows the adj matrix has
 * @param columns_adj_matrix(__uint64_t): The number of rows the adj matrix has
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
 * @param number_of_layers_in_the_model(__uint64_t): is the number of layers the model possesses.
 * @param model_layers(Layer): An ordered array containing the layers of the model, the first layer is the INPUT the last layer the OUTPUT while everything else the SECRET LAYER
 * @param model_weights(double***): An ordered array containing the pointer to the weights matrices. 
 */
typedef struct Model{

    __uint64_t number_of_layers_in_the_model;
    Layer* model_layers;
    double*** model_weights;

}Model;


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
 * @param prompt(array of doubles): The prompt is what will be processed by the model to create an output, the single parameters inside the array will be fed to the INPUT nodes (first element of the array goes into the top node, first node in list). ATTENTION: If the list of parameters terminates early an error is given (I would actually like to do that the missing parameters will be assumed as 0.)
 * @param model(Model): The model struct, the model that will be used to calculate the output.
 * @return (Output_struct): The array of output values calculated by the model. (The order is first element of the array comes from the top node)
 */
Output calculate_output(Model used_model, Prompt prompt){
    Output output;
    // Calculate output of the first layer (to handle invalid prompts) and then continue with next layers.
    if (prompt.length != used_model.model_layers[0].rows_of_adj_matrix){ //// TODO: control if the number of input nodes is actually the layers
        //// logic for handling incorrect prompts goes here
        printf("Invalid pompt lenght. Accepted lenght by model: %d; Given prompt lenght: %d.\n", used_model.model_layers[0].rows_of_adj_matrix, prompt.length);
        output.length = 0; output.data = NULL;
        return(output);
    }
    else{
        // For the first layer we just need to pass everything trough the activation function of the neurons.
        size_t first_layer_size = used_model.model_layers[0].rows_of_adj_matrix;        // Number of nodes in the first layer
        double* input_layer_result = malloc(first_layer_size * sizeof(double));         // Allocate space for the first layer’s result
        if (input_layer_result == NULL) {                                               // Handle allocation failure
            printf("Not enough memory in input layer result allocation\n");
            output.length = 0; output.data = NULL;
            return(output);
        }
        for (size_t j; j < first_layer_size; j++){
            input_layer_result[j] = used_model.model_layers[0].layer_array_of_nodes[j].activation(prompt.data[j]);
        }
        // Now handle layers 1..N-1 (since layer 0 is handled above)
        for (size_t i = 1; i < used_model.number_of_layers_in_the_model; i++) {
            double* layer_result = malloc(used_model.model_layers[i].rows_of_adj_matrix * sizeof(double));
            if (layer_result == NULL) {                                                         // Handle allocation failure
                free(input_layer_result);
                printf("Not enough memory in layer %d result allocation\n", i);
                output.length = 0; output.data = NULL;
                return(output);
            }
        // 1) Multiply the previous results by the adjacency matrix
        // 2) Pass the result to the activation function of each node

        // E.g. (pseudocode):
        // for each node j in layer i:
        //     double sum = 0;
        //     for k in 0..(previous layer size - 1):
        //         sum += (some_matrix[i][j][k]) * input_layer_result[k];
        //     sum += bias; // optional
        //     layer_result[j] = used_model.model_layers[i]
        //                              .layer_array_of_nodes[j]
        //                              .activation(sum);
        }
        free(input_layer_result);
        return(output); //// maybe I should make an output struct, like for prompts so that I alaways know the dimension of the output array 
    }
    
}