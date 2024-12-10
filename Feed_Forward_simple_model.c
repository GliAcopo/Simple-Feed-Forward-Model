/* I want to create a super simple ai model using C language, The model must be a standard FF model composed of an input layer, a secret layer and an output layer.
   But apart from that, I can use whathever method I want.
   */

#include "model_functions.h"
#include "matrix.h"
#include "settings.h"

#define VERBOSE = 1;

//  TODO I should probably make a file with the data structure definitions.
//  TODO Should I add a different file containing the functions to every layer?
#define autoMode 0  // This variable is in boolean logic, and is used for the choice between auto training and standard training.  

/* Let's do a brief recap, for what I understand a model is exentially a graph (un gafo), with nodes, and nodes are connected to each other by arcs, 
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


void train(int feedback){
    //  TODO Training function goes here.
}

int main(int argc, char **argv){
    /*
    #define LIVELLI 4
    #define NODI_PER_LIVELLO 4
    float** matrix = create_adj_matrix_float_square(LIVELLI, NODI_PER_LIVELLO);

    // For efficiency's sake, I'll check the user's preferences once and then I'll do two loops.
    // One for the automatic training and one for the manual training. 
    if (autoMode){
        non_auto_loop();
    }
    else{
        auto_loop();
    }
    */
   return(0);
}