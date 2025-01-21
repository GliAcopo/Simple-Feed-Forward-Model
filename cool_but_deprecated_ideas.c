/**
 * @brief the "////////////////////" indicate where the interesting part starts
 * 
 */

/**
 * @brief Calculates the output using the given prompt and model. 
 * 
 * @param prompt (Prompt Object): The prompt that will be processed by the model
 * @param used_model (pointer to Model): The model struct used to calculate the output
 * @return (Output Object): The array of output values calculated by the model
 */
Output calculate_output(const Model* used_model, Prompt prompt) {

    size_t first_layer_size = used_model->model_layers[0].rows_of_adj_matrix;
    // Check the prompt length vs. input layer size
    if (prompt.length != first_layer_size) {
        printf("Invalid prompt length. "
               "Accepted length by model: %ld; Given prompt length: %ld.\n",
               used_model->model_layers[0].rows_of_adj_matrix,
               prompt.length);
        return empty_output();
    }

    /** @brief After the first check we create the dynamically allocated arrays of the Output for the inputs_to_layers and layer_inputs_and_outputs.
     * @note The logic is that in this array we store first the input array of the layer, then the output of the layer. Alternating between input/output*/
    Output output;
    // The number of layers is also the number of rows in the "matrix of inputs"
    output.layer_inputs_and_outputs = malloc((used_model->number_of_layers_in_the_model * 2) * sizeof(double*));
    // For each layer we need to allocate a number of cells corresponding to the number of nodes in the layer * 2 (input as well as output for each layer)
    if(output.layer_inputs_and_outputs == NULL){
            printf("Not enough memory in output.inputs_to_layers allocation.\n");
            return empty_output();
        }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief We are allocating space for the input array as well as the output arrays, so we need to make TWO arrays for each layer (input array and output array) 
     */
    size_t j = 0;   // We need this variable so we can create a sub-loop inside the for loop.
    for (size_t i = 0; i < used_model->number_of_layers_in_the_model * 2; i += 2){
        // when i reaches the "critical point" we basically reset the loop for the matrices.
        if (i = used_model->number_of_layers_in_the_model){
            j = 0;
        }
        output.layer_inputs_and_outputs[i] = malloc(used_model->model_layers[j].rows_of_adj_matrix * sizeof(double));
        output.layer_inputs_and_outputs[i+1] = malloc(used_model->model_layers[j].rows_of_adj_matrix * sizeof(double));

        if(output.layer_inputs_and_outputs[i] == NULL || output.layer_inputs_and_outputs[i+1] == NULL){
            printf("Not enough memory in output.inputs_to_layers[%zu] or output.inputs_to_layers[%zu] allocation.\n", i, i+1);
            return empty_output();
        }
        j += 2;
    }

    // Handle first layer: simply apply activation functions directly on prompt inputs
    double* layer_input = malloc(first_layer_size * sizeof(double));
    if (layer_input == NULL) {
        printf("Not enough memory in input layer result allocation.\n");
        return empty_output();
    }

    for (size_t j = 0; j < first_layer_size; j++) {
        prompt.data[j] += used_model->model_layers[0].layer_array_of_nodes[j].bias;                                 // Summing the bias of the node
        layer_input[j] =  used_model->model_layers[0].layer_array_of_nodes[j].activation(prompt.data[j]);           // Calculating the activation function of the node using the input
    }
    output.layer_inputs_and_outputs[0] = layer_input;  // The output of the first layer of the model.


    // Process the layers after the input layer (i > 0)
    size_t i = 1;                                                                                                   // Since we use layer information even outside the loop, we need the variable to remain visible 
    for (; i < used_model->number_of_layers_in_the_model; i++) {
        // When we arrive at the end of the model we simply calculate the output layer node function and the bias
        if (used_model->model_weights[i] == NULL){                                                                  
            for(size_t k = 0; k < used_model->model_layers[i].rows_of_adj_matrix; k++){
                layer_input[k] += used_model->model_layers[i].layer_array_of_nodes[k].bias;                         // Summing the bias of the node
                layer_input[k] = used_model->model_layers[i].layer_array_of_nodes[k].activation(layer_input[k]);    // Passing the input trough the output layer and registering it in the layer_input for the sake of convenience.
                // output.inputs_to_layers[i][k] = layer_input[k];                                                     // We register the output of the last layer for backpropagation
            }
            break;
        }

        size_t output_size = used_model->model_layers[i].columns_of_adj_matrix;
        size_t input_size  = used_model->model_layers[i].rows_of_adj_matrix;

        double* layer_output = malloc(output_size * sizeof(double));                // We dinamically allocate the vector of the output model
        ////    TODO  add here the layer output to the output array 
        ////    TODO  
        if (layer_output == NULL) {
            free(layer_input);
            printf("Not enough memory in layer %zu result allocation.\n", i);
            return empty_output();
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
            layer_output[col] = used_model->model_layers[i].layer_array_of_nodes[col].activation(sum);      // Pass the sum through the layer's activation function
            used_model->model_layers[i].layer_array_of_nodes[col].output = layer_output[col];
        }
        // Use the output of this layer as input for the next one:
        output.layer_inputs_and_outputs[i] = layer_output;
    }
    // Once finished, 'layer_input' should contain the final output.
    output.length = used_model->model_layers[i-1].columns_of_adj_matrix;
    output.data   = output.layer_inputs_and_outputs[used_model->number_of_layers_in_the_model];
    
    return output;
}


