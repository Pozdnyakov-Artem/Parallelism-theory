#include <vector>
#include <cmath>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

auto parse_arguments(int argc, char* argv[]) {

    int grid_size, tolerance, max_iter;
    po::options_description desc("Heat Equation Solver - Parameters");
    desc.add_options()
        ("help,h", "Show this help message")
        ("size,s", po::value<int>(&grid_size)->default_value(1024),
         "Grid dimension N (creates N x N matrix)")
        ("tolerance,t", po::value<double>(&tolerance)->default_value(1e-6),
         "Convergence tolerance (epsilon)")
        ("max-iter,m", po::value<int>(&max_iter)->default_value(1000000),
         "Maximum number of iterations");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } 
    catch (const po::error& e) {
        std::cerr << "Command line error: " << e.what() << "\n" << desc << "\n";
        exit(EXIT_FAILURE);
    }

    // Обработка флага помощи
    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(EXIT_SUCCESS);
    }

    if (grid_size < 3) {
        std::cerr << "Error: Grid size must be >= 3 (to fit boundaries + interior).\n";
        exit(EXIT_FAILURE);
    }
    if (tolerance <= 0.0) {
        std::cerr << "Error: Tolerance must be > 0.\n";
        exit(EXIT_FAILURE);
    }
    if (max_iter <= 0) {
        std::cerr << "Error: Max iterations must be > 0.\n";
        exit(EXIT_FAILURE);
    }

    return grid_size, tolerance, max_iter;
}

int main() {

    int rows, max_iter, tol;
    int iter = 0;

    rows, tol, max_iter = parse_arguments(argc, argv);

    std::vector<double> A(rows*rows, 0);
    A[0] = 10; A[rows-1]=20; A[rows*rows-1] = 30; A[rows*(rows-1)-1] = 20;
    std::vector<double> Anew(rows*rows, 0);

    #pragma acc data copy(A[:rows*rows]) copyin(Anew[:rows*rows])
    while ( err > tol && iter < iter_max ) {
        err=0.0;

        #pragma acc parallel loop reduction(max:err) tile(32,32) present(A[0:rows*rows]) copy(Anew[0:rows*rows])
        for( int j = 1; j < rows-1; j++) {
            for(int i = 1; i < rows-1; i++) {
                Anew[j*rows+i] = 0.25 * (A[j*rows+i+1] + A[j*rows+i-1] + A[(j-1)*rows+i] + A[(j+1)*rows+i]);
                err = fmax(err, abs(Anew[j*rows+i] - A[j*rows+i]));
            }
        }

        #pragma acc parallel loop tile(32,32) present(Anew[0:rows*rows]) copyout(A[0:rows*rows])
        for( int j = 1; j < rows-1; j++) {
            for( int i = 1; i < rows-1; i++ ) {
                A[j*rows+i] = Anew[j*rows+i];
            }
        }
        iter++;
    }

    std::cout<<"Iter: "<<iter<<" err: "<<err<<std::endl;

    return 0;
}