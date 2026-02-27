#include <omp.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <vector>

double func(double x)
{
    return exp(-x * x);
}

double integrate_omp(double (*func)(double), double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++)
        sum += func(a + h * (i + 0.5));
    
    sum *= h;
    return sum;
}

int main()
{
    std::vector<double> time;

    for(int i = 0; i < 4; i++)
    {
        const auto start{std::chrono::steady_clock::now()};

        std::cout<<integrate_omp(func,-4,4,40000000)<<std::endl;

        const auto end{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> dur{end-start};
        
        std::cout<<"time "<<dur.count()<<std::endl;
        time.push_back(dur.count());
    }

    double count = 0;

    for(auto i : time)
        count+=i;

    std::cout<<"avg_time: "<<count/4;

    return 0;
}