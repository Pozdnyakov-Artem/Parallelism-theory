#include <omp.h>
#include <chrono>
#include <iostream>
#include <vector>


int n = 20000;


void matrix_vector_product_omp(std::vector<double> &a, std::vector<double> &b, std::vector<double> &c)
{
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++)
        {
            c[i] = 0.0;
            for (int j = 0; j < n; j++)
                c[i] += a[i * n + j] * b[j];
        }
    }
}

int main()
{
    std::vector<double> a(n*n);
    std::vector<double> b(n);
    std::vector<double> c(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    for (int j = 0; j < n; j++)
        b[j] = j;
    const auto start{std::chrono::steady_clock::now()};
    matrix_vector_product_omp(a, b, c);
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> dur{end-start};
    std::cout<<dur.count();
}