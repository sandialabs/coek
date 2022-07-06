#pragma once

#include <coek/coek.hpp>
//#include <coek/coek_small.hpp>

//#include <coek/ast/varray.hpp>

//
// Inlined functions used to define reverse operators
//


namespace coek {

//
// numerical operators
//

// LT

inline Constraint Parameter_operator_lt(const Parameter& arg, int rhs)
    {return arg < rhs;}
inline Constraint Parameter_operator_lt(const Parameter& arg, double rhs)
    {return arg < rhs;}
inline Constraint Parameter_operator_lt(const Parameter& arg, const Parameter& rhs)
    {return arg < rhs;}
inline Constraint Parameter_operator_lt(const Parameter& arg, const IndexParameter& rhs)
    {return arg < rhs;}
inline Constraint Parameter_operator_lt(const Parameter& arg, const Variable& rhs)
    {return arg < rhs;}
inline Constraint Parameter_operator_lt(const Parameter& arg, const Expression& rhs)
    {return arg < rhs;}

inline Constraint IndexParameter_operator_lt(const IndexParameter& arg, int rhs)
    {return arg < rhs;}
inline Constraint IndexParameter_operator_lt(const IndexParameter& arg, double rhs)
    {return arg < rhs;}
inline Constraint IndexParameter_operator_lt(const IndexParameter& arg, const Parameter& rhs)
    {return arg < rhs;}
inline Constraint IndexParameter_operator_lt(const IndexParameter& arg, const IndexParameter& rhs)
    {return arg < rhs;}
inline Constraint IndexParameter_operator_lt(const IndexParameter& arg, const Variable& rhs)
    {return arg < rhs;}
inline Constraint IndexParameter_operator_lt(const IndexParameter& arg, const Expression& rhs)
    {return arg < rhs;}

inline Constraint Variable_operator_lt(const Variable& arg, int rhs)
    {return arg < rhs;}
inline Constraint Variable_operator_lt(const Variable& arg, double rhs)
    {return arg < rhs;}
inline Constraint Variable_operator_lt(const Variable& arg, const Parameter& rhs)
    {return arg < rhs;}
inline Constraint Variable_operator_lt(const Variable& arg, const IndexParameter& rhs)
    {return arg < rhs;}
inline Constraint Variable_operator_lt(const Variable& arg, const Variable& rhs)
    {return arg < rhs;}
inline Constraint Variable_operator_lt(const Variable& arg, const Expression& rhs)
    {return arg < rhs;}

inline Constraint Expression_operator_lt(const Expression& arg, int rhs)
    {return arg < rhs;}
inline Constraint Expression_operator_lt(const Expression& arg, double rhs)
    {return arg < rhs;}
inline Constraint Expression_operator_lt(const Expression& arg, const Parameter& rhs)
    {return arg < rhs;}
inline Constraint Expression_operator_lt(const Expression& arg, const Variable& rhs)
    {return arg < rhs;}
inline Constraint Expression_operator_lt(const Expression& arg, const Expression& rhs)
    {return arg < rhs;}

// LE

inline Constraint Parameter_operator_le(const Parameter& arg, int rhs)
    {return arg <= rhs;}
inline Constraint Parameter_operator_le(const Parameter& arg, double rhs)
    {return arg <= rhs;}
inline Constraint Parameter_operator_le(const Parameter& arg, const Parameter& rhs)
    {return arg <= rhs;}
inline Constraint Parameter_operator_le(const Parameter& arg, const IndexParameter& rhs)
    {return arg <= rhs;}
inline Constraint Parameter_operator_le(const Parameter& arg, const Variable& rhs)
    {return arg <= rhs;}
inline Constraint Parameter_operator_le(const Parameter& arg, const Expression& rhs)
    {return arg <= rhs;}

inline Constraint IndexParameter_operator_le(const IndexParameter& arg, int rhs)
    {return arg <= rhs;}
inline Constraint IndexParameter_operator_le(const IndexParameter& arg, double rhs)
    {return arg <= rhs;}
inline Constraint IndexParameter_operator_le(const IndexParameter& arg, const Parameter& rhs)
    {return arg <= rhs;}
inline Constraint IndexParameter_operator_le(const IndexParameter& arg, const IndexParameter& rhs)
    {return arg <= rhs;}
inline Constraint IndexParameter_operator_le(const IndexParameter& arg, const Variable& rhs)
    {return arg <= rhs;}
inline Constraint IndexParameter_operator_le(const IndexParameter& arg, const Expression& rhs)
    {return arg <= rhs;}

inline Constraint Variable_operator_le(const Variable& arg, int rhs)
    {return arg <= rhs;}
inline Constraint Variable_operator_le(const Variable& arg, double rhs)
    {return arg <= rhs;}
inline Constraint Variable_operator_le(const Variable& arg, const Parameter& rhs)
    {return arg <= rhs;}
inline Constraint Variable_operator_le(const Variable& arg, const IndexParameter& rhs)
    {return arg <= rhs;}
inline Constraint Variable_operator_le(const Variable& arg, const Variable& rhs)
    {return arg <= rhs;}
inline Constraint Variable_operator_le(const Variable& arg, const Expression& rhs)
    {return arg <= rhs;}

inline Constraint Expression_operator_le(const Expression& arg, int rhs)
    {return arg <= rhs;}
inline Constraint Expression_operator_le(const Expression& arg, double rhs)
    {return arg <= rhs;}
inline Constraint Expression_operator_le(const Expression& arg, const Parameter& rhs)
    {return arg <= rhs;}
inline Constraint Expression_operator_le(const Expression& arg, const Variable& rhs)
    {return arg <= rhs;}
inline Constraint Expression_operator_le(const Expression& arg, const Expression& rhs)
    {return arg <= rhs;}

// GT

inline Constraint Parameter_operator_gt(const Parameter& arg, int rhs)
    {return arg > rhs;}
inline Constraint Parameter_operator_gt(const Parameter& arg, double rhs)
    {return arg > rhs;}
inline Constraint Parameter_operator_gt(const Parameter& arg, const Parameter& rhs)
    {return arg > rhs;}
inline Constraint Parameter_operator_gt(const Parameter& arg, const IndexParameter& rhs)
    {return arg > rhs;}
inline Constraint Parameter_operator_gt(const Parameter& arg, const Variable& rhs)
    {return arg > rhs;}
inline Constraint Parameter_operator_gt(const Parameter& arg, const Expression& rhs)
    {return arg > rhs;}

inline Constraint IndexParameter_operator_gt(const IndexParameter& arg, int rhs)
    {return arg > rhs;}
inline Constraint IndexParameter_operator_gt(const IndexParameter& arg, double rhs)
    {return arg > rhs;}
inline Constraint IndexParameter_operator_gt(const IndexParameter& arg, const Parameter& rhs)
    {return arg > rhs;}
inline Constraint IndexParameter_operator_gt(const IndexParameter& arg, const IndexParameter& rhs)
    {return arg > rhs;}
inline Constraint IndexParameter_operator_gt(const IndexParameter& arg, const Variable& rhs)
    {return arg > rhs;}
inline Constraint IndexParameter_operator_gt(const IndexParameter& arg, const Expression& rhs)
    {return arg > rhs;}

inline Constraint Variable_operator_gt(const Variable& arg, int rhs)
    {return arg > rhs;}
inline Constraint Variable_operator_gt(const Variable& arg, double rhs)
    {return arg > rhs;}
inline Constraint Variable_operator_gt(const Variable& arg, const Parameter& rhs)
    {return arg > rhs;}
inline Constraint Variable_operator_gt(const Variable& arg, const IndexParameter& rhs)
    {return arg > rhs;}
inline Constraint Variable_operator_gt(const Variable& arg, const Variable& rhs)
    {return arg > rhs;}
inline Constraint Variable_operator_gt(const Variable& arg, const Expression& rhs)
    {return arg > rhs;}

inline Constraint Expression_operator_gt(const Expression& arg, int rhs)
    {return arg > rhs;}
inline Constraint Expression_operator_gt(const Expression& arg, double rhs)
    {return arg > rhs;}
inline Constraint Expression_operator_gt(const Expression& arg, const Parameter& rhs)
    {return arg > rhs;}
inline Constraint Expression_operator_gt(const Expression& arg, const Variable& rhs)
    {return arg > rhs;}
inline Constraint Expression_operator_gt(const Expression& arg, const Expression& rhs)
    {return arg > rhs;}

// GE

inline Constraint Parameter_operator_ge(const Parameter& arg, int rhs)
    {return arg >= rhs;}
inline Constraint Parameter_operator_ge(const Parameter& arg, double rhs)
    {return arg >= rhs;}
inline Constraint Parameter_operator_ge(const Parameter& arg, const Parameter& rhs)
    {return arg >= rhs;}
inline Constraint Parameter_operator_ge(const Parameter& arg, const IndexParameter& rhs)
    {return arg >= rhs;}
inline Constraint Parameter_operator_ge(const Parameter& arg, const Variable& rhs)
    {return arg >= rhs;}
inline Constraint Parameter_operator_ge(const Parameter& arg, const Expression& rhs)
    {return arg >= rhs;}

inline Constraint IndexParameter_operator_ge(const IndexParameter& arg, int rhs)
    {return arg >= rhs;}
inline Constraint IndexParameter_operator_ge(const IndexParameter& arg, double rhs)
    {return arg >= rhs;}
inline Constraint IndexParameter_operator_ge(const IndexParameter& arg, const Parameter& rhs)
    {return arg >= rhs;}
inline Constraint IndexParameter_operator_ge(const IndexParameter& arg, const IndexParameter& rhs)
    {return arg >= rhs;}
inline Constraint IndexParameter_operator_ge(const IndexParameter& arg, const Variable& rhs)
    {return arg >= rhs;}
inline Constraint IndexParameter_operator_ge(const IndexParameter& arg, const Expression& rhs)
    {return arg >= rhs;}

inline Constraint Variable_operator_ge(const Variable& arg, int rhs)
    {return arg >= rhs;}
inline Constraint Variable_operator_ge(const Variable& arg, double rhs)
    {return arg >= rhs;}
inline Constraint Variable_operator_ge(const Variable& arg, const Parameter& rhs)
    {return arg >= rhs;}
inline Constraint Variable_operator_ge(const Variable& arg, const IndexParameter& rhs)
    {return arg >= rhs;}
inline Constraint Variable_operator_ge(const Variable& arg, const Variable& rhs)
    {return arg >= rhs;}
inline Constraint Variable_operator_ge(const Variable& arg, const Expression& rhs)
    {return arg >= rhs;}

inline Constraint Expression_operator_ge(const Expression& arg, int rhs)
    {return arg >= rhs;}
inline Constraint Expression_operator_ge(const Expression& arg, double rhs)
    {return arg >= rhs;}
inline Constraint Expression_operator_ge(const Expression& arg, const Parameter& rhs)
    {return arg >= rhs;}
inline Constraint Expression_operator_ge(const Expression& arg, const Variable& rhs)
    {return arg >= rhs;}
inline Constraint Expression_operator_ge(const Expression& arg, const Expression& rhs)
    {return arg >= rhs;}

// EQ

inline Constraint Parameter_operator_eq(const Parameter& arg, int rhs)
    {return arg == rhs;}
inline Constraint Parameter_operator_eq(const Parameter& arg, double rhs)
    {return arg == rhs;}
inline Constraint Parameter_operator_eq(const Parameter& arg, const Parameter& rhs)
    {return arg == rhs;}
inline Constraint Parameter_operator_eq(const Parameter& arg, const IndexParameter& rhs)
    {return arg == rhs;}
inline Constraint Parameter_operator_eq(const Parameter& arg, const Variable& rhs)
    {return arg == rhs;}
inline Constraint Parameter_operator_eq(const Parameter& arg, const Expression& rhs)
    {return arg == rhs;}

inline Constraint IndexParameter_operator_eq(const IndexParameter& arg, int rhs)
    {return arg == rhs;}
inline Constraint IndexParameter_operator_eq(const IndexParameter& arg, double rhs)
    {return arg == rhs;}
inline Constraint IndexParameter_operator_eq(const IndexParameter& arg, const Parameter& rhs)
    {return arg == rhs;}
inline Constraint IndexParameter_operator_eq(const IndexParameter& arg, const IndexParameter& rhs)
    {return arg == rhs;}
inline Constraint IndexParameter_operator_eq(const IndexParameter& arg, const Variable& rhs)
    {return arg == rhs;}
inline Constraint IndexParameter_operator_eq(const IndexParameter& arg, const Expression& rhs)
    {return arg == rhs;}

inline Constraint Variable_operator_eq(const Variable& arg, int rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, double rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const Parameter& rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const IndexParameter& rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const Variable& rhs)
    {return arg == rhs;}
inline Constraint Variable_operator_eq(const Variable& arg, const Expression& rhs)
    {return arg == rhs;}

inline Constraint Expression_operator_eq(const Expression& arg, int rhs)
    {return arg == rhs;}
inline Constraint Expression_operator_eq(const Expression& arg, double rhs)
    {return arg == rhs;}
inline Constraint Expression_operator_eq(const Expression& arg, const Parameter& rhs)
    {return arg == rhs;}
inline Constraint Expression_operator_eq(const Expression& arg, const Variable& rhs)
    {return arg == rhs;}
inline Constraint Expression_operator_eq(const Expression& arg, const Expression& rhs)
    {return arg == rhs;}

//
// +
//

inline Expression Parameter_operator_pos(const Parameter& arg)
    {return +arg;}
inline Expression Variable_operator_pos(const Variable& arg)
    {return +arg;}
inline Expression Expression_operator_pos(const Expression& arg)
    {return +arg;}

//
// -
//

inline Expression Parameter_operator_neg(const Parameter& arg)
    {return -arg;}
inline Expression Variable_operator_neg(const Variable& arg)
    {return -arg;}
inline Expression Expression_operator_neg(const Expression& arg)
    {return -arg;}

//
// radd
//

inline Expression Parameter_operator_radd(const Parameter& rhs, int lhs)
    {return lhs + rhs;}
inline Expression Parameter_operator_radd(const Parameter& rhs, double lhs)
    {return lhs + rhs;}

inline Expression Variable_operator_radd(const Variable& rhs, int lhs)
    {return lhs + rhs;}
inline Expression Variable_operator_radd(const Variable& rhs, double lhs)
    {return lhs + rhs;}

inline Expression Expression_operator_radd(const Expression& rhs, int lhs)
    {return lhs + rhs;}
inline Expression Expression_operator_radd(const Expression& rhs, double lhs)
    {return lhs + rhs;}

//
// rsub
//

inline Expression Parameter_operator_rsub(const Parameter& rhs, int lhs)
    {return lhs - rhs;}
inline Expression Parameter_operator_rsub(const Parameter& rhs, double lhs)
    {return lhs - rhs;}

inline Expression Variable_operator_rsub(const Variable& rhs, int lhs)
    {return lhs - rhs;}
inline Expression Variable_operator_rsub(const Variable& rhs, double lhs)
    {return lhs - rhs;}

inline Expression Expression_operator_rsub(const Expression& rhs, int lhs)
    {return lhs - rhs;}
inline Expression Expression_operator_rsub(const Expression& rhs, double lhs)
    {return lhs - rhs;}

//
// rmul
//

inline Expression Parameter_operator_rmul(const Parameter& rhs, int lhs)
    {return lhs * rhs;}
inline Expression Parameter_operator_rmul(const Parameter& rhs, double lhs)
    {return lhs * rhs;}

inline Expression Variable_operator_rmul(const Variable& rhs, int lhs)
    {return lhs * rhs;}
inline Expression Variable_operator_rmul(const Variable& rhs, double lhs)
    {return lhs * rhs;}

inline Expression Expression_operator_rmul(const Expression& rhs, int lhs)
    {return lhs * rhs;}
inline Expression Expression_operator_rmul(const Expression& rhs, double lhs)
    {return lhs * rhs;}

//
// rtruediv
//

inline Expression Parameter_operator_rtruediv(const Parameter& rhs, int lhs)
    {return lhs / rhs;}
inline Expression Parameter_operator_rtruediv(const Parameter& rhs, double lhs)
    {return lhs / rhs;}

inline Expression Variable_operator_rtruediv(const Variable& rhs, int lhs)
    {return lhs / rhs;}
inline Expression Variable_operator_rtruediv(const Variable& rhs, double lhs)
    {return lhs / rhs;}

inline Expression Expression_operator_rtruediv(const Expression& rhs, int lhs)
    {return lhs / rhs;}
inline Expression Expression_operator_rtruediv(const Expression& rhs, double lhs)
    {return lhs / rhs;}

//
// truediv
//

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

//
// pow
//

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
