#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <barrier>
#include <atomic>

int N = 20000;

void simple_iteration(const std::vector<double> &A, std::vector<double> &c,const std::vector<double> &b, int threads)
{

    std::vector<std::jthread> arr_threads(threads);
    for(int thread_id = 0; thread_id < threads; thread_id ++)
    {
        arr_threads[thread_id]=std::jthread([&,thread_id](std::stop_token){
            int items_per_thread = N / threads;
            int lb = thread_id * items_per_thread;
            int ub = (thread_id == threads - 1) ? (N-1) : (lb + items_per_thread - 1);

            for (int i = lb; i <= ub; i++)
            {
                double sum = 0.0;
                for (int j = 0; j < N; j++)
                    sum += A[i * N + j] * b[j];
                c[i] = sum;
            }            

        });
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

            std::vector<std::jthread> arr_of_threads(threads);

            std::vector<double> A(N*N);
            std::vector<double> b(N);
            std::vector<double> c(N, 0);

            for(int thread_id = 0; thread_id < threads; thread_id++)
            {
                arr_of_threads[thread_id] = std::jthread([&,thread_id](std::stop_token){
                    int items_per_thread = N / threads;
                    int lb = thread_id * items_per_thread;
                    int ub = (thread_id == threads - 1) ? (N-1) : (lb + items_per_thread - 1);

                    for (int i = lb; i <= ub; i++) {
                        for (int j = 0; j < N; j++)
                            A[i * N + j] = i + j;
                        b[i] = i;
                    }
                });
            }

            const auto start{std::chrono::steady_clock::now()};

            simple_iteration(A,c,b,threads);
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