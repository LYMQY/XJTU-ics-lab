#include "poly.h"
#include <time.h>

void poly_optim(const double a[], double x, long degree, double *result) {
    
    long i;
    double r = ((x * x) * (x * x)) * ((x * x) * (x * x)) * (x * x); 

    double acc0 = 0;
    double acc1 = 0;
    double acc2 = 0;
    double acc3 = 0;
    double acc4 = 0;
    double acc5 = 0;
    double acc6 = 0;
    double acc7 = 0;
    double acc8 = 0;
    double acc9 = 0;

    for (i = degree; i >= 10; i-=10) {
        acc0 = a[i]  + acc0 * r; 
        acc1 = a[i-1] + acc1 * r;
        acc2 = a[i-2] + acc2 * r;
        acc3 = a[i-3] + acc3 * r;
        acc4 = a[i-4] + acc4 * r;
        acc5 = a[i-5] + acc5 * r;
        acc6 = a[i-6] + acc6 * r;
        acc7 = a[i-7] + acc7 * r;
        acc8 = a[i-8] + acc8 * r;
        acc9 = a[i-9] + acc9 * r;
    }
    double other = a[i], mul = x;
    i--;
    while(i >= 0)
    {
        other = a[i] + other * x;
        mul *= x;
        i--;
    }
    *result = other + mul * (((((((((acc0 * x + acc1) * x + acc2) * x + acc3) * x + acc4) * x + acc5) * x + acc6) * x + acc7) * x + acc8) * x +acc9);
}

void measure_time(poly_func_t poly, const double a[], double x, long degree,
                  double *time) {
    double b = 0;
    double *result = &b;
    double sum = 0;
    poly(a, x, degree, result);
    for(int i = 0; i < 20; i++)
    {
        struct timespec t1 = {0, 0};
        struct timespec t2 = {0, 0};

        clock_gettime(CLOCK_REALTIME, &t1);
        poly(a, x, degree, result);
        clock_gettime(CLOCK_REALTIME, &t2);

        sum += t2.tv_nsec - t1.tv_nsec;
    }

    *time = sum / 20;
}