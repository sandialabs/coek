from poek._expr import ffi, lib
from poek.expr import *

__all__ = ['value', 'quicksum']


def value(obj):
    if isinstance(obj, NumericValue):
        return obj.eval()
    return obj


def quicksum(args):
    # NOTE:  We could simplify this logic by having the summation object
    #   maintain a list of things being summed.
    try:
        first = next(args, None)
    except:
        try:
            args = args.__iter__()
            first = next(args, None)
        except:
            raise TypeError("The argument to quicksum() is not iterable!")
    if first is None:
        return 0

    start = ZeroParameter
    if first is 0:
        ptr = start.ptr
    else:
        estart = start + first
        ptr = estart.ptr
    const = []
    for arg in args:
        try:
            ptr = lib.add_expr_expression(ptr, arg.ptr)
        except AttributeError:
            const.append(arg)
    return expression(ptr) + sum(const)

