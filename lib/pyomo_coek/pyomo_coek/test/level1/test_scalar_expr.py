#
# Test scalar expression operations
#
import pytest

#
# Test fixtures
#

@pytest.fixture
def model_M(pyomo_module):
    return pyomo_module.ConcreteModel()

@pytest.fixture
def var_a(model_M, pyomo_module):
    model_M.a = pyomo_module.Var(initialize=0.0)
    return model_M.a

@pytest.fixture
def var_b(model_M, pyomo_module):
    model_M.b = pyomo_module.Var()
    return model_M.b
    
@pytest.fixture
def var_c(model_M, pyomo_module):
    model_M.c = pyomo_module.Var()
    return model_M.c
    
@pytest.fixture
def var_d(model_M, pyomo_module):
    model_M.d = pyomo_module.Var()
    return model_M.d
    
@pytest.fixture
def var_v(model_M, pyomo_module):
    model_M.v = pyomo_module.Var()
    return model_M.v
    
@pytest.fixture
def param_p(model_M, pyomo_module):
    model_M.p = pyomo_module.Param(initialize=0, mutable=True)
    return model_M.p

@pytest.fixture
def param_q(model_M, pyomo_module):
    model_M.q = pyomo_module.Param(initialize=0, mutable=True)
    return model_M.q

@pytest.fixture
def param_r(model_M, pyomo_module):
    model_M.r = pyomo_module.Param(initialize=1, mutable=True)
    return model_M.r

#
# Sum expression tests
#

def test_sum_error1(pyomo_module, var_a):
    pe,a = pyomo_module,var_a
    with pytest.raises((TypeError,KeyError)) as einfo:
        class TMP(object):
            pass
        TMP() + var_a

def test_simpleSum(pyomo_module, var_a, var_b):
    pe,a,b = pyomo_module,var_a,var_b
    # a + b
    e = a + b
    assert pe.to_list(e) == ["+", "a", "b"]

    e = a
    e += b
    assert pe.to_list(e) == ["+", "a", "b"]

def test_simpleSum_API(pyomo_module, var_a, var_b):
    pe,a,b = pyomo_module,var_a,var_b
    # a + b + 2*a
    e = a + b
    e += 2 * a
    #
    assert pe.to_list(e) == ["+", "a", "b", ["*", "2.000000", "a"]]

def test_constSum(pyomo_module, var_a):
    pe,a = pyomo_module,var_a
    # a + 5

    e = a + 5
    assert pe.to_list(e) == ["+", "a", "5.000000"]

    e = 5 + a
    assert pe.to_list(e) == ["+", "5.000000", "a"]

    e = a
    e += 5
    assert pe.to_list(e) == ["+", "a", "5.000000"]

    e = a + 5.0
    assert pe.to_list(e) == ["+", "a", "5.000000"]

    e = 5.0 + a
    assert pe.to_list(e) == ["+", "5.000000", "a"]

    e = a
    e += 5.0
    assert pe.to_list(e) == ["+", "a", "5.000000"]


