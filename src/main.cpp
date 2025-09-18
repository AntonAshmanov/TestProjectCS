#include <iostream>
#include <thread>
#include <vector>
#include "concurrent_queue.h"


#include "matrix_general.h"

int main() {

    const matrix_general A ({
                            {2, 1, 1},
                            {1, 3, -1},
                            {-1, 1, 2}
                          }
                        );

    const vector_general B({6, 0, 3});

    vector_general x ({0, 0, 0});
    vector_general x_new ({0, 0, 0});
    vector_general y ({11, 12, 15});


    auto [D, E, F] =factorization_edf(A);

    std::cout << D << std::endl;
    std::cout << E << std::endl;
    std::cout << F << std::endl;

    for (size_t i = 0; i < 20; ++i) {
      x_new = inv(D) * (-1*(E + F)*x + B);
      x = x_new;
    }    

    std::cout << x << std::endl;
    std::cout << A*x - B << std::endl;

    const diag_general d ({
                            {2, 0, 0},
                            {0, 3, 0},
                            {0, 0, 5}
                          }
                        );

    

    return 0;
}
