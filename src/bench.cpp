#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <omp.h>

#include "NeuralNetwork.h"
#include "Phenotype.h"
#include "PopulationEvolver.h"
#include <cmath>
#include <map>

vector< pair<char, vector<double> > > dataset;

int main (int argc, char * const argv[]) {
    int benched = 0;
    if (argc > 1) {
        benched = atoi(argv[1]);
    }
    
    fstream file;
    
    file.open("Dataset.data");
    
    dataset.resize(20000);
    for (int i = 0; i < 20000; i++) {
        file >> dataset[i].first;
        dataset[i].second.resize(16);
        for (int j = 0; j < 16; j++) {
            file >> dataset[i].second[j];
        }
    }
    
    PopulationEvolver ev(16, 26, NULL, 1000, 50, 12, 0.1, true);
    
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
    
    map<char, int> wins;
    for (int i = 16000; i < dataset.size(); i++) {
        if (ev.population_[benched]->network_ == NULL) {
            ev.population_[benched]->network_ = Phenotype::get_network(ev.population_[benched]);
        }
        NeuralNetwork* network = ev.population_[benched]->network_;
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
            wins[dataset[i].first]++;
            
        }
        else if (wins.find(dataset[i].first) == wins.end()) {
            wins[dataset[i].first] = 0;
        }
        else {
            if (dataset[i].first == 'T') {
                cout << dataset[i].first << " " << (char)(max + 65)<< " ";
                for (int j = 0; j < output.size(); j++) {
                    cout << output[j] << " ";
                }
                cout << endl;
            }
        }
    }
    
    int total = 0;
    map<char, int>::iterator it;
    for(it = wins.begin(); it != wins.end(); it++) {
        total += it->second;
        cout << it->first << " = " << it->second << endl;
    }
    
    cout << "total = " << total << endl;
    
    return 0;
}
