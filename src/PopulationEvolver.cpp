/*
 *  PopulationEvolver.cpp
 *  letters
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
    double (*evaluate_fitness)(NeuralNetwork*, int),
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
        if (i >= point1 && i <= point2) {
            c1->genes_.push_back(p2->genes_[i]);
            c2->genes_.push_back(p1->genes_[i]);
        }
        else {
            c1->genes_.push_back(p1->genes_[i]);
            c2->genes_.push_back(p2->genes_[i]);
        }
        
        if (random(0.0, 1.0) < mutation_rate_) {
             c1->genes_[i] += rand() % 256;
        }
        
        if (random(0.0, 1.0) < mutation_rate_) {
             c2->genes_[i] += rand() % 256;
        }
    }
    
    return pair<Individual*, Individual*>(c1, c2);
}

void PopulationEvolver::get_population_fitness(int generation) {
    #pragma omp parallel for
    for (int i = 0; i < population_.size(); i++) {
        if (population_[i]->network_ == NULL) {
            population_[i]->network_ = Phenotype::get_network(population_[i]);
        }
        population_[i]->fitness_ = evaluate_fitness_(population_[i]->network_, generation);
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
        get_population_fitness(g);
        
        // Select individuals using tournament selectoin.
        vector<Individual*> selected(crossovers_);
        vector<Individual*> candidates = population_;
        for (int i = 0; i < selected.size(); i++) {
            vector<Individual*> contestants(population_.size() / crossovers_);
            for (int j = 0; j < contestants.size() && !candidates.empty(); j++) {
                int index = rand() % candidates.size();
                contestants[j] = candidates[index];
                candidates.erase(candidates.begin() + index);
            }
            
            if (maximize_fitness_) {
                sort(contestants.begin(), contestants.end(), compare_fitness_max);
            }
            else {
                sort(contestants.begin(), contestants.end(), compare_fitness_min);
            }
            
            selected[i] = contestants[0];
        }
        
        // Mate selected.
        vector<Individual*> offspring;
        for (int i = 0; i < selected.size(); i++) {
            Individual* p1 = selected[i];
            Individual* p2 = selected[rand() % selected.size()];
            
            pair<Individual*, Individual*> childs = crossover(p1, p2);
            
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
            
            if (population_[choice]->network_ != NULL) {
                delete population_[choice]->network_;
            }
            
            delete population_[choice];
            
            population_[choice] = offspring[i];
            
            replaceable.erase(replaceable.begin() + random_replaceable);
        }
    }
}
