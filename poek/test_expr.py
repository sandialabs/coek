import math
import pyutilib.th as unittest 

from expr import *


class TestSumExpression(unittest.TestCase):

    def setUp(self):
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.d = variable(name='d')
        self.p = parameter(0, True, name='p')
        self.visitor = ValueVisitor()

    def test_simpleSum(self):
        # a + b
        a = self.a
        b = self.b
        e = a + b
        #
        self.assertEqual( self.visitor.walk(e), ['+','a','b'] )
        self.assertEqual(e.size(), 3)

    def test_simpleSum_API(self):
        # a + b + 2*a
        a = self.a
        b = self.b
        e = a + b
        e += (2*a)
        #
        self.assertEqual( self.visitor.walk(e), ['+', ['+', 'a', 'b'], ['*', '2', 'a']] )
        self.assertEqual(e.size(), 7)

    def test_constSum_int(self):
        # a + 5
        a = self.a
        e = a + 5
        #
        self.assertEqual( self.visitor.walk(e), ['+','a','5'] )
        self.assertEqual(e.size(), 3)

        # 5 + a
        e = 5 + a
        #
        self.assertEqual( self.visitor.walk(e), ['+','5','a'] )
        self.assertEqual(e.size(), 3)

    def test_constSum_real(self):
        # a + 5.0
        a = self.a
        e = a + 5.0
        #
        self.assertEqual( self.visitor.walk(e), ['+','a','5.000'] )
        self.assertEqual(e.size(), 3)

        # 5.0 + a
        e = 5.0 + a
        #
        self.assertEqual( self.visitor.walk(e), ['+','5.000','a'] )
        self.assertEqual(e.size(), 3)

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
        self.assertEqual( self.visitor.walk(e), ['+',['+','a','b'], '5'] )
        self.assertEqual(e.size(), 5)

        #       + 
        #      / \ 
        #     5   +
        #        / \
        #       a   b
        e1 = a + b
        e = 5 + e1
        #
        self.assertEqual( self.visitor.walk(e), ['+','5', ['+','a','b']] )
        self.assertEqual(e.size(), 5)

        #           +
        #          / \
        #         +   c
        #        / \
        #       a   b
        e1 = a + b
        e = e1 + c
        #
        self.assertEqual( self.visitor.walk(e), ['+',['+','a','b'],'c'] )
        self.assertEqual(e.size(), 5)

        #       + 
        #      / \ 
        #     c   +
        #        / \
        #       a   b
        e1 = a + b
        e = c + e1
        #
        self.assertEqual( self.visitor.walk(e), ['+','c', ['+','a','b']] )
        self.assertEqual(e.size(), 5)

        #            +
        #          /   \
        #         +     +
        #        / \   / \
        #       a   b c   d
        e1 = a + b
        e2 = c + d
        e = e1 + e2
        #
        self.assertEqual( self.visitor.walk(e), ['+',['+','a','b'],['+','c','d']] )
        self.assertEqual(e.size(), 7)

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
        e = 2*e1 + c
        #
        self.assertEqual( self.visitor.walk(e), ['+',['*','2',['+','a','b']],'c'] )
        self.assertEqual(e.size(), 7)

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
        e = 3*(2*e1 + c)
        #
        self.assertEqual( self.visitor.walk(e), ['*','3',['+',['*','2',['+','a','b']],'c']] )
        self.assertEqual(e.size(), 9)

    def test_trivialSum(self):
        #
        # Check that adding zero doesn't change the expression
        #
        a = self.a
        e = a + 0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = 0 + a
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)
        #
        # Adding zero to a sum will not change the sum
        #
        e = a + a
        f = e + 0
        self.assertEqual(id(e), id(f))

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
        self.assertEqual( self.visitor.walk(e), ['+',['*','a','5'],'b'] )
        self.assertEqual(e.size(), 5)

        #       +
        #      / \
        #     b   *
        #        / \
        #       a   5
        e = b + e1
        #
        self.assertEqual( self.visitor.walk(e), ['+','b',['*','a','5']] )
        self.assertEqual(e.size(), 5)

        #            +
        #          /   \
        #         *     +
        #        / \   / \
        #       a   5 b   c
        e2 = b + c
        e = e1 + e2
        #
        self.assertEqual( self.visitor.walk(e), ['+',['*','a','5'],['+','b','c']] )
        self.assertEqual(e.size(), 7)

        #            +
        #          /   \
        #         +     *
        #        / \   / \
        #       b   c a   5
        e2 = b + c
        e = e2 + e1
        #
        self.assertEqual( self.visitor.walk(e), ['+',['+','b','c'],['*','a','5']] )
        self.assertEqual(e.size(), 7)


