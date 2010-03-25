/*
 *  Neuron.h
 *  brains
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#ifndef NEURON_H
#define NEURON_H

#include <vector>

using namespace std;

class Neuron {

public:
    Neuron();

    double get_output();

    // Label of this neuron.
    int label_;

    // Last output.
    double output_;
    
    // Bias of the neuron.
    signed char bias_;

    // Neurons linked to the input of this neuron.
    vector< pair<signed char, Neuron*> > inputs_;
    
    // Neurons linked to the output of this neuron.
    vector<Neuron*> outputs_;
};

#endif