def test_nestedSum(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested sums
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #           +
    #          / \
    #         +   5
    #        / \
    #       a   b
    e1 = a + b
    e = e1 + 5
    #
    assert pe.to_list(e) == ["+", "a", "b", "5.000000"] or \
           pe.to_list(e) == ["+", "5.000000", "a", "b"]

    #       +
    #      / \
    #     5   +
    #        / \
    #       a   b
    e1 = a + b
    e = 5 + e1
    #
    assert pe.to_list(e) == ["+", "5.000000", ["+", "a", "b"]] or \
           pe.to_list(e) == ["+", "a", "b", "5.000000"]

    #           +
    #          / \
    #         +   c
    #        / \
    #       a   b
    e1 = a + b
    e = e1 + c
    #
    assert pe.to_list(e) == ["+", "a", "b", "c"]

    #       +
    #      / \
    #     c   +
    #        / \
    #       a   b
    e1 = a + b
    e = c + e1
    #
    assert pe.to_list(e) == ["+", "c", ["+", "a", "b"]] or \
           pe.to_list(e) == ["+", "a", "b", "c"]

    #            +
    #          /   \
    #         +     +
    #        / \   / \
    #       a   b c   d
    e1 = a + b
    e2 = c + d
    e = e1 + e2
    #
    assert pe.to_list(e) == ["+", "a", "b", ["+", "c", "d"]] or \
           pe.to_list(e) == ["+", "a", "b", "c", "d"]

def test_nestedSum2(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested sums
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #           +
    #          / \
    #         *   c
    #        / \
    #       2   +
    #          / \
    #         a   b
    e1 = a + b
    e = 2 * e1 + c
    #
    assert pe.to_list(e) == ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]

    #         *
    #        / \
    #       3   +
    #          / \
    #         *   c
    #        / \
    #       2   +
    #          / \
    #         a   b
    e1 = a + b
    e = 3 * (2 * e1 + c)
    #
    assert pe.to_list(e) == ["*", "3.000000", ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]]

def test_trivialSum(pyomo_module, var_a):
    #
    # Check that adding zero doesn't change the expression
    #
    pe,a = pyomo_module,var_a

    e = a + 0
    assert pe.to_list(e) == ["a"]

    e = 0 + a
    assert pe.to_list(e) == ["a"]

    e = a + 0.0
    assert pe.to_list(e) == ["a"]

    e = 0.0 + a
    assert pe.to_list(e) == ["a"]

    e = a
    e += 0
    assert pe.to_list(e) == ["a"]

    e = a
    e += 0.0
    assert pe.to_list(e) == ["a"]

    #
    # Adding zero to a sum will not change the sum
    #
    e = a + a
    f = e + 0
    assert pe.to_list(e) == ["+", "a", "a"]

def test_sumOf_nestedTrivialProduct(pyomo_module, var_a, var_b, var_c):
    #
    # Check sums with nested products
    #
    pe,a,b,c = pyomo_module,var_a,var_b,var_c

    #       +
    #      / \
    #     *   b
    #    / \
    #   a   5
    e1 = a * 5
    e = e1 + b
    #
    assert pe.to_list(e) == ["+", ["*", "5.000000", "a"], "b"]

    #       +
    #      / \
    #     b   *
    #        / \
    #       a   5
    e = b + e1
    #
    assert pe.to_list(e) == ["+", "b", ["*", "5.000000", "a"]]

    #            +
    #          /   \
    #         *     +
    #        / \   / \
    #       a   5 b   c
    e2 = b + c
    e = e1 + e2
    #
    assert pe.to_list(e) == ["+", ["*", "5.000000", "a"], ["+", "b", "c"]] or \
           pe.to_list(e) == ["+", "b", "c", ["*", "5.000000", "a"]]

    #            +
    #          /   \
    #         +     *
    #        / \   / \
    #       b   c a   5
    e2 = b + c
    e = e2 + e1
    #
    assert pe.to_list(e) == ["+", "b", "c", ["*", "5.000000", "a"]]

#
# Diff expression tests
#

def test_diff_error1(pyomo_module, var_a):
    pe,a = pyomo_module,var_a
    with pytest.raises((TypeError,KeyError)) as einfo:
        class TMP(object):
            pass
        TMP() - var_a

def test_simpleDiff(pyomo_module, var_a, var_b):
    #
    # Check the structure of a simple difference with two variables
    #
    pe,a,b = pyomo_module,var_a,var_b

    #    -
    #   / \
    #  a   b
    e = a - b
    assert pe.to_list(e) == ["+", "a", ["*", "-1.000000", "b"]]

    e = a
    e -= b
    assert pe.to_list(e) == ["+", "a", ["*", "-1.000000", "b"]]

