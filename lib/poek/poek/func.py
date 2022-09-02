
def seq(*args):
    if len(args) > 1:
        tmp = list(args)
        tmp[1] += 1
        args = tuple(tmp)
    return range(*args)

def prod(args):
    """
    A utility function to compute the product of Poek expressions.

    Args:
        args: A generator for terms in the product.

    Returns:
        The value of the product, which may be a Poek expression object.
    """
    expr = 1
    for v_ in args.values():
        expr *= v_
    return expr


def quicksum(args, start=0):
    """
    A utility function to compute a sum of Poek expressions.

    The behavior of :func:`quicksum` is similar to the builtin :func:`sum`
    function, but this function generates a more compact Poek
    expression.

    Args:
        args: A generator for terms in the sum.

        start: A value that is initializes the sum.  If
            this value is not a numeric constant, then the +=
            operator is used to add terms to this object.
            Defaults to zero.

    Returns:
        The value of the sum, which may be a Poek expression object.
    """
    e = start
    for arg in args:
        e += arg
    return e

