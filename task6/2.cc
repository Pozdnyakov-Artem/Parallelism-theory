#include <vector>
#include <cmath>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct Config {
    int grid_size;
    double tolerance;
    int max_iter;
};

Config parse_arguments(int argc, char* argv[]) {
    Config cfg{1024, 1e-6, 1000000}; // Значения по умолчанию
    
    po::options_description desc("Heat Equation Solver");
    desc.add_options()
        ("help,h", "Show help")
        ("size,s", po::value<int>(&cfg.grid_size), "Grid size N")
        ("tolerance,t", po::value<double>(&cfg.tolerance), "Convergence tolerance")
        ("max-iter,m", po::value<int>(&cfg.max_iter), "Max iterations");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\n" << desc << "\n";
        exit(EXIT_FAILURE);
    }
    
    if (vm.count("help")) { std::cout << desc << "\n"; exit(EXIT_SUCCESS); }
    if (cfg.grid_size < 3) { std::cerr << "Grid size must be >= 3\n"; exit(EXIT_FAILURE); }
    
    return cfg;
}

int main(int argc, char* argv[]) {

    Config cfg = parse_arguments(argc, argv);
    const int rows = cfg.grid_size;
    const double tol = cfg.tolerance;
    const int max_iter = cfg.max_iter;

    double err = 1;
    int iter = 0;

    std::vector<double> A(rows*rows, 0);
    std::vector<double> Anew(rows*rows, 0);

    for (int j = 0; j < rows; j++) {
        A[j*rows + 0] = 10.0 + (20.0 - 10.0) * static_cast<double>(j) / (rows - 1);
        A[j*rows + rows-1] = 20.0 + (30.0 - 20.0) * static_cast<double>(j) / (rows - 1);
    }
    for (int i = 0; i < rows; i++) {
        A[0*rows + i] = 10.0 + (20.0 - 10.0) * static_cast<double>(i) / (rows - 1);
        A[(rows-1)*rows + i] = 20.0 + (30.0 - 20.0) * static_cast<double>(i) / (rows - 1);
    }

    #pragma acc data copy(A[:rows*rows]) create(Anew[:rows*rows]) 
    {
        while ( err > tol && iter < max_iter ) {
            err=0.0;

            #pragma acc parallel loop collapse(2) vector reduction(max:err) present(A, Anew)
            for( int j = 1; j < rows-1; j++) {
                for(int i = 1; i < rows-1; i++) {
                    Anew[j*rows+i] = 0.25 * (A[j*rows+i+1] + A[j*rows+i-1] + A[(j-1)*rows+i] + A[(j+1)*rows+i]);
                    err = fmax(err, fabs(Anew[j*rows+i] - A[j*rows+i]));
                }
            }

            #pragma acc parallel loop collapse(2) vector present(Anew, A)
            for( int j = 1; j < rows-1; j++) {
                for( int i = 1; i < rows-1; i++ ) {
                    A[j*rows+i] = Anew[j*rows+i];
                }
            }
            iter++;
        }
    }

    std::cout<<"Iter: "<<iter<<" err: "<<err<<std::endl;

    if (rows == 10 || rows == 13) {
        for (int j = 0; j < rows; j++) {
            for (int i = 0; i < rows; i++) {
                std::cout << A[j*rows + i] << "\t";
            }
            std::cout << "\n";
        }
    }

    return 0;
}