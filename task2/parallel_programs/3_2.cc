#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>

double E = 1e-10;
int N = 12000;
double lr = 0.9 * (2.0/(N+1));

void simple_iteration(const std::vector<double> &A, std::vector<double> &x, const std::vector<double> &b, int threads, double norm_b)
{
    // double norm_b = 0;

    // #pragma omp parallel for reduction(+:norm_b) num_threads(threads)
    // for(int i = 0; i < N; i++)
    // {
    //     norm_b+=b[i]*b[i];
    // }

    std::vector<double> Ax_b(N);

    while (true)
    {   
        #pragma omp parallel for schedule(static) num_threads(threads)
        for(int i = 0; i<N; i++)
        {
            double su = 0;
            for(int j = 0; j<N; j++)
            {
                su+=A[i*N+j]*x[j];
            }
            Ax_b[i] = (su - b[i]);
        }

        double norm_Ax_b = 0;

        #pragma omp parallel for reduction(+:norm_Ax_b) num_threads(threads)
        for(int i = 0; i<N; i++)
        {
            norm_Ax_b+=Ax_b[i]*Ax_b[i];
        }
        if(norm_Ax_b/norm_b < E) break;

        #pragma omp parallel for schedule(static) num_threads(threads)
        for(int i = 0; i<N; i++)
            x[i] = x[i] - lr*Ax_b[i];

    }
}

int main()
{
    int iter = 25;
    std::vector<int> threads_num = {2,4,6,8,16,20,40};

    for(const int threads : threads_num)
    {
        std::vector<double> time;

        for(int h = 0; h < iter; h++)
        {
            const auto start{std::chrono::steady_clock::now()};

            std::vector<double> A(N*N);
            std::vector<double> b(N);
            std::vector<double> x(N);
            double norm_b = 0;

            #pragma omp parallel for schedule(static) num_threads(threads)

            for(int i = 0; i<N; i++)
            {
                for(int j = 0; j<N; j++)
                {
                    if(i==j) A[i*N+j] = 2;
                    else A[i*N+j] = 1;
                }
                b[i] = N+1;
                x[i] = 0;
                norm_b += b[i] * b[i];
            }

            simple_iteration(A,x,b, threads, norm_b);

            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> dur{end-start};
            std::cout<<"time "<<dur.count()<<std::endl;

            time.push_back(dur.count());

            // for(int i = 0; i<5; i++)
            // {   
            //     std::cout<<x[i]<<std::endl;
            // }

        }

        double count = 0;
        for(int i = 0; i<4; i++)
            count+=time[i];

        std::cout<<"threads_num: "<<threads<<" avg_time: "<<count/4<<std::endl;
    }


    return 0;
}