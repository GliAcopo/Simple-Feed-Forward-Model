#include "settings.h"
#include "node_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

/*  Since every node needs an activation function ad a threshold funtion I'll use a pointer to function in every node to point to said functions.*/
typedef double (*activation_function)(double x);
typedef double (*threshold_function)(double x);

double mySigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double myThresholdFunc(double x) {
    // Just a dummy example threshold
    return (x > 0.5) ? 1.0 : 0.0;
}

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

/**
 * @brief Create a node object
 * 
 * @param index 
 * @param bias 
 * @param activation 
 * @param threshold 
 * @return Node 
 */
Node create_node(int index, double bias, activation_function activation, threshold_function threshold){
    Node n;
    n.index = index;
    n.output = 0.0;         // Default output
    n.bias = bias;
    n.activation = activation;
    n.threshold = threshold; 
    n.delta = 0.0;          // Default delta
    return n;
}

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

Layer create_layer(size_t num_nodes,
                   size_t rows_of_adj_matrix,
                   size_t columns_of_adj_matrix,
                   activation_function activation,
                   threshold_function threshold){
    Layer layer;
    layer.rows_of_adj_matrix = rows_of_adj_matrix;
    layer.columns_of_adj_matrix = columns_of_adj_matrix;

    // Allocate space for the nodes
    layer.layer_array_of_nodes = malloc(num_nodes * sizeof(Node));
    if (layer.layer_array_of_nodes == NULL) {
        fprintf(stderr, "Failed to allocate layer_array_of_nodes\n");
        // For a real project, handle errors more gracefully
        Layer empty = {0};
        return empty;
    }

    // Initialize each node
    for (size_t i = 0; i < num_nodes; i++) {
        // Example: bias=0.0 for all nodes
        layer.layer_array_of_nodes[i] = create_node(
            (int)i,            /* index */
            0.0,               /* bias  */
            activation,        /* activation  */
            threshold          /* threshold   */
        );
    }

    return layer;
}
/*
Layer init_layer(int layer_number, Node * array_of_nodes_present_in_the_layer, double*** vector_containing_the_matrices){
    //// TODO -> Make this function.
    (void)layer_number;
    (void)array_of_nodes_present_in_the_layer;
    (void)vector_containing_the_matrices;
    
    Layer empty_layer = {0};
    return empty_layer;
}
*/

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

/**
 * @brief A first try for a function to Create a model object (not complete, I'm not so sure I want it to be a pointer)
 * 
 * @param name(const char*): The name of the model, passed as a dynamically allocated array of chars.abort
 * @param model_layer(Layer*): The pointer to the array of layers.
 * @param model_weights: The array of matrices containing the weights of the model.
 * @return Model* 
 */