def test_constDiff(pyomo_module, var_a):
    pe,a = pyomo_module,var_a

    e = a - 5
    assert pe.to_list(e) == ["+", "a", "-5.000000"]

    e = 5 - a
    assert pe.to_list(e) == ["+", "5.000000", ["*", "-1.000000", "a"]]

    e = a
    e -= 5
    assert pe.to_list(e) == ["+", "a", "-5.000000"]

    e = a - 5.0
    assert pe.to_list(e) == ["+", "a", "-5.000000"]

    e = 5.0 - a
    assert pe.to_list(e) == ["+", "5.000000", ["*", "-1.000000", "a"]]

    e = a
    e -= 5.0
    assert pe.to_list(e) == ["+", "a", "-5.000000"]

def test_paramDiff(pyomo_module, var_a, param_p):
    # a - p
    pe,a,p = pyomo_module,var_a,param_p

    e = a - p
    assert pe.to_list(e) == ["+", "a", ["-", "p"]]

    # p - a
    e = p - a
    assert pe.to_list(e) == ["+", "p", ["*", "-1.000000", "a"]]

def test_termDiff(pyomo_module, var_a):
    #
    # Check the structure of a simple difference with a term
    #
    pe,a = pyomo_module,var_a

    #
    #   -
    #  / \
    # 5   *
    #    / \
    #   2   a
    #

    e = 5 - 2 * a
    assert pe.to_list(e) == ["+", "5.000000", ["*", "-2.000000", "a"]]

