#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>

double E = 1e-10;
int N = 12000;
double lr = 0.9 * (2.0/(N+1));

void simple_iteration(std::vector<double> &A, std::vector<double> &x, std::vector<double> &b, std::vector<double> &Ax, int threads)
{
    bool status = false;
    double norm2 = 0;

    #pragma omp parallel num_threads(threads)
    {
        #pragma omp for schedule(dynamic, 200) reduction(+:norm2)
        for(int i = 0; i < N; i++){
            norm2 += b[i] * b[i];
        }

        while (!status)
        {
            #pragma omp for schedule(dynamic, 200)
            for(int i = 0; i < N; i++)
            {
                double su = 0;
                for(int j = 0; j<N; j++)
                {
                    su+=A[i*N+j]*x[j];
                }
                Ax[i] = (su - b[i]);
            }

            double norm1 = 0;
            #pragma omp for schedule(dynamic, 200) reduction(+:norm1)
            for(int i = 0; i < N; i++)
            {
                norm1 += Ax[i]*Ax[i];
            }

            #pragma omp single
            {
                status = (norm1/norm2 < E);
            }
            #pragma omp barrier

            #pragma omp for schedule(dynamic, 200)
            for(int i = 0; i < N; i++)
            {
                x[i] -= lr*Ax[i];
            }
        }
    }
}

int main()
{
    int iter = 50;
    std::vector<int> threads_num = {2,4,6,8,16,20,40};

    for(const int threads : threads_num)
    {
        std::vector<double> time;

        for(int i=0;i<iter;i++)
        {

            std::vector<double> A(N*N);
            std::vector<double> b(N);
            std::vector<double> x(N, 0);
            std::vector<double> Ax(N);

            #pragma omp parallel num_threads(threads) for schedule(dynamic, 200)
            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    if(i==j) A[i*N+j] = 2;
                    else A[i*N+j] = 1;
                }
                b[i] = N+1;
            }

            const auto start{std::chrono::steady_clock::now()};

            simple_iteration(A,x,b,Ax,threads);
            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> dur{end-start};
            std::cout<<"time "<<dur.count()<<std::endl;

            time.push_back(dur.count());

        }

        double count=0;
        for(int i = 0; i < iter; i++)
            count+=time[i];

        std::cout<<"avg_time: "<<count/iter<<std::endl;
    }

    return 0;
}