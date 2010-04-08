/*
 *  utilities.cpp
 *  letters
 *
 *  Created by Yann Dauphin on 15/03/10.
 *  Copyright 2010 lambdatree. All rights reserved.
 *
 */

#include "utilities.h"

/*
 *  Return a random double between parameters low and high.
 */
double random(double low, double high) {
  double temp = (rand() / (((double) RAND_MAX) + 1.0)) * (high - low) + low;
  
  return temp;
}

/*
 *  Return a random integer between parameters low and high.
 */
int random(int low, int high) {
  int temp = (rand() % (high - low)) + low;
  
  return temp;
}

/*
 *  Return the result of the logistic function with parameter t.
 */
double logistic_function(double t) {
  return 1.0 / (1.0 + exp(-t));
}
