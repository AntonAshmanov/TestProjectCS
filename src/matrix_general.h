#ifndef MATRIX_GENERAL_H
#define MATRIX_GENERAL_H

#include <vector>
#include <initializer_list>
#include "matrix_general_algebra.h"
#include <cmath>
#include <functional>

template<typename T>
struct base {
    const T& self() const {
        return static_cast<const T&>(*this);
    }

    T& self() {
        return static_cast<T&>(*this);
    }
};

template<typename T>
struct matrix_expression {
    const T& self() const {
        return static_cast<const T&>(*this);
    }

    double operator()(size_t i, size_t j) const {
        return self()(i, j);
    }

    size_t size() const {
        return self().size();
    }
};

template<typename T>
struct vector_expression {
    const T& self() const {
        return static_cast<const T&>(*this);
    }

    double operator()(size_t i) const {
        return self()(i);
    }

    size_t size() const {
        return self().size();
    }
};

struct add{};
struct mult{};
struct minus{};

struct constant_m : matrix_expression<constant_m> {
    constant_m(double val) : val_{val} {
    }

    double operator()() const {
        return val_;
    }
    double val_;
};

struct constant_v : vector_expression<constant_v> {
    constant_v(double val) : val_{val} {
    }

    double operator()() const {
        return val_;
    }
    double val_;
};

class matrix_general : public matrix_expression<matrix_general> {
    std::vector<std::vector<double>> data_;
    size_t N;
    public:
    matrix_general(std::initializer_list<std::initializer_list<double>> data) : N{data.size()} {
        for (auto el : data) {
            data_.push_back(el);
        }
    }
    
    template<typename T>
    matrix_general(const matrix_expression<T>& A, std::function<bool(size_t, size_t)> func): N{A.size()} {
        for (size_t i = 0; i < N; ++i) {
            data_.push_back(std::vector<double>(N, 0));
            for (size_t j = 0; j < N; ++j) {
                data_[i][j] = func(i, j) ? A(i, j) : 0;
            }
        }
    }
    
    double operator()(size_t i, size_t j) const {
        return data_.at(i).at(j);
    }

    double operator()(size_t i, size_t j) {
        return data_[i][j];
    }

    size_t size() const {
        return N;
    }
};

class diag_general : public matrix_expression<diag_general> {
    std::vector<std::vector<double>> data_;
    size_t N;
    public:
    diag_general(std::initializer_list<std::initializer_list<double>> data) : N{data.size()} {
        for (auto el : data) {
            data_.push_back(el);
        }
    }
    
    template<typename T>
    diag_general(const matrix_expression<T>& A): N{A.size()} {
        for (size_t i = 0; i < N; ++i) {
            data_.push_back(std::vector<double>(N, 0));
            data_[i][i] = A(i, i);
        }
    }
    
    double operator()(size_t i, size_t j) const {
        return data_.at(i).at(j);
    }

    double& operator()(size_t i, size_t j) {
        return data_[i][j];
    }

    size_t size() const {
        return N;
    }
};

class vector_general : public vector_expression<vector_general> {
    std::vector<double> data_;
    size_t N;
    public:
    vector_general(std::initializer_list<double> data) : N{data.size()}, data_{data} {
    } 
    
    double operator()(size_t j) const {
        return data_.at(j);
    }

    double& operator()(size_t j) {
        return data_[j];
    }

    size_t size() const {
        return N;
    }

    template<typename T>
    vector_general& operator=(const vector_expression<T>& expr) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = expr(i);
        }
        return *this;
    }
};

template<typename E1, class OP, typename E2> 
struct binary_expression_matrix_matrix : 
matrix_expression<binary_expression_matrix_matrix<E1, OP, E2> > {
     binary_expression_matrix_matrix(const matrix_expression<E1> &expr1, 
                        const OP &op, const matrix_expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){}

    double operator()(size_t i, size_t j) const {
        double res = 0;
        if constexpr (std::is_same<OP, add>::value) {
            res = expr1(i, j) + expr2(i, j); 
        } else if (std::is_same<OP, mult>::value) {
            for (size_t col = 0; col < expr2.size(); ++col) {
                res += expr1(i, col) * expr2(col, j);
            }
        } 
        return res;  
    }

    size_t size() const {
        return expr2.size();
    }

    private: 
        const E1 expr1; 
        const OP op; 
        const E2 expr2; 
};

template<class OP, typename E2> 
struct binary_expression_matrix_matrix<constant_m, OP, E2> : 
matrix_expression<binary_expression_matrix_matrix<constant_m, OP, E2> > {
     binary_expression_matrix_matrix(const matrix_expression<constant_m> &expr1, 
                        const OP &op, const matrix_expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){           }

    double operator()(size_t i, size_t j) const {  
        return expr1() * expr2(i, j);  
    }

    size_t size() const {
        return expr2.size();
    }

    private: 
        const constant_m expr1; 
        const OP op; 
        const E2 expr2; 
};

