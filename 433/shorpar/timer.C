#include <iostream>
#include <sys/time.h>

using namespace std;

double  get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, NULL);
   if (ok<0) { cout << "gettimeofday error" << endl;  }
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6);
}