def test_nestedDiff(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested differences
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #       -
    #      / \
    #     -   5
    #    / \
    #   a   b

    e1 = a - b
    e = e1 - 5
    #
    assert pe.to_list(e) == ["+", "a", ["*", "-1.000000", "b"], "-5.000000"]

    #       -
    #      / \
    #     5   -
    #        / \
    #       a   b

    e1 = a - b
    e = 5 - e1
    #
    assert pe.to_list(e) == ["+", "5.000000", ["-", ["+", "a", ["*", "-1.000000", "b"]]]]

    #       -
    #      / \
    #     -   c
    #    / \
    #   a   b

    e1 = a - b
    e = e1 - c
    #
    assert pe.to_list(e) == ["+", "a", ["*", "-1.000000", "b"], ["*", "-1.000000", "c"]]

    #            -
    #          /   \
    #         -     -
    #        / \   / \
    #       a   b c   d

    e1 = a - b
    e2 = c - d
    e = e1 - e2
    #
    assert pe.to_list(e) == ["+", "a", ["*", "-1.000000", "b"], ["-", ["+", "c", ["*", "-1.000000", "d"]]]] or \
           pe.to_list(e) == ["+", ["+", "a", ["*", "-1.000000", "b"]], ["-", ["+", "c", ["*", "-1.000000", "d"]]]]

def test_negation_param(pyomo_module, param_p):
    #
    # Check logic for negations
    #
    pe,p = pyomo_module,param_p

    e = -p
    assert pe.to_list(e) == ["-", "p"]

    e = -e
    #
    # TODO: Can we detect negations of negations?
    #
    assert pe.to_list(e) == ["-", ["-", "p"]] or \
           pe.to_list(e) == ["p"]

def test_negation_terms(pyomo_module, param_p, var_v):
    #
    # Check logic for negations with terms
    #
    pe,p,v = pyomo_module,param_p,var_v

    e = -p * v
    assert pe.to_list(e) == ["*", ["-", "p"], "v"]

    e = -e
    assert pe.to_list(e) == ["-", ["*", ["-", "p"], "v"]] or \
           pe.to_list(e) == ["*", "p", "v"]
    #
    e = -5 * v
    assert pe.to_list(e) == ["*", "-5.000000", "v"]
    e = -e
    assert pe.to_list(e) == ["*", "5.000000", "v"]

def test_trivialDiff(pyomo_module, var_a, param_p):
    #
    # Check that subtracting zero doesn't change the expression
    #
    pe,a,p = pyomo_module,var_a,param_p

    # a - 0
    e = a - 0
    assert pe.to_list(e) == ["a"]

    e = a - 0.0
    assert pe.to_list(e) == ["a"]

    e = a
    e -= 0
    assert pe.to_list(e) == ["a"]

    e = a
    e -= 0.0
    assert pe.to_list(e) == ["a"]

    # 0 - a
    e = 0 - a
    assert pe.to_list(e) == ["*", "-1.000000", "a"]

    e = 0.0 - a
    assert pe.to_list(e) == ["*", "-1.000000", "a"]

    # p - 0
    e = p - 0
    assert pe.to_list(e) == ["p"]

    # 0 - p
    e = 0 - p
    assert pe.to_list(e) == ["-", "p"]

    # 0 - 5*a
    e = 0 - 5 * a
    assert pe.to_list(e) == ["*", "-5.000000", "a"]

    # 0 - p*a
    e = 0 - p * a
    assert pe.to_list(e) == ["-", ["*", "p", "a"]] or \
           pe.to_list(e) == ["*", ["-", "p"], "a"]

    # 0 - a*a
    e = 0 - a * a
    assert pe.to_list(e) == ["-", ["*", "a", "a"]]

def test_sumOf_nestedTrivialProduct2(pyomo_module, var_a, var_b, var_c):
    #
    # Check the structure of sum of products
    #
    pe,a,b,c = pyomo_module,var_a,var_b,var_c

    #       -
    #      / \
    #     *   b
    #    / \
    #   a   5
    e1 = a * 5
    e = e1 - b
    assert pe.to_list(e) == ["+", ["*", "5.000000", "a"], ["*", "-1.000000", "b"]]

    #       -
    #      / \
    #     b   *
    #        / \
    #       a   5
    e1 = a * 5
    e = b - e1
    assert pe.to_list(e) == ["+", "b", ["*", "-5.000000", "a"]]

    #            -
    #          /   \
    #         *     -
    #        / \   / \
    #       a   5 b   c
    e1 = a * 5
    e2 = b - c
    e = e1 - e2
    assert pe.to_list(e) == ["+", ["*", "5.000000", "a"], ["-", ["+", "b", ["*", "-1.000000", "c"]]]]

    #            -
    #          /   \
    #         -     *
    #        / \   / \
    #       b   c a   5
    e1 = a * 5
    e2 = b - c
    e = e2 - e1
    assert pe.to_list(e) == ["+", "b", ["*", "-1.000000", "c"], ["*", "-5.000000", "a"]]

#
# Mul expression tests
#

def test_mul_error1(pyomo_module, var_a):
    pe,a = pyomo_module,var_a
    with pytest.raises((TypeError,KeyError)) as einfo:
        class TMP(object):
            pass
        TMP() * var_a

def test_simpleProduct(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of a simple product of variables
    #
    pe,a,b = pyomo_module,var_a,var_b

    #    *
    #   / \
    #  a   b
    e = a * b
    assert pe.to_list(e) == ["*", "a", "b"]

    #    *
    #   / \
    #  a   b
    e = a
    e *= b
    assert pe.to_list(e) == ["*", "a", "b"]

def test_constProduct(pyomo_module, var_a):
    #
    # Check the structure of a simple product with a constant
    #
    pe,a = pyomo_module,var_a

    #    *
    #   / \
    #  a   5
    e = a * 5
    assert pe.to_list(e) == ["*", "5.000000", "a"]

    e = a * 5.0
    assert pe.to_list(e) == ["*", "5.000000", "a"]

    e = a
    e *= 5
    assert pe.to_list(e) == ["*", "5.000000", "a"]

    #    *
    #   / \
    #  5   a
    e = 5.0 * a
    assert pe.to_list(e) == ["*", "5.000000", "a"]

    e = a
    e *= 5.0
    assert pe.to_list(e) == ["*", "5.000000", "a"]

    e = 5.0 * a
    assert pe.to_list(e) == ["*", "5.000000", "a"]

def test_nestedProduct(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested products
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #       *
    #      / \
    #     *   5
    #    / \
    #   a   b
    e1 = a * b
    e = e1 * 5
    assert pe.to_list(e) == ["*", ["*", "a", "b"], "5.000000"]

    #       *
    #      / \
    #     5   *
    #        / \
    #       a   b
    e1 = a * b
    e = 5 * e1
    assert pe.to_list(e) == ["*", "5.000000", ["*", "a", "b"]]

    #       *
    #      / \
    #     *   c
    #    / \
    #   a   b
    e1 = a * b
    e = e1 * c
    assert pe.to_list(e) == ["*", ["*", "a", "b"], "c"]

    #       *
    #      / \
    #     c   *
    #        / \
    #       a   b
    e1 = a * b
    e = c * e1
    assert pe.to_list(e) == ["*", "c", ["*", "a", "b"]]

    #            *
    #          /   \
    #         *     *
    #        / \   / \
    #       a   b c   d
    e1 = a * b
    e2 = c * d
    e = e1 * e2
    assert pe.to_list(e) == ["*", ["*", "a", "b"], ["*", "c", "d"]]

def test_nestedProduct2(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested products
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #
    # Check the structure of nested products
    #
    #            *
    #          /   \
    #         +     +
    #        / \   / \
    #       c    +    d
    #           / \
    #          a   b
    e1 = a + b
    e2 = c + e1
    e3 = e1 + d
    e = e2 * e3
    assert pe.to_list(e) == ["*", ["+", "c", ["+", "a", "b"]], ["+", "a", "b", "d"]] or \
           pe.to_list(e) == ['*', ['+', 'a', 'b', 'c'], ['+', 'a', 'b', 'd']]

    #
    # Check the structure of nested products
    #
    #            *
    #          /   \
    #         *     *
    #        / \   / \
    #       c    +    d
    #           / \
    #          a   b
    e1 = a + b
    e2 = c * e1
    e3 = e1 * d
    e = e2 * e3
    assert pe.to_list(e) == ["*", ["*", "c", ["+", "a", "b"]], ["*", ["+", "a", "b"], "d"]]

def test_nestedProduct3(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested products
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #       *
    #      / \
    #     *   5
    #    / \
    #   3   b
    e1 = 3 * b
    e = e1 * 5
    assert pe.to_list(e) == ["*", ["*", "3.000000", "b"], "5.000000"] or \
           pe.to_list(e) == ["*", "15.000000", "b"]

    #       *
    #      / \
    #     *   5
    #    / \
    #   a   b
    e1 = a * b
    e = e1 * 5
    assert pe.to_list(e) == ["*", ["*", "a", "b"], "5.000000"]

    #       *
    #      / \
    #     5   *
    #        / \
    #       3   b
    e1 = 3 * b
    e = 5 * e1
    assert pe.to_list(e) == ["*", "5.000000", ["*", "3.000000", "b"]] or \
           pe.to_list(e) == ["*", "15.000000", "b"]

    #       *
    #      / \
    #     5   *
    #        / \
    #       a   b
    e1 = a * b
    e = 5 * e1
    assert pe.to_list(e) == ["*", "5.000000", ["*", "a", "b"]]

    #       *
    #      / \
    #     *   c
    #    / \
    #   a   b
    e1 = a * b
    e = e1 * c
    assert pe.to_list(e) == ["*", ["*", "a", "b"], "c"]

    #       *
    #      / \
    #     c   *
    #        / \
    #       a   b
    e1 = a * b
    e = c * e1
    assert pe.to_list(e) == ["*", "c", ["*", "a", "b"]]

    #            *
    #          /   \
    #         *     *
    #        / \   / \
    #       a   b c   d
    e1 = a * b
    e2 = c * d
    e = e1 * e2
    assert pe.to_list(e) == ["*", ["*", "a", "b"], ["*", "c", "d"]]

def test_trivialProduct(pyomo_module, var_a, param_p, param_q, param_r):
    #
    # Check that multiplying by zero gives zero
    #
    pe,a,p,q,r = pyomo_module,var_a,param_p,param_q,param_r

    e = a * 0
    assert pe.value(e) == 0.0

    e = a * 0.0
    assert pe.value(e) == 0.0

    e = a
    e *= 0
    assert pe.value(e) == 0.0

    e = a
    e *= 0.0
    assert pe.value(e) == 0.0

    e = 0 * a
    assert pe.value(e) == 0.0

    e = 0.0 * a
    assert pe.value(e) == 0.0

    e = a * p
    assert pe.to_list(e) == ["*", "a", "p"] or \
           pe.to_list(e) == ["*", "p", "a"]

    e = p * a
    assert pe.to_list(e) == ["*", "p", "a"] or \
           pe.to_list(e) == ["*", "p", "a"]

    #
    # Check that multiplying by one gives the original expression
    #
    e = a * 1
    assert pe.to_list(e) == ["a"]

    e = a * 1.0
    assert pe.to_list(e) == ["a"]

    e = a
    e *= 1
    assert pe.to_list(e) == ["a"]

    e = a
    e *= 1.0
    assert pe.to_list(e) == ["a"]

    e = 1 * a
    assert pe.to_list(e) == ["a"]

    e = 1.0 * a
    assert pe.to_list(e) == ["a"]

#
# Div expression tests
#

def test_div_error1(pyomo_module, var_a):
    pe,a = pyomo_module,var_a
    with pytest.raises((TypeError,KeyError)) as einfo:
        class TMP(object):
            pass
        TMP() / var_a

def test_simpleDivision(pyomo_module, var_a, var_b):
    #
    # Check the structure of a simple division with variables
    #
    pe,a,b = pyomo_module,var_a,var_b

    #    /
    #   / \
    #  a   b
    e = a / b
    assert pe.to_list(e) == ["/", "a", "b"]

    e = a
    e /= b
    assert pe.to_list(e) == ["/", "a", "b"]

def test_constDivision(pyomo_module, var_a):
    #
    # Check the structure of a simple division with a constant
    #
    pe,a = pyomo_module,var_a

    e = a / 5
    assert pe.to_list(e) == ["*", "0.200000", "a"]

    e = 5 / a
    assert pe.to_list(e) == ["/", "5.000000", "a"]

    e = a
    e /= 5
    assert pe.to_list(e) == ["*", "0.200000", "a"]

    e = a / 5.0
    assert pe.to_list(e) == ["*", "0.200000", "a"]

    e = 5.0 / a
    assert pe.to_list(e) == ["/", "5.000000", "a"]

    e = a
    e /= 5.0
    assert pe.to_list(e) == ["*", "0.200000", "a"]

def test_nestedDivision(pyomo_module, var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested divisions
    #
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    #       /
    #      / \
    #     *   5
    #    / \
    #   3   b
    e1 = 3 * b
    e = e1 / 5
    assert pe.to_list(e) == ["/", ["*", "3.000000", "b"], "5.000000"] or \
           pe.to_list(e) == ["*", "0.600000", "b"]

    #       /
    #      / \
    #     /   5
    #    / \
    #   a   b
    e1 = a / b
    e = e1 / 5
    assert pe.to_list(e) == ["/", ["/", "a", "b"], "5.000000"]

    #       /
    #      / \
    #     5   /
    #        / \
    #       a   b
    e1 = a / b
    e = 5 / e1
    assert pe.to_list(e) == ["/", "5.000000", ["/", "a", "b"]]

    #       /
    #      / \
    #     /   c
    #    / \
    #   a   b
    e1 = a / b
    e = e1 / c
    assert pe.to_list(e) == ["/", ["/", "a", "b"], "c"]

    #       /
    #      / \
    #     c   /
    #        / \
    #       a   b
    e1 = a / b
    e = c / e1
    assert pe.to_list(e) == ["/", "c", ["/", "a", "b"]]

    #            /
    #          /   \
    #         /     /
    #        / \   / \
    #       a   b c   d
    e1 = a / b
    e2 = c / d
    e = e1 / e2
    assert pe.to_list(e) == ["/", ["/", "a", "b"], ["/", "c", "d"]]

def test_trivialDivision(pyomo_module, var_a, param_p, param_q, param_r):
    #
    # Check that dividing by zero generates an exception
    #
    pe,a,p,q,r = pyomo_module,var_a,param_p,param_q,param_r

    with pytest.raises((ValueError,TypeError,ZeroDivisionError)) as einfo:
        a.__truediv__(0)
    with pytest.raises((ValueError,TypeError,ZeroDivisionError)) as einfo:
        a.__truediv__(0.0)

    #
    # Check that dividing zero by anything non-zero gives zero
    #
    e = 0 / a
    assert pe.to_list(e) == ["0.000000"] or \
           pe.to_list(e) == ["/", "0.000000", "a"]
    #self.assertAlmostEqual(e.value, 0.0)

    e = 0.0 / a
    assert pe.to_list(e) == ["0.000000"] or \
           pe.to_list(e) == ["/", "0.000000", "a"]
    #self.assertAlmostEqual(e.value, 0.0)

    #
    # Check that dividing by one 1 gives the original expression
    #
    e = a / 1
    assert pe.to_list(e) == ["a"]

    e = a / 1.0
    assert pe.to_list(e) == ["a"]

    e = a
    e /= 1
    assert pe.to_list(e) == ["a"]

    e = a
    e /= 1.0
    assert pe.to_list(e) == ["a"]

    e = a / r
    assert pe.to_list(e) == ["/", "a", "r"] or \
           pe.to_list(e) == ["*", ["/", "1.000000", "r"], "a"]

    #
    # Check the structure dividing 1 by an expression
    #
    e = 1 / a
    assert pe.to_list(e) == ["/", "1.000000", "a"]

    e = 1.0 / a
    assert pe.to_list(e) == ["/", "1.000000", "a"]

    #
    # Check the structure dividing 1 by a mutable zero
    #
    e = 1 / p
    assert pe.to_list(e) == ["/", "1.000000", "p"]

    e = 1.0 / p
    assert pe.to_list(e) == ["/", "1.000000", "p"]

#
# Pow expression tests
#

def test_pow_error1(pyomo_module, var_a):
    pe,a = pyomo_module,var_a
    with pytest.raises((TypeError,KeyError)) as einfo:
        class TMP(object):
            pass
        TMP() ** var_a

def test_simplePow(pyomo_module, var_a, var_b):
    pe,a,b = pyomo_module,var_a,var_b

    e = a**b
    assert pe.to_list(e) == ["pow", "a", "b"]

    e = a
    e **= b
    assert pe.to_list(e) == ["pow", "a", "b"]

def test_constPow(pyomo_module, var_a):
    #
    # Check the structure of a simple power with a constant
    #
    pe,a = pyomo_module,var_a

    e = a**5
    assert pe.to_list(e) == ["pow", "a", "5.000000"]

    e = 5**a
    assert pe.to_list(e) == ["pow", "5.000000", "a"]

    e = a
    e **= 5
    assert pe.to_list(e) == ["pow", "a", "5.000000"]

    e = a**5.0
    assert pe.to_list(e) == ["pow", "a", "5.000000"]

    e = 5.0**a
    assert pe.to_list(e) == ["pow", "5.000000", "a"]

    e = a
    e **= 5.0
    assert pe.to_list(e) == ["pow", "a", "5.000000"]

def test_trivialPow(pyomo_module, var_a, param_p, param_q, param_r):
    #
    pe,a,p,q,r = pyomo_module,var_a,param_p,param_q,param_r

    #
    # Check that taking the first power returns the original object
    #
    e = a**1
    assert pe.to_list(e) == ["a"]

    e = a**1.0
    assert pe.to_list(e) == ["a"]

    e = a
    e **= 1
    assert pe.to_list(e) == ["a"]

    e = a
    e **= 1.0
    assert pe.to_list(e) == ["a"]

    e = a**r
    assert pe.to_list(e) == ["pow", "a", "r"]

    #
    # Check that taking the zeroth power returns one
    #
    e = a**0
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "a", "0.000000"]

    e = a**0.0
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "a", "0.000000"]

    e = a
    e **= 0
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "a", "0.000000"]

    e = a
    e **= 0.0
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "a", "0.000000"]

    e = a**q
    assert pe.to_list(e) == ["pow", "a", "q"]

    #
    # Check that taking powers of 0 and 1 are easy
    #
    e = 0**a
    assert pe.to_list(e) == ["0.000000"] or \
           pe.to_list(e) == ["pow", "0.000000", "a"]

    e = 0.0**a
    assert pe.to_list(e) == ["0.000000"] or \
           pe.to_list(e) == ["pow", "0.000000", "a"]

    e = 1**a
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "1.000000", "a"]

    e = 1.0**a
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "1.000000", "a"]

