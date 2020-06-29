import sys
import math
import pyutilib.th as unittest

from poek import *
import poek

if poek.__using_cppyy__:
    def is_constraint(val):
        return type(val) == poek.constraint
else:
    def is_constraint(val):
        return val.is_constraint()


class Test_ConditionalValue(unittest.TestCase):

    def setUp(self):
        self.model = model()
        # Do we expect arithmetic operations to return expressions?
        self.expectExpression = True
        # Do we expect relational tests to return constant expressions?
        self.expectConstExpression = True

    def tearDown(self):
        self.model = None

    def create(self, val):
        # Create the type of expression term that we are testing
        return parameter(val)

    @unittest.nottest
    def value_test(self, exp, val, expectExpression=None):
        """ Test the value of the expression. """
        #
        # Override the class value of 'expectExpression'
        #
        if expectExpression is None:
            expectExpression = self.expectExpression
        #
        # Confirm whether 'exp' is an expression
        #
        self.assertEqual(is_constraint(exp), False)
        #
        # Confirm that 'exp' has the expected value
        #
        self.assertEqual(exp.value, val)

    @unittest.nottest
    def relation_test(self, exp, val, expectConstExpression=None):
        """ Test a relationship expression. """
        #
        # Override the class value of 'expectConstExpression'
        #
        if expectConstExpression is None:
            expectConstExpression = self.expectConstExpression
        #
        # Confirm that this is a relational expression
        #
        self.assertTrue(is_constraint(exp))
        #
        # Check that the expression evaluates correctly
        #
        self.assertEqual(exp.is_feasible(), val)
        #
        # Check that the expression evaluates correctly in a Boolean context
        #
        if expectConstExpression:
            #
            # The relational expression should be a constant.
            #
            # Check that 'val' equals the boolean value of the expression.
            #
            self.assertEqual(exp.is_feasible(), val)
        else:
            #
            # Check that the expression evaluates to 'val'
            #
            self.assertEqual(exp.is_feasible(), val)

    def test_lt(self):
        #
        # Test the 'less than' operator
        #
        a=self.create(1.3)
        b=self.create(2.0)
        self.relation_test(a<b, True)
        self.relation_test(a<a, False)
        self.relation_test(b<a, False)
        self.relation_test(a<2.0, True)
        self.relation_test(a<1.3, False)
        self.relation_test(b<1.3, False)
        self.relation_test(1.3<b, True)
        self.relation_test(1.3<a, False)
        self.relation_test(2.0<a, False)

    def test_gt(self):
        #
        # Test the 'greater than' operator
        #
        a=self.create(1.3)
        b=self.create(2.0)
        self.relation_test(a>b, False)
        self.relation_test(a>a, False)
        self.relation_test(b>a, True)
        self.relation_test(a>2.0, False)
        self.relation_test(a>1.3, False)
        self.relation_test(b>1.3, True)
        self.relation_test(1.3>b, False)
        self.relation_test(1.3>a, False)
        self.relation_test(2.0>a, True)

    def test_eq(self):
        #
        # Test the 'equals' operator
        #
        a=self.create(1.3)
        b=self.create(2.0)
        self.relation_test(a==b, False, True)
        self.relation_test(a==a, True, True)
        self.relation_test(b==a, False, True)
        self.relation_test(a==2.0, False, True)
        self.relation_test(a==1.3, True, True)
        self.relation_test(b==1.3, False, True)
        self.relation_test(1.3==b, False, True)
        self.relation_test(1.3==a, True, True)
        self.relation_test(2.0==a, False, True)

    def test_arithmetic(self):
        #
        #
        # Test binary arithmetic operators
        #
        a=self.create(-0.5)
        b=self.create(2.0)
        self.value_test(a-b, -2.5)
        self.value_test(a+b, 1.5)
        self.value_test(a*b, -1.0)
        self.value_test(b/a, -4.0)
        self.value_test(a**b, 0.25)

        self.value_test(a-2.0, -2.5)
        self.value_test(a+2.0, 1.5)
        self.value_test(a*2.0, -1.0)
        self.value_test(b/(0.5), 4.0)
        self.value_test(a**2.0, 0.25)

        self.value_test(0.5-b, -1.5)
        self.value_test(0.5+b, 2.5)
        self.value_test(0.5*b, 1.0)
        self.value_test(2.0/a, -4.0)
        self.value_test((0.5)**b, 0.25)

        self.value_test(-a, 0.5)
        self.value_test(+a, -0.5, False)        # This doesn't generate an expression
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
        tmp=variable()
        tmp.value=val
        return tmp


