#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct Config {
    int grid_size;
    double tolerance;
    int max_iter;
};

Config parse_arguments(int argc, char* argv[]) {
    Config cfg{1024, 1e-6, 1000000};
    
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

    double err = 1.0;
    int iter = 0;

    // Выделяем векторы
    std::vector<double> A(rows * rows, 0.0);
    std::vector<double> Anew(rows * rows, 0.0);

    // ✅ Получаем сырые указатели для OpenACC
    double* __restrict__ A_ptr = A.data();
    double* __restrict__ Anew_ptr = Anew.data();

    // Граничные условия
    for (int j = 0; j < rows; ++j) {
        double coeff = static_cast<double>(j) / static_cast<double>(rows - 1);
        A_ptr[j * rows + 0] = 10.0 + (20.0 - 10.0) * coeff;
        A_ptr[j * rows + rows - 1] = 20.0 + (30.0 - 20.0) * coeff;
    }
    for (int i = 0; i < rows; ++i) {
        double coeff = static_cast<double>(i) / static_cast<double>(rows - 1);
        A_ptr[0 * rows + i] = 10.0 + (20.0 - 10.0) * coeff;
        A_ptr[(rows - 1) * rows + i] = 20.0 + (30.0 - 20.0) * coeff;
    }

    const auto start{std::chrono::steady_clock::now()};

    // ✅ OpenACC регион с сырыми указателями
    #pragma acc data copy(A_ptr[:rows*rows]) copy(Anew_ptr[:rows*rows])
    {
        while (err > tol && iter < max_iter) {
            err = 0.0;

            // ✅ Вычисления: используем A_ptr, Anew_ptr
            #pragma acc parallel loop reduction(max:err) present(A_ptr, Anew_ptr)
            for (int j = 1; j < rows - 1; ++j) {
                for (int i = 1; i < rows - 1; ++i) {
                    int idx = j * rows + i;
                    Anew_ptr[idx] = 0.25 * (A_ptr[idx + 1] + A_ptr[idx - 1] + 
                                            A_ptr[idx - rows] + A_ptr[idx + rows]);
                    err = fmax(err, fabs(Anew_ptr[idx] - A_ptr[idx]));
                }
            }

            // ✅ Обновление
            #pragma acc parallel loop present(A_ptr, Anew_ptr)
            for (int j = 1; j < rows - 1; ++j) {
                for (int i = 1; i < rows - 1; ++i) {
                    A_ptr[j * rows + i] = Anew_ptr[j * rows + i];
                }
            }
            ++iter;
        }
    }

    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> dur{end - start};
    std::cout << "Iter: " << iter << " err: " << err << " time " << dur.count() << std::endl;

    // Вывод для отчёта
    if (rows == 10 || rows == 13) {
        std::cout << "\nFinal grid (" << rows << "x" << rows << "):\n";
        for (int j = 0; j < rows; ++j) {
            for (int i = 0; i < rows; ++i) {
                std::cout << A_ptr[j * rows + i] << "\t";
            }
            std::cout << "\n";
        }
    }

    return 0;
}