Model* create_model(const char* name, Layer* model_layers, double*** model_weights) {
    Model* model = malloc(sizeof(Model));
    if (!model) {
        fprintf(stderr, "Allocation error\n");
        return NULL;
    }
    // Initialize fields
    model->number_of_layers_in_the_model = 0;
    model->model_layers = model_layers;
    model->model_weights = model_weights;
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


/**
 * @brief A function that creates an empty output object (used for good practice/security purpose in returns of functions)
 * @return Output object, with all parameters set to NULL
 */
Output empty_output(){
    Output empty_output;
    empty_output.is_valid = -1;
    empty_output.data = NULL;
    empty_output.used_model = NULL;
    empty_output.length = 0;
    empty_output.layer_inputs = NULL;
    empty_output.layer_outputs = NULL;
    return(empty_output);
}

Output calculate_output(Prompt* prompt, Model* model){
    #if DEBUG
    fprintf(stderr, "Started checks in %s function with\nprompt: %p\nmodel: %p\n", __func__, prompt, model);
    #endif
    if (prompt == NULL) {
        fprintf(stderr, "Error in %s: 'prompt' is NULL.\n", __func__);
        return empty_output();
    }
    if (model == NULL) {
        fprintf(stderr, "Error in %s: 'model' is NULL.\n", __func__);
        return empty_output();
    }
    if (prompt->data == NULL){
        fprintf(stderr, "Error in %s: 'prompt data' is NULL.\n", __func__);
        return empty_output();
    }
    if (model->model_layers == NULL){
        fprintf(stderr, "Error in %s: 'model_layers' is NULL.\n", __func__);
        return empty_output();
    }
    if (prompt->length != model->model_layers[0].rows_of_adj_matrix){
        fprintf(stderr, "Error in %s: prompt size (%zu) is different from the size of the first layer of model (%zu).\n", __func__, prompt->length, model->model_layers[0].rows_of_adj_matrix);
        return empty_output();
    }
    #if DEBUG
    fprintf(stderr, "Successfully exited checks in %s function with\nprompt: %p\nmodel: %p\n", __func__, prompt, model);
    #endif

    Output output;
    output.is_valid = 1;
    output.used_model = model;

    /** @brief we need to allocate space for the inputs and outputs of each layer,
     * @note every layer has a vector of inputs and produces a vector as an output (dimension is the number of nodes), so we need an array of dimension number_of_layers * 2
     * @details this array is structured as follows: starts with the prompt input, follows with the output of the first layer (input-output)
     */
    output.layer_inputs = malloc(model->number_of_layers_in_the_model * sizeof(double*));
    if (output.layer_inputs == NULL){
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_inputs_and_outputs' is NULL.\n", __func__);
        return empty_output();
    }
    output.layer_outputs = malloc(model->number_of_layers_in_the_model * sizeof(double*));
    if (output.layer_outputs == NULL){
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_inputs_and_outputs' is NULL.\n", __func__);
        return empty_output();
    }

    /** We copy the prompt into the layer_inputs_and_outputs first array in order for it to be registered for 
     * training purposes while also maintaining the main loop as straightforward as possible */
    output.layer_inputs[0] = malloc(prompt->length * sizeof(double*));
    if (output.layer_inputs[0] == NULL){
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_inputs[%d]' is NULL.\n", __func__, 0);
        return empty_output();
    }
    for (size_t i; i < prompt->length; i++){
        output.layer_inputs[0][i] = prompt->data[i];
    }
    #if DEBUG
        printf("Prompt was copied into first input array.\nInput array:\n 1) ");
        for (size_t i; i < prompt->length; i++){
            printf("%d, ", output.layer_inputs[0][i]);
        }
            printf("Prompt array:\n2) ")
        for (size_t i; i < prompt->length; i++){
            printf("%d, ", prompt->data[i]);
        }
    #endif

    // MAIN LOOP
    for (size_t i; i < model->number_of_layers_in_the_model - 1; i++){
        /** 1) pass the input trough each node */
        output.layer_outputs[i] = malloc(model->model_layers[i].rows_of_adj_matrix * sizeof(double));
        if (output.layer_outputs[i] == NULL){
            fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_outputs[%zu]' is NULL.\n", __func__, i);
            return empty_output();
        }
        for (size_t j; j < model->model_layers[i].rows_of_adj_matrix; j++){
            /** 2) register output of each node into the output array */
            double input_to_node = output.layer_inputs[i][j] + model->model_layers[i].layer_array_of_nodes[j].bias; // variable to enhance code readability (input + bias)
            output.layer_outputs[i][j] = model->model_layers[i].layer_array_of_nodes[j].activation(input_to_node);      
        }

        /** MATRIX MULTIPLICATION
         * My layers output a vector in the form of V[n], where n is the number of nodes in the layer (also represented by the rows of the adj matrix)
         *  while the matrix is M[n][m]; 
         *  Row vector V[1][n] * M[n][m] is the operation to do.
         * **/
        // Allocate memory for the next layer's input vector.
        // model->model_layers[i+1].rows_of_adj_matrix represents the number of nodes (m) in the next layer.
        output.layer_inputs[i+1] = malloc(model->model_layers[i+1].rows_of_adj_matrix * sizeof(double));

        // Iterate over each node in the next layer (each column of the weight matrix)
        for (size_t column = 0; column < model->model_layers[i+1].rows_of_adj_matrix; column++) {
            double sum = 0.0;

            // For each node in the current layer (each row of the weight matrix)
            for (size_t row = 0; row < model->model_layers[i].rows_of_adj_matrix; row++) {
                // Multiply the output from the current layer's node (row index)
                // by the weight connecting that node to the next layer's node (column index)
                sum += output.layer_outputs[i][row] * model->model_weights[i][row][column];
            }
            // Store the computed sum into the next layer's input vector at the index corresponding to the node (column)
            output.layer_inputs[i+1][column] = sum;
        }

    }
    /** 5) Handling last layer (output layer) */
    size_t const k = model->number_of_layers_in_the_model - 1; // enhancing readability
    output.length = model->model_layers[k].rows_of_adj_matrix; // is this varible really useful? It can be obtained from model->model_layers[k].rows_of_adj_matrix where k = model->number_of_layers_in_the_model - 1
    /** 5.1) pass the input trough each node */
    output.layer_outputs[k] = malloc(model->model_layers[k].rows_of_adj_matrix * sizeof(double));
    if (output.layer_outputs[k] == NULL)
    {
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_outputs[%zu]' (last layer) is NULL.\n", __func__, k);
        return empty_output();
    }
    for (size_t j; j < model->model_layers[k].rows_of_adj_matrix; j++)
    {
        /** 5.2) register output of each node into the output array */
        double input_to_node = output.layer_inputs[k][j] + model->model_layers[k].layer_array_of_nodes[j].bias;
        output.layer_outputs[k][j] = model->model_layers[k].layer_array_of_nodes[j].activation(input_to_node);
    }
    output.data = output.layer_outputs[k];
    return(output);
}

void test_calculate_output(void) 
{
    printf("Starting test_calculate_output()\n");

    // 1) Create a small Model with two layers:
    //    - First layer: 2 nodes (input layer)
    //    - Second layer: 1 node (output layer)
    Model testModel;
    testModel.model_name = "TestModel";
    testModel.number_of_layers_in_the_model = 2;
    printf("Model '%s' with %zu layers\n", testModel.model_name, testModel.number_of_layers_in_the_model);

    // Allocate the layer array
    printf("Allocating model_layers...\n");
    testModel.model_layers = malloc(testModel.number_of_layers_in_the_model * sizeof(Layer));
    if (!testModel.model_layers) {
        fprintf(stderr, "Failed to allocate model_layers\n");
        return;
    }
    printf("model_layers allocated successfully\n");

    // Create first layer: 2 nodes, with a 2x2 adjacency matrix
    printf("Creating first layer (index 0)...\n");
    testModel.model_layers[0] = create_layer(
                                    /* num_nodes */ 2,
                                    /* rows_of_adj_matrix */ 2,
                                    /* columns_of_adj_matrix */ 2,
                                    mySigmoid,
                                    myThresholdFunc);
    printf("First layer created\n");

    // Create second layer: 1 node, expecting 2 inputs (so 2x1 matrix)
    printf("Creating second layer (index 1)...\n");
    testModel.model_layers[1] = create_layer(
                                    /* num_nodes */ 1,
                                    /* rows_of_adj_matrix */ 2,
                                    /* columns_of_adj_matrix */ 1,
                                    mySigmoid,
                                    NULL);
    printf("Second layer created\n");

    // 2) Allocate weight matrices.
    // We allocate an array with as many elements as there are layers.
    // Convention: For layer 0 (input) we don't need weights, so set to NULL.
    printf("Allocating model_weights array...\n");
    testModel.model_weights = malloc(testModel.number_of_layers_in_the_model * sizeof(double**));
    if (!testModel.model_weights) {
        fprintf(stderr, "Failed to allocate model_weights\n");
        free(testModel.model_layers);
        return;
    }
    printf("model_weights array allocated successfully\n");

    // For layer 0, no weight matrix is needed.
    testModel.model_weights[0] = NULL;
    printf("model_weights[0] set to NULL\n");

    // For layer 1, allocate a [2 x 1] matrix (2 rows, 1 column)
    // IMPORTANT: Allocate at index 1, not index 0.
    printf("Allocating weight matrix for second layer (model_weights[1])...\n");
    testModel.model_weights[0] = malloc(2 * sizeof(double*));
    if (!testModel.model_weights[1]) {
        fprintf(stderr, "Failed to allocate weight matrix for layer 1\n");
        free(testModel.model_layers);
        free(testModel.model_weights);
        return;
    }
    for (size_t r = 0; r < 2; r++) {
        testModel.model_weights[1][r] = malloc(1 * sizeof(double));
        if (!testModel.model_weights[1][r]) {
            fprintf(stderr, "Failed to allocate weight matrix row %zu for layer 1\n", r);
            for (size_t j = 0; j < r; j++) {
                free(testModel.model_weights[1][j]);
            }
            free(testModel.model_weights[1]);
            free(testModel.model_layers);
            free(testModel.model_weights);
            return;
        }
    }
    printf("Weight matrix for layer 1 allocated successfully\n");

    // Set weight matrix values:
    // Weight matrix W = [ [0.5],
    //                     [0.8] ]
    printf("Setting weight values...\n");
    testModel.model_weights[1][0][0] = 0.5;
    testModel.model_weights[1][1][0] = 0.8;
    printf("Weights set: W[0][0]=0.5, W[1][0]=0.8\n");

    // Set bias for the single node in layer 1
    printf("Setting bias for layer 1 node...\n");
    testModel.model_layers[1].layer_array_of_nodes[0].bias = 0.1;
    printf("Bias for layer 1 node set to 0.1\n");

    // 3) Create a Prompt with 2 inputs
    printf("Allocating prompt with 2 inputs...\n");
    Prompt p;
    p.length = 2;
    p.data = malloc(2 * sizeof(double));
    if (!p.data) {
        fprintf(stderr, "Failed to allocate prompt data\n");
        // Cleanup allocated memory before returning
        for (size_t r = 0; r < 2; r++) {
            free(testModel.model_weights[1][r]);
        }
        free(testModel.model_weights[1]);
        free(testModel.model_weights);
        free(testModel.model_layers);
        return;
    }
    p.data[0] = 0.2; // Input #1
    p.data[1] = 0.3; // Input #2
    printf("Prompt data set: [0.2, 0.3]\n");

    // 4) Calculate output
    printf("Calling calculate_output()...\n");
    Output result = calculate_output(&p, &testModel);
    printf("Output calculated\n");

    // 5) Print results
    printf("calculate_output => length: %zu, [", result.length);
    for (size_t i = 0; i < result.length; i++) {
        printf("%f", result.data[i]);
        if (i < result.length - 1)
            printf(", ");
    }
    printf("]\n");

    // Cleanup:
    printf("Cleaning up allocated memory...\n");
    free(p.data);
    free(result.data);

    // Free layer node arrays
    free(testModel.model_layers[0].layer_array_of_nodes);
    free(testModel.model_layers[1].layer_array_of_nodes);

    // Free weight matrices for layer 1 (2 rows)
    for (size_t r = 0; r < 2; r++) {
        free(testModel.model_weights[1][r]);
    }
    free(testModel.model_weights[1]);
    free(testModel.model_weights);

    // Free the layers array
    free(testModel.model_layers);

    printf("test_calculate_output() finished successfully.\n");
}

