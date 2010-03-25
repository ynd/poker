/*
 *  PopulationEvolver.cpp
 *  brains
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#include "PopulationEvolver.h"
#include "utilities.h"
#include "Phenotype.h"
#include <algorithm>
#include <iostream>

PopulationEvolver::PopulationEvolver(int input_units, int output_units,
    double (*evaluate_fitness)(NeuralNetwork*, vector<NeuralNetwork*>),
    int chromosome_size, int population_size, int crossovers, double mutation_rate, bool maximize_fitness)
: input_units_(input_units),
  output_units_(output_units),
  evaluate_fitness_(evaluate_fitness),
  chromosome_size_(chromosome_size),
  population_size_(population_size),
  crossovers_(crossovers), 
  mutation_rate_(mutation_rate),
  maximize_fitness_(maximize_fitness) {
    srand (time(NULL));
    
    // Initialize population with random individuals.
    for (int i = 0; i < population_size_; i++) {
        Individual* individual = new Individual();
        
        for (int j = 0; j < chromosome_size_; j++) {
            individual->genes_.push_back(random(-128, 127));
        }
        
        population_.push_back(pair<double, Individual*>(0.0, individual));
    }
}

bool PopulationEvolver::compare_fitness_max(pair<double, Individual*> p1, pair<double, Individual*> p2) {
    return p1.first > p2.first;
}

bool PopulationEvolver::compare_fitness_min(pair<double, Individual*> p1, pair<double, Individual*> p2) {
    return p1.first < p2.first;
}

pair<Individual*, Individual*> PopulationEvolver::crossover(Individual* p1, Individual* p2) {
    Individual* c1 = new Individual();
    Individual* c2 = new Individual();
    
    int point1 = rand() % p1->genes_.size();
    int point2 = point1 + rand() % (p1->genes_.size() - point1);
    
    for (int i = 0; i < p1->genes_.size(); i++) {
        if (i > point1 && i < point2) {
            c1->genes_.push_back(p2->genes_[i]);
            c2->genes_.push_back(p1->genes_[i]);
        }
        else {
            c1->genes_.push_back(p1->genes_[i]);
            c2->genes_.push_back(p2->genes_[i]);
        }
    }
    
    return pair<Individual*, Individual*>(c1, c2);
}

void PopulationEvolver::mutate(Individual* individual) {
    int point  = rand() % individual->genes_.size();
    
    individual->genes_[point] += random(-128, 127);
}

void PopulationEvolver::compute_fitness() {
    vector<NeuralNetwork*> networks;
    for (int i = 0; i < population_.size(); i++) {
        NeuralNetwork* network = Phenotype::get_network(population_[i].second, input_units_, output_units_);
            
        networks.push_back(network);
    }
    for (int i = 0; i < population_.size(); i++) {
        population_[i].first = evaluate_fitness_(networks[i], networks);
    }
    for (int i = 0; i < networks.size(); i++) {
        delete networks[i];
    }
    
    if (maximize_fitness_) {
        sort(population_.begin(), population_.end(), compare_fitness_max);
    }
    else {
        sort(population_.begin(), population_.end(), compare_fitness_min);
    }

}

void PopulationEvolver::evolve(int generations) {
    for (int g = 0; g < generations; g++) {
        // Evaluate fitness of each individual.
        compute_fitness();
        
        // Mate best individuals.
        vector<Individual*> offspring;
        for (int i = 0; i < crossovers_; i++) {
            Individual* p1 = population_[i].second;
            Individual* p2 = population_[rand() % crossovers_].second;
            
            pair<Individual*, Individual*> childs = crossover(p1, p2);
            
            // Mutation.
            if (random(0.0, 1.0) < mutation_rate_) {
                mutate(childs.first);
            }
            if (random(0.0, 1.0) < mutation_rate_) {
                mutate(childs.second);
            }
            
            offspring.push_back(childs.first);
            offspring.push_back(childs.second);
        }
        
        // Replace unfit individuals.
        vector<int> replaceable;
        for (int i = crossovers_; i < population_.size(); i++) {
            replaceable.push_back(i);
        }
        for (int i = 0; i < offspring.size() && !replaceable.empty(); i++) {
            int random_replaceable = rand() % replaceable.size();
            int choice = replaceable[random_replaceable];
            
            delete population_[choice].second;
            
            population_[choice] = pair<double, Individual*>(0.0, offspring[i]);
            
            replaceable.erase(replaceable.begin() + random_replaceable);
        }
    }
}
