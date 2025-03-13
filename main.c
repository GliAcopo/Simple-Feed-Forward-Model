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
Farò un modello di intelligenza artificiale per il calcolo delle radici quadrate approssimate per eccesso o per difetto.
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

void test_init_model(void){
    const size_t number_of_layers = 3;
    const size_t number_of_nodes_per_layer = 4;
    const char   name[10] = "test model";
    activation_function test_activation = mySigmoid;
    threshold_function test_threshold = myThresholdFunc;

    double*** test_weights = create_FF_model_matrices(number_of_layers, number_of_nodes_per_layer);
    Model* test_model = init_model(&name, number_of_layers, test_weights, number_of_nodes_per_layer, test_activation, test_threshold);
    if (!test_model){fprintf(stderr,
        "Error in %s: init_model returned NULL pointer.\n",
        __func__);
        return;
    }
        
}

void test_calculate_output(void){
    /* To test the functionality of output generation let's create a 3 layer model 
        1 input, 1 secret, 1 output.
        Every layer will be composed of 4 nodes
        To do so we'll only use the given creation functions */
}

int main(){

    //test1();

    /*
    #define LIVELLI 4
    #define NODI_PER_LIVELLO 4
    double** matrix = create_adj_matrix_double_square(LIVELLI, NODI_PER_LIVELLO);
    */
   return(0);
}
