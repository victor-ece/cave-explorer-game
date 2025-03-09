#include<stdio.h>
#include<time.h>
#include<limits.h>
#include"random.h"

static unsigned int _seed;

/* Sets the seed */
void set_seed(unsigned int seed) 
{
	if (seed == 0) {
		_seed = (unsigned int)time(NULL);
	}
	else {
		_seed = seed;
	}
}

/* Returns a random unsigned int */
static unsigned int get_rand_int() 
{
	_seed ^= _seed << 13;
	_seed ^= _seed << 17;
	_seed ^= _seed << 5;
	return _seed;
}

/* Returns a random unsigned int in the range [from, to) 
 *
 * 'from' should be less than 'to'. If not, it prints a warning and makes
 * the following assumptions: 
 * If from == to, it assumes the caller meant [from, from+1).
 * If from > to, it assumes the caller flipped them by accident and flips them back.
*
 * If possible, it avoids returning the same number in consecutive calls.
 */
unsigned int get_rand_range(unsigned int from, unsigned int to) 
{
	static unsigned int prev = UINT_MAX;
	if (to == from) {
		fprintf(stderr, "WARNING: range of zero size in get_rand_range.\n");
		return to;
	}
	if (to < from) {
		to = to ^ from;
		from = to ^ from;
		to = to ^ from;
		fprintf(stderr, "WARNING: range of negative direction in get_rand_range.\n");
	}
	unsigned int next = get_rand_int() % (to-from) + from;
	if (next == prev && to-from >= 2) {
		if (next + 1 < to) { // next is guaranteed < UINT_MAX so will not overflow
			next++;
		}
		else if (next-1 >= from) { // here, next is guaranteed > 0 so will not underflow
			next--;
		}
	}
	prev = next;
	return next;
}

