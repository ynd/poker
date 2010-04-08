/*
 *  Individual.cpp
 *  letters
 *
 *  Created by Yann Dauphin on 25/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#include "Individual.h"
#include "Phenotype.h"
#include <sstream>
#include <iostream>

Individual::Individual(int input_units, int output_units)
: input_units_(input_units), output_units_(output_units),
  network_(NULL), fitness_(0.0) { }

string Individual::save() {
    if (network_ == NULL) {
        network_ = Phenotype::get_network(this);
    }
    
    stringstream label;
    label << "fitness = " << fitness_;
    
    stringstream graph;
    
    // Save individual in comments
    graph << "/*" << endl;
    graph << "input_units: " << input_units_ << endl
          << "output_units: " << output_units_ << endl;
    for (int i = 0; i < genes_.size(); i++) {
        graph << (unsigned int) genes_[i] << " ";
    }
    graph << endl;
    graph << "*/" << endl;
    
    // Output neural network structure.
    graph << "digraph neural_network {" << endl
          << "label=\"" << label.str() << "\";" << endl;
    graph << "rankdir=LR;" << endl;
    
    graph << "subgraph cluster_0 {" << endl
          << "style=filled;" << endl
          << "color=lightgrey;" << endl
          << "label = \"Input\"" << endl
          << "node [shape = doublecircle];" << endl;
    for (int i = 0; i < network_->input_neurons_.size(); i++) {
        graph << "I_" << i << ";";
    }
    graph << endl << "}" << endl;
    
    graph << "subgraph cluster_1 {" << endl
          << "style=filled;" << endl
          << "color=lightgrey;" << endl
          << "label = \"Output\"" << endl
          << "node [shape = doublecircle];" << endl;
    for (int i = 0; i < network_->output_neurons_.size(); i++) {
        graph << "O_" << i
              << " [ label = \"" << "O_" << i << "\\n(" << (int) network_->output_neurons_[i].bias_ << ")\"];";
    }
    graph << endl << "}" << endl;
    
    graph << "node [shape = circle];" << endl;
    for (int i = 0; i < network_->hidden_neurons_.size(); i++) {
        graph << "H_" << network_->hidden_neurons_[i].label_
              << " [ label = \""
              << "H_" << network_->hidden_neurons_[i].label_
              << "\\n(" << (int) network_->hidden_neurons_[i].bias_ << ")\"];" << endl;
    }
    for (int i = 0; i < network_->hidden_neurons_.size(); i++) {
        Neuron* neuron = &network_->hidden_neurons_[i];
        
        for (int j = 0; j < neuron->inputs_.size(); j++) {
            string type;
            
            bool found = false;
            for (int k = 0; k < network_->input_neurons_.size(); k++) {
                if (neuron->inputs_[j].second == &network_->input_neurons_[k]) {
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
    
    for (int i = 0; i < network_->output_neurons_.size(); i++) {
        Neuron* neuron = &network_->output_neurons_[i];
        
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
