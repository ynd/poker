#include "random.h"

/*
	TODO Get a real random
*/
int get_random(int max)
{
    return (int)(rand() / ((double) RAND_MAX + 1.0f) * (double)max);
}
