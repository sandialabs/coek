#ifndef COEK_API_INTRINSIC_FN_H
#define COEK_API_INTRINSIC_FN_H
#pragma once

namespace coek {


class Expression;

//
// intrinsic functions
//
Expression abs(const Expression& body);
Expression ceil(const Expression& body);
Expression floor(const Expression& body);
Expression exp(const Expression& body);
Expression log(const Expression& body);
Expression log10(const Expression& body);
Expression sqrt(const Expression& body);
Expression sin(const Expression& body);
Expression cos(const Expression& body);
Expression tan(const Expression& body);
Expression sinh(const Expression& body);
Expression cosh(const Expression& body);
Expression tanh(const Expression& body);
Expression asin(const Expression& body);
Expression acos(const Expression& body);
Expression atan(const Expression& body);
Expression asinh(const Expression& body);
Expression acosh(const Expression& body);
Expression atanh(const Expression& body);

Expression pow(const Expression&, const Expression&);
Expression pow(const Expression&, double);
Expression pow(double, const Expression&);

}
#endif
