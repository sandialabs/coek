import math
import pyutilib.th as unittest

from poek import *
import poek


class TestValue(unittest.TestCase):
    def test_var(self):
        p = variable(value=2)
        self.assertEqual(p.value, 2)
        p.value = 3
        self.assertEqual(p.value, 3)

    def test_expr(self):
        p = variable(value=2)
        self.assertEqual(p.value, 2)
        e = p + 3
        self.assertEqual(e.value, 5)
        with self.assertRaisesRegex(AttributeError, "can't set attribute"):
            e.value = 0

    def test_constraint(self):
        p = variable(value=2)
        self.assertEqual(p.value, 2)
        e = p < 3
        self.assertEqual(e.value, 2)
        with self.assertRaisesRegex(AttributeError, "can't set attribute"):
            e.value = 0

    def test_param1(self):
        p = parameter(value=-1)
        self.assertEqual(p.value, -1)
        p.value = 3
        self.assertEqual(p.value, 3)

    def test_float(self):
        p = parameter(value=-1)
        with self.assertRaisesRegex(TypeError, "float\\(\\) argument must.*"):
            float(p)

    def test_int(self):
        p = parameter(value=-1)
        with self.assertRaisesRegex(TypeError, "int\\(\\) argument must.*"):
            int(p)


class Test_SumExpression(unittest.TestCase):
    def setUp(self):
        self.a = variable(name="a")
        self.b = variable(name="b")
        self.c = variable(name="c")
        self.d = variable(name="d")
        self.p = parameter("p", value=0)

    def test_error1(self):
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__radd__\\(\\): incompatible function arguments.*"
            ):

                class TMP(object):
                    pass

                TMP() + self.a
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):

                class TMP(object):
                    pass

                TMP() + self.a

    def test_error2(self):
        x = variable(10)
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError, "unsupported operand type.*"):
                self.a + x
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):
                self.a + x

    def test_simpleSum(self):
        # a + b
        a = self.a
        b = self.b

        e = a + b
        self.assertEqual(e.to_list(), ["+", "a", "b"])

        e = a
        e += b
        self.assertEqual(e.to_list(), ["+", "a", "b"])

    def test_simpleSum_API(self):
        # a + b + 2*a
        a = self.a
        b = self.b
        e = a + b
        e += 2 * a
        #
        self.assertEqual(e.to_list(), ["+", "a", "b", ["*", "2", "a"]])

    def test_constSum(self):
        # a + 5
        a = self.a

        e = a + 5
        self.assertEqual(e.to_list(), ["+", "a", "5.000000"])

        e = 5 + a
        self.assertEqual(e.to_list(), ["+", "5.000000", "a"])

        e = a
        e += 5
        self.assertEqual(e.to_list(), ["+", "a", "5.000000"])

        e = a + 5.0
        self.assertEqual(e.to_list(), ["+", "a", "5.000000"])

        e = 5.0 + a
        self.assertEqual(e.to_list(), ["+", "5.000000", "a"])

        e = a
        e += 5.0
        self.assertEqual(e.to_list(), ["+", "a", "5.000000"])

    def test_nestedSum(self):
        #
        # Check the structure of nested sums
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        #           +
        #          / \
        #         +   5
        #        / \
        #       a   b
        e1 = a + b
        e = e1 + 5
        #
        self.assertEqual(e.to_list(), ["+", "a", "b", "5.000000"])

        #       +
        #      / \
        #     5   +
        #        / \
        #       a   b
        e1 = a + b
        e = 5 + e1
        #
        self.assertEqual(e.to_list(), ["+", "5.000000", ["+", "a", "b"]])

        #           +
        #          / \
        #         +   c
        #        / \
        #       a   b
        e1 = a + b
        e = e1 + c
        #
        self.assertEqual(e.to_list(), ["+", "a", "b", "c"])

        #       +
        #      / \
        #     c   +
        #        / \
        #       a   b
        e1 = a + b
        e = c + e1
        #
        self.assertEqual(e.to_list(), ["+", "c", ["+", "a", "b"]])

        #            +
        #          /   \
        #         +     +
        #        / \   / \
        #       a   b c   d
        e1 = a + b
        e2 = c + d
        e = e1 + e2
        #
        self.assertEqual(e.to_list(), ["+", "a", "b", ["+", "c", "d"]])

    def test_nestedSum2(self):
        #
        # Check the structure of nested sums
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

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
        self.assertEqual(e.to_list(), ["+", ["*", "2.000000", ["+", "a", "b"]], "c"])

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
        self.assertEqual(
            e.to_list(),
            ["*", "3.000000", ["+", ["*", "2.000000", ["+", "a", "b"]], "c"]],
        )

    def test_trivialSum(self):
        #
        # Check that adding zero doesn't change the expression
        #
        a = self.a

        e = a + 0
        self.assertEqual(e.to_list(), ["a"])

        e = 0 + a
        self.assertEqual(e.to_list(), ["a"])

        e = a + 0.0
        self.assertEqual(e.to_list(), ["a"])

        e = 0.0 + a
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e += 0
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e += 0.0
        self.assertEqual(e.to_list(), ["a"])

        #
        # Adding zero to a sum will not change the sum
        #
        e = a + a
        f = e + 0
        self.assertEqual(e.to_list(), ["+", "a", "a"])

    def test_sumOf_nestedTrivialProduct(self):
        #
        # Check sums with nested products
        #
        a = self.a
        b = self.b
        c = self.c

        #       +
        #      / \
        #     *   b
        #    / \
        #   a   5
        e1 = a * 5
        e = e1 + b
        #
        self.assertEqual(e.to_list(), ["+", ["*", "5", "a"], "b"])

        #       +
        #      / \
        #     b   *
        #        / \
        #       a   5
        e = b + e1
        #
        self.assertEqual(e.to_list(), ["+", "b", ["*", "5", "a"]])

        #            +
        #          /   \
        #         *     +
        #        / \   / \
        #       a   5 b   c
        e2 = b + c
        e = e1 + e2
        #
        self.assertEqual(e.to_list(), ["+", ["*", "5", "a"], ["+", "b", "c"]])

        #            +
        #          /   \
        #         +     *
        #        / \   / \
        #       b   c a   5
        e2 = b + c
        e = e2 + e1
        #
        self.assertEqual(e.to_list(), ["+", "b", "c", ["*", "5", "a"]])

    def test_affine(self):
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        e = affine_expression([1, 2, 3, 4], [a, b, c, d], 5)
        #
        self.assertEqual(
            e.to_list(),
            ["+", "5.000000", "a", ["*", "2", "b"], ["*", "3", "c"], ["*", "4", "d"]],
        )


