
from _expr.lib import *
from expr import expression

#
# Intrinsic functions
#
# abs() and pow() are not defined here because they are Python operators.
#

def ceil(arg):
    return expression(intrinsic_ceil(arg.ptr))

def floor(arg):
    return expression(intrinsic_floor(arg.ptr))

def exp(arg):
    return expression(intrinsic_exp(arg.ptr))

def log(arg):
    return expression(intrinsic_log(arg.ptr))

def log10(arg):
    return expression(intrinsic_log10(arg.ptr))

def sqrt(arg):
    return expression(intrinsic_sqrt(arg.ptr))

def sin(arg):
    return expression(intrinsic_sin(arg.ptr))

def cos(arg):
    return expression(intrinsic_cos(arg.ptr))

def tan(arg):
    return expression(intrinsic_tan(arg.ptr))

def sinh(arg):
    return expression(intrinsic_sinh(arg.ptr))

def cosh(arg):
    return expression(intrinsic_cosh(arg.ptr))

def tanh(arg):
    return expression(intrinsic_tanh(arg.ptr))

def asin(arg):
    return expression(intrinsic_asin(arg.ptr))

def acos(arg):
    return expression(intrinsic_acos(arg.ptr))

def atan(arg):
    return expression(intrinsic_atan(arg.ptr))

def asinh(arg):
    return expression(intrinsic_asinh(arg.ptr))

def acosh(arg):
    return expression(intrinsic_acosh(arg.ptr))

def atanh(arg):
    return expression(intrinsic_atanh(arg.ptr))

