#ifndef MATRIX_H
#define MATRIX_H
#include <initializer_list>
#include "matrix_algebra.h"

struct diag_t{};
struct general_t{};

template<template <typename> class E, typename matrix_storage_t>
class matrix : public expression<matrix<E, matrix_storage_t>>{
    matrix_storage_t storage_;
    public:
        void D() {
            self().D();
        }
        
        E<matrix_storage_t>& self(){ 
            return static_cast<E<matrix_storage_t>&>(*this); 
        } 

        const E<matrix_storage_t>& self() const { 
            return static_cast<const E<matrix_storage_t>&>(*this); 
        }
};

template<typename matrix_storage_t> 
class diag_matrix : public matrix<diag_matrix, matrix_storage_t> {
    public:
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



#endif