class TestDiffExpression(unittest.TestCase):
    def setUp(self):
        self.a = variable(name="a")
        self.b = variable(name="b")
        self.c = variable(name="c")
        self.d = variable(name="d")
        self.v = variable(name="v")
        self.p = parameter("p", value=0)

    def test_error1(self):
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__rsub__\\(\\): incompatible function arguments.*"
            ):

                class TMP(object):
                    pass

                TMP() - self.a
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):

                class TMP(object):
                    pass

                TMP() - self.a

    def test_error2(self):
        x = variable(10)
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__sub__\\(\\): incompatible function arguments.*"
            ):
                self.a - x
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):
                self.a - x

    def test_simpleDiff(self):
        #
        # Check the structure of a simple difference with two variables
        #
        a = self.a
        b = self.b

        #    -
        #   / \
        #  a   b
        e = a - b
        self.assertEqual(e.to_list(), ["+", "a", ["*", "-1", "b"]])

        e = a
        e -= b
        self.assertEqual(e.to_list(), ["+", "a", ["*", "-1", "b"]])

    def test_constDiff(self):
        a = self.a

        e = a - 5
        self.assertEqual(e.to_list(), ["+", "a", "-5.000000"])

        e = 5 - a
        self.assertEqual(e.to_list(), ["+", "5.000000", ["*", "-1", "a"]])

        e = a
        e -= 5
        self.assertEqual(e.to_list(), ["+", "a", "-5.000000"])

        e = a - 5.0
        self.assertEqual(e.to_list(), ["+", "a", "-5.000000"])

        e = 5.0 - a
        self.assertEqual(e.to_list(), ["+", "5.000000", ["*", "-1", "a"]])

        e = a
        e -= 5.0
        self.assertEqual(e.to_list(), ["+", "a", "-5.000000"])

    def test_paramDiff(self):
        # a - p
        a = self.a
        p = self.p
        e = a - p
        #
        self.assertEqual(e.to_list(), ["+", "a", ["-", "p"]])

        # p - a
        e = p - a
        #
        self.assertEqual(e.to_list(), ["+", "p", ["*", "-1", "a"]])

    def test_termDiff(self):
        #
        # Check the structure of a simple difference with a term
        #
        a = self.a

        #
        #   -
        #  / \
        # 5   *
        #    / \
        #   2   a
        #

        e = 5 - 2 * a
        #
        self.assertEqual(e.to_list(), ["+", "5.000000", ["*", "-2", "a"]])

    def test_nestedDiff(self):
        #
        # Check the structure of nested differences
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        #       -
        #      / \
        #     -   5
        #    / \
        #   a   b

        e1 = a - b
        e = e1 - 5
        #
        self.assertEqual(e.to_list(), ["+", "a", ["*", "-1", "b"], "-5.000000"])

        #       -
        #      / \
        #     5   -
        #        / \
        #       a   b

        e1 = a - b
        e = 5 - e1
        #
        self.assertEqual(e.to_list(), ["+", "5.000000", ["-", ["+", "a", ["*", "-1", "b"]]]])

        #       -
        #      / \
        #     -   c
        #    / \
        #   a   b

        e1 = a - b
        e = e1 - c
        #
        self.assertEqual(e.to_list(), ["+", "a", ["*", "-1", "b"], ["*", "-1", "c"]])

        #            -
        #          /   \
        #         -     -
        #        / \   / \
        #       a   b c   d

        e1 = a - b
        e2 = c - d
        e = e1 - e2
        #
        self.assertEqual(
            e.to_list(),
            ["+", "a", ["*", "-1", "b"], ["-", ["+", "c", ["*", "-1", "d"]]]],
        )

    def test_negation_param(self):
        #
        # Check logic for negations
        #

        p = self.p
        e = -p
        #
        self.assertEqual(e.to_list(), ["-", "p"])

        e = -e
        #
        # TODO: Can we detect negations of negations?
        #
        self.assertEqual(e.to_list(), ["-", ["-", "p"]])

    def test_negation_terms(self):
        #
        # Check logic for negations with terms
        #
        p = self.p
        v = self.v

        e = -p * v
        self.assertEqual(e.to_list(), ["*", ["-", "p"], "v"])

        e = -e
        self.assertEqual(e.to_list(), ["-", ["*", ["-", "p"], "v"]])
        #
        e = -5 * v
        self.assertEqual(e.to_list(), ["*", "-5", "v"])
        e = -e
        self.assertEqual(e.to_list(), ["*", "5", "v"])

    def test_trivialDiff(self):
        #
        # Check that subtracting zero doesn't change the expression
        #
        a = self.a
        p = self.p

        # a - 0
        e = a - 0
        self.assertEqual(e.to_list(), ["a"])

        e = a - 0.0
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e -= 0
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e -= 0.0
        self.assertEqual(e.to_list(), ["a"])

        # 0 - a
        e = 0 - a
        self.assertEqual(e.to_list(), ["*", "-1", "a"])

        e = 0.0 - a
        self.assertEqual(e.to_list(), ["*", "-1", "a"])

        # p - 0
        e = p - 0
        self.assertEqual(e.to_list(), ["p"])

        # 0 - p
        e = 0 - p
        self.assertEqual(e.to_list(), ["-", "p"])

        # 0 - 5*a
        e = 0 - 5 * a
        self.assertEqual(e.to_list(), ["*", "-5", "a"])

        # 0 - p*a
        e = 0 - p * a
        self.assertEqual(e.to_list(), ["-", ["*", "p", "a"]])

        # 0 - a*a
        e = 0 - a * a
        self.assertEqual(e.to_list(), ["-", ["*", "a", "a"]])

    def test_sumOf_nestedTrivialProduct2(self):
        #
        # Check the structure of sum of products
        #
        a = self.a
        b = self.b
        c = self.c

        #       -
        #      / \
        #     *   b
        #    / \
        #   a   5
        e1 = a * 5
        e = e1 - b
        self.assertEqual(e.to_list(), ["+", ["*", "5", "a"], ["*", "-1", "b"]])

        #       -
        #      / \
        #     b   *
        #        / \
        #       a   5
        e1 = a * 5
        e = b - e1
        self.assertEqual(e.to_list(), ["+", "b", ["*", "-5", "a"]])

        #            -
        #          /   \
        #         *     -
        #        / \   / \
        #       a   5 b   c
        e1 = a * 5
        e2 = b - c
        e = e1 - e2
        self.assertEqual(e.to_list(), ["+", ["*", "5", "a"], ["-", ["+", "b", ["*", "-1", "c"]]]])

        #            -
        #          /   \
        #         -     *
        #        / \   / \
        #       b   c a   5
        e1 = a * 5
        e2 = b - c
        e = e2 - e1
        self.assertEqual(e.to_list(), ["+", "b", ["*", "-1", "c"], ["*", "-5", "a"]])


