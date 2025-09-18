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
    vector_general w ({5, 6, 7});


    auto [D, E, F] =factorization_edf(A);

    size_t iter = 0;
    const double epsilon = 0.0001;
    
    while(true) {
      x_new = inv(D) * (-1*(E + F)*x + B);
      if (norma(x_new - x) < epsilon)
        break;
      x = x_new;
      ++iter;
    }    

    std::cout << "number iteration: " << iter << std::endl;
    std::cout << x << std::endl;
    std::cout << A*x - B << std::endl;


    //examples of matrix algebra
    
    std::cout << y << std::endl;
    std::cout << w + 2*y << std::endl;
    std::cout << 2*A*w + y << std::endl;
    
    

    return 0;
}
