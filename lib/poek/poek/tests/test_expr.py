import math
import pytest

from poek import *
import poek

try:
    import numpy as np

    numpy_available = True
except:
    numpy_available = False

#
# Test fixtures
#


@pytest.fixture
def var_a():
    return variable(name="a")


@pytest.fixture
def var_b():
    return variable(name="b")


@pytest.fixture
def var_c():
    return variable(name="c")


@pytest.fixture
def var_d():
    return variable(name="d")


@pytest.fixture
def var_v():
    return variable(name="v")


@pytest.fixture
def param_p():
    return parameter(name="p", value=0)


@pytest.fixture
def param_q():
    return parameter(name="q", value=0)


@pytest.fixture
def param_r():
    return parameter(name="r", value=1)

@pytest.fixture
def data_d1():
    return data(name="d1", value=0)


@pytest.fixture
def param_d2():
    return data(name="d2", value=0)


@pytest.fixture
def param_d3():
    return data(name="d3", value=1)


#
# Value tests
#


def test_var_value():
    p = variable(value=2)
    assert p.value == 2
    p.value = 3
    assert p.value == 3


def test_expr_value():
    p = variable(value=2)
    p.value == 2
    e = p + 3
    assert e.value == 5

    if numpy_available:
        p.value == np.int32(2)
        e = p + 3
        assert e.value == 5

    with pytest.raises(AttributeError) as einfo:
        e.value = 0


def test_constraint_value():
    p = variable(value=2)
    assert p.value == 2
    e = p < 3
    assert p.value == 2
    with pytest.raises(AttributeError) as einfo:
        e.value = 0

    if numpy_available:
        z = np.int32(3)
        e = p < z


def test_data1_value():
    p = data(value=-1)
    assert p.value == -1
    p.value = 3
    assert p.value == 3


def test_data_float():
    p = data(value=-1)
    with pytest.raises(TypeError) as einfo:
        float(p)
    if numpy_available:
        z = np.float32(-1)
        p = data(value=z)


def test_data_int():
    p = data(value=-1)
    with pytest.raises(TypeError) as einfo:
        int(p)
    if numpy_available:
        z = np.int32(-1)
        p = data(value=z)


def test_param1_value():
    p = parameter(value=-1)
    assert p.value == -1
    p.value = 3
    assert p.value == 3


def test_param_float():
    p = parameter(value=-1)
    with pytest.raises(TypeError) as einfo:
        float(p)
    if numpy_available:
        z = np.float32(-1)
        p = parameter(value=z)


def test_param_int():
    p = parameter(value=-1)
    with pytest.raises(TypeError) as einfo:
        int(p)
    if numpy_available:
        z = np.int32(-1)
        p = parameter(value=z)


def test_sum_error1(var_a):
    a = var_a

    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() + a
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() + a


def test_sum_error2(var_a):
    a = var_a
    x = variable(10)
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:
            a + x
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:
            a + x


def test_simpleSum(var_a, var_b):
    # a + b
    a, b = var_a, var_b

    e = a + b
    assert e.to_list() == ["+", "a", "b"]

    e = a
    e += b
    assert e.to_list() == ["+", "a", "b"]


def test_simpleSum_API(var_a, var_b):
    # a + b + 2*a
    a, b = var_a, var_b

    e = a + b
    e += 2 * a
    assert e.to_list() == ["+", "a", "b", ["*", "2.000000", "a"]]

    if numpy_available:
        e = a + b
        e += np.int32(2) * a
        assert e.to_list() == ["+", "a", "b", ["*", "2.000000", "a"]]


def test_constSum(var_a):
    # a + 5
    a = var_a

    e = a + 5
    assert e.to_list() == ["+", "a", "5.000000"]

    e = 5 + a
    assert e.to_list() == ["+", "5.000000", "a"]

    e = a
    e += 5
    assert e.to_list() == ["+", "a", "5.000000"]

    e = a + 5.0
    assert e.to_list() == ["+", "a", "5.000000"]

    e = 5.0 + a
    assert e.to_list() == ["+", "5.000000", "a"]

    e = a
    e += 5.0
    assert e.to_list() == ["+", "a", "5.000000"]

    if numpy_available:
        z = np.int32(5)
        e = a + z
        assert e.to_list() == ["+", "a", "5.000000"]

        e = z + a
        assert e.to_list() == ["+", "5.000000", "a"]

        e = a
        e += z
        assert e.to_list() == ["+", "a", "5.000000"]

        z = np.float32(5.0)
        e = a + z
        assert e.to_list() == ["+", "a", "5.000000"]

        e = z + a
        assert e.to_list() == ["+", "5.000000", "a"]

        e = a
        e += z
        assert e.to_list() == ["+", "a", "5.000000"]


