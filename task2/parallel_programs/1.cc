#include <omp.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>


int n = 40000;

int main()
{
    int iter = 25;
    std::vector<int> threads_num = {2,4,6,8,16,20,40};

    for(const int threads : threads_num)
    {
        std::vector<double> time;
        
        for(int h = 0 ; h<iter; h++)
        {
            std::vector<double> a(n*n);
            std::vector<double> b(n);
            std::vector<double> c(n);

            #pragma omp parallel num_threads(threads)
            {

                int nthreads = omp_get_num_threads();
                int threadid = omp_get_thread_num();
                int items_per_thread = n / nthreads;
                int lb = threadid * items_per_thread;
                int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);

                for (int i = lb; i <= ub; i++) {
                    for (int j = 0; j < n; j++)
                        a[i * n + j] = i + j;
                    b[i] = i;
                }
                
            }

            const auto start{std::chrono::steady_clock::now()};

            #pragma omp parallel num_threads(threads)
            {

                int nthreads = omp_get_num_threads();
                int threadid = omp_get_thread_num();
                int items_per_thread = n / nthreads;
                int lb = threadid * items_per_thread;
                int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);

                for (int i = lb; i <= ub; i++)
                {
                    double sum = 0.0;
                    for (int j = 0; j < n; j++)
                        sum += a[i * n + j] * b[j];
                    c[i] = sum;
                }
            }
            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> dur{end-start};

            time.push_back(dur.count());
            std::cout<<h<<std::endl;
        }

        double count = 0;

        for(auto i : time)
            count+=i;

        std::cout<<"num_threads:"<<threads<<" avg_time: "<<count/iter;
    }

}