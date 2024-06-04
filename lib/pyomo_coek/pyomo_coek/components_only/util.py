from .variable import Var
from pyomo.core.base.expression import Expression
from pyomo.core.expr.numeric_expr import mutable_expression


def sum_product(*args, **kwds):
    """
    A utility function to compute a generalized dot product.

    This function accepts one or more components that provide terms
    that are multiplied together.  These products are added together
    to form a sum.

    Args:
        *args: Variable length argument list of generators that
            create terms in the summation.
        **kwds: Arbitrary keyword arguments.

    Keyword Args:
        index: A set that is used to index the components used to
            create the terms
        denom: A component or tuple of components that are used to
            create the denominator of the terms
        start: The initial value used in the sum

    Returns:
        The value of the sum.
    """
    denom = kwds.pop("denom", tuple())
    if type(denom) not in (list, tuple):
        denom = [denom]
    nargs = len(args)
    ndenom = len(denom)

    if nargs == 0 and ndenom == 0:
        raise ValueError(
            "The sum_product() command requires at least an " + "argument or a denominator term"
        )

    if "index" in kwds:
        index = kwds["index"]
    else:
        if nargs > 0:
            iarg = args[-1]
            if not isinstance(iarg, Var) and not isinstance(iarg, Expression):
                raise ValueError(
                    "Error executing sum_product(): The last argument value must be a variable or expression object if no 'index' option is specified"
                )
        else:
            iarg = denom[-1]
            if not isinstance(iarg, Var) and not isinstance(iarg, Expression):
                raise ValueError(
                    "Error executing sum_product(): The last denom argument value must be a variable or      expression object if no 'index' option is specified"
                )
        index = iarg.index_set()

    start = kwds.get("start", 0)

    if ndenom == 0:
        #
        # Sum of polynomial terms
        #
        expr = 0
        expr += start
        if nargs == 1:
            arg1 = args[0]
            for i in index:
                expr += arg1[i]
        elif nargs == 2:
            arg1, arg2 = args
            for i in index:
                expr += arg1[i] * arg2[i]
        else:
            for i in index:
                expr += prod(arg[i] for arg in args)
        return expr
    elif nargs == 0:
        #
        # Sum of reciprocals
        #
        return quicksum((1 / prod(den[i] for den in denom) for i in index), start)
    else:
        #
        # Sum of fractions
        #
        return quicksum(
            (prod(arg[i] for arg in args) / prod(den[i] for den in denom) for i in index),
            start,
        )
