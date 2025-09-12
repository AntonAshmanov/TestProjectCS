#ifndef MATRIX_H
#define MATRIX_H
#include <initializer_list>
#include "matrix_algebra.h"

struct diag_t{};
struct general_t{};

template<template <typename> class E, typename matrix_storage_t>
class matrix : public expression<matrix<E, matrix_storage_t>>{
    //matrix_storage_t storage_;
    size_t N;
    public:
        matrix_storage_t storage_;
        matrix() = default;
        matrix(std::initializer_list<std::initializer_list<double>> data) : storage_{data}, N{data.size()} {}
        void D() {
            self().D();
        }
        
        E<matrix_storage_t>& self(){ 
            return static_cast<E<matrix_storage_t>&>(*this); 
        } 

        const E<matrix_storage_t>& self() const { 
            return static_cast<const E<matrix_storage_t>&>(*this); 
        }

        double operator()(size_t i, size_t j) const {
            return storage_(i, j);
        }

        size_t get_size() const {
            return N;
        }
};

template<typename matrix_storage_t> 
class diag_matrix : public matrix<diag_matrix, matrix_storage_t> {
    public:
        diag_matrix() = default;
        diag_matrix(std::initializer_list<std::initializer_list<double>> data) : 
                    matrix<diag_matrix, matrix_storage_t>{std::move(data)} {}
        void D() {
            std::cout << "diag matrix" << std::endl;
        }
};

template<typename matrix_storage_t> 
class general_matrix : public matrix<general_matrix, matrix_storage_t> {
    public:
        void D() {
            std::cout << "general matrix" << std::endl;
        }
};

template<template <typename> class E, typename matrix_storage_t>
struct get_storage_type {
    typedef matrix<E, matrix_storage_t>::matrix_storage_type matrix_storage_type;
};

template<typename T>
void print_matrix(const expression<T>& expr, size_t N) {
    std::cout << "======================= " << std::endl;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << expr(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

#endif