template<class OP, typename E2> 
struct binary_expression_matrix_matrix<diag_general, OP, E2>: 
matrix_expression<binary_expression_matrix_matrix<diag_general, OP, E2> > {
     binary_expression_matrix_matrix(const matrix_expression<diag_general>& expr1, 
                        const OP &op, const matrix_expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){}

    double operator()(size_t i, size_t j) const {
        double res = 0;
        if constexpr (std::is_same<OP, mult>::value) {
            res = expr1(i, i) * expr2(i, j); 
        } else {
            res = 1;
        }
        return res;   
    }

    size_t size() const {
        return expr2.size();
    }

    private: 
        const diag_general expr1; 
        const OP op; 
        const E2 expr2; 
};

template<typename E1, class OP, typename E2> 
struct binary_expression_matrix_vector : 
vector_expression<binary_expression_matrix_vector<E1, OP, E2> > {
     binary_expression_matrix_vector(const matrix_expression<E1> &expr1, 
                        const OP &op, const vector_expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){}

    double operator()(size_t i) const {
        double res = 0;
        if constexpr (std::is_same<OP, mult>::value) {
            for (size_t col = 0; col < expr2.size(); ++col) {
                res += expr1(i, col) * expr2(col);
            }
        } 
        return res;  
    }

    size_t size() const {
        return expr2.size();
    }

    private: 
        const E1 expr1; 
        const OP op; 
        const E2 expr2; 
};

template<typename E1, class OP, typename E2> 
struct binary_expression_vector_vector : 
vector_expression<binary_expression_vector_vector<E1, OP, E2> > {
     binary_expression_vector_vector(const vector_expression<E1> &expr1, 
                        const OP &op, const vector_expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){}

    double operator()(size_t i) const {
        double res = 0;
        if constexpr (std::is_same<OP, add>::value) {
            res = expr1(i) + expr2(i);
        } else if (std::is_same<OP, minus>::value) {
            res = expr1(i) - expr2(i);
        }
        return res;  
    }

    size_t size() const {
        return expr2.size();
    }

    private: 
        const E1 expr1; 
        const OP op; 
        const E2 expr2; 
};

template<class OP, typename E2> 
struct binary_expression_vector_vector<constant_v, OP, E2> : 
vector_expression<binary_expression_vector_vector<constant_v, OP, E2> > {
     binary_expression_vector_vector(const vector_expression<constant_v> &expr1, 
                        const OP &op, const vector_expression<E2> &expr2) : 
                    expr1(expr1.self()), op(op), expr2(expr2.self()){}

    double operator()(size_t i) const {
        double res = 0;
        if constexpr (std::is_same<OP, mult>::value) {
            res = expr1() * expr2(i);
        } 
        return res;  
    }

    size_t size() const {
        return expr2.size();
    }

    private: 
        const constant_v expr1; 
        const OP op; 
        const E2 expr2; 
};


template<typename E1, typename E2>
auto operator+(const matrix_expression<E1>& m1, const matrix_expression<E2>& m2) {
    return binary_expression_matrix_matrix(m1, add{}, m2);
}

template<typename E1, typename E2>
auto operator*(const matrix_expression<E1>& m1, const matrix_expression<E2>& m2) {
    return binary_expression_matrix_matrix(m1, mult{}, m2);
}

template<typename E2>
auto operator*(const matrix_expression<diag_general>& m1, const matrix_expression<E2>& m2) {
    return binary_expression_matrix_matrix<diag_general, mult, E2>(m1, mult{}, m2);
}

template<typename E2>
auto operator*(double val, const matrix_expression<E2>& m2) {
    const constant_m c(val);
    return binary_expression_matrix_matrix(c, mult{}, m2);
}

template<typename E2>
auto operator*(double val, const vector_expression<E2>& m2) {
    const constant_v c(val);
    return binary_expression_vector_vector(c, mult{}, m2);
}

template<typename E1, typename E2>
auto operator*(const matrix_expression<E1>& m1, const vector_expression<E2>& m2) {
    return binary_expression_matrix_vector(m1, mult{}, m2);
}

template<typename E1, typename E2>
auto operator+(const vector_expression<E1>& m1, const vector_expression<E2>& m2) {
    return binary_expression_vector_vector(m1, add{}, m2);
}

template<typename E1, typename E2>
auto operator-(const vector_expression<E1>& m1, const vector_expression<E2>& m2) {
    return binary_expression_vector_vector(m1, minus{}, m2);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const matrix_expression<T>& m) {
    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m.size(); ++j) {
            std::cout << m(i, j) << " ";
        }
        std::cout << std::endl;
    }
    return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const vector_expression<T>& m) {
    for (size_t j = 0; j < m.size(); ++j) 
        std::cout << m(j) << " ";
    std::cout << std::endl;
    return out;
}

template<typename T>
double norma(const vector_expression<T>& expr) {
    double res = 0;
    for (size_t i = 0; i < expr.size(); ++i) {
        res += std::abs(expr(i) * expr(i));
    }
    return std::sqrt(res);
}

template<typename T>
auto factorization_edf(const matrix_expression<T>& A) {
    return std::make_tuple(
        diag_general(A),
        matrix_general(A, [](size_t i, size_t j){ return j > i;}),
        matrix_general(A, [](size_t i, size_t j){ return j < i;}));
}

struct inv : matrix_expression<inv> {
    const diag_general& expr;
    inv(const diag_general& expr) : expr{expr} {} 
    double operator()(size_t i, size_t j) const {
        return i == j ? 1 / expr(i, i) : 0;
    } 

    size_t size() const {
        return expr.size();
    }
};

#endif