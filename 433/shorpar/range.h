#ifndef RANGE_H
#define RANGE_H

//The i'th element of q_range_lower is the lower limit that the i'th
//process element should process, and q_range_upper's i'th element is
//the upper limit on what the i'th thread should process.
//The can be used in for loops like this:
//for(int i = q_range_lower[proc_rank] ; i <= q_range_upper[proc_rank] ; i++)
//Please note the <= in the boolean portion.

extern int * q_range_lower;
extern int * q_range_upper;

//Just like q_range for n, the number to be factored.
extern int * n_range_lower;
extern int * n_range_upper;

void init_ranges(int num_threads, int num_factor, int q);

#endif
