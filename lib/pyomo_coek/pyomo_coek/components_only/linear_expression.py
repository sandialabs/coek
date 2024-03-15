
def LinearExpression(args=None, constant=None, linear_coefs=None, linear_vars=None):

        """A linear expression of the form `const + sum_i(c_i*x_i).

        You can specify args OR (constant, linear_coefs, and
        linear_vars).  If args is provided, it should be a list that
        contains the constant, followed by a series of
        :py:class:`MonomialTermExpression` objects. Alternatively, you
        can specify the constant, the list of linear_coeffs and the list
        of linear_vars separately. Note that these lists are NOT copied.

        """
        if args is not None and any(arg is not None for arg in (constant, linear_coefs, linear_vars)):
            raise ValueError(
                "Cannot specify both args and any of "
                "{constant, linear_coeffs, or linear_vars}"
            )
        constant if constant is not None else 0
        linear_coefs = linear_coefs if linear_coefs else []
        linear_vars = linear_vars if linear_vars else []

        return constant + sum(a*b for a,b in zip(linear_coefs, linear_vars))

