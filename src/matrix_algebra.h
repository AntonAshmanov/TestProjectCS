#ifndef MATRIX_ALGEBRA_H
#define MATRIX_ALGEBRA_H

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
            return expr1(i, j) * expr2(i, j);
        }
    }

    private: 
        const E1 expr1; 
        const OP op; 
        const E2 expr2; 
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