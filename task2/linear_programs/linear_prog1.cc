#include <iostream>
#include <chrono>
#include <vector>

int n = 20000;

int main()
{
    std::vector<double> time;
    for(int h = 0 ; h<100; h++)
    {

        const auto start{std::chrono::steady_clock::now()};

        std::vector<double> a(n*n);
        std::vector<double> b(n);
        std::vector<double> c(n);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                a[i * n + j] = i + j;
            b[i] = i;
        }

        // for (int j = 0; j < n; j++)
        //     b[j] = j;
        

        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++)
                sum += a[i * n + j] * b[j];
            c[i] = sum;
        }

        const auto end{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> dur{end-start};
        std::cout<<"time: "<<dur.count()<<std::endl;

        time.push_back(dur.count());

        double count = 0;

        for(int i = 0; i < n; i++)
        {
            count+=c[i];
        }

        std::cout<<count<<std::endl;

    }

    double count = 0;

    for(auto i : time)
        count+=i;

    std::cout<<"avg_time: "<<count/100;


}