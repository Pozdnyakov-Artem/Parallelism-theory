#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>

double E = 1e-10;
int N = 12000;
double lr = 0.9 * (2.0/(N+1));

void simple_iteration(std::vector<double> &A, std::vector<double> &x, std::vector<double> &b, int threads)
{
    std::vector<double> Ax(N);
    int max_threads = omp_get_max_threads();
    std::vector<double> norm_Ax(max_threads,0);
    bool status = false;
    double norm2 = 0;
    // for(int i = 0; i<N; i++)
    // {
    //     b[i] = N+1;
    //     norm2 += b[i]*b[i];
    // }

    #pragma omp parallel num_threads(threads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = N / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (N-1) : (lb + items_per_thread - 1);

        for(int i = lb; i <= ub; i++)
        {
            for(int j = lb; j <= ub; j++)
            {
                if(i==j) A[i*N+j] = 2;
                else A[i*N+j] = 1;
            }
            b[i] = N+1;
            norm2 += b[i]*b[i];
        }

        #pragma omp barrier

        while (!status)
        {
            for(int i = lb; i<=ub; i++)
            {
                double su = 0;
                for(int j = 0; j<N; j++)
                {
                    su+=A[i*N+j]*x[j];
                }
                Ax[i] = (su - b[i]);
            }
            #pragma omp barrier 

            double sum1 = 0;
            for(int i = lb; i<=ub; i++)
            {
                sum1+= Ax[i]*Ax[i];
            }
            norm_Ax[threadid] = sum1;

            #pragma omp barrier

            #pragma omp single
            {
                double norm1 = 0;
                for(int i = 0; i<nthreads; i++)
                {
                    norm1 += norm_Ax[i];
                }

                status = (norm1/norm2 < E);
            }
            #pragma omp barrier
            if(status)break;

            for(int i = lb; i <= ub; i++)
            {
                x[i] -= lr*Ax[i];
            }
            #pragma omp barrier
        }
    }
}

int main()
{
    std::vector<int> threads_num = {2,4,6,8,16,20,40};

    for(const int threads : threads_num)
    {
        std::vector<double> time;

        for(int i=0;i<100;i++)
        {
            const auto start{std::chrono::steady_clock::now()};
            // std::cout<<lr;
            std::vector<double> A(N*N);
            std::vector<double> b(N);
            std::vector<double> x(N, 0);

            simple_iteration(A,x,b,threads);
            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> dur{end-start};
            // std::cout<<"time: "<<dur.count()<<std::endl;

            time.push_back(dur.count());
            // for(int i = 0; i<5; i++)
            // {   
            //     std::cout<<x[i]<<std::endl;
            // }

        }

        double count=0;
        for(int i=0; i<4; i++)
            count+=time[i];

        std::cout<<"avg_time: "<<count/100<<std::endl;
    }

    return 0;
}