class Test_MulExpression(unittest.TestCase):
    def setUp(self):
        self.a = variable(name="a")
        self.b = variable(name="b")
        self.c = variable(name="c")
        self.d = variable(name="d")
        self.v = variable(name="v")
        self.p = parameter("p", value=0)
        self.q = parameter("q", value=0)
        self.r = parameter("r", value=1)

    def test_error1(self):
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__rmul__\\(\\): incompatible function arguments.*"
            ):

                class TMP(object):
                    pass

                TMP() * self.a
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):

                class TMP(object):
                    pass

                TMP() * self.a

    def test_error2(self):
        x = variable(10)
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__mul__\\(\\): incompatible function arguments.*"
            ):
                self.a * x
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):
                self.a * x

    def test_simpleProduct(self):
        #
        # Check the structure of a simple product of variables
        #
        a = self.a
        b = self.b

        #    *
        #   / \
        #  a   b
        e = a * b
        self.assertEqual(e.to_list(), ["*", "a", "b"])

        #    *
        #   / \
        #  a   b
        e = a
        e *= b
        self.assertEqual(e.to_list(), ["*", "a", "b"])

    def test_constProduct(self):
        #
        # Check the structure of a simple product with a constant
        #
        a = self.a

        #    *
        #   / \
        #  a   5
        e = a * 5
        self.assertEqual(e.to_list(), ["*", "5", "a"])

        e = a * 5.0
        self.assertEqual(e.to_list(), ["*", "5", "a"])

        e = a
        e *= 5
        self.assertEqual(e.to_list(), ["*", "5", "a"])

        #    *
        #   / \
        #  5   a
        e = 5.0 * a
        self.assertEqual(e.to_list(), ["*", "5", "a"])

        e = a
        e *= 5.0
        self.assertEqual(e.to_list(), ["*", "5", "a"])

        e = 5.0 * a
        self.assertEqual(e.to_list(), ["*", "5", "a"])

    def test_nestedProduct(self):
        #
        # Check the structure of nested products
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * 5
        self.assertEqual(e.to_list(), ["*", ["*", "a", "b"], "5.000000"])

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = 5 * e1
        self.assertEqual(e.to_list(), ["*", "5.000000", ["*", "a", "b"]])

        #       *
        #      / \
        #     *   c
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * c
        self.assertEqual(e.to_list(), ["*", ["*", "a", "b"], "c"])

        #       *
        #      / \
        #     c   *
        #        / \
        #       a   b
        e1 = a * b
        e = c * e1
        self.assertEqual(e.to_list(), ["*", "c", ["*", "a", "b"]])

        #            *
        #          /   \
        #         *     *
        #        / \   / \
        #       a   b c   d
        e1 = a * b
        e2 = c * d
        e = e1 * e2
        self.assertEqual(e.to_list(), ["*", ["*", "a", "b"], ["*", "c", "d"]])

    def test_nestedProduct2(self):
        #
        # Check the structure of nested products
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

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
        self.assertEqual(e.to_list(), ["*", ["+", "c", ["+", "a", "b"]], ["+", "a", "b", "d"]])

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
        self.assertEqual(
            e.to_list(), ["*", ["*", "c", ["+", "a", "b"]], ["*", ["+", "a", "b"], "d"]]
        )

    def test_nestedProduct3(self):
        #
        # Check the structure of nested products
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        #       *
        #      / \
        #     *   5
        #    / \
        #   3   b
        e1 = 3 * b
        e = e1 * 5
        self.assertEqual(e.to_list(), ["*", ["*", "3", "b"], "5.000000"])

        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * 5
        self.assertEqual(e.to_list(), ["*", ["*", "a", "b"], "5.000000"])

        #       *
        #      / \
        #     5   *
        #        / \
        #       3   b
        e1 = 3 * b
        e = 5 * e1
        self.assertEqual(e.to_list(), ["*", "5.000000", ["*", "3", "b"]])

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = 5 * e1
        self.assertEqual(e.to_list(), ["*", "5.000000", ["*", "a", "b"]])

        #       *
        #      / \
        #     *   c
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * c
        self.assertEqual(e.to_list(), ["*", ["*", "a", "b"], "c"])

        #       *
        #      / \
        #     c   *
        #        / \
        #       a   b
        e1 = a * b
        e = c * e1
        self.assertEqual(e.to_list(), ["*", "c", ["*", "a", "b"]])

        #            *
        #          /   \
        #         *     *
        #        / \   / \
        #       a   b c   d
        e1 = a * b
        e2 = c * d
        e = e1 * e2
        self.assertEqual(e.to_list(), ["*", ["*", "a", "b"], ["*", "c", "d"]])

    def test_trivialProduct(self):
        #
        # Check that multiplying by zero gives zero
        #
        a = self.a
        p = self.p
        q = self.q
        r = self.r

        e = a * 0
        self.assertEqual(e.value, 0)

        e = a * 0.0
        self.assertEqual(e.value, 0.0)

        e = a
        e *= 0
        self.assertEqual(e.value, 0)

        e = a
        e *= 0.0
        self.assertEqual(e.value, 0.0)

        e = 0 * a
        self.assertEqual(e.value, 0)

        e = 0.0 * a
        self.assertEqual(e.value, 0)

        e = a * p
        self.assertEqual(e.to_list(), ["*", "a", "p"])

        e = p * a
        self.assertEqual(e.to_list(), ["*", "p", "a"])

        #
        # Check that multiplying by one gives the original expression
        #
        e = a * 1
        self.assertEqual(e.to_list(), ["a"])

        e = a * 1.0
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e *= 1
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e *= 1.0
        self.assertEqual(e.to_list(), ["a"])

        e = 1 * a
        self.assertEqual(e.to_list(), ["a"])

        e = 1.0 * a
        self.assertEqual(e.to_list(), ["a"])


