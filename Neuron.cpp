/*
 *  Neuron.cpp
 *  brains
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Neuron.h"
#include "utilities.h"

Neuron::Neuron()
: label_(0), output_(0.0), bias_(0){
}

double Neuron::get_output() {
    double sum = bias_;
    
    for (int i = 0; i < inputs_.size(); i++) {
        sum += inputs_[i].first * inputs_[i].second->output_;
    }
    
    output_ = logistic_function(sum);
    
    return output_;
}