/*
 *  Individual.h
 *  brains
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "NeuralNetwork.h"

using namespace std;

class Individual {
public:
    
    // Create a new individual.
    Individual(int input_units, int output_units_);
    
    // Save this individual.
    string save();
    
    // Genes of the individual.
    vector<char> genes_;
    
    // Neural network generated from the genes.
    NeuralNetwork* network_;
    
    // Number of input units of the individual.
    int input_units_;
    
    // Number of output units of the individual.
    int output_units_;
    
    // Fitness of the individual.
    double fitness_;
};

#endif