def test_trivialRPow(pyomo_module, var_a, param_p, param_q, param_r):
    #
    pe,a,p,q,r = pyomo_module,var_a,param_p,param_q,param_r

    #
    # Check that taking any power of 1 is 1
    #
    e = 1**a
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "1.000000", "a"]

    e = 1.0**a
    assert pe.to_list(e) == ["1.000000"] or \
           pe.to_list(e) == ["pow", "1.000000", "a"]

    e = r**a
    assert pe.to_list(e) == ["pow", "r", "a"]

    #
    # Check that taking the zeroth power returns one
    #
    e = 0**a
    assert pe.to_list(e) == ["0.000000"] or \
           pe.to_list(e) == ["pow", "0.000000", "a"]

    e = 0.0**a
    assert pe.to_list(e) == ["0.000000"] or \
           pe.to_list(e) == ["pow", "0.000000", "a"]

def test_pow_expresions(pyomo_module, var_a, param_r):
    pe,a,r = pyomo_module,var_a,param_r

    e = a**r
    assert pe.to_list(e) == ["pow", "a", "r"]

    e = a**2
    assert pe.to_list(e) == ["pow", "a", "2.000000"]

    e = a**2.0
    assert pe.to_list(e) == ["pow", "a", "2.000000"]

    e = 2**a
    assert pe.to_list(e) == ["pow", "2.000000", "a"]

    e = 2.0**a
    assert pe.to_list(e) == ["pow", "2.000000", "a"]

#
# Test entangled expression errors
#

def test_sumexpr_add_entangled(pyomo_module, var_a):
    pe,a = pyomo_module,var_a

    e = a * 2 + 1
    assert pe.to_list(e) == ["+", ["*", "2.000000", "a"], "1.000000"]
    e += 1
    assert pe.to_list(e) == ["+", ["*", "2.000000", "a"], "1.000000", "1.000000"]

def test_entangled_test1(pyomo_module, var_a, var_b, var_c, var_d):
    pe,a,b,c,d = pyomo_module,var_a,var_b,var_c,var_d

    e1 = a + b
    e2 = c + e1
    e3 = d + e1

    assert pe.to_list(e1) == ["+", "a", "b"]
    assert pe.to_list(e2) == ["+", "c", ["+", "a", "b"]] or \
           pe.to_list(e2) == ["+", "a", "b", "c"]
    assert pe.to_list(e3) == ["+", "d", ["+", "a", "b"]] or \
           pe.to_list(e3) == ["+", "a", "b", "d"]