class Test_Expr(unittest.TestCase):

    def setUp(self):
        self.model = model()
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.x = variable(10, name='x')

    def tearDown(self):
        self.model = None

    def test_simpleEquality(self):
        #
        # Check the structure of a simple equality statement
        #
        a = self.a
        b = self.b

        e = a == b
        self.assertEqual( e.to_list(), ['==', ['+', 'a', ['*', '-1', 'b']], '0.000'] )

    def test_error1(self):
        #
        # Confirm problems with chained relational operators
        #
        a = self.a
        b = self.b

        #       =
        #      / \
        #     =   5
        #    / \
        #   a   b
        if poek.__using_pybind11__:
            e = a == b == 5
        else:
            with self.assertRaisesRegex(TypeError, "bool\(\) argument must.*"):
                e = a == b == 5

        a.value = 3
        b.value = 3
        if poek.__using_pybind11__:
            e = a == b == 5
        else:
            with self.assertRaisesRegex(TypeError, "bool\(\) argument must.*"):
                e = a == b == 5

        e = a == b
        try:
            e2 = e == 5
            self.fail("Expected TypeError")
        except:
            pass

    def test_error2(self):
        #
        # Check errors in nested relational expressions
        #
        a = self.a
        b = self.b
        x = self.x

        e = a == b
        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__eq__\(\): incompatible function arguments.*"):
                e == a
            with self.assertRaisesRegex(TypeError,"__eq__\(\): incompatible function arguments.*"):
                a == e
        else:
            with self.assertRaisesRegex(TypeError, "Constraint argument cannot be used in a boolean expression."):
                e == a
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a == e

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__eq__\(\): incompatible function arguments.*"):
                a == x
            with self.assertRaisesRegex(TypeError,"__eq__\(\): incompatible function arguments.*"):
                x == a
        else:
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a == x
            with self.assertRaisesRegex(TypeError,"VariableArray argument cannot be used in a boolean expression."):
                x == a

    def test_error_nested(self):
        #
        # Verify that constraints are not allow to be nested.
        #
        a = self.a
        b = self.b
        
        e = a == b
        E = a

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__add__\(\): incompatible function arguments.*"):
                a + e
            with self.assertRaisesRegex(TypeError,"__radd__\(\): incompatible function arguments.*"):
                e + a
            with self.assertRaisesRegex(TypeError,"__add__\(\): incompatible function arguments.*"):
                E += e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                2 + e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                e += 2
        else:
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a + e
            with self.assertRaises(NotImplementedError):
                e + a
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                E += e
            with self.assertRaises(NotImplementedError):
                2 + e
            with self.assertRaises(NotImplementedError):
                e += 2

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__sub__\(\): incompatible function arguments.*"):
                a - e
            with self.assertRaisesRegex(TypeError,"__rsub__\(\): incompatible function arguments.*"):
                e - a
            with self.assertRaisesRegex(TypeError,"__sub__\(\): incompatible function arguments.*"):
                E -= e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                2 - e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                e -= 2
        else:
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a - e
            with self.assertRaises(NotImplementedError):
                e - a
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                E -= e
            with self.assertRaises(NotImplementedError):
                2 - e
            with self.assertRaises(NotImplementedError):
                e -= 2

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__mul__\(\): incompatible function arguments.*"):
                a * e
            with self.assertRaisesRegex(TypeError,"__rmul__\(\): incompatible function arguments.*"):
                e * a
            with self.assertRaisesRegex(TypeError,"__mul__\(\): incompatible function arguments.*"):
                E *= e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                2 * e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                e *= 2
        else:
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a * e
            with self.assertRaises(NotImplementedError):
                e * a
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                E *= e
            with self.assertRaises(NotImplementedError):
                2 * e
            with self.assertRaises(NotImplementedError):
                e *= 2

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__truediv__\(\): incompatible function arguments.*"):
                a / e
            with self.assertRaisesRegex(TypeError,"__rtruediv__\(\): incompatible function arguments.*"):
                e / a
            with self.assertRaisesRegex(TypeError,"__truediv__\(\): incompatible function arguments.*"):
                E /= e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                2 / e
            with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
                e /= 2
        else:
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a / e
            with self.assertRaises(NotImplementedError):
                e / a
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                E /= e
            with self.assertRaises(NotImplementedError):
                2 / e
            with self.assertRaises(NotImplementedError):
                e /= 2

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__pow__\(\): incompatible function arguments.*"):
                a ** e
            with self.assertRaisesRegex(TypeError,"__rpow__\(\): incompatible function arguments.*"):
                e ** a
            with self.assertRaisesRegex(TypeError,"__pow__\(\): incompatible function arguments.*"):
                E **= e
        else:
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                a ** e
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                e ** a
            with self.assertRaisesRegex(TypeError,"none of the .* overloaded methods succeeded.*"):
                E **= e
        with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
            2 ** e
        with self.assertRaisesRegex(TypeError, "unsupported operand type\(s\) .*"):
            e **= 2

    def test_error_chained(self):
        #
        # Verify that constraints are not allow to be nested.
        #
        a = self.a
        b = self.b
        
        e = a == b

        if poek.__using_pybind11__:
            with self.assertRaisesRegex(TypeError,"__lt__\(\): incompatible function arguments.*"):
                e < a
            with self.assertRaisesRegex(TypeError,"__le__\(\): incompatible function arguments.*"):
                e <= a
            with self.assertRaisesRegex(TypeError,"__gt__\(\): incompatible function arguments.*"):
                e > a
            with self.assertRaisesRegex(TypeError,"__ge__\(\): incompatible function arguments.*"):
                e >= a
            with self.assertRaisesRegex(TypeError,"__eq__\(\): incompatible function arguments.*"):
                e == a
        else:
            with self.assertRaisesRegex(TypeError, "Constraint argument cannot be used in a boolean expression."):
                e < a
            with self.assertRaisesRegex(TypeError, "Constraint argument cannot be used in a boolean expression."):
                e <= a
            with self.assertRaisesRegex(TypeError, "Constraint argument cannot be used in a boolean expression."):
                e > a
            with self.assertRaisesRegex(TypeError, "Constraint argument cannot be used in a boolean expression."):
                e >= a
            with self.assertRaisesRegex(TypeError, "Constraint argument cannot be used in a boolean expression."):
                e == a

if __name__ == "__main__":
    unittest.main()
