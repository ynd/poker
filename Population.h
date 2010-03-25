/*
 *  Population.h
 *  brains
 *
 *  Created by Yann Dauphin on 17/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"

class Population {
public:
    
    // Individuals in the population.
    vector<Individual> individuals_;
};

#endif