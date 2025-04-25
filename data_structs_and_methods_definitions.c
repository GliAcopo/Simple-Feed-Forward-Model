#include "settings.h"
#include "data_structs_and_methods_definitions.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <stddef.h>
// #include <math.h>

/* -+-+-+-+-+-+-+-+-+-+-+- THRESHOLD AND ACTIVATION FUNCTIONS -+-+-+-+-+-+-+-+-+-+-+- */

double mySigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double myLinear(double x){
    return (x);
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
        DEBUG_PRINT(
                "Error in %s: argument passed as NULL pointer. 'node = %p'.\n",
                __func__, (void*)node);
        return ERROR_NULL_POINTER_AS_PARAMETER;
    }
    
    // If node is not NULL, free it
    free(node);
    
    // Return success
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
/**
 * @brief Create a layer object
 *
 * @param num_nodes The number of nodes that will be in the layer
 * @param number_of_nodes_in_the_layer 
 * @param number_of_nodes_in_the_next_layer 
 * @param activation The activation function that will be used in the nodes of the returned layer
 * @param threshold The threshold function that will be used in the nodes of the returned layer
 * @return Layer struct
 */
Layer create_layer(size_t number_of_nodes_in_the_layer,
                   // size_t number_of_nodes_in_the_next_layer,
                   activation_function activation,
                   threshold_function threshold){
    Layer layer;
    layer.number_of_nodes_in_the_layer = number_of_nodes_in_the_layer;
    // layer.number_of_nodes_in_the_next_layer = number_of_nodes_in_the_next_layer;

    // Allocate space for the nodes
    layer.layer_array_of_nodes = malloc(number_of_nodes_in_the_layer * sizeof(Node));
    if (layer.layer_array_of_nodes == NULL) {
        DEBUG_PRINT("Error in %s: Failed to allocate layer_array_of_nodes. 'layer.layer_array_of_nodes = %p'.\n",
            __func__, (void*)layer.layer_array_of_nodes);
        Layer empty = {0};
        return empty;
    }

    // Initialize each node
    for (size_t i = 0; i < number_of_nodes_in_the_layer; i++) {
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
// PROBLEM IN FUNCTIONS (Layer* model_layers), MAKES NO SENSE AS A VARIABLE.
Model* create_model(const char* name, Layer* model_layers, double*** model_weights) {
    Model* model = malloc(sizeof(Model));
    if (!model) {
        DEBUG_PRINT("Allocation error\n");
        return NULL;
    }
    // Initialize fields
    model->number_of_layers_in_the_model = 0;
    model->model_layers = model_layers;
    model->model_weights = model_weights;
    // Allocate space for the name and copy it
    model->model_name = malloc(strlen(name) + 1);
    if (!model->model_name) {
        DEBUG_PRINT("Allocation error\n");
        free(model);  // Clean up partially allocated model
        return NULL;
    }
#if ASSIGN_MODEL_NAME
    strcpy(model->model_name, name);
    #endif

    return model;
}

/**
 * @brief A function that merges the create model function and the create_layer function for a fast initialization of the model
 * 
 * @param name(const char*): The name of the model, passed as a dynamically allocated array of chars.abort
 * @param model_layer(Layer*): The pointer to the array of layers.
 * @param model_weights: The array of matrices containing the weights of the model.
 * @return Model* 
 */
Model* init_model(const char* name, size_t number_of_layers_in_the_model, double*** model_weights,
                    size_t number_of_nodes_in_the_layer,
                    activation_function activation,
                    threshold_function threshold) {
    Model* model = malloc(sizeof(Model));
    if (!model) {
        fprintf(stderr, "Allocation error\n");
        return NULL;
    }
    // Initialize fields
    model->number_of_layers_in_the_model = number_of_layers_in_the_model;
    model->model_layers = calloc(number_of_layers_in_the_model, sizeof(Layer));
    model->model_weights = model_weights;

    // Create the layers and put them in the model
    for (size_t i = 0; i < number_of_layers_in_the_model; i++){
        model->model_layers[i] = create_layer(number_of_nodes_in_the_layer, activation, threshold);
    }

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

Prompt init_prompt(size_t number_of_tokens){
  Prompt initialized_prompt;
  initialized_prompt.data = (double*)malloc(number_of_tokens * sizeof(double));
    initialized_prompt.length = number_of_tokens;
    for (size_t i = 0; i < number_of_tokens; i++) {
        initialized_prompt.data[i] = (double)i;
    }
    return initialized_prompt;
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

void print_output(Output* pointer_to_output){
    if(!pointer_to_output){fprintf(stderr,"Error in %s function, NULL pointer received\n", __func__); return;}
    if(pointer_to_output->data == NULL){fprintf(stderr,"Error in %s function, ponter to data is NULL (pointer_to_output->data == NULL)\n", __func__); return;}
    if(pointer_to_output->length == 0){fprintf(stderr,"Possible Error in %s function, lenght of output is 0\n", __func__);}

    printf("{");
    for(size_t i = 0; i < pointer_to_output->length; i++){
        printf(" [%zu]:(%f) ", i, pointer_to_output->data[i]);
    }
    printf("}\n");
    return;
}



