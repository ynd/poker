#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <omp.h>

#include "NeuralNetwork.h"
#include "Phenotype.h"
#include "PopulationEvolver.h"
#include <cmath>

vector< pair<char, vector<double> > > dataset;

vector<int> choice;
int last_generation = -2;

double evaluate_fitness (NeuralNetwork* network, int generation) {
    double predictions = 0.0;
    for (int r = 0; r < dataset.size()*0.80; r++) {
        vector<double> output = network->get_output(dataset[r].second);
        
        int prediction = (output[0] <= 0.5)? 'A':'B';
        
        if (prediction == dataset[r].first) {
            predictions++;
        }
    }
      
    return predictions;
}

int main (int argc, char * const argv[]) {
    fstream file;
    
    file.open("wdbc.data");
    
    dataset.resize(683);
    for (int i = 0; i < dataset.size(); i++) {
        int id;
        file >> id;
        dataset[i].second.resize(9);
        for (int j = 0; j < dataset[i].second.size(); j++) {
            file >> dataset[i].second[j];
        }
        file >> dataset[i].first;
    }
    
    PopulationEvolver ev(9, 1, evaluate_fitness, 2000, 50, 12, 0.01, true);
    
    for (int i = 0; i < ev.population_.size(); i++) {
        ifstream save;
        
        stringstream filename;
        filename << "networks/network" << i << ".gv";
        save.open(filename.str().c_str());
        
        if (save.is_open()) {
            string current = "";
            while (current != "input_units:") {
                save >> current;
            }
            save >> ev.population_[i]->input_units_;
            
            while (current != "output_units:") {
                save >> current;
            }
            save >> ev.population_[i]->output_units_;
            
            save >> current;
            for (int j = 0; current != "*/"; j++) {
                ev.population_[i]->genes_[j] = atoi(current.c_str());
                save >> current;
            }
        }
    }
    
    for (int g = 0; g < 10000; g++) {
        ev.evolve(100);
        
        ev.get_population_fitness(-1);
    
        cerr << g << " - Best fitness = " << ev.population_[0]->fitness_;
        
        double average_neurons = 0.0;
        double average_fitness = 0.0;
        for (int i = 0; i < ev.population_.size(); i++) {
            average_neurons += ev.population_[i]->network_->hidden_neurons_.size();
            average_fitness += ev.population_[i]->fitness_;
        
            stringstream filename;
            filename << "networks/network" << i << ".gv";
        
            ofstream file;
            file.open(filename.str().c_str());
            file << ev.population_[i]->save();
            file.close();
        }
        
        average_neurons = average_neurons / ev.population_.size();
        average_fitness = average_fitness / ev.population_.size();
        
        int predicted = 0;
        for (int i = 0; i < dataset.size(); i++) {
            NeuralNetwork* network = ev.population_[0]->network_;
            vector<double> output = network->get_output(dataset[i].second);

            int prediction = (output[0] <= 0.5)? 'A':'B';

            if (prediction == dataset[i].first) {
                predicted++;
            }
        }
    
        cout << ", Average neurons = " << average_neurons << ", Average fitness = " << average_fitness << ", Correctly predicted = " << predicted << endl;
    }
    
    return 0;
}
