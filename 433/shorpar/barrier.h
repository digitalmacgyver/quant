#include <pthread.h>

#ifndef BARRIER_H
#define BARRIER_H

extern int num_threads;

void barrier(int* barrier_counter, int total_threads, 
	     pthread_mutex_t* barrier_mutex, pthread_cond_t* barrier_cond);

void final_barrier(int* barrier_counter, int total_threads, 
		   pthread_mutex_t* barrier_mutex, pthread_cond_t* barrier_cond);

extern pthread_mutex_t global_barrier_mutex;
extern pthread_cond_t global_barrier_cond;
extern int global_barrier_counter;

extern pthread_mutex_t global_barrier_mutex1;
extern pthread_cond_t global_barrier_cond1;
extern int global_barrier_counter1;

extern pthread_mutex_t global_barrier_mutex2;
extern pthread_cond_t global_barrier_cond2;
extern int global_barrier_counter2;

extern pthread_mutex_t global_barrier_mutex3;
extern pthread_cond_t global_barrier_cond3;
extern int global_barrier_counter3;

extern pthread_mutex_t global_barrier_mutex4;
extern pthread_cond_t global_barrier_cond4;
extern int global_barrier_counter4;

extern pthread_mutex_t global_barrier_mutex5;
extern pthread_cond_t global_barrier_cond5;
extern int global_barrier_counter5;

extern pthread_mutex_t global_barrier_mutex6;
extern pthread_cond_t global_barrier_cond6;
extern int global_barrier_counter6;

extern pthread_mutex_t global_barrier_mutex7;
extern pthread_cond_t global_barrier_cond7;
extern int global_barrier_counter7;

extern pthread_mutex_t global_barrier_mutex8;
extern pthread_cond_t global_barrier_cond8;
extern int global_barrier_counter8;

extern pthread_mutex_t final_barrier_mutex;
extern pthread_cond_t final_barrier_cond;
extern int final_barrier_counter;

#endif
