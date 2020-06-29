#pragma once

#include "coek/coek_expr.hpp"
#include "coek/expr/varray.hpp"
#include "coek/coek_model.hpp"

//
// Inlined functions used to define reverse operators
//


namespace coek {

//
// numerical operators
//
// TODO - Rename these methods to make them class-specific
//

inline Constraint Variable_operator_eq(const Variable& arg, int rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, double rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const Parameter& rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const Variable& rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const Expression& rhs)
    {return arg == rhs;}

inline Expression Parameter_operator_pos(const Parameter& arg)
    {return +arg;}
inline Expression Variable_operator_pos(const Variable& arg)
    {return +arg;}
inline Expression Expression_operator_pos(const Expression& arg)
    {return +arg;}

inline Expression Parameter_operator_neg(const Parameter& arg)
    {return -arg;}
inline Expression Variable_operator_neg(const Variable& arg)
    {return -arg;}
inline Expression Expression_operator_neg(const Expression& arg)
    {return -arg;}

inline Expression Parameter_operator_radd(const Parameter& rhs, int lhs)
    {return lhs + rhs;}
inline Expression Variable_operator_radd(const Variable& rhs, int lhs)
    {return lhs + rhs;}
inline Expression Expression_operator_radd(const Expression& rhs, int lhs)
    {return lhs + rhs;}
inline Expression Parameter_operator_radd(const Parameter& rhs, double lhs)
    {return lhs + rhs;}
inline Expression Variable_operator_radd(const Variable& rhs, double lhs)
    {return lhs + rhs;}
inline Expression Expression_operator_radd(const Expression& rhs, double lhs)
    {return lhs + rhs;}

inline Expression Parameter_operator_rsub(const Parameter& rhs, int lhs)
    {return lhs - rhs;}
inline Expression Variable_operator_rsub(const Variable& rhs, int lhs)
    {return lhs - rhs;}
inline Expression Expression_operator_rsub(const Expression& rhs, int lhs)
    {return lhs - rhs;}
inline Expression Parameter_operator_rsub(const Parameter& rhs, double lhs)
    {return lhs - rhs;}
inline Expression Variable_operator_rsub(const Variable& rhs, double lhs)
    {return lhs - rhs;}
inline Expression Expression_operator_rsub(const Expression& rhs, double lhs)
    {return lhs - rhs;}

inline Expression Parameter_operator_rmul(const Parameter& rhs, int lhs)
    {return lhs * rhs;}
inline Expression Variable_operator_rmul(const Variable& rhs, int lhs)
    {return lhs * rhs;}
inline Expression Expression_operator_rmul(const Expression& rhs, int lhs)
    {return lhs * rhs;}
inline Expression Parameter_operator_rmul(const Parameter& rhs, double lhs)
    {return lhs * rhs;}
inline Expression Variable_operator_rmul(const Variable& rhs, double lhs)
    {return lhs * rhs;}
inline Expression Expression_operator_rmul(const Expression& rhs, double lhs)
    {return lhs * rhs;}

inline Expression Parameter_operator_rtruediv(const Parameter& rhs, int lhs)
    {return lhs / rhs;}
inline Expression Variable_operator_rtruediv(const Variable& rhs, int lhs)
    {return lhs / rhs;}
inline Expression Expression_operator_rtruediv(const Expression& rhs, int lhs)
    {return lhs / rhs;}
inline Expression Parameter_operator_rtruediv(const Parameter& rhs, double lhs)
    {return lhs / rhs;}
inline Expression Variable_operator_rtruediv(const Variable& rhs, double lhs)
    {return lhs / rhs;}
inline Expression Expression_operator_rtruediv(const Expression& rhs, double lhs)
    {return lhs / rhs;}

inline Expression Parameter_operator_truediv(const Parameter& lhs, int rhs)
    {return lhs / rhs;}
inline Expression Parameter_operator_truediv(const Parameter& lhs, double rhs)
    {return lhs / rhs;}
inline Expression Parameter_operator_truediv(const Parameter& lhs, const Parameter& rhs)
    {return lhs / rhs;}
inline Expression Parameter_operator_truediv(const Parameter& lhs, const Variable& rhs)
    {return lhs / rhs;}
inline Expression Parameter_operator_truediv(const Parameter& lhs, const Expression& rhs)
    {return lhs / rhs;}

inline Expression Variable_operator_truediv(const Variable& lhs, int rhs)
    {return lhs / rhs;}
inline Expression Variable_operator_truediv(const Variable& lhs, double rhs)
    {return lhs / rhs;}
inline Expression Variable_operator_truediv(const Variable& lhs, const Parameter& rhs)
    {return lhs / rhs;}
inline Expression Variable_operator_truediv(const Variable& lhs, const Variable& rhs)
    {return lhs / rhs;}
inline Expression Variable_operator_truediv(const Variable& lhs, const Expression& rhs)
    {return lhs / rhs;}

inline Expression Expression_operator_truediv(const Expression& lhs, int rhs)
    {return lhs / rhs;}
inline Expression Expression_operator_truediv(const Expression& lhs, double rhs)
    {return lhs / rhs;}
inline Expression Expression_operator_truediv(const Expression& lhs, const Parameter& rhs)
    {return lhs / rhs;}
inline Expression Expression_operator_truediv(const Expression& lhs, const Variable& rhs)
    {return lhs / rhs;}
inline Expression Expression_operator_truediv(const Expression& lhs, const Expression& rhs)
    {return lhs / rhs;}


inline Expression Parameter_pow(const Parameter& lhs, int rhs)
    {return pow(lhs,rhs);}
inline Expression Parameter_pow(const Parameter& lhs, double rhs)
    {return pow(lhs,rhs);}
inline Expression Parameter_pow(const Parameter& lhs, const Parameter& rhs)
    {return pow(lhs,rhs);}
inline Expression Parameter_pow(const Parameter& lhs, const Variable& rhs)
    {return pow(lhs,rhs);}
inline Expression Parameter_pow(const Parameter& lhs, const Expression& rhs)
    {return pow(lhs,rhs);}

inline Expression Variable_pow(const Variable& lhs, int rhs)
    {return pow(lhs,rhs);}
inline Expression Variable_pow(const Variable& lhs, double rhs)
    {return pow(lhs,rhs);}
inline Expression Variable_pow(const Variable& lhs, const Parameter& rhs)
    {return pow(lhs,rhs);}
inline Expression Variable_pow(const Variable& lhs, const Variable& rhs)
    {return pow(lhs,rhs);}
inline Expression Variable_pow(const Variable& lhs, const Expression& rhs)
    {return pow(lhs,rhs);}

inline Expression Expression_pow(const Expression& lhs, int rhs)
    {return pow(lhs,rhs);}
inline Expression Expression_pow(const Expression& lhs, double rhs)
    {return pow(lhs,rhs);}
inline Expression Expression_pow(const Expression& lhs, const Parameter& rhs)
    {return pow(lhs,rhs);}
inline Expression Expression_pow(const Expression& lhs, const Variable& rhs)
    {return pow(lhs,rhs);}
inline Expression Expression_pow(const Expression& lhs, const Expression& rhs)
    {return pow(lhs,rhs);}

inline Expression Parameter_rpow(const Parameter& rhs, int lhs)
    {return pow(lhs,rhs);}
inline Expression Parameter_rpow(const Parameter& rhs, double lhs)
    {return pow(lhs,rhs);}
inline Expression Variable_rpow(const Variable& rhs, int lhs)
    {return pow(lhs,rhs);}
inline Expression Variable_rpow(const Variable& rhs, double lhs)
    {return pow(lhs,rhs);}
inline Expression Expression_rpow(const Expression& rhs, int lhs)
    {return pow(lhs,rhs);}
inline Expression Expression_rpow(const Expression& rhs, double lhs)
    {return pow(lhs,rhs);}

}
