#include <omp.h>
#include <cmath>
#include <iostream>
#include <chrono>

double func(double x)
{
    return exp(-x * x);
}

double integrate_omp(double (*func)(double), double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.0;
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);
        double sumloc = 0.0;
        for (int i = lb; i <= ub; i++)
            sumloc += func(a + h * (i + 0.5));
        #pragma omp atomic
            sum += sumloc;
    }
    sum *= h;
    return sum;
}

int main()
{
    const auto start{std::chrono::steady_clock::now()};
    std::cout<<integrate_omp(func,-4,4,40000000)<<std::endl;
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> dur{end-start};
    std::cout<<dur.count()<<std::endl;

    return 0;
}