def test_nestedSum(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested sums
    #
    a, b, c, d = var_a, var_b, var_c, var_d

    #           +
    #          / \
    #         +   5
    #        / \
    #       a   b
    e1 = a + b
    e = e1 + 5
    #
    assert e.to_list() == ["+", "a", "b", "5.000000"]

    #       +
    #      / \
    #     5   +
    #        / \
    #       a   b
    e1 = a + b
    e = 5 + e1
    #
    assert e.to_list() == ["+", "5.000000", ["+", "a", "b"]]

    if numpy_available:
        #           +
        #          / \
        #         +   5
        #        / \
        #       a   b
        e1 = a + b
        e = e1 + np.int32(5)
        #
        assert e.to_list() == ["+", "a", "b", "5.000000"]

        #       +
        #      / \
        #     5   +
        #        / \
        #       a   b
        e1 = a + b
        e = np.int32(5) + e1
        #
        assert e.to_list() == ["+", "5.000000", ["+", "a", "b"]]

    #           +
    #          / \
    #         +   c
    #        / \
    #       a   b
    e1 = a + b
    e = e1 + c
    #
    assert e.to_list() == ["+", "a", "b", "c"]

    #       +
    #      / \
    #     c   +
    #        / \
    #       a   b
    e1 = a + b
    e = c + e1
    #
    assert e.to_list() == ["+", "c", ["+", "a", "b"]]

    #            +
    #          /   \
    #         +     +
    #        / \   / \
    #       a   b c   d
    e1 = a + b
    e2 = c + d
    e = e1 + e2
    #
    assert e.to_list() == ["+", "a", "b", ["+", "c", "d"]]


def test_nestedSum2(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested sums
    #
    a, b, c, d = var_a, var_b, var_c, var_d

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
    assert e.to_list() == ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]

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
    assert e.to_list() == ["*", "3.000000", ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]]

    if numpy_available:
        #           +
        #          / \
        #         *   c
        #        / \
        #       2   +
        #          / \
        #         a   b
        e1 = a + b
        e = np.int32(2) * e1 + c
        #
        assert e.to_list() == ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]

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
        e = np.int32(3) * (np.int32(2) * e1 + c)
        #
        assert e.to_list() == ["*", "3.000000", ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]]


def test_trivialSum(var_a):
    #
    # Check that adding zero doesn't change the expression
    #
    a = var_a

    e = a + 0
    assert e.to_list() == ["a"]

    e = 0 + a
    assert e.to_list() == ["a"]

    e = a + 0.0
    assert e.to_list() == ["a"]

    e = 0.0 + a
    assert e.to_list() == ["a"]

    e = a
    e += 0
    assert e.to_list() == ["a"]

    e = a
    e += 0.0
    assert e.to_list() == ["a"]

    #
    # Adding zero to a sum will not change the sum
    #
    e = a + a
    f = e + 0
    assert e.to_list() == ["+", "a", "a"]

    if numpy_available:
        z = np.int32(0)
        e = a + z
        assert e.to_list() == ["a"]

        e = z + a
        assert e.to_list() == ["a"]

        e = a
        e += z
        assert e.to_list() == ["a"]

        z = np.float32(0)
        e = a + z
        assert e.to_list() == ["a"]

        e = z + a
        assert e.to_list() == ["a"]

        e = a
        e += z
        assert e.to_list() == ["a"]

        #
        # Adding zero to a sum will not change the sum
        #
        e = a + a
        f = e + np.int32(0)
        assert e.to_list() == ["+", "a", "a"]


def test_sumOf_nestedTrivialProduct(var_a, var_b, var_c):
    #
    # Check sums with nested products
    #
    a, b, c = var_a, var_b, var_c

    if True:
        #       +
        #      / \
        #     *   b
        #    / \
        #   a   5
        e1 = a * 5
        e = e1 + b
        #
        assert e.to_list() == ["+", ["*", "5.000000", "a"], "b"]

        #       +
        #      / \
        #     b   *
        #        / \
        #       a   5
        e = b + e1
        #
        assert e.to_list() == ["+", "b", ["*", "5.000000", "a"]]

        #            +
        #          /   \
        #         *     +
        #        / \   / \
        #       a   5 b   c
        e2 = b + c
        e = e1 + e2
        #
        assert e.to_list() == ["+", ["*", "5.000000", "a"], ["+", "b", "c"]]

        #            +
        #          /   \
        #         +     *
        #        / \   / \
        #       b   c a   5
        e2 = b + c
        e = e2 + e1
        #
        assert e.to_list() == ["+", "b", "c", ["*", "5.000000", "a"]]

    if numpy_available:
        #       +
        #      / \
        #     *   b
        #    / \
        #   a   5
        e1 = a * np.int32(5)
        e = e1 + b
        #
        assert e.to_list() == ["+", ["*", "5.000000", "a"], "b"]

        #       +
        #      / \
        #     b   *
        #        / \
        #       a   5
        e = b + e1
        #
        assert e.to_list() == ["+", "b", ["*", "5.000000", "a"]]

        #            +
        #          /   \
        #         *     +
        #        / \   / \
        #       a   5 b   c
        e2 = b + c
        e = e1 + e2
        #
        assert e.to_list() == ["+", ["*", "5.000000", "a"], ["+", "b", "c"]]

        #            +
        #          /   \
        #         +     *
        #        / \   / \
        #       b   c a   5
        e2 = b + c
        e = e2 + e1
        #
        assert e.to_list() == ["+", "b", "c", ["*", "5.000000", "a"]]


def test_sum_affine(var_a, var_b, var_c, var_d):
    a, b, c, d = var_a, var_b, var_c, var_d

    e = affine_expression([1, 2, 3, 4], [a, b, c, d], 5)
    assert e.to_list() == [
        "+",
        "5.000000",
        "a",
        ["*", "2.000000", "b"],
        ["*", "3.000000", "c"],
        ["*", "4.000000", "d"],
    ]

    if numpy_available:
        e = affine_expression(
            [np.int32(1), np.int32(2), np.int32(3), np.int32(4)], [a, b, c, d], np.int32(5)
        )
        assert e.to_list() == [
            "+",
            "5.000000",
            "a",
            ["*", "2.000000", "b"],
            ["*", "3.000000", "c"],
            ["*", "4.000000", "d"],
        ]


def test_diff_error1(var_a):
    a = var_a
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() - a
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() - a


def test_diff_error2(var_a):
    a = var_a
    x = variable(10)
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:
            a - x
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:
            a - x


def test_simpleDiff(var_a, var_b):
    #
    # Check the structure of a simple difference with two variables
    #
    a, b = var_a, var_b

    #    -
    #   / \
    #  a   b
    e = a - b
    assert e.to_list() == ["+", "a", ["*", "-1.000000", "b"]]

    e = a
    e -= b
    assert e.to_list() == ["+", "a", ["*", "-1.000000", "b"]]


