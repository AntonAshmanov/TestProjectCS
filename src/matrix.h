#ifndef MATRIX_H
#define MATRIX_H
#include <functional>
#include <initializer_list>
#include "matrix_algebra_new.h"

struct diag_t{};
struct general_t{};

template<typename matrix_storage_t> 
class general_matrix;

template<template <typename> class E, typename matrix_storage_t>
class matrix : public expression<matrix<E, matrix_storage_t>>{
    //matrix_storage_t storage_;
    protected:
    size_t N;
    public:
        matrix_storage_t storage_;
        matrix() = default;
        matrix(std::initializer_list<std::initializer_list<double>> data) : storage_{data}, N{data.size()} {}
        matrix(const matrix_storage_t& other_storage, 
                std::function<bool(size_t, size_t)> func) 
                    : storage_{other_storage, func} {}
        
        E<matrix_storage_t>& self(){ 
            return static_cast<E<matrix_storage_t>&>(*this); 
        } 

        const E<matrix_storage_t>& self() const { 
            return static_cast<const E<matrix_storage_t>&>(*this); 
        }

        double operator()(size_t i, size_t j) const {
            return storage_(i, j);
        }

        double operator()(size_t i) const {
            return self()(i);
        }

        size_t get_size() const {
            return N;
        }

        const matrix_storage_t& get_storage() const {
            return storage_;
        }
};

template<typename matrix_storage_t> 
class diag_matrix : public matrix<diag_matrix, matrix_storage_t> {
    public:
        //diag_matrix() = default;
        /*diag_matrix(std::initializer_list<std::initializer_list<double>> data) : 
                    matrix<diag_matrix, matrix_storage_t>{std::move(data)} {}*/
        diag_matrix(const general_matrix<matrix_storage_t>& other_matrix) 
                    :  matrix<diag_matrix, matrix_storage_t>(other_matrix.get_storage(), [](size_t i, size_t j){
                        return i == j;
                    }) {
                        this->N = other_matrix.get_size();
                    }            
};

template<typename matrix_storage_t> 
class general_matrix : public matrix<general_matrix, matrix_storage_t> {
    public:
        general_matrix(std::initializer_list<std::initializer_list<double>> data) : 
                matrix<general_matrix, matrix_storage_t>(data) {}
        general_matrix(const general_matrix<matrix_storage_t>& other_matrix, 
                std::function<bool(size_t, size_t)> func = 
                [](size_t i, size_t j){return true;}) 
                    :  matrix<general_matrix, matrix_storage_t>(other_matrix.get_storage(), func) {
                        this->N = other_matrix.get_size();
                    }
};

template<typename matrix_storage_t> 
class sparse_vector : public matrix<sparse_vector, matrix_storage_t> {
    public:
        sparse_vector(std::initializer_list<double> data) : 
                    matrix<sparse_vector, matrix_storage_t>{std::move(data)} {
                        this->N = data.size();
                    }


        double operator()(size_t i) const {
            return this->storage_(0, i);
        }
        
        double& operator()(size_t i) {
            return this->storage_(0, i);
        }

        sparse_vector<matrix_storage_t>& operator=(sparse_vector<matrix_storage_t>& other) {
            for (size_t i = 0; i < other.get_size(); ++i) {
                this->storage_(0, i) = other(i);
            }

            return *this;
        }

        friend std::ostream& operator<< (std::ostream& out, const sparse_vector<matrix_storage_t>& vec) {
            for (size_t i = 0; i < vec.get_size(); ++i) {
                out << vec(i) << " ";
            }
            return out;
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



template<typename matrix_storage_t>
auto factorization_matrix(const general_matrix<matrix_storage_t>& A) {
    return std::make_tuple(
        diag_matrix<matrix_storage_t>(A),
        general_matrix<matrix_storage_t>(A, [](size_t i, size_t j){ return j > i;}),
        general_matrix<matrix_storage_t>(A, [](size_t i, size_t j){ return j < i;}));
}

#endif