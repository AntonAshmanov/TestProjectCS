#include <iostream>
#include <thread>
#include <vector>
#include "concurrent_queue.h"
#include "matrix_algebra.h"



template<typename matrix_type>
struct matrix : expression<matrix<matrix_type>> {};


int main() {

    matrix<void> A, B, C;

    auto add_op = A + B + C;
    auto add_op = A + B + C;


    return 0;
}
