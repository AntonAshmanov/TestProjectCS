#include <iostream>
#include <thread>
#include <vector>
#include "concurrent_queue.h"
#include "matrix_algebra.h"
#include "matrix.h"
#include "csr.h"

int main() {

    diag_matrix<csr> d({
                            {1, 2, 3, 4},
                            {5, 6, 7, 8},
                            {9, 10, 11, 12},
                            {13, 14, 15, 16}
                          }
                        );
    //D(d);

    general_matrix<csr> g({
                            {1, 2, 3, 4},
                            {5, 6, 7, 8},
                            {9, 10, 11, 12},
                            {13, 14, 15, 16}
                          }
                        );
    auto aa = g.storage_.get_aa();                    
    //D(g);

    auto madd = 5 * d + 5 * d;
    //auto dm = 5 * d;

    std::cout << madd(2, 3) << std::endl;

    //std::cout << std::is_same<typename get_storage_type<diag_matrix>::matrix_storage_type 

    return 0;
}
