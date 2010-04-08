#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "NeuralNetwork.h"
#include "Phenotype.h"
#include "PopulationEvolver.h"
#include <cmath>

double evaluate_fitness_xor (NeuralNetwork* network, int generation) {
    double error = 0.0;
    vector<double> input(2);
    input[0] = 0.0;
    input[1] = 0.0;
    error += abs(0.0 - network->get_output(input)[0]);
    
    input[0] = 0.0;
    input[1] = 1.0;
    error += abs(1.0 - network->get_output(input)[0]);
    
    input[0] = 1;
    input[1] = 0;
    error += abs(1.0 - network->get_output(input)[0]);
    
    input[0] = 1.0;
    input[1] = 1.0;
    error += abs(0.0 - network->get_output(input)[0]);
      
    return error;
}

int main (int argc, char * const argv[]) {    
    PopulationEvolver ev(2, 1, evaluate_fitness_xor, 150, 50, 12, 0.04, false);
    
    ev.evolve(1000);
    
    ev.get_population_fitness(0);
    
    cout << "Best fitness  = " << ev.population_[0]->fitness_ << endl;
    
    for (int i = 0; i < ev.population_.size(); i++) {
        stringstream filename;
        filename << "networks/network" << i << ".gv";
        
        ofstream file;
        file.open(filename.str().c_str());
        file << ev.population_[i]->save();
        file.close();
    }
    
    return 0;
    
    NeuralNetwork* net = Phenotype::get_network(ev.population_[0]);
    while (true) {
        vector<double> input(2);
    
        cout << "x0 = ";
        cin >> input[0];
        cout << "x1 = ";
        cin >> input[1];
        cout << "y = " << net->get_output(input)[0] << endl;
        cout << "------" << endl;
    }
    
    return 0;
}
