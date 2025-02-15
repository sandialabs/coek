import sys
import math
import pytest

from poek import *
import poek

if poek.__using_cppyy__:

    def is_constraint(val):
        return type(val) == poek.constraint

else:

    def is_constraint(val):
        return val.is_constraint()


class Test_ConditionalValue(object):

    # Do we expect arithmetic operations to return expressions?
    expectExpression = True

    # Do we expect relational tests to return constant expressions?
    expectConstExpression = True

    def X__init__(self):
        # Do we expect arithmetic operations to return expressions?
        self.expectExpression = True
        # Do we expect relational tests to return constant expressions?
        self.expectConstExpression = True

    def create(self, val):
        # Create the type of expression term that we are testing
        return parameter(value=val)

    def value_test(self, exp, val, expectExpression=None):
        """Test the value of the expression."""
        #
        # Override the class value of 'expectExpression'
        #
        if expectExpression is None:
            expectExpression = self.expectExpression
        #
        # Confirm whether 'exp' is an expression
        #
        assert is_constraint(exp) == False
        #
        # Confirm that 'exp' has the expected value
        #
        assert exp.value == val

    def relation_test(self, exp, val, expectConstExpression=None):
        """Test a relationship expression."""
        #
        # Override the class value of 'expectConstExpression'
        #
        if expectConstExpression is None:
            expectConstExpression = self.expectConstExpression
        #
        # Confirm that this is a relational expression
        #
        assert is_constraint(exp)
        #
        # Check that the expression evaluates correctly
        #
        assert exp.is_feasible() == val
        #
        # Check that the expression evaluates correctly in a Boolean context
        #
        if expectConstExpression:
            #
            # The relational expression should be a constant.
            #
            # Check that 'val' equals the boolean value of the expression.
            #
            assert exp.is_feasible() == val
        else:
            #
            # Check that the expression evaluates to 'val'
            #
            assert exp.is_feasible() == val

    def test_lt(self):
        #
        # Test the 'less than' operator
        #
        a = self.create(1.3)
        b = self.create(2.0)
        self.relation_test(a < b, True)
        self.relation_test(a < a, False)
        self.relation_test(b < a, False)
        self.relation_test(a < 2.0, True)
        self.relation_test(a < 1.3, False)
        self.relation_test(b < 1.3, False)
        self.relation_test(1.3 < b, True)
        self.relation_test(1.3 < a, False)
        self.relation_test(2.0 < a, False)

    def test_le(self):
        #
        # Test the 'less than' operator
        #
        a = self.create(1.3)
        b = self.create(2.0)
        self.relation_test(a <= b, True)
        self.relation_test(a <= a, True)
        self.relation_test(b <= a, False)
        self.relation_test(a <= 2.0, True)
        self.relation_test(a <= 1.3, True)
        self.relation_test(b <= 1.3, False)
        self.relation_test(1.3 <= b, True)
        self.relation_test(1.3 <= a, True)
        self.relation_test(2.0 <= a, False)

    def test_gt(self):
        #
        # Test the 'greater than' operator
        #
        a = self.create(1.3)
        b = self.create(2.0)
        self.relation_test(a > b, False)
        self.relation_test(a > a, False)
        self.relation_test(b > a, True)
        self.relation_test(a > 2.0, False)
        self.relation_test(a > 1.3, False)
        self.relation_test(b > 1.3, True)
        self.relation_test(1.3 > b, False)
        self.relation_test(1.3 > a, False)
        self.relation_test(2.0 > a, True)

    def test_ge(self):
        #
        # Test the 'greater than' operator
        #
        a = self.create(1.3)
        b = self.create(2.0)
        self.relation_test(a >= b, False)
        self.relation_test(a >= a, True)
        self.relation_test(b >= a, True)
        self.relation_test(a >= 2.0, False)
        self.relation_test(a >= 1.3, True)
        self.relation_test(b >= 1.3, True)
        self.relation_test(1.3 >= b, False)
        self.relation_test(1.3 >= a, True)
        self.relation_test(2.0 >= a, True)

    def test_eq(self):
        #
        # Test the 'equals' operator
        #
        a = self.create(1.3)
        b = self.create(2.0)
        self.relation_test(a == b, False, True)
        self.relation_test(a == a, True, True)
        self.relation_test(b == a, False, True)
        self.relation_test(a == 2.0, False, True)
        self.relation_test(a == 1.3, True, True)
        self.relation_test(b == 1.3, False, True)
        self.relation_test(1.3 == b, False, True)
        self.relation_test(1.3 == a, True, True)
        self.relation_test(2.0 == a, False, True)

    def test_arithmetic(self):
        #
        #
        # Test binary arithmetic operators
        #
        a = self.create(-0.5)
        b = self.create(2.0)
        self.value_test(a - b, -2.5)
        self.value_test(a + b, 1.5)
        self.value_test(a * b, -1.0)
        self.value_test(b / a, -4.0)
        self.value_test(a**b, 0.25)

        self.value_test(a - 2.0, -2.5)
        self.value_test(a + 2.0, 1.5)
        self.value_test(a * 2.0, -1.0)
        self.value_test(b / (0.5), 4.0)
        self.value_test(a**2.0, 0.25)

        self.value_test(0.5 - b, -1.5)
        self.value_test(0.5 + b, 2.5)
        self.value_test(0.5 * b, 1.0)
        self.value_test(2.0 / a, -4.0)
        self.value_test((0.5) ** b, 0.25)

        self.value_test(-a, 0.5)
        self.value_test(+a, -0.5, False)  # This doesn't generate an expression
        self.value_test(abs(-a), 0.5)


