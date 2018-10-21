
from poek.expr import expression
from poek._coek import lib

__all__ = ['ceil', 'floor', 'exp', 'log', 'log10', 'sqrt', 'sin',
    'cos', 'tan', 'sinh', 'cosh', 'tanh', 'asin', 'acos', 'atan',
    'asinh', 'acosh', 'atanh']


#
# Intrinsic functions
#
# abs() and pow() are not defined here because they are Python operators.
#

def ceil(arg):
    return expression(lib.intrinsic_ceil(arg.ptr))

def floor(arg):
    return expression(lib.intrinsic_floor(arg.ptr))

def exp(arg):
    return expression(lib.intrinsic_exp(arg.ptr))

def log(arg):
    return expression(lib.intrinsic_log(arg.ptr))

def log10(arg):
    return expression(lib.intrinsic_log10(arg.ptr))

def sqrt(arg):
    return expression(lib.intrinsic_sqrt(arg.ptr))

def sin(arg):
    return expression(lib.intrinsic_sin(arg.ptr))

def cos(arg):
    return expression(lib.intrinsic_cos(arg.ptr))

def tan(arg):
    return expression(lib.intrinsic_tan(arg.ptr))

def sinh(arg):
    return expression(lib.intrinsic_sinh(arg.ptr))

def cosh(arg):
    return expression(lib.intrinsic_cosh(arg.ptr))

def tanh(arg):
    return expression(lib.intrinsic_tanh(arg.ptr))

def asin(arg):
    return expression(lib.intrinsic_asin(arg.ptr))

def acos(arg):
    return expression(lib.intrinsic_acos(arg.ptr))

def atan(arg):
    return expression(lib.intrinsic_atan(arg.ptr))

def asinh(arg):
    return expression(lib.intrinsic_asinh(arg.ptr))

def acosh(arg):
    return expression(lib.intrinsic_acosh(arg.ptr))

def atanh(arg):
    return expression(lib.intrinsic_atanh(arg.ptr))

