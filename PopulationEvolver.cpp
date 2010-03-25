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
    int chromosome_size, int population_size,
    int crossovers, double mutation_rate,
    bool maximize_fitness)
: evaluate_fitness_(evaluate_fitness),
  population_size_(population_size),
  crossovers_(crossovers), 
  mutation_rate_(mutation_rate),
  maximize_fitness_(maximize_fitness) {
    srand (time(NULL));
    
    // Initialize population with random individuals.
    for (int i = 0; i < population_size_; i++) {
        Individual* individual = new Individual(input_units, output_units);
        
        for (int j = 0; j < chromosome_size; j++) {
            individual->genes_.push_back(rand() % 256);
        }
        
        population_.push_back(individual);
    }
}

bool PopulationEvolver::compare_fitness_max(Individual* p1, Individual* p2) {
    return p1->fitness_ > p2->fitness_;
}

bool PopulationEvolver::compare_fitness_min(Individual* p1, Individual* p2) {
    return p1->fitness_ < p2->fitness_;
}

pair<Individual*, Individual*> PopulationEvolver::crossover(Individual* p1, Individual* p2) {
    Individual* c1 = new Individual(p1->input_units_, p1->output_units_);
    Individual* c2 = new Individual(p1->input_units_, p1->output_units_);
    
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
    
    individual->genes_[point] += rand() % 256;
}

void PopulationEvolver::get_population_fitness() {
    vector<NeuralNetwork*> networks;
    for (int i = 0; i < population_.size(); i++) {
        if (population_[i]->network_ == NULL) {
            population_[i]->network_ = Phenotype::get_network(population_[i]);
        }
        
        networks.push_back(population_[i]->network_);
    }
    
    for (int i = 0; i < population_.size(); i++) {
        population_[i]->fitness_ = evaluate_fitness_(networks[i], networks);
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
        get_population_fitness();
        
        // Mate best individuals.
        vector<Individual*> offspring;
        for (int i = 0; i < crossovers_; i++) {
            Individual* p1 = population_[i];
            Individual* p2 = population_[rand() % crossovers_];
            
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
        
        // Mutate one random individual.
        if (random(0.0, 1.0) < mutation_rate_) {
            Individual* i1 = population_[rand() % population_.size()];
            
            mutate(i1);
            
            delete i1->network_;
            i1->network_ = NULL;
        }
        
        // Replace unfit individuals.
        vector<int> replaceable;
        for (int i = crossovers_; i < population_.size(); i++) {
            replaceable.push_back(i);
        }
        for (int i = 0; i < offspring.size() && !replaceable.empty(); i++) {
            int random_replaceable = rand() % replaceable.size();
            int choice = replaceable[random_replaceable];
            
            if (population_[choice]->network_ != NULL) {
                delete population_[choice]->network_;
            }
            
            delete population_[choice];
            
            population_[choice] = offspring[i];
            
            replaceable.erase(replaceable.begin() + random_replaceable);
        }
    }
}
