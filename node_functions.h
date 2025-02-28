#ifndef NODE_FUNCTIONS_H
#define NODE_FUNCTIONS_H

#include <stddef.h> // for size_t


/*          -+-+-+-+-+-+-+-+-+-+-+- THRESHOLD AND ACTIVATION FUNCTIONS -+-+-+-+-+-+-+-+-+-+-+- */

typedef double (*activation_function)(double x);
typedef double (*threshold_function)(double x);

//                                          FUNCTION PROTOTYPES
double mySigmoid(double x);
double myThresholdFunc(double x);
//                                         END FUNCTION PROTOTYPES

/*          -+-+-+-+-+-+-+-+-+-+-+- END THRESHOLD AND ACTIVATION FUNCTIONS -+-+-+-+-+-+-+-+-+-+-+- */

/*                      -+-+-+-+-+-+-+-+-+-+-+- STRUCT NODE -+-+-+-+-+-+-+-+-+-+-+- */

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

//                                          FUNCTION PROTOTYPES
Node create_node(int index, double bias,
    activation_function activation,
    threshold_function threshold);
//                                         END FUNCTION PROTOTYPES

/*                      -+-+-+-+-+-+-+-+-+-+-+- END STRUCT NODE -+-+-+-+-+-+-+-+-+-+-+- */

/*                      -+-+-+-+-+-+-+-+-+-+-+- STRUCT LAYER -+-+-+-+-+-+-+-+-+-+-+- */

/**
 * @brief A layer is a series of nodes separated by ONE GAP in wich there are the edges. It contains the array of nodes in the layer (top to bottom, left to right).
 * 
 * @param layer_number(int): The identifier of the layer, starts from 0 ATTENTION: If MORE macro is not active then this doesn't exist
 * @param layer_array_of_nodes(Node): An array of the nodes contained in the layer; the array goes top to bottom, left to right
 * @param rows_adj_matrix(__uint64_t): The number of rows the adj matrix has (The number of nodes at the left)
 * @param columns_adj_matrix(__uint64_t): The number of rows the adj matrix has (The number of nodes at the right)
 */
typedef struct Layer
{
    #if MORE        // The layer number isn't really necessary since we already have an ordered array of Layers in the Model struct
    int layer_number;        // The identifier of the layer, starts from 0.
    #endif
    Node* layer_array_of_nodes;
    size_t rows_of_adj_matrix;
    size_t columns_of_adj_matrix;
}Layer;

//                                          FUNCTION PROTOTYPES
Layer create_layer(size_t num_nodes,
    size_t rows_of_adj_matrix,
    size_t columns_of_adj_matrix,
    activation_function activation,
    threshold_function threshold);
//                                         END FUNCTION PROTOTYPES

/*                    -+-+-+-+-+-+-+-+-+-+-+- END STRUCT LAYER -+-+-+-+-+-+-+-+-+-+-+- */

/*                      -+-+-+-+-+-+-+-+-+-+-+- STRUCT MODEL -+-+-+-+-+-+-+-+-+-+-+- */

/**
 * @brief This structs encapsulates every aspect of the model, everything can be accessed from here.
 * 
 * @param model_name(char*): The name of the model, acts as a dynamically allocated array of characters.
 * @param number_of_layers_in_the_model(size_t): is the number of layers the model possesses.
 * @param model_layers(Layer): An ordered array containing the layers of the model, the first layer is the INPUT the last layer the OUTPUT while everything else the SECRET LAYER
 * @param model_weights(double***): An ordered array containing the pointer to the weights matrices. 
 */
typedef struct Model{
    char* model_name;
    size_t number_of_layers_in_the_model;
    Layer* model_layers;
    double*** model_weights;

}Model;

//                                          FUNCTION PROTOTYPES
Model* create_model(const char* name,
    Layer* model_layers,
    double*** model_weights);
//                                         END FUNCTION PROTOTYPES

/*                      -+-+-+-+-+-+-+-+-+-+-+- END STRUCT MODEL -+-+-+-+-+-+-+-+-+-+-+- */


/*                      -+-+-+-+-+-+-+-+-+-+-+- STRUCT PROMPT -+-+-+-+-+-+-+-+-+-+-+- */

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

//                                          FUNCTION PROTOTYPES
Prompt create_prompt(size_t length, double* tokens);
//                                         END FUNCTION PROTOTYPES

/*                     -+-+-+-+-+-+-+-+-+-+-+- END STRUCT PROMPT -+-+-+-+-+-+-+-+-+-+-+- */

/*                       -+-+-+-+-+-+-+-+-+-+-+- STRUCT OUTPUT -+-+-+-+-+-+-+-+-+-+-+- */

/**
 * @brief A simple struct to incapsulate the output array and the lenght of the array
 * 
 * @param is_valid(int): ( != 1 non valid; else then is valid) indicates if the output is valid or if something went wrong in its creation
 * @param used_model(Model*): The model used to calculate the output
 * @param data(double*): array of output tokens @attention: the data value is just a pointer to the last layer_outputs array, NOT A COPY
 * @param lenght(size_t): The positive integer number, how many elements are in the prompt array
 * @param layer_inputs(double**): a pointer to an array of pointers to arrays (basically matrix) containing the outputs generated by each layer
 * @param layer_outputs(double**): @todo
 * @deprecated (the following was a previous implementation that could theoretically reduce cache misses) The dimension of the array layer_inputs_and_outputs[i] are: i < used_model->number_of_layers_in_the_model * 2 ; i follows the logic of having first the input vector to the layer and then the output vector (input-output stored one after the other) so the dimension of the input/output of the layer [i] is used_model->model_layers.rows_of_adj_matrix
 */
typedef struct Output{
    char is_valid;
    Model* used_model;
    size_t length;  // size of the last output array
    double* data;
    /** @note To self: layer_inputs[i][j] -> i is the layer you want to take the input from; j is the specific input given to the j node */
    double** layer_inputs;
    double** layer_outputs;
} Output;

//                                          FUNCTION PROTOTYPES
Output empty_output(void);
Output calculate_output(Prompt* prompt, Model* model);
//                                         END FUNCTION PROTOTYPES

/*                     -+-+-+-+-+-+-+-+-+-+-+- END STRUCT OUTPUT -+-+-+-+-+-+-+-+-+-+-+- */

#endif // NODE_FUNCTIONS_H