class TestDiffExpression(unittest.TestCase):

    def setUp(self):
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.d = variable(name='d')
        self.v = variable(name='v')
        self.p = parameter(0, True, name='p')
        self.visitor = ValueVisitor()

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
        #
        self.assertEqual( self.visitor.walk(e), ['-','a','b'] )
        self.assertEqual(e.size(), 3)

    def test_constDiff_int(self):
        # a - 5
        a = self.a
        e = a - 5
        #
        self.assertEqual( self.visitor.walk(e), ['-','a','5'] )
        self.assertEqual(e.size(), 3)

        # 5 - a
        e = 5 - a
        #
        self.assertEqual( self.visitor.walk(e), ['-','5','a'] )
        self.assertEqual(e.size(), 3)

    def test_constDiff_real(self):
        # a - 5.0
        a = self.a
        e = a - 5.0
        #
        self.assertEqual( self.visitor.walk(e), ['-','a','5.000'] )
        self.assertEqual(e.size(), 3)

        # 5.0 - a
        e = 5.0 - a
        #
        self.assertEqual( self.visitor.walk(e), ['-','5.000','a'] )
        self.assertEqual(e.size(), 3)

    def test_paramDiff(self):
        # a - p
        a = self.a
        p = self.p
        e = a - p
        #
        self.assertEqual( self.visitor.walk(e), ['-','a','p'] )
        self.assertEqual(e.size(), 3)

        # p - a
        e = p - a
        #
        self.assertEqual( self.visitor.walk(e), ['-','p','a'] )
        self.assertEqual(e.size(), 3)

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

        e = 5 - 2*a
        #
        self.assertEqual( self.visitor.walk(e), ['-','5',['*', '2','a']] )
        self.assertEqual(e.size(),5)

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
        self.assertEqual( self.visitor.walk(e), ['-',['-','a','b'],'5'] )
        self.assertEqual(e.size(),5)

        #       -
        #      / \
        #     5   -
        #        / \
        #       a   b

        e1 = a - b
        e = 5 - e1
        #
        self.assertEqual( self.visitor.walk(e), ['-','5',['-','a','b']] )
        self.assertEqual(e.size(),5)

        #       -
        #      / \
        #     -   c
        #    / \
        #   a   b

        e1 = a - b
        e = e1 - c
        #
        self.assertEqual( self.visitor.walk(e), ['-',['-','a','b'],'c'] )
        self.assertEqual(e.size(),5)

        #            -
        #          /   \
        #         -     -
        #        / \   / \
        #       a   b c   d

        e1 = a - b
        e2 = c - d
        e = e1 - e2
        #
        self.assertEqual( self.visitor.walk(e), ['-',['-','a','b'],['-','c','d']] )
        self.assertEqual(e.size(),7)

    def test_negation_param(self):
        #
        # Check logic for negations 
        #

        p = self.p
        e = - p
        #
        self.assertEqual( self.visitor.walk(e), ['-','p'])
        self.assertEqual(e.size(),2)

        e = - e
        #
        # TODO: Can we detect negations of negations?
        #
        self.assertEqual( self.visitor.walk(e), ['-',['-','p']] )
        self.assertEqual(e.size(),3)

    def test_negation_terms(self):
        #
        # Check logic for negations with terms
        #
        p = self.p
        v = self.v

        e = - p*v
        self.assertEqual( self.visitor.walk(e), ['*',['-','p'],'v'] )
        self.assertEqual(e.size(),4)

        e = - e
        self.assertEqual( self.visitor.walk(e), ['-',['*',['-','p'],'v']] )
        self.assertEqual(e.size(),5)
        #
        e = - 5*v
        self.assertEqual( self.visitor.walk(e), ['*','-5','v'] )
        self.assertEqual(e.size(),3)
        e = - e
        self.assertEqual( self.visitor.walk(e), ['-',['*','-5','v']] )
        self.assertEqual(e.size(),4)

    def test_trivialDiff(self):
        #
        # Check that subtracting zero doesn't change the expression
        #
        a = self.a
        p = self.p

        # a - 0
        e = a - 0
        self.assertEqual( self.visitor.walk(e), 'a' )
        self.assertEqual(e.size(),1)

        # 0 - a
        e = 0 - a
        self.assertEqual( self.visitor.walk(e), ['-','a'] )
        self.assertEqual(e.size(),2)

        # p - 0
        e = p - 0
        self.assertEqual( self.visitor.walk(e), 'p' )
        self.assertEqual(e.size(),1)

        # 0 - p
        e = 0 - p
        self.assertEqual( self.visitor.walk(e), ['-','p'] )
        self.assertEqual(e.size(),2)

        # 0 - 5*a
        e = 0 - 5*a
        self.assertEqual( self.visitor.walk(e), ['-', ['*','5','a']] )
        self.assertEqual(e.size(),4)

        # 0 - p*a
        e = 0 - p*a
        self.assertEqual( self.visitor.walk(e), ['-', ['*','p','a']] )
        self.assertEqual(e.size(),4)

        # 0 - a*a
        e = 0 - a*a
        self.assertEqual( self.visitor.walk(e), ['-', ['*','a','a']] )
        self.assertEqual(e.size(),4)

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
        self.assertEqual( self.visitor.walk(e), ['-', ['*','a','5'],'b'] )
        self.assertEqual(e.size(),5)

        #       -
        #      / \
        #     b   *
        #        / \
        #       a   5
        e1 = a * 5
        e = b - e1
        self.assertEqual( self.visitor.walk(e), ['-', 'b', ['*','a','5']] )
        self.assertEqual(e.size(),5)

        #            -
        #          /   \
        #         *     -
        #        / \   / \
        #       a   5 b   c
        e1 = a * 5
        e2 = b - c
        e = e1 - e2
        self.assertEqual( self.visitor.walk(e), ['-', ['*','a','5'],['-','b','c']] )
        self.assertEqual(e.size(),7)

        #            -
        #          /   \
        #         -     *
        #        / \   / \
        #       b   c a   5
        e1 = a * 5
        e2 = b - c
        e = e2 - e1
        self.assertEqual( self.visitor.walk(e), ['-', ['-','b','c'],['*','a','5']] )
        self.assertEqual(e.size(),7)


