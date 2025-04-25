#include "matrix_functions.h"
#include "model_computational_functions.h"
#include "data_structs_and_methods_definitions.h"
#include "settings.h"
#include <stdlib.h>

[[deprecated("This function will simply create a layers x nodes_per_layer matrix, and init it to 0, this function is not really useful nor optimised for the FF model, use other functions intead")]]
double **create_adj_matrix_double_square(int layers, int nodes_per_layer)
{
    /*
    This function creates an ajacence matrix of a model composed of tot layers and tot nodes per layer, this model is extremely simple since it is exentially a square
    The matrix gets checked toroughly and initialized to 0;
    :return: The pointer to the matrix
    */
    double **matrix_pointer = create_matrix_double(layers, nodes_per_layer);
    matrix_pointer = init_matrix_to_double_value(matrix_pointer, layers, nodes_per_layer, 0);

    int check = verify_matrix(matrix_pointer, layers);
    if (check < 0)
    {
        DEBUG_PRINT("\n verify_matrix returned an error: %d\n", check);
        return (NULL);
    }
    else if (check_if_all_elements_of_matrix_are_equal_to_value(matrix_pointer, layers, nodes_per_layer, 0) != RETURN_TRUE)
    {
        DEBUG_PRINT("\n check_if_all_elements_of_matrix_are_equal_to_value returned an error: %d \n", check_if_all_elements_of_matrix_are_equal_to_value(matrix_pointer, layers, nodes_per_layer, 0));
        return (NULL);
    }
#if VERBOSE
    printf("Nothing went wrong, returning matrix pointer...\n");
#endif

    return (matrix_pointer);
}

/**
 * @brief This function will create a vector of matrices, each of the matrices will contain the weights of the edges connected to the nodes. The result to expect is: 1 matrix for the input layer (composed of all 1's to not interfere with the input)
 * Since this is a fully connected FeedForward neural network we will represent the edges as follows: 0 = no connection, positive = edge from left to right (going forward); negative = edge from right to left (backwards)
 * @return matrices_vector (double***). This is a vector, each element of this vector contains the matrices of the weights for each layer of dimension (nodes_per_layer x nodes_per_layer), (yes, this neural network is a square).
 */
double ***create_FF_model_matrices(size_t layers, size_t nodes_per_layer)
{
    // 1) Creating the vector to store the matrices
    // @note: we create (layers - 1) and not (layers) matrices because adj matrices are only present between two layers, so the input layer and the output layer do not need an adjacency matrix themselves. EX: let's imagine a model: [input]{matrix}[secret]{matrix}[output] => 3 layers but two matrices!
    double ***matrices_vector = (double ***)malloc((layers - 1) * sizeof(double **));

    for (size_t i = 0; i < layers; i++)
    {
        double **matrix = create_matrix_double(nodes_per_layer, nodes_per_layer);                  // creating the matrix of nodes x nodes
        matrix = init_matrix_to_double_value(matrix, nodes_per_layer, nodes_per_layer, (double)1); // initiating the matrix to value 1 to symbolise the connections

        // security checks to identify problem in matrix creation{
        int check = verify_matrix(matrix, nodes_per_layer); // verifiyng that the matrix is valid
        if (check < 0)
        {
            DEBUG_PRINT("\n verify_matrix returned an error: %d\n", check);
            return (NULL);
        }
        check = check_if_all_elements_of_matrix_are_equal_to_value(matrix, nodes_per_layer, nodes_per_layer, (double)1);
        if (check != RETURN_TRUE)
        {
            DEBUG_PRINT("\n check_if_all_elements_of_matrix_are_equal_to_value returned an error: %d \n", check);
            return (NULL);
        }
        //}

        matrices_vector[i] = matrix;
        // free_double_matrix(matrix, nodes_per_layer); THIS DOESN'T WORK: memory getting freed is memory containing the matrices, this line should not be uncommented
    }
    return (matrices_vector);
}

void print_matrix_vector_double(double ***matrices_vector, int layers, int nodes_per_layer)
{
    for (int layer = 0; layer < layers; layer++)
    {
        print_matrix_double(matrices_vector[layer], nodes_per_layer, nodes_per_layer);
    }
}

