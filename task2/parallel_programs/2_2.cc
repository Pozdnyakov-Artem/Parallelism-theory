#include <omp.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <vector>

double func(double x)
{
    return exp(-x * x);
}

double integrate_omp(double (*func)(double), double a, double b, int n, int threads)
{
    double h = (b - a) / n;
    double sum = 0.0;
    #pragma omp parallel num_threads(threads)
    {

        #pragma omp for reduction(+:sum) schedule(static)
        for (int i = 0; i < n; i++)
            sum += func(a + h * (i + 0.5));
    }

    return sum * h;
}

int main()
{
    std::vector<int> threads_num = {2,4,6,8,16,20,40};

    for(const int threads : threads_num)
    {

        std::vector<double> time;

        for(int i = 0; i < 100; i++)
        {
            const auto start{std::chrono::steady_clock::now()};

            integrate_omp(func,-4,4,40000000, threads);

            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> dur{end-start};
            
            // std::cout<<"time "<<dur.count()<<std::endl;
            time.push_back(dur.count());
        }

        double count = 0;

        for(auto i : time)
            count+=i;

        std::cout<<"num_threads: "<<threads<<" avg_time: "<<count/100<<std::endl;
    }

    return 0;
}