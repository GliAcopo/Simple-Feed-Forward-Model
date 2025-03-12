#include "settings.h"
#include "node_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

/* -+-+-+-+-+-+-+-+-+-+-+- THRESHOLD AND ACTIVATION FUNCTIONS -+-+-+-+-+-+-+-+-+-+-+- */

/*  Since every node needs an activation function ad a threshold funtion I'll use a pointer to function in every node to point to said functions.*/
typedef double (*activation_function)(double x);
typedef double (*threshold_function)(double x);

double mySigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Just a dummy example threshold
double myThresholdFunc(double x) {
    return (x > 0.5) ? 1.0 : 0.0;
}

/* -+-+-+-+-+-+-+-+-+-+-+- NODE -+-+-+-+-+-+-+-+-+-+-+- */

/**
 * @brief Create a node object
 * 
 * @param index 
 * @param bias 
 * @param activation 
 * @param threshold 
 * @return Node 
 */
Node create_node(int index, double bias, activation_function activation_function, threshold_function threshold_function){
    Node n;
    n.index = index;
    n.output = 0.0;         // Default output
    n.bias = bias;
    n.activation = activation_function;
    n.threshold = threshold_function; 
    n.delta = 0.0;          // Default delta
    return n;
}


ErrorCode free_Node(Node* node)
{
    // Check for NULL first
    if (!node) {
        fprintf(stderr,
                "Error in %s: argument passed as NULL pointer. 'node = %p'.\n",
                __func__, (void*)node);
        return ERROR_NULL_POINTER_AS_PARAMETER;
    }
    
    // If node is not NULL, free it
    free(node);
    
    // Return success (or whatever is appropriate for your function)
    return NO_ERROR;
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

/* -+-+-+-+-+-+-+-+-+-+-+- LAYER -+-+-+-+-+-+-+-+-+-+-+- */

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
        fprintf(stderr,
            "Error in %s: Failed to allocate layer_array_of_nodes. 'layer.layer_array_of_nodes = %p'.\n",
            __func__, (void*)layer.layer_array_of_nodes);
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

/* -+-+-+-+-+-+-+-+-+-+-+- MODEL -+-+-+-+-+-+-+-+-+-+-+- */

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

/* -+-+-+-+-+-+-+-+-+-+-+- PROMPT -+-+-+-+-+-+-+-+-+-+-+- */

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

/* -+-+-+-+-+-+-+-+-+-+-+- OUTPUT -+-+-+-+-+-+-+-+-+-+-+- */

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

/* -+-+-+-+-+-+-+-+-+-+-+- NEURAL NETWORK OUTPUT -+-+-+-+-+-+-+-+-+-+-+- */

Output calculate_output(Prompt* prompt, Model* model){
    //                                  INPUT HEALTH CHECKS
    #if DEBUG
    fprintf(stderr, "\nStarted checks in %s function with\nprompt: %p\nmodel: %p\n", __func__, prompt, model);
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
    //                                  END INPUT HEALTH CHECKS

    //                          INITIALIZATION OF RETURNED OUTPUT STRUCT
    Output output;
    output.is_valid = 1;
    output.used_model = model;

    /** @brief we need to allocate space for the inputs and outputs of each layer,
     * @note every layer has a vector of inputs and produces a vector as an output (dimension is the number of nodes), so we need an array of dimension number_of_layers * 2
     * @details this array is structured as follows: starts with the prompt input, follows with the output of the first layer (input-output)
     */
    DEBUG_PRINT("Allocating space for output.layer_inputs for [%zu] layers in model\n", model->number_of_layers_in_the_model);
    output.layer_inputs = malloc(model->number_of_layers_in_the_model * sizeof(double*));
    if (output.layer_inputs == NULL){
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_inputs_and_outputs' is NULL.\n", __func__);
        return empty_output();
    }
    DEBUG_PRINT("Allocating space for output.layer_outputs for [%zu] layers in model\n", model->number_of_layers_in_the_model);
    output.layer_outputs = malloc(model->number_of_layers_in_the_model * sizeof(double*));
    if (output.layer_outputs == NULL){
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_inputs_and_outputs' is NULL.\n", __func__);
        return empty_output();
    }
    //                          END INITIALIZATION OF RETURNED OUTPUT STRUCT

    /** We copy the prompt into the layer_inputs_and_outputs first array in order for it to be registered for 
     * training purposes while also maintaining the main loop as straightforward as possible */
    DEBUG_PRINT("Allocating space for first layer's output.layer_inputs[0] with a prompt->length = [%zu]\n", prompt->length);
    output.layer_inputs[0] = malloc(prompt->length * sizeof(double*));
    if (output.layer_inputs[0] == NULL){
        fprintf(stderr, "Error in %s: memory allocation error. 'output.layer_inputs[%d]' is NULL.\n", __func__, 0);
        return empty_output();
    }
    for (size_t i = 0; i < prompt->length; i++){
        output.layer_inputs[0][i] = prompt->data[i];
    }
    #if DEBUG ////////////////////////
        printf("Prompt was copied into first input array. Copied values:\nInput array:\n 1) ");
        for (size_t i = 0; i < prompt->length; i++){
            printf("%lf, ", output.layer_inputs[0][i]);
        }
            printf("\nPrompt array:\n2) ");
        for (size_t i = 0; i < prompt->length; i++){
            printf("%lf, ", prompt->data[i]);
        }
    #endif

    //                                      MAIN CALCULATION LOOP
    DEBUG_PRINT("Entering main loop... Stop value of i will be %zu\n", model->number_of_layers_in_the_model - 1);
    for (size_t i = 0; i < model->number_of_layers_in_the_model - 1; i++){
        DEBUG_PRINT("\nLoop at index [%zu]:\n", i); 
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
            DEBUG_PRINT("Passed values trough each node input_to_node[%zu] = %lf -> %lf (= output.layer_outputs[%zu][%zu])", j, input_to_node, output.layer_outputs[i][j], i, j);  
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
    //                                      END MAIN CALCULATION LOOP

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


