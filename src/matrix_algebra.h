#ifndef MATRIX_ALGEBRA_H
#define MATRIX_ALGEBRA_H

template<template <typename> class E, typename matrix_storage_t>
class matrix;

template<typename matrix_storage_t> 
class sparse_vector;

template<class O> 
struct math_object_base { 
    O& self(){ 
        return static_cast<O&>(*this); 
    } 
    const O& self() const { 
        return static_cast<const O&>(*this); 
    } 
};

template<class E> 
struct expression : math_object_base<E>{
    double operator()(size_t i, size_t j) {
        return this->self()(i, j);
    }

    double operator()(size_t i, size_t j) const {
        return this->self()(i, j);
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

    double operator()(size_t i, size_t j) const {
        if constexpr (std::is_same<OP, add>::value) {
            return expr1(i, j) + expr2(i, j);
        } else if (std::is_same<OP, multi>::value) {
            std::cout << "general " << std::endl;
            return expr1(i, j) * expr2(i, j) + 5;
        } else if (std::is_same<OP, min>::value) {
            return expr1(i, j) - expr2(i, j);
        }
    }

    auto get_left() const {
        return expr1;
    }

    auto get_right() const {
        return expr2;
    }

    const OP get_op() const {
        return op;
    }

    private: 
        const E1& expr1; 
        const OP op; 
        const E2& expr2; 
};

template<template <typename> class E1, template <typename> class E2, typename matrix_storage_t> 
struct binary_expression<matrix<E1, matrix_storage_t>, 
                        multi, 
                        matrix<E2, matrix_storage_t>> : 
expression<binary_expression<matrix<E1, matrix_storage_t>, 
                             multi, 
                             matrix<E2, matrix_storage_t>> > 
{ 
    binary_expression(const expression<matrix<E1, matrix_storage_t>> &expr1, 
                      const multi& op,
                      const expression<matrix<E2, matrix_storage_t>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}

    private: 
        const matrix<E1, matrix_storage_t>& expr1; 
        const multi op; 
        const matrix<E2, matrix_storage_t>& expr2; 
};

template<template <typename> class E1, typename matrix_storage_t> 
struct binary_expression<matrix<E1, matrix_storage_t>, 
                        multi, 
                        matrix<sparse_vector, matrix_storage_t>> : 
expression<binary_expression<matrix<E1, matrix_storage_t>, 
                             multi, 
                             matrix<sparse_vector, matrix_storage_t>> > 
{ 
    binary_expression(const expression<matrix<E1, matrix_storage_t>> &expr1, 
                      const multi& op,
                      const expression<matrix<sparse_vector, matrix_storage_t>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}

    double operator()(size_t i) const {

        
        double res = 0;
        for (size_t col = 0; col < expr1.get_size(); ++col) {
            res += expr1(i, col) * expr2(col);
        }

        return res;
    }

    private: 
        const matrix<E1, matrix_storage_t>& expr1; 
        const multi op; 
        const matrix<sparse_vector, matrix_storage_t>& expr2; 
};

template<template <typename> class E1, template <typename> class E2, typename bin_op, typename matrix_storage_t> 
struct binary_expression<binary_expression<matrix<E1, matrix_storage_t>, 
                        bin_op, 
                        matrix<E2, matrix_storage_t>>, 
                        multi, 
                        matrix<sparse_vector, matrix_storage_t>> : 
expression<binary_expression<binary_expression<matrix<E1, matrix_storage_t>, 
                        bin_op, 
                        matrix<E2, matrix_storage_t>>, 
                             multi, 
                             matrix<sparse_vector, matrix_storage_t>> > 
{ 
    binary_expression(const expression<binary_expression<matrix<E1, matrix_storage_t>, 
                        bin_op, 
                        matrix<E2, matrix_storage_t>>> &expr1, 
                      const multi& op,
                      const expression<matrix<sparse_vector, matrix_storage_t>> &expr2) : 
                    expr1(expr1.self()), op{op}, expr2(expr2.self()){}

    double operator()(size_t i) const {
         if constexpr (std::is_same<decltype(expr1.get_op()), const add>::value) {
            return (expr1.get_left() * expr2)(i) + (expr1.get_right() * expr2)(i);
        } else if (std::is_same<decltype(expr1.get_op()), const multi>::value) {
            return (expr1.get_left() * expr2)(i) * (expr1.get_right() * expr2)(i);
        } else if (std::is_same<decltype(expr1.get_op()), min>::value) {
            return (expr1.get_left() * expr2)(i) - (expr1.get_right() * expr2)(i);
        }
    }

    private: 
        const binary_expression<
                        matrix<E1, matrix_storage_t>, 
                        bin_op, 
                        matrix<E2, matrix_storage_t>>& expr1; 
        const multi op; 
        const matrix<sparse_vector, matrix_storage_t>& expr2; 
};



struct constant : expression<constant> 
{ 
    constant(double value) : value(value){} 
    double operator()(double x) const { 
        return value;
    } 

    double operator()(size_t i, size_t j) const {
        return value;
    }

    private: 
    double value; 
};

template<typename type_left, typename type_right>
auto operator+(const expression<type_left>& left, const expression<type_right>& right) {
    add add_op;
    return binary_expression(left, add_op, right);
}

template<typename type_left, typename type_right>
auto operator-(const expression<type_left>& left, const expression<type_right>& right) {
    min min_op;
    return binary_expression(left, min_op, right);
} 

template<typename type_right>
auto operator*(double left, const expression<type_right>& right) {
    multi multi_op;
    return binary_expression(constant(left), multi_op, right);
} 

template<typename type_left, typename type_right>
auto operator*(const expression<type_left>& left, const expression<type_right>& right) {
    multi multi_op;
    return binary_expression(left, multi_op, right);
} 

#endif