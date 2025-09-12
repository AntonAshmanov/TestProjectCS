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
                            {1, 0, 0, 4, 0},
                            {0, 0, 0, 0, 5}
                          }
                        );                    
    print_matrix(A, A.get_size());
    print_matrix(2*A + B, A.get_size());
    print_matrix(2*A - 2*A, A.get_size());
    print_matrix(B + C, B.get_size());

    return 0;
}
