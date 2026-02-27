#include <omp.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>


int n = 20000;

int main()
{
    // std::vector<double> time;
    // for(int h = 0 ; h<4; h++)
    // {
        const auto start{std::chrono::steady_clock::now()};

        std::vector<double> a(n*n);
        std::vector<double> b(n);
        std::vector<double> c(n);

        for (int j = 0; j < n; j++)
                b[j] = j;

        #pragma omp parallel
        {

            int nthreads = omp_get_num_threads();
            // std::cout<<nthreads<<std::endl;
            int threadid = omp_get_thread_num();
            int items_per_thread = n / nthreads;
            int lb = threadid * items_per_thread;
            int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);

            for (int i = lb; i <= ub; i++) {
                for (int j = 0; j < n; j++)
                    a[i * n + j] = i + j;
            }

            // for (int j = lb; j < ub; j++)
            //     b[j] = j;
            
            // #pragma omp barrier

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
        std::cout<<"time: "<<dur.count()<<std::endl;

        // time.push_back(dur.count());

        double count = 0;

        for(int i = 0; i < n; i++)
        {
            count+=c[i];
        }

        std::cout<<count<<std::endl;
    // }

    // double count = 0;

    // for(auto i : time)
    //     count+=i;

    // std::cout<<"avg_time: "<<count/4;

}