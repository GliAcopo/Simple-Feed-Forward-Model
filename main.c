/* I want to create a super simple ai model using C language, The model must be a standard FF model composed of an input layer, a secret layer and an output layer.
   But apart from that, I can use whathever method I want.
*/
/* 
    Let's do a brief recap, for what I understand a model is exentially a graph (un gafo), with nodes, and nodes are connected to each other by arcs, 
    :param arcs - these arcs connect two nodes toghether all have a weight (a value that influence the activation threshold of the node).
    :param weight - the weight of an arc influences the value that is passing trough the arc. The value must be multiplied by the arc weight.
    :param node - each node is made up by an activation threshold, an activation function, and an output value.
        :param activation_function - different nodes CAN (but not so often) use different activation funtions, these functions take the weighted sum of every's arc inputs and then uses it as the parameter for it's calculations.
            :notif! for this program I'm gonna use the SIGMOID function, wich output is between [1, 0]
*/
/*
PENSO DI AVER TROVATO L'ESERCIZIO GIUSTO! Farò un modello di intelligenza artificiale per il calcolo delle radici quadrate approssimate per eccesso o per difetto!!! 
    :param: input - il valore in input alla rete neurale sarà un intero a 4 bit, quindi nel range [0, 8]
    :param: output - Il valore in output sarà un (int)char da 4 bit che rappresenterà il numero calcolato. 
    :param: learning - ogni volta che il modello produrrà un output, chiederà all'utente se:
        (-1) = Il valore prodotto è inferiore al risultato atteso.
        (0) = Il valore prodotto è corretto, ovvero uguale al valore atteso.
        (1) = Il valore prodotto è superiore al valore atteso.
     In base a questi dati forniti il modello dovrà modificare i pesi dei suoi archi di conseguenza.
    :idea! Sarà anche il caso di introdurre una funzione di autoapprendimento che fornisca in input i dati di apprendimento autonomamente.
*/
/*
typedef struct {
    int value; // as for now, this representation could be non-optimal, I'm just leaving it here for now.
} neuron; 
*/


#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "matrix_functions.h"
#include "model_functions.h"
#include "node_functions.h"

/**
 * @brief function to test the new functions
 */
void test1(){
    printf("%d\n", VERBOSE);
    int layers = 4;
    int nodes_per_layer = 4;
    
    printf ("given value to layers %d, and nodesperlayer %d\n", layers, nodes_per_layer);
    
    double*** matrices_vector = create_FF_model_matrices(layers, nodes_per_layer);
    #if VERBOSE == 1
        printf("given value to matrices_vector %p\n", matrices_vector);
    #endif
    print_matrix_vector_double(matrices_vector, layers, nodes_per_layer);

}

void test_calculate_output_autogen(void) 
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

void test_calculate_output(void){
    /* To test the functionality of output generation let's create a 3 layer model 
        1 input, 1 secret, 1 output.
        Every layer will be composed of 4 nodes
        To do so we'll only use the given creation functions */
        Layer input = create_layer(4, 4, 4);
}

int main(){

    //test1();
    test_calculate_output();

    /*
    #define LIVELLI 4
    #define NODI_PER_LIVELLO 4
    double** matrix = create_adj_matrix_double_square(LIVELLI, NODI_PER_LIVELLO);
    */
   return(0);
}
