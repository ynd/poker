/*
 *  Phenotype.cpp
 *  brains
 *
 *  Created by Yann Dauphin on 18/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#include <iostream>
#include "Phenotype.h"
#include <sstream>

vector< pair<int, int> > Phenotype::get_markers(Individual* individual) {
    vector< pair<int, int> > markers;
    
    bool wrapped = false;
    int current_start = -1;
    for (int i = 0;; i++) {
        // Wrap around once.
        if (i == individual->genes_.size()) {
            if (current_start == -1 || wrapped) {
                break;
            }
            else {
                wrapped = true;
                i = 0;
            }
        }
        
        if (wrapped && !markers.empty() && i >= markers[0].first) {
             break;
        }
        
        // Start marker found
        if (individual->genes_[i] % 25 == 1 && current_start == -1) {
            current_start = i;
        }
        
        // End marker found
        if (individual->genes_[i] % 25 == 2 && current_start != -1) {
            markers.push_back(pair<int, int>(current_start, i));
            current_start = -1;
        }
    }
    
    return markers;
}

int Phenotype::get_slice_size(Individual* individual, pair<int, int> slice) {
    if (slice.first < slice.second) {
        return slice.second - slice.first - 1;
    }
    else if (slice.first == slice.second) {
        return 0;
    }
    else {
        return slice.second + (individual->genes_.size() - slice.first) - 1;
    }
}

int Phenotype::get_label(Individual* individual, pair<int, int> slice) {
    char label_high = individual->genes_[(slice.first + 1) % individual->genes_.size()];
    char label_low = individual->genes_[(slice.first + 2) % individual->genes_.size()];
    
    short label = (label_high << 8) | label_low;
    
    return abs(label);
}

signed char Phenotype::get_bias(Individual* individual, pair<int, int> slice) {
    int index = (slice.first + 3) % individual->genes_.size();

    return individual->genes_[index];
}

signed char Phenotype::get_nth_key(Individual* individual, pair<int, int> slice, int n) {
    return individual->genes_[(slice.first + 4 + 4*n) % individual->genes_.size()];
}

int Phenotype::get_nth_label(Individual* individual, pair<int, int> slice, int n) {
    char label_high = individual->genes_[(slice.first + 5 + 4*n) % individual->genes_.size()];
    char label_low = individual->genes_[(slice.first + 6 + 4*n) % individual->genes_.size()];
    
    short label = (label_high << 8) | label_low;
    
    return label;
}

int Phenotype::get_hidden_label(Individual* individual, int label) {
    return abs(label);
}

signed char Phenotype::get_nth_weight(Individual* individual, pair<int, int> slice, int n) {
    return individual->genes_[(slice.first + 7 + 4*n) % individual->genes_.size()];
}

Neuron* Phenotype::get_closest_neuron(int label, vector<Neuron>& neurons) {
    int closest = INT_MAX;
    
    Neuron* closest_neuron;
    
    for (int i = 0; i < neurons.size(); i++) {
        int distance = abs(neurons[i].label_ - label);
        if (distance < closest) {
            closest = distance;
            closest_neuron = &neurons[i];
        }
    }
    
    return closest_neuron;
}

NeuralNetwork* Phenotype::get_network(Individual* individual, int input_units, int output_units) {
    NeuralNetwork* network = new NeuralNetwork();

    // Locate markers
    vector< pair<int, int> > raw_markers = get_markers(individual);
    vector< pair<int, int> > markers;
    for (int i = 0; i < raw_markers.size(); i++) {
        if (get_slice_size(individual, raw_markers[i]) >= 7) {
            markers.push_back(raw_markers[i]);
        }
    }
    
    // Create input neurons
    for (int i = 0; i < input_units; i++) {
        Neuron neuron;
            
        neuron.label_ = i;
            
        network->input_neurons_.push_back(neuron);
    }
    
    // Create hidden neurons
    for (int i = 0; i < markers.size(); i++) {
        Neuron neuron;
            
        neuron.label_ = get_label(individual, markers[i]);
        
        neuron.bias_ = get_bias(individual, markers[i]);
        
        neuron.output_ = neuron.bias_;
            
        network->hidden_neurons_.push_back(neuron);
    }
    
    // Create output neurons
    for (int i = 0; i < output_units; i++) {
        Neuron neuron;
            
        neuron.label_ = i;
            
        network->output_neurons_.push_back(neuron);
    }
    
    // Create links
    for (int i = 0; i < markers.size(); i++) {
        Neuron& neuron = network->hidden_neurons_[i];

        int connections = (get_slice_size(individual, markers[i]) - 2) / 3;
        
        for (int j = 0; j < connections; j++) {
            signed char key = get_nth_key(individual, markers[i], j);
            int label = get_nth_label(individual, markers[i], j);
            signed char weight = get_nth_weight(individual, markers[i], j);
            
            // Link to input/output layer
            if (key > 0) {
                // Input
                if (label > 0) {
                    Neuron& input = network->input_neurons_[label % network->input_neurons_.size()];
                    
                    input.outputs_.push_back(&neuron);
                    neuron.inputs_.push_back(pair<signed char, Neuron*>(weight, &input));
                }
                // Output
                else {
                    Neuron& output = network->output_neurons_[abs(label) % network->output_neurons_.size()];
                    
                    neuron.outputs_.push_back(&output);
                    output.inputs_.push_back(pair<signed char, Neuron*>(weight, &neuron));
                }

            }
            // Link to hidden layer
            else {
                int hidden_label = get_hidden_label(individual, label);
                Neuron* hidden = get_closest_neuron(hidden_label, network->hidden_neurons_);
                
                hidden->outputs_.push_back(&neuron);
                neuron.inputs_.push_back(pair<signed char, Neuron*>(weight, hidden));
            }
        }
    }
    
    return network;
}

string Phenotype::get_graph(NeuralNetwork* network, string label) {
    stringstream graph;
    
    graph << "digraph neural_network {" << endl
          << "label=\"" << label << "\";" << endl;
    graph << "rankdir=LR;" << endl;
    
    graph << "subgraph cluster_0 {" << endl
          << "style=filled;" << endl
          << "color=lightgrey;" << endl
          << "label = \"Input\"" << endl
          << "node [shape = doublecircle];" << endl;
    for (int i = 0; i < network->input_neurons_.size(); i++) {
        graph << "I_" << i << ";";
    }
    graph << endl << "}" << endl;
    
    graph << "subgraph cluster_1 {" << endl
          << "style=filled;" << endl
          << "color=lightgrey;" << endl
          << "label = \"Output\"" << endl
          << "node [shape = doublecircle];" << endl;
    for (int i = 0; i < network->output_neurons_.size(); i++) {
        graph << "O_" << i
              << " [ label = \"" << "O_" << i << "\\n(" << (int) network->output_neurons_[i].bias_ << ")\"];";
    }
    graph << endl << "}" << endl;
    
    graph << "node [shape = circle];" << endl;
    for (int i = 0; i < network->hidden_neurons_.size(); i++) {
        graph << "H_" << network->hidden_neurons_[i].label_
              << " [ label = \""
              << "H_" << network->hidden_neurons_[i].label_
              << "\\n(" << (int) network->hidden_neurons_[i].bias_ << ")\"];";
    }
    for (int i = 0; i < network->hidden_neurons_.size(); i++) {
        Neuron* neuron = &network->hidden_neurons_[i];
        
        for (int j = 0; j < neuron->inputs_.size(); j++) {
            string type;
            
            bool found = false;
            for (int k = 0; k < network->input_neurons_.size(); k++) {
                if (neuron->inputs_[j].second == &network->input_neurons_[k]) {
                    found = true;
                }
            }
            
            if (found) {
                // Input neuron
                type = "I_";
            }
            else {
                // Hidden neuron
                type = "H_";
            }
            graph << type << neuron->inputs_[j].second->label_
                  << " -> "
                  << "H_" << neuron->label_
                  << " [ label = \"" << (int) neuron->inputs_[j].first << "\" ];" << endl;
        }
    }
    
    for (int i = 0; i < network->output_neurons_.size(); i++) {
        Neuron* neuron = &network->output_neurons_[i];
        
        for (int j = 0; j < neuron->inputs_.size(); j++) {
            graph << "H_" << neuron->inputs_[j].second->label_
                  << " -> "
                  << "O_" << neuron->label_
                  << " [ label = \"" << (int) neuron->inputs_[j].first << "\" ];" << endl;
        }
    }
    
    graph << "}" << endl;
    
    return graph.str();
}