/* -+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+- NEURAL NETWORK OUTPUT -+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+- */

Output calculate_output(Prompt *prompt, Model *model)
{
    //                                  INPUT HEALTH CHECK
    DEBUG_PRINT("\nStarted checks in %s function with\nprompt: %p\nmodel: %p\n", prompt, model);

    if (prompt == NULL)
    {
        DEBUG_PRINT("Error in %s: 'prompt' is NULL.\n", __func__);
        return empty_output();
    }
    if (model == NULL)
    {
        DEBUG_PRINT("Error in %s: 'model' is NULL.\n", __func__);
        return empty_output();
    }
    if (prompt->data == NULL)
    {
        DEBUG_PRINT("Error in %s: 'prompt data' is NULL.\n", __func__);
        return empty_output();
    }
    if (model->model_layers == NULL)
    {
        DEBUG_PRINT("Error in %s: 'model_layers' is NULL.\n", __func__);
        return empty_output();
    }
    if (prompt->length != model->model_layers[0].number_of_nodes_in_the_layer)
    {
        DEBUG_PRINT("Error in %s: prompt size (%zu) is different from the size of the first layer of model (%zu).\n", __func__, prompt->length, model->model_layers[0].number_of_nodes_in_the_layer);
        return empty_output();
    }
    DEBUG_PRINT("Successfully exited checks in %s function with\nprompt: %p\nmodel: %p\n", __func__, prompt, model);
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
    output.layer_inputs = malloc(model->number_of_layers_in_the_model * sizeof(double *));
    if (output.layer_inputs == NULL)
    {
        DEBUG_PRINT("Error in %s: memory allocation error. 'output.layer_inputs_and_outputs' is NULL.\n", __func__);
        return empty_output();
    }
    DEBUG_PRINT("Allocating space for output.layer_outputs for [%zu] layers in model\n", model->number_of_layers_in_the_model);
    output.layer_outputs = malloc(model->number_of_layers_in_the_model * sizeof(double *));
    if (output.layer_outputs == NULL)
    {
        DEBUG_PRINT("Error in %s: memory allocation error. 'output.layer_inputs_and_outputs' is NULL.\n", __func__);
        return empty_output();
    }
    //                          END INITIALIZATION OF RETURNED OUTPUT STRUCT

    /** We copy the prompt into the layer_inputs_and_outputs first array in order for it to be registered for
     * training purposes while also maintaining the main loop as straightforward as possible */
    DEBUG_PRINT("Allocating space for first layer's output.layer_inputs[0] with a prompt->length = [%zu]\n", prompt->length);
    output.layer_inputs[0] = malloc(prompt->length * sizeof(double *));
    if (output.layer_inputs[0] == NULL)
    {
        DEBUG_PRINT("Error in %s: memory allocation error. 'output.layer_inputs[%d]' is NULL.\n", __func__, 0);
        return empty_output();
    }
    for (size_t i = 0; i < prompt->length; i++)
    {
        output.layer_inputs[0][i] = prompt->data[i];
    }
#if DEBUG //// We print the input values from the prompt array and the input array to check if they were copied properly
    printf("Prompt was copied into first input array. Copied values:\nInput array:\n");
    printf("1) Input array size: %zu\n", prompt->length);
    printf("2) Input array coefficients: ");
    for (size_t i = 0; i < prompt->length; i++)
    {
        printf("[%zu]:(%lf), ", i, output.layer_inputs[0][i]);
    }
    printf("\nPrompt array:\n");
    printf("1) Prompt array size (copied size): %zu\n", prompt->length);
    printf("2) Prompt array coefficients: ");
    if (prompt->data == NULL)
    {
        fprintf(stderr, "Error in [%s:%s:%d]: Prompt->data is null!\n", __FILE__, __func__, __LINE__);
        return (empty_output());
    }
    else
    {
        printf("\n(prompt->data was confirmed not to be a not a NULL pointer)\n");
        for (size_t i = 0; i < prompt->length; i++)
        { // Theoretically the two have the same lenght
            printf("[%zu]:", i);
            printf("(%lf), ", prompt->data[i]);
        }
        printf("\n");
    }
#endif

    //                                      MAIN CALCULATION LOOP
    DEBUG_PRINT("Entering main loop... Stop value of i will be %zu\n", model->number_of_layers_in_the_model - 1);
    for (size_t i = 0; i < model->number_of_layers_in_the_model - 1; i++)
    {
        DEBUG_PRINT("\nLoop at index [%zu]:\n", i);
        /** 1) pass the input trough each node */
        output.layer_outputs[i] = malloc(model->model_layers[i].number_of_nodes_in_the_layer * sizeof(double));
        if (output.layer_outputs[i] == NULL)
        {
            DEBUG_PRINT("Error in %s: memory allocation error. 'output.layer_outputs[%zu]' is NULL.\n", __func__, i);
            return empty_output();
        }
        DEBUG_PRINT("Processing values trough nodes, the number of nodes in this layer is %zu", model->model_layers[i].number_of_nodes_in_the_layer);
        for (size_t j = 0; j < model->model_layers[i].number_of_nodes_in_the_layer; j++)
        {
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
        // model->model_layers[i+1].number_of_nodes_in_the_layer represents the number of nodes (m) in the next layer.
        output.layer_inputs[i + 1] = malloc(model->model_layers[i + 1].number_of_nodes_in_the_layer * sizeof(double));

        // Iterate over each node in the next layer (each column of the weight matrix)
        DEBUG_PRINT("Computed output of the layer is: ["); // Debug print to show the computed values by the inference engine.
        for (size_t column = 0; column < model->model_layers[i + 1].number_of_nodes_in_the_layer; column++)
        {
            double sum = 0.0;

            // For each node in the current layer (each row of the weight matrix)
            for (size_t row = 0; row < model->model_layers[i].number_of_nodes_in_the_layer; row++)
            {
                // Multiply the output from the current layer's node (row index)
                // by the weight connecting that node to the next layer's node (column index)
                sum += output.layer_outputs[i][row] * model->model_weights[i][row][column];
            }
            // Store the computed sum into the next layer's input vector at the index corresponding to the node (column)
            output.layer_inputs[i + 1][column] = sum;
            DEBUG_PRINT("%lf, ", output.layer_inputs[i + 1][column]);
        }
        DEBUG_PRINT("]\n");
    }
    //                                      END MAIN CALCULATION LOOP

    /** 5) Handling last layer (output layer) */
    size_t const k = model->number_of_layers_in_the_model - 1;           // variable created to enhance readability
    output.length = model->model_layers[k].number_of_nodes_in_the_layer; // is this varible really useful? It can be obtained from model->model_layers[k].number_of_nodes_in_the_layer where k = model->number_of_layers_in_the_model - 1
    /** 5.1) pass the input trough each node */
    output.layer_outputs[k] = malloc(model->model_layers[k].number_of_nodes_in_the_layer * sizeof(double));
    if (output.layer_outputs[k] == NULL)
    {
        DEBUG_PRINT("Error in %s: memory allocation error. 'output.layer_outputs[%zu]' (last layer) is NULL.\n", __func__, k);
        return empty_output();
    }
    for (size_t j = 0; j < model->model_layers[k].number_of_nodes_in_the_layer; j++)
    {
        /** 5.2) register output of each node into the output array */
        double input_to_node = output.layer_inputs[k][j] + model->model_layers[k].layer_array_of_nodes[j].bias;
        output.layer_outputs[k][j] = model->model_layers[k].layer_array_of_nodes[j].activation(input_to_node);
        DEBUG_PRINT("Passed values trough each node input_to_node[%zu] = %lf -> %lf (= output.layer_outputs[%zu][%zu])", j, input_to_node, output.layer_outputs[k][j], k, j);
    }
    output.data = output.layer_outputs[k];
    return (output);
}

/* -+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+- END NEURAL NETWORK OUTPUT -+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+- */