def test_constDiff(var_a):
    a = var_a

    if True:
        z = 5
        e = a - z
        assert e.to_list() == ["+", "a", "-5.000000"]

        e = z - a
        assert e.to_list() == ["+", "5.000000", ["*", "-1.000000", "a"]]

        e = a
        e -= z
        assert e.to_list() == ["+", "a", "-5.000000"]

        z = 5.0
        e = a - z
        assert e.to_list() == ["+", "a", "-5.000000"]

        e = z - a
        assert e.to_list() == ["+", "5.000000", ["*", "-1.000000", "a"]]

        e = a
        e -= z
        assert e.to_list() == ["+", "a", "-5.000000"]

    if numpy_available:
        z = np.int32(5)
        e = a - z
        assert e.to_list() == ["+", "a", "-5.000000"]

        e = z - a
        assert e.to_list() == ["+", "5.000000", ["*", "-1.000000", "a"]]

        e = a
        e -= z
        assert e.to_list() == ["+", "a", "-5.000000"]

        z = np.float32(5.0)
        e = a - z
        assert e.to_list() == ["+", "a", "-5.000000"]

        e = z - a
        assert e.to_list() == ["+", "5.000000", ["*", "-1.000000", "a"]]

        e = a
        e -= z
        assert e.to_list() == ["+", "a", "-5.000000"]


def test_paramDiff(var_a, param_p):
    # a - p
    a, p = var_a, param_p

    e = a - p
    #
    assert e.to_list() == ["+", "a", ["-", "p"]]

    # p - a
    e = p - a
    #
    assert e.to_list() == ["+", "p", ["*", "-1.000000", "a"]]


def test_dataDiff(var_a, data_d1):
    # a - p
    a, d1 = var_a, data_d1

    e = a - d1
    #
    assert e.to_list() == ["+", "a", ["-", "d1"]]

    # d1 - a
    e = d1 - a
    #
    assert e.to_list() == ["+", "d1", ["*", "-1.000000", "a"]]


def test_termDiff(var_a):
    #
    # Check the structure of a simple difference with a term
    #
    a = var_a

    #
    #   -
    #  / \
    # 5   *
    #    / \
    #   2   a
    #

    if True:
        e = 5 - 2 * a
        assert e.to_list() == ["+", "5.000000", ["*", "-2.000000", "a"]]

    if numpy_available:
        e = np.int32(5) - np.int32(2) * a
        assert e.to_list() == ["+", "5.000000", ["*", "-2.000000", "a"]]


