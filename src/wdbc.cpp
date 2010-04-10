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
vector<int> success_distribution;

vector<int> choice;
int last_generation = -2;

double evaluate_fitness (NeuralNetwork* network, int generation) {
    double predictions = 0.0;
    for (int r = 138; r < dataset.size(); r++) {
        vector<double> output = network->get_output(dataset[r].second);
        
        int letter = dataset[r].first - 65;
        
        int maximum = 0;
        double max_value = -1;
        for (int j = 0; j < output.size(); j++) {
            if (output[j] > max_value) {
                maximum = j;
                max_value = output[j];
            }
        }
        
        if (maximum == letter) {
            predictions += 5.0 / exp(0.3*success_distribution[r]);
        }
    }
      
    return predictions;
}

void calculate_success_distribution (PopulationEvolver* evolver) {
    success_distribution.resize(dataset.size());
    
    for (int i = 0; i < success_distribution.size(); i++) {
        int successes = 0;
        for (int j = 0; j < evolver->population_.size(); j++) {
            if (evolver->population_[j]->network_ == NULL) {
                evolver->population_[j]->network_ = Phenotype::get_network(evolver->population_[j]);
            }
            vector<double> output = evolver->population_[j]->network_->get_output(dataset[i].second);

            int letter = dataset[i].first - 65;

            int max = 0;
            double max_value = -1;
            for (int k = 0; k < output.size(); k++) {
                if (output[j] > max_value) {
                    max = k;
                    max_value = output[k];
                }
            }

            if (max == letter) {
                successes++;
            }
        }
        if (successes == 0 ) {
            cout << "zero success = " << i << endl;
        }
        
        success_distribution[i] = successes;
    }
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
    
    PopulationEvolver ev(9, 2, evaluate_fitness, 550, 50, 12, 0.01, true);
    
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
    
    calculate_success_distribution(&ev);
    
    for (int g = 0; g < 10000; g++) {
        ev.evolve(100);
        
        ev.get_population_fitness(-1);
        
        calculate_success_distribution(&ev);
    
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
        
        cout << ", Average neurons = " << average_neurons << ", Average fitness = " << average_fitness << endl;
        
        for (int z = 0; z < ev.population_.size(); z++) {
            int predicted = 0;
            for (int i = 0; i < dataset.size(); i++) {
                NeuralNetwork* network = ev.population_[z]->network_;
                vector<double> output = network->get_output(dataset[i].second);

                int max = 0;
                double max_value = -1;
                for (int j = 0; j < output.size(); j++) {
                    if (output[j] > max_value) {
                        max = j;
                        max_value = output[j];
                    }
                }

                if (max == (dataset[i].first - 65)) {
                    predicted++;
                }
            }

            cout << "fitess = " << ev.population_[z]->fitness_ << ", Correctly predicted = " << (double)predicted/dataset.size() << endl;
        }
        
        cout << "--------------------" << endl;
    }
    
    return 0;
}