class Test_DivExpression(unittest.TestCase):
    def setUp(self):
        self.a = variable(name="a")
        self.b = variable(name="b")
        self.c = variable(name="c")
        self.d = variable(name="d")
        self.v = variable(name="v")
        self.p = parameter("p", value=0)
        self.q = parameter("q", value=0)
        self.r = parameter("r", value=1)

    def test_error1(self):
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__rtruediv__\\(\\): incompatible function arguments.*"
            ):

                class TMP(object):
                    pass

                TMP() / self.a
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):

                class TMP(object):
                    pass

                TMP() / self.a

    def test_error2(self):
        x = variable(10)
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__truediv__\\(\\): incompatible function arguments.*"
            ):
                self.a / x
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):
                self.a / x

    def test_simpleDivision(self):
        #
        # Check the structure of a simple division with variables
        #
        a = self.a
        b = self.b

        #    /
        #   / \
        #  a   b
        e = a / b
        self.assertEqual(e.to_list(), ["/", "a", "b"])

        e = a
        e /= b
        self.assertEqual(e.to_list(), ["/", "a", "b"])

    def test_constDivision(self):
        #
        # Check the structure of a simple division with a constant
        #
        a = self.a

        e = a / 5
        self.assertEqual(e.to_list(), ["*", "0.2", "a"])

        e = 5 / a
        self.assertEqual(e.to_list(), ["/", "5.000000", "a"])

        e = a
        e /= 5
        self.assertEqual(e.to_list(), ["*", "0.2", "a"])

        e = a / 5.0
        self.assertEqual(e.to_list(), ["*", "0.2", "a"])

        e = 5.0 / a
        self.assertEqual(e.to_list(), ["/", "5.000000", "a"])

        e = a
        e /= 5.0
        self.assertEqual(e.to_list(), ["*", "0.2", "a"])

    def test_nestedDivision(self):
        #
        # Check the structure of nested divisions
        #
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        #       /
        #      / \
        #     *   5
        #    / \
        #   3   b
        e1 = 3 * b
        e = e1 / 5
        self.assertEqual(e.to_list(), ["/", ["*", "3", "b"], "5.000000"])

        #       /
        #      / \
        #     /   5
        #    / \
        #   a   b
        e1 = a / b
        e = e1 / 5
        self.assertEqual(e.to_list(), ["/", ["/", "a", "b"], "5.000000"])

        #       /
        #      / \
        #     5   /
        #        / \
        #       a   b
        e1 = a / b
        e = 5 / e1
        self.assertEqual(e.to_list(), ["/", "5.000000", ["/", "a", "b"]])

        #       /
        #      / \
        #     /   c
        #    / \
        #   a   b
        e1 = a / b
        e = e1 / c
        self.assertEqual(e.to_list(), ["/", ["/", "a", "b"], "c"])

        #       /
        #      / \
        #     c   /
        #        / \
        #       a   b
        e1 = a / b
        e = c / e1
        self.assertEqual(e.to_list(), ["/", "c", ["/", "a", "b"]])

        #            /
        #          /   \
        #         /     /
        #        / \   / \
        #       a   b c   d
        e1 = a / b
        e2 = c / d
        e = e1 / e2
        self.assertEqual(e.to_list(), ["/", ["/", "a", "b"], ["/", "c", "d"]])

    def test_trivialDivision(self):
        #
        # Check that dividing by zero generates an exception
        #
        a = self.a
        p = self.p
        q = self.q
        r = self.r

        if poek.__using_pybind11__:
            self.assertRaises(ValueError, a.__truediv__, 0)
            self.assertRaises(ValueError, a.__truediv__, 0.0)
        else:
            try:
                a.__truediv__(0)
                self.fail("Expected TypeError when dividing by zero.")
                print("Y")
            except TypeError:
                pass
            try:
                a.__truediv__(0.0)
                self.fail("Expected TypeError when dividing by zero.")
                print("Y")
            except TypeError:
                pass

        #
        # Check that dividing zero by anything non-zero gives zero
        #
        e = 0 / a
        self.assertEqual(e.to_list(), ["0.000000"])
        self.assertAlmostEqual(e.value, 0.0)

        e = 0.0 / a
        self.assertEqual(e.to_list(), ["0.000000"])
        self.assertAlmostEqual(e.value, 0.0)

        #
        # Check that dividing by one 1 gives the original expression
        #
        e = a / 1
        self.assertEqual(e.to_list(), ["a"])

        e = a / 1.0
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e /= 1
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e /= 1.0
        self.assertEqual(e.to_list(), ["a"])

        e = a / r
        self.assertEqual(e.to_list(), ["/", "a", "r"])

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / a
        self.assertEqual(e.to_list(), ["/", "1.000000", "a"])

        e = 1.0 / a
        self.assertEqual(e.to_list(), ["/", "1.000000", "a"])

        #
        # Check the structure dividing 1 by a mutable zero
        #
        e = 1 / p
        self.assertEqual(e.to_list(), ["/", "1.000000", "p"])

        e = 1.0 / p
        self.assertEqual(e.to_list(), ["/", "1.000000", "p"])


