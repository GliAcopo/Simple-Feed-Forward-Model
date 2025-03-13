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

//-----------------------------------------------------------------------------------------------------------------------------
/**
 * @brief A simple struct to incapsulate the output array and the lenght of the array
 * 
 * @param is_valid(int): ( != 1 non valid; else then is valid) indicates if the output is valid or if something went wrong in its creation
 * @param used_model(Model*): The model used to calculate the output
 * @param data(double*): The array of output tokens
 * @param lenght(size_t): The positive integer number, how many elements are in the prompt array
 * @param layer_inputs_and_outputs(double**): a pointer to an array of pointers to arrays (basically matrix) containing the outputs generated by each layer
 */
typedef struct Output{
    char is_valid;
    Model* used_model;
    double* data;   // Pointer to the array of data produced by the model
    size_t length;  // Number of input values
    double** layer_inputs_and_outputs;
    /** @example The array is accessed like this:
     * Lenght of the array (each subarray is composed of input+output of the layer): used_model->number_of_layers_in_the_model
     * Lenght of the subarray (lenght of the input and output of the relative layer) [i]: used_model->model_layers[i].rows_of_adj_matrix
     * Input of layer [i] node [j]: output.layer_inputs_and_outputs[i][j] (upper limit of "used_model->model_layers[i].rows_of_adj_matrix")
     * Output of layer [i] node [j]: output.layer_inputs_and_outputs[i][j + used_model->model_layers[i].rows_of_adj_matrix]
     */
} Output;

/** This was the calculate output function before I chose to rewrite it from the start. */
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
    /**
     * @brief We are allocating space for the input array as well as the output arrays, so we need TWO arrays for each layer (input array and output array).
     * @note We can notice that the dimension of an output array is equal to the previous input array, so we can create an array of double the dimension and separate the input from output simply by saying that if you want to access a value in teh output you multiply the index by *2 output[i] = output[i*2]
     */
    for (size_t i = 0; i < used_model->number_of_layers_in_the_model; i++){
        output.layer_inputs_and_outputs[i] = malloc(used_model->model_layers[i].rows_of_adj_matrix * 2  * sizeof(double));

        if(output.layer_inputs_and_outputs[i] == NULL){
            printf("Not enough memory in output.inputs_to_layers[%zu] or output.inputs_to_layers[%zu] allocation.\n", i, i+1);
            return empty_output();
        }
    }

    for (size_t j = 0; j < first_layer_size; j++) {
        output.layer_inputs_and_outputs[0][j] = prompt.data[j];                              
        // previous: layer_input[j] =  used_model->model_layers[0].layer_array_of_nodes[j].activation(prompt.data[j]);           // Calculating the activation function of the node using the input
        /** since these are two arrays stacked on top of each other, this is now the standard way of accessing the output array */
        output.layer_inputs_and_outputs[0][j + first_layer_size] =  used_model->model_layers[0]                                  // Selecting the layer
                                                                                .layer_array_of_nodes[j]                         // Selecting the node
                                                                                .activation(prompt.data[j] +                     // Passing trough the activation function of the node
                                                                                 used_model->model_layers[0].layer_array_of_nodes[j].bias);   // Summing the bias of the node  
    }


    // Process the layers after the input layer (i > 0)
    size_t i = 1;                                                                                                   // Since we use layer information even outside the loop, we need the variable to remain visible 
    for (; i < used_model->number_of_layers_in_the_model; i++) {
        /** @brief When we arrive at the end of the model we simply calculate the output layer node function and the bias */
        if (used_model->model_weights[i] == NULL){                                                                  
            for(size_t k = 0; k < used_model->model_layers[i].rows_of_adj_matrix; k++){
                /** TODO: il problema è che l'array di input da dare in pasto all'ultimo layer ora come ora è vuoto!  */
                output.layer_inputs_and_outputs[i][k + used_model->model_layers[i].rows_of_adj_matrix] = used_model->model_layers[i]
                                                                                        .layer_array_of_nodes[k]
                                                                                        .activation(output.layer_inputs_and_outputs[i][k] + 
                                                                                        used_model->model_layers[i].layer_array_of_nodes[k].bias);    // Passing the input trough the output layer and registering it in the layer_input for the sake of convenience.
            }
            break;
        }

        size_t output_size = used_model->model_layers[i].columns_of_adj_matrix;
        size_t input_size  = used_model->model_layers[i].rows_of_adj_matrix;

        // 1) Multiply the previous layer outputs by the adjacency matrix 
        // 2) Pass the result to the activation function of each node
        for (size_t col = 0; col < output_size; col++) {
        double sum = 0.0;
            for (size_t row = 0; row < input_size; row++) {
                sum += output.layer_inputs_and_outputs[i-1][row + used_model->model_layers[i].rows_of_adj_matrix] * used_model->model_weights[i][row][col];
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






//-----------------------------------------------------------------------------------------------------------------------------
// A probably flawed test autput function






void test_calculate_output_autogen(void) {
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
    printf("model_weights[0] set to %p\n", testModel.model_weights[0]);

    // For layer 1, allocate a [2 x 1] matrix (2 rows, 1 column)
    // IMPORTANT: Allocate at index 1, not index 0.
    printf("Allocating weight matrix for second layer (model_weights[1])...\n");
    testModel.model_weights[1] = malloc(2 * sizeof(double*));
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