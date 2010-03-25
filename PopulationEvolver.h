/*
 *  PopulationEvolver.h
 *  brains
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
        double (*evaluate_fitness)(NeuralNetwork*, vector<NeuralNetwork*>),
        int chromosome_size=5000, int population_size=50, int crossovers=12,
        double mutation_rate=0.004, bool maximize_fitness=true);
    
    void evolve(int generations);
    
    static bool compare_fitness_max(pair<double, Individual*> p1, pair<double, Individual*> p2);
    
    static bool compare_fitness_min(pair<double, Individual*> p1, pair<double, Individual*> p2);
    
    void compute_fitness();
    
    static pair<Individual*, Individual*> crossover(Individual* p1, Individual* p2);
    
    void mutate(Individual* individual);
    
    // Indicates if the genetic algorithm should try to maximize fitness.
    bool maximize_fitness_;
    
    // Size of the chromosomes.
    int chromosome_size_;
    
    // Size of the population.
    int population_size_;
    
    // Number of crossovers per generation.
    int crossovers_;
    
    // Mutation rate.
    double mutation_rate_;

    // Number of input units wanted.
    int input_units_;
    
    // Number of output units wanted.
    int output_units_;
    
    // Pointer to task-dependent function that computes fitness.
    double (*evaluate_fitness_)(NeuralNetwork*, vector<NeuralNetwork*>);
    
    // Population that will be evolved.
    vector< pair<double, Individual*> > population_;
};

#endif