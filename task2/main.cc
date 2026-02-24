#include <vector>
#include <iostream>
#include <cmath>

double E = std::pow(10, -5);
int N = 5;
float lr = 0.01;

std::vector<float> multiplication(std::vector<float> A, std::vector<float> b)
{
    std::vector<float> ans(N);

    for(int i = 0; i<N; i++)
    {
        float su = 0;
        for(int j = 0; j<N; j++)
        {
            su+=A[i*N+j]*b[i];
        }
        ans[i] = su;
    }

    return ans;
}

std::vector<float> raz_vec(std::vector<float> a, std::vector<float> b)
{
    std::vector<float> ans(N);
    for(int i = 0; i<N; i++)
    {
        ans[i] = a[i] - b[i];
    }

    return ans;
}

float norm(std::vector<float> a)
{
    float ans = 0;
    for(int i = 0; i<N; i++)
    {
        ans+= std::pow(a[i],2);
    }

    return std::sqrt(ans);
}

std::vector<float> scalyar_mult(std::vector<float> a, float lr)
{
    std::vector<float> ans(N);
    for(int i = 0; i<N; i++)
    {
        ans[i] = lr*a[i];
    }

    return ans;
}

bool check_func(std::vector<float> a, std::vector<float> b)
{
    float el = norm(a)/norm(b);
    if(el < E) return false;

    return true;
}

float prikol2(std::vector<float> a, std::vector<float> b)
{
    float ans = 0;

    for(int i = 0; i<N; i++)
    {
        ans+=a[i]*b[i];
    }

    return ans;
}

float prikol(std::vector<float> A, std::vector<float> y)
{
    std::vector<float> Ay = multiplication(A,y);
    
    return prikol2(y,Ay)/prikol2(Ay, Ay);
}

std::vector<float> sum_vec(std::vector<float> a, std::vector<float> b)
{
    std::vector<float> ans(N);

    for(int i = 0; i<N; i++)
    {
        ans[i] = a[i] + b[i];
    }

    return ans;
}

std::vector<float> simple_iteration(std::vector<float> A, std::vector<float> x, std::vector<float> b)
{
    std::vector<float> ans(N);
    auto pred_ans = scalyar_mult(raz_vec(multiplication(A,x),b),lr);
    for(int i = 0; i<N; i++)
    {
        ans[i] = x[i] - pred_ans[i];
    }

    return ans;
}

std::vector<float> method_minimal_inconsistencies(std::vector<float> A, std::vector<float> x, std::vector<float> b)
{
    std::vector<float> y = raz_vec(multiplication(A,x), b);
    float t = prikol(A,y);

    return raz_vec(x,scalyar_mult(y,t));
}

int main()
{
    std::vector<float> A(N*N);
    std::vector<float> b(N);
    std::vector<float> x(N);

    for(int i = 0; i<N; i++)
    {
        for(int j = 0; j<N; j++)
        {
            if(i==j) A[i*N+j] = 2;
            else A[i*N+j] = 1;
        }
        b[i] = N+1;
        x[i] = 0;
    }

    // while (check_func(raz_vec(multiplication(A,x),b),b))
    // {
        // x = simple_iteration(A,x,b);
        // x = method_minimal_inconsistencies(A,x,b);
    // }

    std::vector<float> r = raz_vec(b, multiplication(A,x));
    std::vector<float> z = r;
    std::vector<float> next_r;
    float a, beta;

    while(check_func(r,b))
    {
        a = prikol2(r,r)/prikol2(multiplication(A,z),z);
        x = sum_vec(x,scalyar_mult(z,a));
        next_r = raz_vec(r,scalyar_mult(multiplication(A,z),a));
        beta = prikol2(next_r,next_r)/prikol2(r,r);
        r = next_r;
        z = sum_vec(r, scalyar_mult(z, beta));
    }

    for(int i = 0; i<N; i++)
    {   
        std::cout<<x[i]<<std::endl;
        // for(int j = 0; j<N; j++)
        // {
        //     std::cout<<A[i*N+j]<<" ";
        // }
        // std::cout<<std::endl;
    }


    return 0;
}