class Test_PowExpression(unittest.TestCase):
    def setUp(self):
        self.a = variable(name="a")
        self.b = variable(name="b")
        self.c = variable(name="c")
        self.d = variable(name="d")
        self.v = variable(name="v")
        self.p = parameter("p", value=0)
        self.q = parameter("q", value=0)
        self.r = parameter("r", value=1)

    def test_error1(self):
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__rpow__\\(\\): incompatible function arguments.*"
            ):

                class TMP(object):
                    pass

                TMP() ** self.a
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):

                class TMP(object):
                    pass

                TMP() ** self.a

    def test_error2(self):
        x = variable(10)
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(
                TypeError, "__pow__\\(\\): incompatible function arguments.*"
            ):
                self.a**x
        elif poek.__using_cppyy__:
            with self.assertRaisesRegex(TypeError, "none of the .* overloaded methods succeeded.*"):
                self.a**x

    def test_simplePow(self):
        a = self.a
        b = self.b

        e = a**b
        self.assertEqual(e.to_list(), ["pow", "a", "b"])

        e = a
        e **= b
        self.assertEqual(e.to_list(), ["pow", "a", "b"])

    def test_constPow(self):
        #
        # Check the structure of a simple power with a constant
        #
        a = self.a

        e = a**5
        self.assertEqual(e.to_list(), ["pow", "a", "5.000000"])

        e = 5**a
        self.assertEqual(e.to_list(), ["pow", "5.000000", "a"])

        e = a
        e **= 5
        self.assertEqual(e.to_list(), ["pow", "a", "5.000000"])

        e = a**5.0
        self.assertEqual(e.to_list(), ["pow", "a", "5.000000"])

        e = 5.0**a
        self.assertEqual(e.to_list(), ["pow", "5.000000", "a"])

        e = a
        e **= 5.0
        self.assertEqual(e.to_list(), ["pow", "a", "5.000000"])

    def test_trivialPow(self):
        #
        a = self.a
        p = self.p
        q = self.q
        r = self.r

        #
        # Check that taking the first power returns the original object
        #
        e = a**1
        self.assertEqual(e.to_list(), ["a"])

        e = a**1.0
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e **= 1
        self.assertEqual(e.to_list(), ["a"])

        e = a
        e **= 1.0
        self.assertEqual(e.to_list(), ["a"])

        e = a**r
        self.assertEqual(e.to_list(), ["pow", "a", "r"])

        #
        # Check that taking the zeroth power returns one
        #
        e = a**0
        self.assertEqual(e.to_list(), ["1.000000"])

        e = a**0.0
        self.assertEqual(e.to_list(), ["1.000000"])

        e = a
        e **= 0
        self.assertEqual(e.to_list(), ["1.000000"])

        e = a
        e **= 0.0
        self.assertEqual(e.to_list(), ["1.000000"])

        e = a**q
        self.assertEqual(e.to_list(), ["pow", "a", "q"])

        #
        # Check that taking powers of 0 and 1 are easy
        #
        e = 0**a
        self.assertEqual(e.to_list(), ["0.000000"])

        e = 0.0**a
        self.assertEqual(e.to_list(), ["0.000000"])

        e = 1**a
        self.assertEqual(e.to_list(), ["1.000000"])

        e = 1.0**a
        self.assertEqual(e.to_list(), ["1.000000"])

    def test_trivialRPow(self):
        #
        a = self.a
        p = self.p
        q = self.q
        r = self.r

        #
        # Check that taking any power of 1 is 1
        #
        e = 1**a
        self.assertEqual(e.to_list(), ["1.000000"])

        e = 1.0**a
        self.assertEqual(e.to_list(), ["1.000000"])

        e = r**a
        self.assertEqual(e.to_list(), ["pow", "r", "a"])

        #
        # Check that taking the zeroth power returns one
        #
        e = 0**a
        self.assertEqual(e.to_list(), ["0.000000"])

        e = 0.0**a
        self.assertEqual(e.to_list(), ["0.000000"])

    def test_pow_expresions(self):
        a = self.a
        r = self.r

        e = a**r
        self.assertEqual(e.to_list(), ["pow", "a", "r"])

        e = a**2
        self.assertEqual(e.to_list(), ["pow", "a", "2.000000"])

        e = a**2.0
        self.assertEqual(e.to_list(), ["pow", "a", "2.000000"])

        e = 2**a
        self.assertEqual(e.to_list(), ["pow", "2.000000", "a"])

        e = 2.0**a
        self.assertEqual(e.to_list(), ["pow", "2.000000", "a"])


