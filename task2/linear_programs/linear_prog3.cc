#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>

double E = 1e-10;
int N = 10000;
double lr = 0.9 * (2.0/(N+1));

void simple_iteration(const std::vector<double> &A, std::vector<double> &x, const std::vector<double> &b, const double norm_b)
{
    // double norm_b = 0;

    // for(int i = 0; i < N; i++)
    // {
    //     norm_b+=b[i]*b[i];
    // }

    std::vector<double> Ax_b(N);

    while (true)
    {
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
        for(int i = 0; i<N; i++)
        {
            norm_Ax_b+=Ax_b[i]*Ax_b[i];
        }
        if(norm_Ax_b/norm_b < E) break;

        for(int i = 0; i<N; i++)
            x[i] = x[i] - lr*Ax_b[i];

    }
}

int main()
{
    std::vector<double> time;

    for(int iter = 0; iter<25; iter++)
    {

        std::vector<double> A(N*N);
        std::vector<double> b(N);
        std::vector<double> x(N);
        double norm_b = 0;

        const auto start{std::chrono::steady_clock::now()};

        for(int i = 0; i<N; i++)
        {
            for(int j = 0; j<N; j++)
            {
                if(i==j) A[i*N+j] = 2;
                else A[i*N+j] = 1;
            }
            b[i] = N+1;
            norm_b+=b[i]*b[i];
            x[i] = 0;
        }

        simple_iteration(A,x,b, norm_b);

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

    std::cout<<"avg_time: "<<count/25<<std::endl;


    return 0;
}