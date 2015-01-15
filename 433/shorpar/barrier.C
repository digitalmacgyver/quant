#include <pthread.h>

#include "barrier.h"

int num_threads;

pthread_mutex_t global_barrier_mutex;
pthread_cond_t global_barrier_cond;
int global_barrier_counter = 0;

pthread_mutex_t global_barrier_mutex1;
pthread_cond_t global_barrier_cond1;
int global_barrier_counter1 = 0;

pthread_mutex_t global_barrier_mutex2;
pthread_cond_t global_barrier_cond2;
int global_barrier_counter2 = 0;

pthread_mutex_t global_barrier_mutex3;
pthread_cond_t global_barrier_cond3;
int global_barrier_counter3 = 0;

pthread_mutex_t global_barrier_mutex4;
pthread_cond_t global_barrier_cond4;
int global_barrier_counter4 = 0;

pthread_mutex_t global_barrier_mutex5;
pthread_cond_t global_barrier_cond5;
int global_barrier_counter5 = 0;

pthread_mutex_t global_barrier_mutex6;
pthread_cond_t global_barrier_cond6;
int global_barrier_counter6 = 0;

pthread_mutex_t global_barrier_mutex7;
pthread_cond_t global_barrier_cond7;
int global_barrier_counter7 = 0;

pthread_mutex_t global_barrier_mutex8;
pthread_cond_t global_barrier_cond8;
int global_barrier_counter8 = 0;

pthread_mutex_t final_barrier_mutex;
pthread_cond_t final_barrier_cond;
int final_barrier_counter = 0;

void barrier(int* barrier_counter, int total_threads, 
	     pthread_mutex_t* barrier_mutex, pthread_cond_t* barrier_cond) {
  int reuseCount;
  pthread_mutex_lock(barrier_mutex);
  (*barrier_counter)++;
  reuseCount = (*barrier_counter)/total_threads;
    
  if(*barrier_counter%total_threads == 0) {
    pthread_cond_broadcast(barrier_cond);
  } else {
    while (reuseCount == (*barrier_counter)/total_threads)
      pthread_cond_wait(barrier_cond, barrier_mutex);
  }
  pthread_mutex_unlock(barrier_mutex);
}

void final_barrier(int* barrier_counter, int total_threads, 
	     pthread_mutex_t* barrier_mutex, pthread_cond_t* barrier_cond) {
  int reuseCount;
  pthread_mutex_lock(barrier_mutex);
  (*barrier_counter)++;
  reuseCount = (*barrier_counter)/total_threads;
    
  if(*barrier_counter%total_threads == 0) {
    pthread_cond_broadcast(barrier_cond);
  } else {
    while (reuseCount == (*barrier_counter)/total_threads)
      pthread_cond_wait(barrier_cond, barrier_mutex);
  }
  pthread_mutex_unlock(barrier_mutex);
}


