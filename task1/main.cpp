#include <iostream>
#include <cmath>
#include <string.h>
#include <vector>
#include <chrono>

template <typename T>
void do_arr()
{
    const auto start{std::chrono::steady_clock::now()};

    int n = pow(10,7);
    T count = T(0);
    std::vector<T> arr(n);

    for(int i = 0; i < n; i++)
    {
        arr[i] = sin(2 * M_PI * i / n);
        count += arr[i];
    }

    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> dur{end-start};
    std::cout<<count<<std::endl;
    std::cout<<dur.count();
}


int main(int argc, char * argv[]){

    if(argc != 2)
    {
        std::cout<<"incorrect number of arguments"<<std::endl;
        return 0;
    }
 
    if(!strcmp(argv[1], "float"))
    {
        do_arr<float>();
        return 0;
    }

    if(!strcmp(argv[1], "double"))
    {
        do_arr<double>();
        return 0;
    }

    std::cout<<"incorrect input"<<std::endl;
    return 0;
}