class EntangledExpressionErrors(unittest.TestCase):
    def setUp(self):
        self.a = variable(name="a")
        self.b = variable(name="b")
        self.c = variable(name="c")
        self.d = variable(name="d")
        self.v = variable(name="v")
        self.p = parameter("p", value=0)
        self.q = parameter("q", value=0)
        self.r = parameter("r", value=1)

    def test_sumexpr_add_entangled(self):
        a = self.a
        e = a * 2 + 1
        self.assertEqual(e.to_list(), ["+", ["*", "2", "a"], "1.000000"])
        e += 1
        self.assertEqual(e.to_list(), ["+", ["*", "2", "a"], "1.000000", "1.000000"])

    def test_entangled_test1(self):
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        e1 = a + b
        e2 = c + e1
        e3 = d + e1

        self.assertEqual(e1.to_list(), ["+", "a", "b"])
        self.assertEqual(e2.to_list(), ["+", "c", ["+", "a", "b"]])
        self.assertEqual(e3.to_list(), ["+", "d", ["+", "a", "b"]])


class TestVariables(unittest.TestCase):
    def test_default_value(self):
        v = variable(3, name="v")
        self.assertTrue(math.isnan(v[0].value))
        self.assertTrue(math.isnan(v[1].value))
        self.assertTrue(math.isnan(v[2].value))

    def test_initialize(self):
        v = variable(3, name="v", value=3)
        self.assertEqual(v[0].value, 3)
        self.assertEqual(v[1].value, 3)
        self.assertEqual(v[2].value, 3)

    def test_iterator(self):
        v = variable(3, name="v")
        for i in v:
            math.isnan(v[i].value)

    def test_getitem1(self):
        v = variable(3, name="v")
        v1 = v[0]
        v2 = v[0]
        self.assertEqual(v1.name, v2.name)

    def test_getitem2(self):
        v = variable(3)
        v1 = v[0]
        v2 = v[0]
        self.assertEqual(v1.name, v2.name)

    def test_name1(self):
        v = variable(3, name="v", value=3)
        self.assertEqual(v.name, "v")
        self.assertEqual(v[0].name, "v[0]")

    def test_name2(self):
        v = variable(3, value=3)
        self.assertEqual(v.name, "x")
        self.assertEqual(v[0].name[0], "x")

    def test_name_single(self):
        v = variable(value=3, name="y")
        self.assertEqual(v.name, "y")
        v = variable(value=3)
        self.assertEqual(v.name[0], "x")