def test_nestedDiff(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested differences
    #
    a, b, c, d = var_a, var_b, var_c, var_d

    if True:
        #       -
        #      / \
        #     -   5
        #    / \
        #   a   b

        e1 = a - b
        e = e1 - 5
        #
        assert e.to_list() == ["+", "a", ["*", "-1.000000", "b"], "-5.000000"]

        #       -
        #      / \
        #     5   -
        #        / \
        #       a   b

        e1 = a - b
        e = 5 - e1
        #
        assert e.to_list() == ["+", "5.000000", ["-", ["+", "a", ["*", "-1.000000", "b"]]]]

    if numpy_available:
        #       -
        #      / \
        #     -   5
        #    / \
        #   a   b

        e1 = a - b
        e = e1 - np.int32(5)
        #
        assert e.to_list() == ["+", "a", ["*", "-1.000000", "b"], "-5.000000"]

        #       -
        #      / \
        #     5   -
        #        / \
        #       a   b

        e1 = a - b
        e = np.int32(5) - e1
        #
        assert e.to_list() == ["+", "5.000000", ["-", ["+", "a", ["*", "-1.000000", "b"]]]]

    #       -
    #      / \
    #     -   c
    #    / \
    #   a   b

    e1 = a - b
    e = e1 - c
    #
    assert e.to_list() == ["+", "a", ["*", "-1.000000", "b"], ["*", "-1.000000", "c"]]

    #            -
    #          /   \
    #         -     -
    #        / \   / \
    #       a   b c   d

    e1 = a - b
    e2 = c - d
    e = e1 - e2
    #
    assert e.to_list() == [
        "+",
        "a",
        ["*", "-1.000000", "b"],
        ["-", ["+", "c", ["*", "-1.000000", "d"]]],
    ]


def test_negation_param(param_p):
    #
    # Check logic for negations
    #
    p = param_p

    e = -p
    #
    assert e.to_list() == ["-", "p"]

    e = -e
    #
    # TODO: Identify negations of negations
    #
    assert e.to_list() == ["-", ["-", "p"]]


def test_negation_terms(param_p, var_v):
    #
    # Check logic for negations with terms
    #
    p, v = param_p, var_v

    e = -p * v
    assert e.to_list() == ["*", ["-", "p"], "v"]

    e = -e
    assert e.to_list() == ["-", ["*", ["-", "p"], "v"]]
    #
    if True:
        e = -5 * v
        assert e.to_list() == ["*", "-5.000000", "v"]
        e = -e
        assert e.to_list() == ["*", "5.000000", "v"]

    if numpy_available:
        e = np.int32(-5) * v
        assert e.to_list() == ["*", "-5.000000", "v"]
        e = -e
        assert e.to_list() == ["*", "5.000000", "v"]


def test_trivialDiff(var_a, param_p):
    #
    # Check that subtracting zero doesn't change the expression
    #
    a, p = var_a, param_p

    if True:
        # a - 0
        e = a - 0
        assert e.to_list() == ["a"]

        e = a - 0.0
        assert e.to_list() == ["a"]

        e = a
        e -= 0
        assert e.to_list() == ["a"]

        e = a
        e -= 0.0
        assert e.to_list() == ["a"]

        # 0 - a
        e = 0 - a
        assert e.to_list() == ["*", "-1.000000", "a"]

        e = 0.0 - a
        assert e.to_list() == ["*", "-1.000000", "a"]

        # p - 0
        e = p - 0
        assert e.to_list() == ["p"]

        # 0 - p
        e = 0 - p
        assert e.to_list() == ["-", "p"]

        # 0 - 5*a
        e = 0 - 5 * a
        assert e.to_list() == ["*", "-5.000000", "a"]

        # 0 - p*a
        e = 0 - p * a
        assert e.to_list() == ["-", ["*", "p", "a"]]

        # 0 - a*a
        e = 0 - a * a
        assert e.to_list() == ["-", ["*", "a", "a"]]

    if numpy_available:
        z = np.int32(0)
        # 0 - 5*a
        e = z - np.int32(5) * a
        assert e.to_list() == ["*", "-5.000000", "a"]

        # 0 - p*a
        e = z - p * a
        assert e.to_list() == ["-", ["*", "p", "a"]]

        # 0 - a*a
        e = z - a * a

        # a - 0
        e = a - z
        assert e.to_list() == ["a"]

        e = a
        e -= z
        assert e.to_list() == ["a"]

        # 0 - a
        e = z - a
        assert e.to_list() == ["*", "-1.000000", "a"]

        # p - 0
        e = p - z
        assert e.to_list() == ["p"]

        # z - p
        e = 0 - p
        assert e.to_list() == ["-", "p"]

        z = np.float32(0.0)
        e = a - z
        assert e.to_list() == ["a"]

        e = a
        e -= z
        assert e.to_list() == ["a"]

        e = z - a
        assert e.to_list() == ["*", "-1.000000", "a"]


def test_sumOf_nestedTrivialProduct2(var_a, var_b, var_c):
    #
    # Check the structure of sum of products
    #
    a, b, c = var_a, var_b, var_c

    if True:
        #       -
        #      / \
        #     *   b
        #    / \
        #   a   5
        e1 = a * 5
        e = e1 - b
        assert e.to_list() == ["+", ["*", "5.000000", "a"], ["*", "-1.000000", "b"]]

        #       -
        #      / \
        #     b   *
        #        / \
        #       a   5
        e1 = a * 5
        e = b - e1
        assert e.to_list() == ["+", "b", ["*", "-5.000000", "a"]]

        #            -
        #          /   \
        #         *     -
        #        / \   / \
        #       a   5 b   c
        e1 = a * 5
        e2 = b - c
        e = e1 - e2
        assert e.to_list() == [
            "+",
            ["*", "5.000000", "a"],
            ["-", ["+", "b", ["*", "-1.000000", "c"]]],
        ]

        #            -
        #          /   \
        #         -     *
        #        / \   / \
        #       b   c a   5
        e1 = a * 5
        e2 = b - c
        e = e2 - e1
        assert e.to_list() == ["+", "b", ["*", "-1.000000", "c"], ["*", "-5.000000", "a"]]

    if numpy_available:
        #       -
        #      / \
        #     *   b
        #    / \
        #   a   5
        e1 = a * np.int32(5)
        e = e1 - b
        assert e.to_list() == ["+", ["*", "5.000000", "a"], ["*", "-1.000000", "b"]]

        #       -
        #      / \
        #     b   *
        #        / \
        #       a   5
        e1 = a * np.int32(5)
        e = b - e1
        assert e.to_list() == ["+", "b", ["*", "-5.000000", "a"]]

        #            -
        #          /   \
        #         *     -
        #        / \   / \
        #       a   5 b   c
        e1 = a * np.int32(5)
        e2 = b - c
        e = e1 - e2
        assert e.to_list() == [
            "+",
            ["*", "5.000000", "a"],
            ["-", ["+", "b", ["*", "-1.000000", "c"]]],
        ]

        #            -
        #          /   \
        #         -     *
        #        / \   / \
        #       b   c a   5
        e1 = a * np.int32(5)
        e2 = b - c
        e = e2 - e1
        assert e.to_list() == ["+", "b", ["*", "-1.000000", "c"], ["*", "-5.000000", "a"]]


#
# Multiply
#


def test_mul_error1(var_a):
    a = var_a
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() * a
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() * a


def test_mul_error2(var_a):
    a = var_a
    x = variable(10)
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:
            a * x
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:
            a * x


def test_simpleProduct(var_a, var_b):
    #
    # Check the structure of a simple product of variables
    #
    a, b = var_a, var_b

    #    *
    #   / \
    #  a   b
    e = a * b
    assert e.to_list() == ["*", "a", "b"]

    #    *
    #   / \
    #  a   b
    e = a
    e *= b
    assert e.to_list() == ["*", "a", "b"]


def test_constProduct(var_a):
    #
    # Check the structure of a simple product with a constant
    #
    a = var_a

    if True:
        #    *
        #   / \
        #  a   5
        e = a * 5
        assert e.to_list() == ["*", "5.000000", "a"]

        e = a * 5.0
        assert e.to_list() == ["*", "5.000000", "a"]

        e = a
        e *= 5
        assert e.to_list() == ["*", "5.000000", "a"]

        #    *
        #   / \
        #  5   a
        e = 5.0 * a
        assert e.to_list() == ["*", "5.000000", "a"]

        e = a
        e *= 5.0
        assert e.to_list() == ["*", "5.000000", "a"]

        e = 5.0 * a
        assert e.to_list() == ["*", "5.000000", "a"]

    if numpy_available:
        #    *
        #   / \
        #  a   5
        e = a * np.int32(5)
        assert e.to_list() == ["*", "5.000000", "a"]

        e = a * np.float32(5.0)
        assert e.to_list() == ["*", "5.000000", "a"]

        e = a
        e *= np.int32(5)
        assert e.to_list() == ["*", "5.000000", "a"]

        #    *
        #   / \
        #  5   a
        e = np.float32(5.0) * a
        assert e.to_list() == ["*", "5.000000", "a"]

        e = a
        e *= np.float32(5.0)
        assert e.to_list() == ["*", "5.000000", "a"]

        e = np.float32(5.0) * a
        assert e.to_list() == ["*", "5.000000", "a"]


def test_nestedProduct(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested products
    #
    a, b, c, d = var_a, var_b, var_c, var_d

    if True:
        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * 5
        assert e.to_list() == ["*", ["*", "a", "b"], "5.000000"]

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = 5 * e1
        assert e.to_list() == ["*", "5.000000", ["*", "a", "b"]]

    if numpy_available:
        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * np.int32(5)
        assert e.to_list() == ["*", ["*", "a", "b"], "5.000000"]

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = np.int32(5) * e1
        assert e.to_list() == ["*", "5.000000", ["*", "a", "b"]]

    #       *
    #      / \
    #     *   c
    #    / \
    #   a   b
    e1 = a * b
    e = e1 * c
    assert e.to_list() == ["*", ["*", "a", "b"], "c"]

    #       *
    #      / \
    #     c   *
    #        / \
    #       a   b
    e1 = a * b
    e = c * e1
    assert e.to_list() == ["*", "c", ["*", "a", "b"]]

    #            *
    #          /   \
    #         *     *
    #        / \   / \
    #       a   b c   d
    e1 = a * b
    e2 = c * d
    e = e1 * e2
    assert e.to_list() == ["*", ["*", "a", "b"], ["*", "c", "d"]]


def test_nestedProduct2(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested products
    #
    a, b, c, d = var_a, var_b, var_c, var_d

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
    assert e.to_list() == ["*", ["+", "c", ["+", "a", "b"]], ["+", "a", "b", "d"]]

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
    assert e.to_list() == ["*", ["*", "c", ["+", "a", "b"]], ["*", ["+", "a", "b"], "d"]]


def test_nestedProduct3(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested products
    #
    a, b, c, d = var_a, var_b, var_c, var_d

    if True:
        #       *
        #      / \
        #     *   5
        #    / \
        #   3   b
        e1 = 3 * b
        e = e1 * 5
        assert e.to_list() == ["*", ["*", "3.000000", "b"], "5.000000"]

        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * 5
        assert e.to_list() == ["*", ["*", "a", "b"], "5.000000"]

        #       *
        #      / \
        #     5   *
        #        / \
        #       3   b
        e1 = 3 * b
        e = 5 * e1
        assert e.to_list() == ["*", "5.000000", ["*", "3.000000", "b"]]

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = 5 * e1
        assert e.to_list() == ["*", "5.000000", ["*", "a", "b"]]

    if numpy_available:
        #       *
        #      / \
        #     *   5
        #    / \
        #   3   b
        e1 = np.int32(3) * b
        e = e1 * np.int32(5)
        assert e.to_list() == ["*", ["*", "3.000000", "b"], "5.000000"]

        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * np.int32(5)
        assert e.to_list() == ["*", ["*", "a", "b"], "5.000000"]

        #       *
        #      / \
        #     5   *
        #        / \
        #       3   b
        e1 = np.int32(3) * b
        e = np.int32(5) * e1
        assert e.to_list() == ["*", "5.000000", ["*", "3.000000", "b"]]

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = np.int32(5) * e1
        assert e.to_list() == ["*", "5.000000", ["*", "a", "b"]]

    #       *
    #      / \
    #     *   c
    #    / \
    #   a   b
    e1 = a * b
    e = e1 * c
    assert e.to_list() == ["*", ["*", "a", "b"], "c"]

    #       *
    #      / \
    #     c   *
    #        / \
    #       a   b
    e1 = a * b
    e = c * e1
    assert e.to_list() == ["*", "c", ["*", "a", "b"]]

    #            *
    #          /   \
    #         *     *
    #        / \   / \
    #       a   b c   d
    e1 = a * b
    e2 = c * d
    e = e1 * e2
    assert e.to_list() == ["*", ["*", "a", "b"], ["*", "c", "d"]]


def test_trivialProduct(var_a, param_p, param_q, param_r):
    #
    # Check that multiplying by zero gives zero
    #
    a, p, q, r = var_a, param_p, param_q, param_r

    if True:
        e = a * 0
        assert e.value == 0

        e = a * 0.0
        assert e.value == 0.0

        e = a
        e *= 0
        assert e.value == 0

        e = a
        e *= 0.0
        assert e.value == 0.0

        e = 0 * a
        assert e.value == 0

        e = 0.0 * a
        assert e.value == 0

    if numpy_available:
        z = np.int32(0)
        e = a * z
        assert e.value == 0

        e = a
        e *= z
        assert e.value == 0

        e = z * a
        assert e.value == 0

        z = np.float32(0.0)
        e = a * z
        assert e.value == 0.0

        e = a
        e *= z
        assert e.value == 0.0

        e = z * a
        assert e.value == 0

    e = a * p
    assert e.to_list() == ["*", "a", "p"]

    e = p * a
    assert e.to_list() == ["*", "p", "a"]

    #
    # Check that multiplying by one gives the original expression
    #
    if True:
        e = a * 1
        assert e.to_list() == ["a"]

        e = a
        e *= 1
        assert e.to_list() == ["a"]

        e = 1 * a
        assert e.to_list() == ["a"]

        e = a * 1.0
        assert e.to_list() == ["a"]

        e = a
        e *= 1.0
        assert e.to_list() == ["a"]

        e = 1.0 * a
        assert e.to_list() == ["a"]

    if numpy_available:
        z = np.int32(1)
        e = a * z
        assert e.to_list() == ["a"]

        e = a
        e *= z
        assert e.to_list() == ["a"]

        e = z * a
        assert e.to_list() == ["a"]

        z = np.float32(1.0)
        e = a * z
        assert e.to_list() == ["a"]

        e = a
        e *= z
        assert e.to_list() == ["a"]

        e = z * a
        assert e.to_list() == ["a"]


def test_div_error1(var_a):
    a = var_a
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() / a
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() / a


def test_div_error2(var_a):
    a = var_a
    x = variable(10)
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:
            a / x
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:
            a / x


def test_simpleDivision(var_a, var_b):
    #
    # Check the structure of a simple division with variables
    #
    a, b = var_a, var_b

    #    /
    #   / \
    #  a   b
    e = a / b
    assert e.to_list() == ["/", "a", "b"]

    e = a
    e /= b
    assert e.to_list() == ["/", "a", "b"]


def test_constDivision(var_a):
    #
    # Check the structure of a simple division with a constant
    #
    a = var_a

    if True:
        e = a / 5
        assert e.to_list() == ["*", "0.200000", "a"]

        e = 5 / a
        assert e.to_list() == ["/", "5.000000", "a"]

        e = a
        e /= 5
        assert e.to_list() == ["*", "0.200000", "a"]

        e = a / 5.0
        assert e.to_list() == ["*", "0.200000", "a"]

        e = 5.0 / a
        assert e.to_list() == ["/", "5.000000", "a"]

        e = a
        e /= 5.0
        assert e.to_list() == ["*", "0.200000", "a"]

    if numpy_available:
        z = np.int32(5)
        e = a / z
        assert e.to_list() == ["*", "0.200000", "a"]

        e = z / a
        assert e.to_list() == ["/", "5.000000", "a"]

        e = a
        e /= z
        assert e.to_list() == ["*", "0.200000", "a"]

        z = np.float32(5.0)
        e = a / z
        assert e.to_list() == ["*", "0.200000", "a"]

        e = z / a
        assert e.to_list() == ["/", "5.000000", "a"]

        e = a
        e /= z
        assert e.to_list() == ["*", "0.200000", "a"]


def test_nestedDivision(var_a, var_b, var_c, var_d):
    #
    # Check the structure of nested divisions
    #
    a, b, c, d = var_a, var_b, var_c, var_d

    if True:
        #       /
        #      / \
        #     *   5
        #    / \
        #   3   b
        e1 = 3 * b
        e = e1 / 5
        assert e.to_list() == ["/", ["*", "3.000000", "b"], "5.000000"]

        #       /
        #      / \
        #     /   5
        #    / \
        #   a   b
        e1 = a / b
        e = e1 / 5
        assert e.to_list() == ["/", ["/", "a", "b"], "5.000000"]

        #       /
        #      / \
        #     5   /
        #        / \
        #       a   b
        e1 = a / b
        e = 5 / e1

    if numpy_available:
        #       /
        #      / \
        #     *   5
        #    / \
        #   3   b
        e1 = 3 * b
        e = e1 / np.int32(5)
        assert e.to_list() == ["/", ["*", "3.000000", "b"], "5.000000"]

        #       /
        #      / \
        #     /   5
        #    / \
        #   a   b
        e1 = a / b
        e = e1 / np.int32(5)
        assert e.to_list() == ["/", ["/", "a", "b"], "5.000000"]

        #       /
        #      / \
        #     5   /
        #        / \
        #       a   b
        e1 = a / b
        e = np.int32(5) / e1
        assert e.to_list() == ["/", "5.000000", ["/", "a", "b"]]

    #       /
    #      / \
    #     /   c
    #    / \
    #   a   b
    e1 = a / b
    e = e1 / c
    assert e.to_list() == ["/", ["/", "a", "b"], "c"]

    #       /
    #      / \
    #     c   /
    #        / \
    #       a   b
    e1 = a / b
    e = c / e1
    assert e.to_list() == ["/", "c", ["/", "a", "b"]]

    #            /
    #          /   \
    #         /     /
    #        / \   / \
    #       a   b c   d
    e1 = a / b
    e2 = c / d
    e = e1 / e2
    assert e.to_list() == ["/", ["/", "a", "b"], ["/", "c", "d"]]


def test_trivialDivision(var_a, param_p, param_q, param_r):
    #
    # Check that dividing by zero generates an exception
    #
    a, p, q, r = var_a, param_p, param_q, param_r

    if poek.__using_pybind11__:
        with pytest.raises(ValueError) as einfo:
            a.__truediv__(0)
        with pytest.raises(ValueError) as einfo:
            a.__truediv__(0.0)
    else:
        try:
            a.__truediv__(0)
            pytest.fail("Expected TypeError when dividing by zero.")
            print("Y")
        except TypeError:
            pass
        try:
            a.__truediv__(0.0)
            pytest.fail("Expected TypeError when dividing by zero.")
            print("Y")
        except TypeError:
            pass

    #
    # Check that dividing zero by anything non-zero gives zero
    #
    if True:
        e = 0 / a
        assert e.to_list() == ["0.000000"]
        assert e.value == pytest.approx(0.0)

        e = 0.0 / a
        assert e.to_list() == ["0.000000"]
        assert e.value == pytest.approx(0.0)

    if numpy_available:
        e = np.int32(0) / a
        assert e.to_list() == ["0.000000"]
        assert e.value == pytest.approx(0.0)

        e = np.float32(0.0) / a
        assert e.to_list() == ["0.000000"]
        assert e.value == pytest.approx(0.0)

    #
    # Check that dividing by one 1 gives the original expression
    #
    if True:
        e = a / 1
        assert e.to_list() == ["a"]

        e = a
        e /= 1
        assert e.to_list() == ["a"]

        e = a / 1.0
        assert e.to_list() == ["a"]

        e = a
        e /= 1.0
        assert e.to_list() == ["a"]

    if numpy_available:
        z = np.int32(1)
        e = a / z
        assert e.to_list() == ["a"]

        e = a
        e /= z
        assert e.to_list() == ["a"]

        z = np.float32(1.0)
        e = a / z
        assert e.to_list() == ["a"]

        e = a
        e /= z
        assert e.to_list() == ["a"]

    e = a / r
    assert e.to_list() == ["/", "a", "r"]

    #
    # Check the structure dividing 1 by an expression
    #
    if True:
        e = 1 / a
        assert e.to_list() == ["/", "1.000000", "a"]

        e = 1.0 / a
        assert e.to_list() == ["/", "1.000000", "a"]

    if numpy_available:
        e = np.int32(1) / a
        assert e.to_list() == ["/", "1.000000", "a"]

        e = np.float32(1.0) / a
        assert e.to_list() == ["/", "1.000000", "a"]

    #
    # Check the structure dividing 1 by a mutable zero
    #
    if True:
        e = 1 / p
        assert e.to_list() == ["/", "1.000000", "p"]

        e = 1.0 / p
        assert e.to_list() == ["/", "1.000000", "p"]

    if numpy_available:
        e = np.int32(1) / p
        assert e.to_list() == ["/", "1.000000", "p"]

        e = np.float32(1.0) / p
        assert e.to_list() == ["/", "1.000000", "p"]


def test_pow_error1(var_a):
    a = var_a
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() ** a
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:

            class TMP(object):
                pass

            TMP() ** a


def test_pow_error2(var_a):
    a = var_a
    x = variable(10)
    if poek.__using_pybind11__:
        with pytest.raises(TypeError) as einfo:
            a**x
    elif poek.__using_cppyy__:
        with pytest.raises(TypeError) as einfo:
            a**x


def test_simplePow(var_a, var_b):
    a, b = var_a, var_b

    e = a**b
    assert e.to_list() == ["pow", "a", "b"]

    e = a
    e **= b
    assert e.to_list() == ["pow", "a", "b"]


def test_constPow(var_a):
    #
    # Check the structure of a simple power with a constant
    #
    a = var_a

    if True:
        e = a**5
        assert e.to_list() == ["pow", "a", "5.000000"]

        e = 5**a
        assert e.to_list() == ["pow", "5.000000", "a"]

        e = a
        e **= 5
        assert e.to_list() == ["pow", "a", "5.000000"]

        e = a**5.0
        assert e.to_list() == ["pow", "a", "5.000000"]

        e = 5.0**a
        assert e.to_list() == ["pow", "5.000000", "a"]

        e = a
        e **= 5.0
        assert e.to_list() == ["pow", "a", "5.000000"]

    if numpy_available:
        z = np.int32(5)
        e = a**z
        assert e.to_list() == ["pow", "a", "5.000000"]

        e = z**a
        assert e.to_list() == ["pow", "5.000000", "a"]

        e = a
        e **= z
        assert e.to_list() == ["pow", "a", "5.000000"]

        z = np.float32(5.0)
        e = a**z
        assert e.to_list() == ["pow", "a", "5.000000"]

        e = z**a
        assert e.to_list() == ["pow", "5.000000", "a"]

        e = a
        e **= z
        assert e.to_list() == ["pow", "a", "5.000000"]


def test_trivialPow(var_a, param_p, param_q, param_r):
    #
    a, p, q, r = var_a, param_p, param_q, param_r

    #
    # Check that taking the first power returns the original object
    #
    if True:
        e = a**1
        assert e.to_list() == ["a"]

        e = a
        e **= 1
        assert e.to_list() == ["a"]

        e = a**1.0
        assert e.to_list() == ["a"]

        e = a
        e **= 1.0
        assert e.to_list() == ["a"]

    if numpy_available:
        z = np.int32(1)
        e = a**z
        assert e.to_list() == ["a"]

        e = a
        e **= z
        assert e.to_list() == ["a"]

        z = np.float32(1.0)
        e = a**z
        assert e.to_list() == ["a"]

        e = a
        e **= z
        assert e.to_list() == ["a"]

    e = a**r
    assert e.to_list() == ["pow", "a", "r"]

    #
    # Check that taking the zeroth power returns one
    #
    if True:
        e = a**0
        assert e.to_list() == ["1.000000"]

        e = a
        e **= 0
        assert e.to_list() == ["1.000000"]

        e = a**0.0
        assert e.to_list() == ["1.000000"]

        e = a
        e **= 0.0
        assert e.to_list() == ["1.000000"]

    if numpy_available:
        z = np.int32(0)
        e = a**z
        assert e.to_list() == ["1.000000"]

        e = a
        e **= z
        assert e.to_list() == ["1.000000"]

        z = np.float32(0.0)
        e = a**z
        assert e.to_list() == ["1.000000"]

        e = a
        e **= z
        assert e.to_list() == ["1.000000"]

    e = a**q
    assert e.to_list() == ["pow", "a", "q"]

    #
    # Check that taking powers of 0 and 1 are easy
    #
    if True:
        e = 0**a
        assert e.to_list() == ["0.000000"]

        e = 0.0**a
        assert e.to_list() == ["0.000000"]

        e = 1**a
        assert e.to_list() == ["1.000000"]

        e = 1.0**a
        assert e.to_list() == ["1.000000"]

    if numpy_available:
        e = np.int32(0) ** a
        assert e.to_list() == ["0.000000"]

        e = np.float32(0.0) ** a
        assert e.to_list() == ["0.000000"]

        e = np.int32(1) ** a
        assert e.to_list() == ["1.000000"]

        e = np.float32(1.0) ** a
        assert e.to_list() == ["1.000000"]


def test_trivialRPow(var_a, param_p, param_q, param_r):
    #
    a, p, q, r = var_a, param_p, param_q, param_r

    #
    # Check that taking any power of 1 is 1
    #
    if True:
        e = 1**a
        assert e.to_list() == ["1.000000"]

        e = 1.0**a
        assert e.to_list() == ["1.000000"]

    if numpy_available:
        e = np.int32(1) ** a
        assert e.to_list() == ["1.000000"]

        e = np.float32(1.0) ** a
        assert e.to_list() == ["1.000000"]

    e = r**a
    assert e.to_list() == ["pow", "r", "a"]

    #
    # Check that taking the zeroth power returns one
    #
    if True:
        e = 0**a
        assert e.to_list() == ["0.000000"]

        e = 0.0**a
        assert e.to_list() == ["0.000000"]

    if numpy_available:
        e = np.int32(0) ** a
        assert e.to_list() == ["0.000000"]

        e = np.float32(0.0) ** a
        assert e.to_list() == ["0.000000"]


def test_pow_expresions(var_a, param_r):
    a, r = var_a, param_r

    e = a**r
    assert e.to_list() == ["pow", "a", "r"]

    if True:
        e = a**2
        assert e.to_list() == ["pow", "a", "2.000000"]

        e = a**2.0
        assert e.to_list() == ["pow", "a", "2.000000"]

        e = 2**a
        assert e.to_list() == ["pow", "2.000000", "a"]

        e = 2.0**a
        assert e.to_list() == ["pow", "2.000000", "a"]

    if numpy_available:
        z = np.int32(2)
        e = a**z
        assert e.to_list() == ["pow", "a", "2.000000"]

        e = z**a
        assert e.to_list() == ["pow", "2.000000", "a"]

        z = np.float32(2.0)
        e = a**z
        assert e.to_list() == ["pow", "a", "2.000000"]

        e = z**a
        assert e.to_list() == ["pow", "2.000000", "a"]


#
# Expression entanglement tests
#


def test_sumexpr_add_entangled(var_a):
    a = var_a
    e = a * 2 + 1
    assert e.to_list() == ["+", ["*", "2.000000", "a"], "1.000000"]
    e += 1
    assert e.to_list() == ["+", ["*", "2.000000", "a"], "1.000000", "1.000000"]


def test_entangled_test1(var_a, var_b, var_c, var_d):
    a, b, c, d = var_a, var_b, var_c, var_d
    e1 = a + b
    e2 = c + e1
    e3 = d + e1

    assert e1.to_list() == ["+", "a", "b"]
    assert e2.to_list() == ["+", "c", ["+", "a", "b"]]
    assert e3.to_list() == ["+", "d", ["+", "a", "b"]]


#
# Variable Tests
#


def test_var_default_value():
    v = variable(3, name="v")
    assert math.isnan(v[0].value)
    assert math.isnan(v[1].value)
    assert math.isnan(v[2].value)


def test_var_initialize():
    v = variable(3, name="v", value=3)
    assert v[0].value, 3
    assert v[1].value, 3
    assert v[2].value, 3


def test_var_iterator():
    v = variable(3, name="v")
    for i in v:
        assert math.isnan(v[i].value)


def test_var_getitem1():
    v = variable(3, name="v")
    v1 = v[0]
    v2 = v[0]
    assert v1.name == v2.name


def test_var_getitem2():
    v = variable(3)
    v1 = v[0]
    v2 = v[0]
    assert v1.name == v2.name


def test_name1():
    v = variable(3, name="v", value=3).generate_names()
    assert v.name == "v"
    assert v[0].name == "v[0]"


def test_name2():
    v = variable(3, value=3)
    assert v.name == "X"
    assert v[0].name[0] == "X"


def test_name_single():
    v = variable(value=3, name="y")
    assert v.name == "y"
    v = variable(value=3)
    assert v.name[0] == "X"


#
# ND Variable Tests
#
def test_ndvar_default_value():
    v = variable((3, 2, 4), name="v")
    assert math.isnan(v[0, 1, 3].value)
    assert math.isnan(v[1, 0, 0].value)
    assert math.isnan(v[2, 1, 2].value)


def test_ndvar_initialize():
    v = variable((3, 2, 4), name="v", value=3)
    assert v[0, 1, 3].value == 3
    assert v[1, 0, 0].value == 3
    assert v[2, 1, 2].value == 3


def test_ndvar_iterator():
    v = variable((3, 2, 4), name="v")
    for i in v:
        assert math.isnan(v[i].value)


def test_ndvar_getitem1():
    v = variable((3, 2, 4), name="v")
    v1 = v[0, 0, 0]
    v2 = v[0, 0, 0]
    assert v1.name == v2.name


def test_ndvar_getitem2():
    v = variable((3, 2, 4))
    v1 = v[0, 0, 0]
    v2 = v[0, 0, 0]
    assert v1.name == v2.name


def test_ndvar_name1():
    v = variable((3, 2, 4), name="v", value=3).generate_names()
    assert v.name, "v"
    assert v[2, 1, 3].name == "v[2,1,3]"
    assert v[0, 0, 0].name == "v[0,0,0]"


def test_ndvar_name2():
    v = variable((3, 2, 4), value=3)
    assert v.name == "X"
    assert v[2, 1, 3].name[0] == "X"
    assert v[0, 0, 0].name[0] == "X"
