/*
 *  NeuralNetwork.cpp
 *  letters
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#include <queue>
#include <set>
#include "NeuralNetwork.h"

vector<double> NeuralNetwork::get_output(vector<double> input) {
    queue<Neuron*> open_queue;
    set<Neuron*> closed_set;
    
    for (int i = 0; i < input_neurons_.size(); i++) {
        input_neurons_[i].output_ = input[i];
        
        for (int j = 0; j < input_neurons_[i].outputs_.size(); j++) {
            open_queue.push(input_neurons_[i].outputs_[j]);
            closed_set.insert(input_neurons_[i].outputs_[j]);
        }
    }
        
    for (int i = 0; i < output_neurons_.size(); i++) {
        closed_set.insert(&output_neurons_[i]);
    
        output_neurons_[i].output_ = 0.0;
    }
    
    while (!open_queue.empty()) {
        Neuron* neuron = open_queue.front();
        
        open_queue.pop();
        
        neuron->get_output();
        
        for (int i = 0; i < neuron->outputs_.size(); i++) {
            if (closed_set.find(neuron->outputs_[i]) == closed_set.end()) {
                open_queue.push(neuron->outputs_[i]);
                closed_set.insert(neuron->outputs_[i]);
            }
        }
    }
    
    vector<double> output(output_neurons_.size());
    for (int i = 0; i < output.size(); i++) {
        output_neurons_[i].get_output();
    
        output[i] = output_neurons_[i].output_;
    }
    
    return output;
}

void NeuralNetwork::clear_memory() {
    for (int i = 0; i < hidden_neurons_.size(); i++) {
        hidden_neurons_[i].output_ = hidden_neurons_[i].bias_;
    }
}