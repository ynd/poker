#ifndef random_h
#define random_h 

#include <cstdlib>

#define RAND_INV_RANGE(r) ((int) ((RAND_MAX + 1) / (r)))

int get_random(int max);

#endif
