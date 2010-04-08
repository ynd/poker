/*
 *  NeuralNetwork.h
 *  letters
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "Neuron.h"

class NeuralNetwork {

public:
    vector<double> get_output(vector<double> input);

    // Neurons that belong to the input layer.
    vector<Neuron> input_neurons_;
    
    // Neurons that belong to the hidden layer.
    vector<Neuron> hidden_neurons_;
    
    // Neurons that belong to the output layer.
    vector<Neuron> output_neurons_;
};

#endif
