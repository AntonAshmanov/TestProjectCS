#include <iostream>
#include <thread>
#include <vector>
#include "concurrent_queue.h"
#include "matrix_algebra.h"
#include "matrix.h"
#include "csr.h"

int main() {

    general_matrix<csr> A({
                            {1, 0, 0, 2, 0},
                            {3, 4, 0, 5, 0},
                            {6, 0, 7, 8, 9},
                            {0, 0, 10, 11, 0},
                            {0, 0, 0, 0, 12}
                          }
                        );

    general_matrix<csr> B({
                            {1, 0, 0, 4, 0},
                            {3, 4, 0, 5, 0},
                            {6, 0, 7, 8, 9},
                            {0, 0, 10, 11, 0},
                            {0, 0, 0, 0, 12}
                          }
                        );                            

    auto [D, E, F] = factorization_matrix(A);

    //print_matrix(A, A.get_size());
    //print_matrix(D, D.get_size());
    //print_matrix(E, E.get_size());
    //print_matrix(F, F.get_size());

    sparse_vector<csr> AV({1, 0, 0, 2, 0});

    auto expr = (A + B) * AV;

    std::cout << expr(0) << std::endl;
    std::cout << expr(1) << std::endl;

    return 0;
}
