#include "range.h"

int * q_range_lower;
int * q_range_upper;

//Just like q_range for n, the number to be factored.
int * n_range_lower;
int * n_range_upper;

void init_ranges(int num_threads, int num_factor, int q) {
  for (int i = 0 ; i < num_threads ; i++) {
    q_range_lower[i] = i*(q/num_threads);
    q_range_upper[i] = (i+1)*(q/num_threads) - 1;
    if (i == num_threads - 1) {
      q_range_upper[i] += q%num_threads;
    }
  }

  for (int i = 0 ; i < num_threads ; i++) {
    n_range_lower[i] = i*(num_factor/num_threads);
    n_range_upper[i] = (i+1)*(num_factor/num_threads) - 1;
    if (i == num_threads - 1) {
      n_range_upper[i] += num_factor%num_threads;
    }
  }
}