/**
 * @brief Calculates the output using the given prompt and model. 
 * 
 * @param prompt (Prompt Object): The prompt that will be processed by the model
 * @param used_model (pointer to Model): The model struct used to calculate the output
 * @return (Output Object): The array of output values calculated by the model
 */
[[deprecated("This function doesn't support backpropagation techniques (yet), it ashould be meant to be a faster function to calculate the otput when training isn't necessary")]]
Output calculate_output_old(const Model* used_model, Prompt prompt) {

    size_t first_layer_size = used_model->model_layers[0].rows_of_adj_matrix;
    // Check the prompt length vs. input layer size
    if (prompt.length != first_layer_size) {
        printf("Invalid prompt length. "
               "Accepted length by model: %ld; Given prompt length: %ld.\n",
               used_model->model_layers[0].rows_of_adj_matrix,
               prompt.length);
        return empty_output();
    }

    /** After the first check we create the dynamically allocated arrays of the Output for the inputs_to_layers and 
     * layer_inputs_and_outputs.*/
    Output output;
    // The number of layers is also the number of rows in the "matrix of inputs"
    output.inputs_to_layers = malloc(used_model->number_of_layers_in_the_model * sizeof(double*));
    // For each layer we need to allocate a number of cells corresponding to the number of nodes in the layer
    if(output.inputs_to_layers == NULL){
            printf("Not enough memory in output.inputs_to_layers allocation.\n");
            return empty_output();
        }
    for (size_t i = 0; i < used_model->number_of_layers_in_the_model; i++){
        output.inputs_to_layers[i] = malloc(used_model->model_layers[i].rows_of_adj_matrix * sizeof(double));
        if(output.inputs_to_layers[i] == NULL){
            printf("Not enough memory in output.inputs_to_layers[%zu] allocation.\n", i);
            return empty_output();
        }
    }

    // Handle first layer: simply apply activation functions directly on prompt inputs
    double* layer_input = malloc(first_layer_size * sizeof(double));
    if (layer_input == NULL) {
        printf("Not enough memory in input layer result allocation.\n");
        return empty_output();
    }

    for (size_t j = 0; j < first_layer_size; j++) {
        prompt.data[j] += used_model->model_layers[0].layer_array_of_nodes[j].bias;                                 // Summing the bias of the node
        layer_input[j] =  used_model->model_layers[0].layer_array_of_nodes[j].activation(prompt.data[j]);           // Calculating the activation function of the node using the input
        output.inputs_to_layers[0][j] = layer_input[j];                                                             // Saving the output in the node's output for later training.
    }

    // Process the layers after the input layer (i > 0)
    size_t i = 1;                                                                                                   // Since we use layer information even outside the loop, we need the variable to remain visible 
    for (; i < used_model->number_of_layers_in_the_model; i++) {
        // When we arrive at the end of the model we simply calculate the output layer node function and the bias
        if (used_model->model_weights[i] == NULL){                                                                  
            for(size_t k = 0; k < used_model->model_layers[i].rows_of_adj_matrix; k++){
                layer_input[k] += used_model->model_layers[i].layer_array_of_nodes[k].bias;                         // Summing the bias of the node
                layer_input[k] = used_model->model_layers[i].layer_array_of_nodes[k].activation(layer_input[k]);    // Passing the input trough the output layer and registering it in the layer_input for the sake of convenience.
                output.inputs_to_layers[i][k] = layer_input[k];                                                     // We register the output of the last layer for backpropagation
            }
            break;
        }

        size_t output_size = used_model->model_layers[i].columns_of_adj_matrix;
        size_t input_size  = used_model->model_layers[i].rows_of_adj_matrix;

        double* layer_output = malloc(output_size * sizeof(double));                // We dinamically allocate the vector of the output model
        ////    TODO  add here the layer output to the output array 
        ////    TODO  
        if (layer_output == NULL) {
            free(layer_input);
            printf("Not enough memory in layer %zu result allocation.\n", i);
            return empty_output();
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
            layer_output[col] = used_model->model_layers[i].layer_array_of_nodes[col].activation(sum);      // Pass the sum through the layer's activation function
            used_model->model_layers[i].layer_array_of_nodes[col].output = layer_output[col];
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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