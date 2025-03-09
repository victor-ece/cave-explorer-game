#ifndef _HW3_RANDOM
#define _HW3_RANDOM

/* 
 * set_seed
 *
 * Sets the seed for our RNG. If the parameter is zero, 
 * then the seed is set to the current time, otherwise
 * it is set to the value of the parameter. 
 */
void set_seed(unsigned int seed); 

/* 
 * get_rand_range
 *
 * Returns a pseudorandom unsigned int in the range [from, to) 
 */
unsigned int get_rand_range(unsigned int from, unsigned int to);

#endif

