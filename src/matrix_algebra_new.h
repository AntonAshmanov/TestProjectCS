#ifndef MATRIX_ALGEBRA_NEW_H
#define MATRIX_ALGEBRA_NEW_H

template<template <typename> class E, typename matrix_storage_t>
class matrix;

template<typename matrix_storage_t> 
class diag_matrix;

template<typename matrix_storage_t> 
class sparse_vector;

template<class E> 
struct expression {

    double operator()(size_t i, size_t j) const {
        return static_cast<const E&>(*this)(i, j);
    }

    const E& self() const { 
        return static_cast<const E&>(*this); 
    } 
}; 

struct add{};
struct multi{};
struct min{};

template<class E1, class OP, class E2> 
struct binary_expression : 
expression<binary_expression<E1, OP, E2> > 
{ 
    binary_expression(const expression<E1> &expr1, 
                        const OP &op, const expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){}

    private: 
        const E1& expr1; 
        const OP op; 
        const E2& expr2; 
};

template<template <typename> class E1, template <typename> class E2, typename matrix_storage_t> 
struct binary_expression<matrix<diag_matrix, matrix_storage_t>, 
                        multi, 
                        binary_expression<matrix<E1, matrix_storage_t>, 
                        add, 
                        matrix<E2, matrix_storage_t>>> : 
expression<binary_expression<matrix<diag_matrix, matrix_storage_t>, 
                             multi, 
                             binary_expression<matrix<E1, matrix_storage_t>, 
                             add, 
                             matrix<E2, matrix_storage_t>>> > 
{ 
    binary_expression(const expression<matrix<diag_matrix, matrix_storage_t>> &expr1, 
                      const multi& op,
                      const expression<binary_expression<matrix<E1, matrix_storage_t>, 
                            add, 
                            matrix<E2, matrix_storage_t>>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}
    double operator()(size_t i, size_t j) const {
        double res = 0;
        return expr1(i, i) * expr2(i, j);
    }

    private: 
        const matrix<diag_matrix, matrix_storage_t>& expr1; 
        const multi op; 
        const binary_expression<matrix<E1, matrix_storage_t>, 
                                add, 
                                matrix<E2, matrix_storage_t>>& expr2; 
};

template<template <typename> class E1, template <typename> class E2, typename matrix_storage_t> 
struct binary_expression<matrix<E1, matrix_storage_t>, 
                        add, 
                        matrix<E2, matrix_storage_t>> : 
expression<binary_expression<matrix<E1, matrix_storage_t>, 
                             add, 
                             matrix<E2, matrix_storage_t>> > 
{ 
    binary_expression(const expression<matrix<E1, matrix_storage_t>> &expr1, 
                      const add& op,
                      const expression<matrix<E2, matrix_storage_t>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}
    double operator()(size_t i, size_t j) const {
        double res = 0;
        return expr1(i, j) + expr2(i, j);
    }

    private: 
        const matrix<E1, matrix_storage_t>& expr1; 
        const add op; 
        const matrix<E2, matrix_storage_t>& expr2; 
};

template<class E1, typename matrix_storage_t> 
struct binary_expression<E1, 
                        multi, 
                        matrix<sparse_vector, matrix_storage_t>> : 
expression<binary_expression<E1, 
                             multi, 
                             matrix<sparse_vector, matrix_storage_t>> > 
{ 
    binary_expression(const E1 &expr1, 
                      const multi& op,
                      const expression<matrix<sparse_vector, matrix_storage_t>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}
    double operator()(size_t i) const {
        double res = 0;

        for (size_t j = 0; j < expr2.get_size(); ++j) {
            res += expr1(i, j) * expr2(j) ;
        }
        
        return res;
    }

    private: 
        const E1& expr1; 
        const multi op; 
        const matrix<sparse_vector, matrix_storage_t>& expr2; 
};


template<template <typename> class E1, template <typename> class E2, typename matrix_storage_t> 
auto operator+(const matrix<E1, matrix_storage_t>& left, const matrix<E2, matrix_storage_t>& right) {
    add add_op;
    return binary_expression(left, add_op, right);
}

template<template <typename> class E1, template <typename> class E2, typename matrix_storage_t> 
auto operator*(const matrix<diag_matrix, matrix_storage_t>& left, 
               const binary_expression<matrix<E1, matrix_storage_t>, 
                             add, 
                             matrix<E2, matrix_storage_t>>& right) {
    multi multi_op;
    return binary_expression(left, multi_op, right);
}

template<class E1, class E2, class op, typename matrix_storage_t> 
auto operator*(const binary_expression<E1, 
                             op, 
                             E2>& left, 
               const matrix<sparse_vector, matrix_storage_t>& right) {
    multi multi_op;
    return binary_expression(left, multi_op, right);
}

template<typename matrix_storage_t> 
struct inv_expression : 
expression<inv_expression<matrix_storage_t>> 
{ 
    inv_expression(const expression<matrix<diag_matrix, matrix_storage_t>> &expr1) : expr1(expr1.self()) {}

    double operator()(size_t i, size_t j) const {
        return i == j ? 1.0 / expr1(i, j) : 0;
    }
    private: 
        const matrix<diag_matrix, matrix_storage_t>& expr1;
};

template<typename E1, typename matrix_storage_t> 
auto operator*(const expression<inv_expression<matrix_storage_t>>& left, 
               const E1& right) {
    multi multi_op;
    return binary_expression(left, multi_op, right);
}

template<typename matrix_storage_t> 
auto operator*(const expression<inv_expression<matrix_storage_t>>& left, 
               const matrix<sparse_vector, matrix_storage_t>& right) {
    multi multi_op;
    return binary_expression(left, multi_op, right);
}

template<typename matrix_storage_t> 
auto operator*(const expression<inv_expression<matrix_storage_t>>& left, 
               const expression<matrix<sparse_vector, matrix_storage_t>>& right) {
    multi multi_op;
    return binary_expression(left, multi_op, right);
}

template<class E1, typename matrix_storage_t> 
struct binary_expression<inv_expression<matrix_storage_t>, 
                        multi, 
                        E1> : 
expression<binary_expression<inv_expression<matrix_storage_t>, 
                             multi, 
                             E1 >> 
{ 
    binary_expression(const expression<inv_expression<matrix_storage_t>> &expr1, 
                      const multi& op,
                      const E1 &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}
    double operator()(size_t i, size_t j) const {
        return expr1(i, i) * expr2(i, j);
    }

    private: 
        const inv_expression<matrix_storage_t>& expr1; 
        const multi op; 
        const E1& expr2; 
};

template<typename matrix_storage_t> 
struct binary_expression<inv_expression<matrix_storage_t>, 
                        multi, 
                        matrix<sparse_vector, matrix_storage_t>> : 
expression<binary_expression<inv_expression<matrix_storage_t>, 
                             multi, 
                             matrix<sparse_vector, matrix_storage_t> >> 
{ 
    binary_expression(const expression<inv_expression<matrix_storage_t>> &expr1, 
                      const multi& op,
                      const expression<matrix<sparse_vector, matrix_storage_t>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}
    double operator()(size_t i) const {
        return expr1(i, i) * expr2(i);
    }

    private: 
        const inv_expression<matrix_storage_t>& expr1; 
        const multi op; 
        const matrix<sparse_vector, matrix_storage_t>& expr2; 
};


#endif