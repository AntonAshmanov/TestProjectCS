#include <iostream>
#include <thread>
#include <vector>
#include "concurrent_queue.h"
#include "matrix_algebra.h"
#include "matrix.h"

struct csr{};
struct vector_storage{};

template<template<typename> class E, typename storage_t>
void D(matrix<E, storage_t>& m) {
    std::cout << "call " << std::endl;
    m.D(); 
}

int main() {

    diag_matrix<csr> d;
    //D(d);

    general_matrix<csr> g;
    //D(g);

    auto madd = d + g;
    auto dm = 5 * d;

    return 0;
}