class TestNDVariables(unittest.TestCase):
    def test_default_value(self):
        v = variable((3, 2, 4), name="v")
        self.assertTrue(math.isnan(v[0, 1, 3].value))
        self.assertTrue(math.isnan(v[1, 0, 0].value))
        self.assertTrue(math.isnan(v[2, 1, 2].value))

    def test_initialize(self):
        v = variable((3, 2, 4), name="v", value=3)
        self.assertEqual(v[0, 1, 3].value, 3)
        self.assertEqual(v[1, 0, 0].value, 3)
        self.assertEqual(v[2, 1, 2].value, 3)

    def test_iterator(self):
        v = variable((3, 2, 4), name="v")
        for i in v:
            math.isnan(v[i].value)

    def test_getitem1(self):
        v = variable((3, 2, 4), name="v")
        v1 = v[0, 0, 0]
        v2 = v[0, 0, 0]
        self.assertEqual(v1.name, v2.name)

    def test_getitem2(self):
        v = variable((3, 2, 4))
        v1 = v[0, 0, 0]
        v2 = v[0, 0, 0]
        self.assertEqual(v1.name, v2.name)

    def test_name1(self):
        v = variable((3, 2, 4), name="v", value=3)
        self.assertEqual(v.name, "v")
        self.assertEqual(v[2, 1, 3].name, "v[2,1,3]")
        self.assertEqual(v[0, 0, 0].name, "v[0,0,0]")

    def test_name2(self):
        v = variable((3, 2, 4), value=3)
        self.assertEqual(v.name, "x")
        self.assertEqual(v[2, 1, 3].name[0], "x")
        self.assertEqual(v[0, 0, 0].name[0], "x")


if __name__ == "__main__":
    unittest.main()
