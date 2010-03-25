/*
 *  Phenotype.h
 *  brains
 *
 *  Created by Yann Dauphin on 18/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#ifndef PHENOTYPE_H
#define PHENOTYPE_H

#include "NeuralNetwork.h"
#include "Individual.h"
#include <string>

class Phenotype {

public:
    static NeuralNetwork* get_network(Individual* individual, int input_units, int output_units);
    
    static string get_graph(NeuralNetwork* network, string label="");

private:
    static vector< pair<int, int> > get_markers(Individual* individual);
    
    static int get_slice_size(Individual* individual, pair<int, int> slice);

    static int get_label(Individual* individual, pair<int, int> slice);

    static signed char get_bias(Individual* individual, pair<int, int> slice);
    
    static signed char get_nth_key(Individual* individual, pair<int, int> slice, int n);
    
    static int get_nth_label(Individual* individual, pair<int, int> slice, int n);
    
    static int get_hidden_label(Individual* individual,int label);
    
    static signed char get_nth_weight(Individual* individual, pair<int, int> slice, int n);
    
    static Neuron* get_closest_neuron(int label, vector<Neuron>& neurons);
};

#endif