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
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {1, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0}
                          }
                        );                    
    
    diag_matrix<csr> C({
                            {1, 0, 0, 0, 0},
                            {0, 2, 0, 0, 0},
                            {0, 0, 3, 0, 0},
                            {0, 0, 0, 4, 0},
                            {0, 0, 0, 0, 5}
                          }
                        );                    
    //print_matrix(A, A.get_size());
    //print_matrix(2*A + B, A.get_size());
    //print_matrix(2*A - 2*A, A.get_size());
    //print_matrix(B + C, B.get_size());

    csr CA({
                            {1, 0, 0, 2, 0},
                            {3, 4, 0, 5, 0},
                            {6, 0, 7, 8, 9},
                            {0, 0, 10, 11, 0},
                            {0, 0, 0, 0, 12}
                          }
                        );

    csr CB(CA);

    for(auto el : CA.AA)
                        std::cout << el << " ";
    std::cout << std::endl;                    

    for(auto el : CB.AA)
                        std::cout << el << " ";
    std::cout << std::endl;                    

    for(auto el : CA.JA)
                        std::cout << el << " ";
    std::cout << std::endl;                    

    for(auto el : CB.JA)
                        std::cout << el << " ";
    std::cout << std::endl;                    

    for(auto el : CA.IA)
                        std::cout << el << " ";
    std::cout << std::endl;                    

    for(auto el : CB.IA)
                        std::cout << el << " ";
    std::cout << std::endl;                    

    general_matrix<csr> A2(A, [](size_t i, size_t j){ return j == i;});
    print_matrix(A2, A2.get_size());
    
    auto [D, E, F] = factorization_matrix(A);

    return 0;
}
