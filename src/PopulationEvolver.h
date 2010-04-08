/*
 *  PopulationEvolver.h
 *  letters
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#ifndef POPULATION_EVOLVER_H
#define POPULATION_EVOLVER_H

#include "Individual.h"
#include "NeuralNetwork.h"

class PopulationEvolver {

public:
    PopulationEvolver(int input_units, int output_units,
        double (*evaluate_fitness)(NeuralNetwork*, int),
        int chromosome_size=5000, int population_size=50, int crossovers=12,
        double mutation_rate=0.004, bool maximize_fitness=true);
    
    void evolve(int generations);
    
    pair<Individual*, Individual*> crossover(Individual* p1, Individual* p2);
    
    void slide(Individual* i1);
    
    static bool compare_fitness_max(Individual* p1, Individual* p2);
    
    static bool compare_fitness_min(Individual* p1, Individual* p2);
    
    void get_population_fitness(int generation);
    
    // Indicates if the genetic algorithm should try to maximize fitness.
    bool maximize_fitness_;
    
    // Size of the population.
    int population_size_;
    
    // Number of crossovers per generation.
    int crossovers_;
    
    // Mutation rate.
    double mutation_rate_;
    
    // Pointer to task-dependent function that computes fitness.
    double (*evaluate_fitness_)(NeuralNetwork*, int);
    
    // Population that will be evolved.
    vector<Individual*> population_;
};

#endif