class Test_VarValue(Test_ConditionalValue):
    def setUp(self):
        self.model = model()
        #
        # Create Model
        #
        Test_ConditionalValue.setUp(self)
        #
        # Create model instance
        #
        self.expectExpression = True
        self.expectConstExpression = False

    def tearDown(self):
        self.model = None

    def create(self, val):
        tmp = variable(value=val)
        return tmp


class Test_Expr(object):
    @pytest.fixture
    def var_a(self):
        return variable(name="a")

    @pytest.fixture
    def var_b(self):
        return variable(name="b")

    @pytest.fixture
    def var_x(self):
        return variable(10, name="x")

    def X__init(self):
        # self.a = variable(name="a")
        # self.b = variable(name="b")
        self.c = variable(name="c")
        self.x = variable(10, name="x")

    def test_simpleEquality(self, var_a, var_b):
        #
        # Check the structure of a simple equality statement
        #
        a, b = var_a, var_b

        e = a == b
        assert e.to_list() == ["==", ["+", "a", ["*", "-1.000000", "b"]], "0.000000"]

    def test_error1(self, var_a, var_b):
        #
        # Confirm problems with chained relational operators
        #
        a, b = var_a, var_b

        #       =
        #      / \
        #     =   5
        #    / \
        #   a   b
        if poek.__using_pybind11__:
            e = a == b == 5
        else:
            with pytest.raises(TypeError) as e_info:
                e = a == b == 5

        a.value = 3
        b.value = 3
        if poek.__using_pybind11__:
            e = a == b == 5
        else:
            with pytest.raises(TypeError) as e_info:
                e = a == b == 5

        e = a == b
        try:
            e2 = e == 5
            pytest.fail("Expected TypeError")
        except:
            pass

    def test_error2(self, var_a, var_b, var_x):
        #
        # Check errors in nested relational expressions
        #
        a, b, x = var_a, var_b, var_x

        e = a == b
        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                e == a
            with pytest.raises(TypeError) as e_info:
                a == e
        else:
            with pytest.raises(TypeError) as e_info:
                e == a
            with pytest.raises(TypeError) as e_info:
                a == e

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                a == x
            with pytest.raises(TypeError) as e_info:
                x == a
        else:
            with pytest.raises(TypeError) as e_info:
                a == x
            with pytest.raises(TypeError) as e_info:
                x == a

    def test_error_nested(self, var_a, var_b):
        #
        # Verify that constraints are not allow to be nested.
        #
        a, b = var_a, var_b

        e = a == b
        E = a

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                a + e
            with pytest.raises(TypeError) as e_info:
                e + a
            with pytest.raises(TypeError) as e_info:
                E += e
            with pytest.raises(TypeError) as e_info:
                2 + e
            with pytest.raises(TypeError) as e_info:
                e += 2
        else:
            with pytest.raises(TypeError) as e_info:
                a + e
            with pytest.raises(NotImplementedError) as e_info:
                e + a
            with pytest.raises(TypeError) as e_info:
                E += e
            with pytest.raises(NotImplementedError) as e_info:
                2 + e
            with pytest.raises(NotImplementedError) as e_info:
                e += 2

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                a - e
            with pytest.raises(TypeError) as e_info:
                e - a
            with pytest.raises(TypeError) as e_info:
                E -= e
            with pytest.raises(TypeError) as e_info:
                2 - e
            with pytest.raises(TypeError) as e_info:
                e -= 2
        else:
            with pytest.raises(TypeError) as e_info:
                a - e
            with pytest.raises(NotImplementedError) as e_info:
                e - a
            with pytest.raises(TypeError) as e_info:
                E -= e
            with pytest.raises(NotImplementedError) as e_info:
                2 - e
            with pytest.raises(NotImplementedError) as e_info:
                e -= 2

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                a * e
            with pytest.raises(TypeError) as e_info:
                e * a
            with pytest.raises(TypeError) as e_info:
                E *= e
            with pytest.raises(TypeError) as e_info:
                2 * e
            with pytest.raises(TypeError) as e_info:
                e *= 2
        else:
            with pytest.raises(TypeError) as e_info:
                a * e
            with pytest.raises(NotImplementedError) as e_info:
                e * a
            with pytest.raises(TypeError) as e_info:
                E *= e
            with pytest.raises(NotImplementedError) as e_info:
                2 * e
            with pytest.raises(NotImplementedError) as e_info:
                e *= 2

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                a / e
            with pytest.raises(TypeError) as e_info:
                e / a
            with pytest.raises(TypeError) as e_info:
                E /= e
            with pytest.raises(TypeError) as e_info:
                2 / e
            with pytest.raises(TypeError) as e_info:
                e /= 2
        else:
            with pytest.raises(TypeError) as e_info:
                a / e
            with pytest.raises(NotImplementedError) as e_info:
                e / a
            with pytest.raises(TypeError) as e_info:
                E /= e
            with pytest.raises(NotImplementedError) as e_info:
                2 / e
            with pytest.raises(NotImplementedError) as e_info:
                e /= 2

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                a**e
            with pytest.raises(TypeError) as e_info:
                e**a
            with pytest.raises(TypeError) as e_info:
                E **= e
        else:
            with pytest.raises(TypeError) as e_info:
                a**e
            with pytest.raises(TypeError) as e_info:
                e**a
            with pytest.raises(TypeError) as e_info:
                E **= e
            with pytest.raises(TypeError) as e_info:
                2**e
            with pytest.raises(TypeError) as e_info:
                e **= 2

    def test_error_chained(self, var_a, var_b):
        #
        # Verify that constraints are not allow to be nested.
        #
        a, b = var_a, var_b

        e = a == b

        if poek.__using_pybind11__:
            with pytest.raises(TypeError) as e_info:
                e < a
            with pytest.raises(TypeError) as e_info:
                e <= a
            with pytest.raises(TypeError) as e_info:
                e > a
            with pytest.raises(TypeError) as e_info:
                e >= a
            with pytest.raises(TypeError) as e_info:
                e == a
        else:
            with pytest.raises(TypeError) as e_info:
                e < a
            with pytest.raises(TypeError) as e_info:
                e <= a
            with pytest.raises(TypeError) as e_info:
                e > a
            with pytest.raises(TypeError) as e_info:
                e >= a
            with pytest.raises(TypeError) as e_info:
                e == a