class TestGenerate_ProductExpression(unittest.TestCase):

    def setUp(self):
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.d = variable(name='d')
        self.v = variable(name='v')
        self.p = parameter(0, True, name='p')
        self.q = parameter(0, False, name='q')
        self.r = parameter(1, False, name='r')
        self.visitor = ValueVisitor()

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
        self.assertEqual( self.visitor.walk(e), ['*','a','b'] )
        self.assertEqual(e.size(),3)

    def test_constProduct(self):
        #
        # Check the structure of a simple product with a constant
        #
        a = self.a

        #    *
        #   / \
        #  a   5
        e = a * 5
        self.assertEqual( self.visitor.walk(e), ['*','a','5'] )
        self.assertEqual(e.size(),3)

        #    *
        #   / \
        #  5   a
        e = 5 * a
        self.assertEqual( self.visitor.walk(e), ['*','5','a'] )
        self.assertEqual(e.size(),3)

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
        self.assertEqual( self.visitor.walk(e), ['*',['*','a','b'],'5'] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = 5 * e1
        self.assertEqual( self.visitor.walk(e), ['*','5',['*','a','b']] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     *   c
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * c
        self.assertEqual( self.visitor.walk(e), ['*',['*','a','b'],'c'] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     c   *
        #        / \
        #       a   b
        e1 = a * b
        e = c * e1
        self.assertEqual( self.visitor.walk(e), ['*','c',['*','a','b']] )
        self.assertEqual(e.size(),5)

        #            *
        #          /   \
        #         *     *
        #        / \   / \
        #       a   b c   d
        e1 = a * b
        e2 = c * d
        e = e1 * e2
        self.assertEqual( self.visitor.walk(e), ['*',['*','a','b'],['*','c','d']] )
        self.assertEqual(e.size(),7)

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
        self.assertEqual( self.visitor.walk(e), ['*',['+','c',['+','a','b']],['+',['+','a','b'],'d']] )
        self.assertEqual(e.size(),11)

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
        self.assertEqual( self.visitor.walk(e), ['*',['*','c',['+','a','b']],['*',['+','a','b'],'d']] )
        self.assertEqual(e.size(),11)

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
        self.assertEqual( self.visitor.walk(e), ['*',['*','3','b'],'5'] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     *   5
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * 5
        self.assertEqual( self.visitor.walk(e), ['*',['*','a','b'],'5'] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     5   *
        #        / \
        #       3   b
        e1 = 3 * b
        e = 5 * e1
        self.assertEqual( self.visitor.walk(e), ['*','5',['*','3','b']] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     5   *
        #        / \
        #       a   b
        e1 = a * b
        e = 5 * e1
        self.assertEqual( self.visitor.walk(e), ['*','5',['*','a','b']] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     *   c
        #    / \
        #   a   b
        e1 = a * b
        e = e1 * c
        self.assertEqual( self.visitor.walk(e), ['*',['*','a','b'],'c'] )
        self.assertEqual(e.size(),5)

        #       *
        #      / \
        #     c   *
        #        / \
        #       a   b
        e1 = a * b
        e = c * e1
        self.assertEqual( self.visitor.walk(e), ['*','c',['*','a','b']] )
        self.assertEqual(e.size(),5)

        #            *
        #          /   \
        #         *     *
        #        / \   / \
        #       a   b c   d
        e1 = a * b
        e2 = c * d
        e = e1 * e2
        self.assertEqual( self.visitor.walk(e), ['*',['*','a','b'],['*','c','d']] )
        self.assertEqual(e.size(),7)


    def test_trivialProduct(self):
        #
        # Check that multiplying by zero gives zero
        #
        a = self.a
        p = self.p
        q = self.q
        r = self.r

        e = a * 0
        self.assertEqual(e, 0)
        self.assertIs(type(e), int)

        e = 0 * a
        self.assertEqual(e, 0)
        self.assertIs(type(e), int)

        e = a * p
        self.assertEqual( self.visitor.walk(e), ['*','a','p'] )
        self.assertEqual(e.size(),3)

        e = p * a
        self.assertEqual( self.visitor.walk(e), ['*','p','a'] )
        self.assertEqual(e.size(),3)

        e = a * q
        self.assertEqual( self.visitor.walk(e), '0' )
        self.assertIs(type(e), parameter)

        e = q * a
        self.assertEqual( self.visitor.walk(e), '0' )
        self.assertIs(type(e), parameter)

        #
        # Check that multiplying by one gives the original expression
        #
        e = a * 1
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = 1 * a
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a * r
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = r * a
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        #
        # Check that numeric constants are simply multiplied out
        #
        # TODO: combine constant terms
        #
        #e = parameter(3, mutable=False) * parameter(2, mutable=False)
        #self.assertIs(type(e), int)
        #self.assertEqual(e, 6)

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
        self.assertEqual( self.visitor.walk(e), ['/','a','b'] )
        self.assertEqual(e.size(),3)

    def test_constDivision(self):
        #
        # Check the structure of a simple division with a constant
        #
        a = self.a

        #    /
        #   / \
        #  a   5
        e = a / 5
        self.assertEqual( self.visitor.walk(e), ['/','a','5'] )
        self.assertEqual(e.size(),3)

        #    /
        #   / \
        #  5   a
        e = 5 / a
        self.assertEqual( self.visitor.walk(e), ['/','5','a'] )
        self.assertEqual(e.size(),3)

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
        self.assertEqual( self.visitor.walk(e), ['/',['*','3','b'],'5'] )
        self.assertEqual(e.size(),5)

        #       /
        #      / \
        #     /   5
        #    / \
        #   a   b
        e1 = a / b
        e = e1 / 5
        self.assertEqual( self.visitor.walk(e), ['/',['/','a','b'],'5'] )
        self.assertEqual(e.size(),5)

        #       /
        #      / \
        #     5   /
        #        / \
        #       a   b
        e1 = a / b
        e = 5 / e1
        self.assertEqual( self.visitor.walk(e), ['/','5',['/','a','b']] )
        self.assertEqual(e.size(),5)

        #       /
        #      / \
        #     /   c
        #    / \
        #   a   b
        e1 = a / b
        e = e1 / c
        self.assertEqual( self.visitor.walk(e), ['/',['/','a','b'],'c'] )
        self.assertEqual(e.size(),5)

        #       /
        #      / \
        #     c   /
        #        / \
        #       a   b
        e1 = a / b
        e = c / e1
        self.assertEqual( self.visitor.walk(e), ['/','c',['/','a','b']] )
        self.assertEqual(e.size(),5)

        #            /
        #          /   \
        #         /     /
        #        / \   / \
        #       a   b c   d
        e1 = a / b
        e2 = c / d
        e = e1 / e2
        self.assertEqual( self.visitor.walk(e), ['/',['/','a','b'],['/','c','d']] )
        self.assertEqual(e.size(),7)

    def test_trivialDivision(self):
        #
        # Check that dividing by zero generates an exception
        #
        a = self.a
        p = self.p
        q = self.q
        r = self.r

        self.assertRaises(ZeroDivisionError, a.__div__, 0)

        #
        # Check that dividing zero by anything non-zero gives zero
        #
        e = 0 / a
        self.assertIs(type(e), int)
        self.assertAlmostEqual(e, 0.0)

        #
        # Check that dividing by one 1 gives the original expression
        #
        e = a / 1
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / a
        self.assertEqual( self.visitor.walk(e), ['/','1','a'] )
        self.assertEqual(e.size(),3)

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / p
        self.assertEqual( self.visitor.walk(e), ['/','1','p'] )
        self.assertEqual(e.size(),3)

        #
        # Check the structure dividing 1 by an expression
        #
        try:
            e = 1 / q
            self.fail("Expected division by zero error.")
        except ZeroDivisionError:
            pass

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / r
        self.assertEqual( self.visitor.walk(e), '1' )
        self.assertEqual(e.size(),1)


class EntangledExpressionErrors(unittest.TestCase):

    def setUp(self):
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.d = variable(name='d')
        self.v = variable(name='v')
        self.p = parameter(0, True, name='p')
        self.q = parameter(0, False, name='q')
        self.r = parameter(1, False, name='r')
        self.visitor = ValueVisitor()

    def test_sumexpr_add_entangled(self):
        a = self.a
        e = a*2 + 1
        self.assertEqual( self.visitor.walk(e), ['+',['*','a','2'],'1'] )
        e += 1
        self.assertEqual( self.visitor.walk(e), ['+',['+',['*','a','2'],'1'],'1'] )

    def test_entangled_test1(self):
        a = self.a
        b = self.b
        c = self.c
        d = self.d

        e1 = a + b
        e2 = c + e1
        e3 = d + e1

        self.assertEqual( self.visitor.walk(e1), ['+','a','b'] )
        self.assertEqual( self.visitor.walk(e2), ['+','c',['+','a','b']] )
        self.assertEqual( self.visitor.walk(e3), ['+','d',['+','a','b']] )


if __name__ == "__main__":
    unittest.main()
