#include <iostream>
#include <cmath>
#include <string.h>

template <typename T>
void do_arr()
{
    int n = pow(10,7);
    T count = T(0);
    T* arr = (T*)malloc(n * sizeof(T));

    for(int i = 0; i < n; i++)
    {
        arr[i] = sin(2 * M_PI * i / n);
        count += arr[i];
    }
    std::cout<<count;

    free(arr);
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