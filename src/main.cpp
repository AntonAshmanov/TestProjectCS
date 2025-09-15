#include <iostream>
#include <thread>
#include <vector>
#include "concurrent_queue.h"
#include "matrix_algebra_new.h"
#include "matrix.h"
#include "csr.h"

int main() {

    general_matrix<csr> A({
                            {2, 1, 1},
                            {1, 3, -1},
                            {-1, 1, 2}
                          }
                        );                            

    sparse_vector<csr> B({6, 0, 3});
    sparse_vector<csr> X({1, 1, 1});
    sparse_vector<csr> X_new({1, 1, 1});

    auto [D, E, F] = factorization_matrix(A);

    for (size_t i = 0; i < X.get_size(); ++i) {
      std::cout << -(inv_expression(D) * (E + F) * X)(i) + (inv_expression(D) * B)(i) << " ";
    }

    return 0;
}
