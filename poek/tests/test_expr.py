import math
import pyutilib.th as unittest 

from poek import *


class TestValue(unittest.TestCase):

    def test_var(self):
        p = variable(initialize=2)
        self.assertEqual(p.value, 2)
        p.value = 3
        self.assertEqual(p.value, 3)

    def test_expr(self):
        p = variable(initialize=2)
        self.assertEqual(p.value, 2)
        e = p + 3
        self.assertEqual(e.value, 5)
        with self.assertRaisesRegex(TypeError, "Cannot set the value of an expression"):
            e.value = 0

    def test_constraint(self):
        p = variable(initialize=2)
        self.assertEqual(p.value, 2)
        e = p < 3
        self.assertEqual(e.value, -1)
        with self.assertRaisesRegex(TypeError, "Cannot set the value of a constraint"):
            e.value = 0

    def test_param1(self):
        p = parameter(-1,True)
        p.value = 3
        self.assertEqual(p.value, 3)

        q = parameter(-1,False)
        self.assertEqual(q.value, -1)
        with self.assertRaisesRegex(TypeError, "Cannot set the value of an immutable parameter"):
            q.value = 3

    def test_float(self):
        p = parameter(-1)
        with self.assertRaises(TypeError):
            float(p)
            
    def test_int(self):
        p = parameter(-1)
        with self.assertRaises(TypeError):
            int(p)

    def test_bool(self):
        p = parameter(-1)
        with self.assertRaises(TypeError):
            bool(p)
            

class Test_SumExpression(unittest.TestCase):

    def setUp(self):
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.d = variable(name='d')
        self.p = parameter(0, True, name='p')
        self.visitor = ValueVisitor()

    def test_error1(self):
        with self.assertRaisesRegex(TypeError,"Unexpected type \(LHS\) .*"):
            class TMP(object): pass
            TMP() + self.a

    def test_error2(self):
        x = variable(10,0,0)
        with self.assertRaisesRegex(TypeError,"Cannot treat argument as a single numeric value:.*"):
            self.a + x

    def test_simpleSum(self):
        # a + b
        a = self.a
        b = self.b

        e = a + b
        self.assertEqual( self.visitor.walk(e), ['+','a','b'] )
        self.assertEqual(e.size(), 3)

        e = a
        e += b
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

    def test_constSum(self):
        # a + 5
        a = self.a

        e = a + 5
        self.assertEqual( self.visitor.walk(e), ['+','a','5'] )
        self.assertEqual(e.size(), 3)

        e = 5 + a
        self.assertEqual( self.visitor.walk(e), ['+','5','a'] )
        self.assertEqual(e.size(), 3)

        e = a
        e += 5
        self.assertEqual( self.visitor.walk(e), ['+','a','5'] )
        self.assertEqual(e.size(), 3)

        e = a + 5.0
        self.assertEqual( self.visitor.walk(e), ['+','a','5.000'] )
        self.assertEqual(e.size(), 3)

        e = 5.0 + a
        self.assertEqual( self.visitor.walk(e), ['+','5.000','a'] )
        self.assertEqual(e.size(), 3)

        e = a
        e += 5.0
        self.assertEqual( self.visitor.walk(e), ['+','a', '5.000'] )
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

        e = a + 0.0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = 0.0 + a
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a
        e += 0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a
        e += 0.0
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

    def test_error1(self):
        with self.assertRaisesRegex(TypeError,"Unexpected type \(LHS\).*"):
            class TMP(object): pass
            TMP() - self.a

    def test_error2(self):
        x = variable(10,0,0)
        with self.assertRaisesRegex(TypeError,"Cannot treat argument as a single numeric value:.*"):
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
        self.assertEqual( self.visitor.walk(e), ['-','a','b'] )
        self.assertEqual(e.size(), 3)

        e = a
        e -= b
        self.assertEqual( self.visitor.walk(e), ['-','a','b'] )
        self.assertEqual(e.size(), 3)

    def test_constDiff(self):
        a = self.a

        e = a - 5
        self.assertEqual( self.visitor.walk(e), ['-','a','5'] )
        self.assertEqual(e.size(), 3)

        e = 5 - a
        self.assertEqual( self.visitor.walk(e), ['-','5','a'] )
        self.assertEqual(e.size(), 3)

        e = a
        e -= 5
        self.assertEqual( self.visitor.walk(e), ['-','a','5'] )
        self.assertEqual(e.size(), 3)

        e = a - 5.0
        self.assertEqual( self.visitor.walk(e), ['-','a','5.000'] )
        self.assertEqual(e.size(), 3)

        e = 5.0 - a
        self.assertEqual( self.visitor.walk(e), ['-','5.000','a'] )
        self.assertEqual(e.size(), 3)

        e = a
        e -= 5.0
        self.assertEqual( self.visitor.walk(e), ['-','a','5.000'] )
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

        e = a - 0.0
        self.assertEqual( self.visitor.walk(e), 'a' )
        self.assertEqual(e.size(),1)

        e = a
        e -= 0
        self.assertEqual( self.visitor.walk(e), 'a' )
        self.assertEqual(e.size(),1)

        e = a
        e -= 0.0
        self.assertEqual( self.visitor.walk(e), 'a' )
        self.assertEqual(e.size(),1)

        # 0 - a
        e = 0 - a
        self.assertEqual( self.visitor.walk(e), ['-','a'] )
        self.assertEqual(e.size(),2)

        e = 0.0 - a
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


class Test_MulExpression(unittest.TestCase):

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

    def test_error1(self):
        with self.assertRaisesRegex(TypeError,"Unexpected type \(LHS\).*"):
            class TMP(object): pass
            TMP() * self.a

    def test_error2(self):
        x = variable(10,0,0)
        with self.assertRaisesRegex(TypeError,"Cannot treat argument as a single numeric value:.*"):
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
        self.assertEqual( self.visitor.walk(e), ['*','a','b'] )
        self.assertEqual(e.size(),3)

        #    *
        #   / \
        #  a   b
        e = a
        e *= b
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

        e = a * 5.0
        self.assertEqual( self.visitor.walk(e), ['*','a','5.000'] )
        self.assertEqual(e.size(),3)

        e = a
        e *= 5
        self.assertEqual( self.visitor.walk(e), ['*','a','5'] )
        self.assertEqual(e.size(),3)

        #    *
        #   / \
        #  5   a
        e = 5.0 * a
        self.assertEqual( self.visitor.walk(e), ['*','5.000','a'] )
        self.assertEqual(e.size(),3)

        e = a
        e *= 5.0
        self.assertEqual( self.visitor.walk(e), ['*','a','5.000'] )
        self.assertEqual(e.size(),3)

        e = 5.0 * a
        self.assertEqual( self.visitor.walk(e), ['*','5.000','a'] )
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

        e = a * 0.0
        self.assertEqual(e, 0.0)
        self.assertIs(type(e), float)

        e = a
        e *= 0
        self.assertEqual(e, 0)
        self.assertIs(type(e), int)

        e = a
        e *= 0.0
        self.assertEqual(e, 0.0)
        self.assertIs(type(e), float)

        e = 0 * a
        self.assertEqual(e, 0)
        self.assertIs(type(e), int)

        e = 0.0 * a
        self.assertEqual(e, 0)
        self.assertIs(type(e), float)

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

        e = a * 1.0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a
        e *= 1
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a
        e *= 1.0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = 1 * a
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = 1.0 * a
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

class Test_DivExpression(unittest.TestCase):

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

    def test_error1(self):
        with self.assertRaisesRegex(TypeError,"Unexpected type \(LHS\).*"):
            class TMP(object): pass
            TMP() / self.a

    def test_error2(self):
        x = variable(10,0,0)
        with self.assertRaisesRegex(TypeError,"Cannot treat argument as a single numeric value:.*"):
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
        self.assertEqual( self.visitor.walk(e), ['/','a','b'] )
        self.assertEqual(e.size(),3)

        e = a
        e /= b
        self.assertEqual( self.visitor.walk(e), ['/','a','b'] )
        self.assertEqual(e.size(),3)

    def test_constDivision(self):
        #
        # Check the structure of a simple division with a constant
        #
        a = self.a

        e = a / 5
        self.assertEqual( self.visitor.walk(e), ['/','a','5'] )
        self.assertEqual(e.size(),3)

        e = 5 / a
        self.assertEqual( self.visitor.walk(e), ['/','5','a'] )
        self.assertEqual(e.size(),3)

        e = a
        e /= 5
        self.assertEqual( self.visitor.walk(e), ['/','a','5'] )
        self.assertEqual(e.size(),3)

        e = a / 5.0
        self.assertEqual( self.visitor.walk(e), ['/','a','5.000'] )
        self.assertEqual(e.size(),3)

        e = 5.0 / a
        self.assertEqual( self.visitor.walk(e), ['/','5.000','a'] )
        self.assertEqual(e.size(),3)

        e = a
        e /= 5.0
        self.assertEqual( self.visitor.walk(e), ['/','a','5.000'] )
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
        self.assertRaises(ZeroDivisionError, a.__div__, 0.0)
        self.assertRaises(ZeroDivisionError, a.__idiv__, 0)
        self.assertRaises(ZeroDivisionError, a.__idiv__, 0.0)
        self.assertRaises(ZeroDivisionError, r.__div__, q)

        #
        # Check that dividing zero by anything non-zero gives zero
        #
        e = 0 / a
        self.assertIs(type(e), int)
        self.assertAlmostEqual(e, 0.0)

        e = 0.0 / a
        self.assertIs(type(e), float)
        self.assertAlmostEqual(e, 0.0)

        e = q / a
        self.assertIs(type(e), parameter)
        self.assertIs(e, ZeroParameter)

        #
        # Check that dividing by one 1 gives the original expression
        #
        e = a / 1
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a / 1.0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a
        e /= 1
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a
        e /= 1.0
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        e = a / r
        self.assertIs(type(e), type(a))
        self.assertIs(e, a)

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / a
        self.assertEqual( self.visitor.walk(e), ['/','1','a'] )
        self.assertEqual(e.size(),3)

        e = 1.0 / a
        self.assertEqual( self.visitor.walk(e), ['/','1.000','a'] )
        self.assertEqual(e.size(),3)

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / p
        self.assertEqual( self.visitor.walk(e), ['/','1','p'] )
        self.assertEqual(e.size(),3)

        e = 1.0 / p
        self.assertEqual( self.visitor.walk(e), ['/','1.000','p'] )
        self.assertEqual(e.size(),3)

        #
        # Check the structure dividing 1 by an expression
        #
        try:
            e = 1 / q
            self.fail("Expected division by zero error.")
        except ZeroDivisionError:
            pass

        try:
            e = 1.0 / q
            self.fail("Expected division by zero error.")
        except ZeroDivisionError:
            pass

        #
        # Check the structure dividing 1 by an expression
        #
        e = 1 / r
        self.assertEqual( self.visitor.walk(e), '1' )
        self.assertEqual(e.size(),1)

        e = 1.0 / r
        self.assertEqual( self.visitor.walk(e), '1.000' )
        self.assertEqual(e.size(),1)


class Test_PowExpression(unittest.TestCase):

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

    def test_error1(self):
        with self.assertRaisesRegex(TypeError,"Unexpected type \(LHS\).*"):
            class TMP(object): pass
            TMP() ** self.a

    def test_error2(self):
        x = variable(10,0,0)
        with self.assertRaisesRegex(TypeError,"Cannot treat argument as a single numeric value:.*"):
            self.a ** x

    def test_simplePow(self):
        a = self.a
        b = self.b

        e = a**b
        self.assertEqual( self.visitor.walk(e), ['**','a','b'] )
        self.assertEqual(e.size(),3)

        e = a
        e **= b
        self.assertEqual( self.visitor.walk(e), ['**','a','b'] )
        self.assertEqual(e.size(),3)

    def test_constPow(self):
        #
        # Check the structure of a simple power with a constant
        #
        a = self.a

        e = a ** 5
        self.assertEqual( self.visitor.walk(e), ['**','a','5'] )
        self.assertEqual(e.size(),3)

        e = 5 ** a
        self.assertEqual( self.visitor.walk(e), ['**','5','a'] )
        self.assertEqual(e.size(),3)

        e = a
        e **= 5
        self.assertEqual( self.visitor.walk(e), ['**','a','5'] )
        self.assertEqual(e.size(),3)

        e = a ** 5.0
        self.assertEqual( self.visitor.walk(e), ['**','a','5.000'] )
        self.assertEqual(e.size(),3)

        e = 5.0 ** a
        self.assertEqual( self.visitor.walk(e), ['**','5.000','a'] )
        self.assertEqual(e.size(),3)

        e = a
        e **= 5.0
        self.assertEqual( self.visitor.walk(e), ['**','a','5.000'] )
        self.assertEqual(e.size(),3)

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
        self.assertIs(a, e)

        e = a**1.0
        self.assertIs(a, e)

        e = a
        e **= 1
        self.assertIs(a, e)

        e = a
        e **= 1.0
        self.assertIs(a, e)

        e = a**r
        self.assertIs(a, e)

        #
        # Check that taking the zeroth power returns one
        #
        e = a**0
        self.assertEqual(e, 1)

        e = a**0.0
        self.assertEqual(e, 1)

        e = a
        e **= 0
        self.assertEqual(e, 1)

        e = a
        e **= 0.0
        self.assertEqual(e, 1)

        e = a**q
        self.assertEqual(e, 1)

        #
        # Check that taking powers of 0 and 1 are easy
        #
        e = 0**a
        self.assertEqual(e, 0)

        e = 0.0**a
        self.assertEqual(e, 0.0)

        e = 1**a
        self.assertEqual(e, 1)

        e = 1.0**a
        self.assertEqual(e, 1.0)

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
        self.assertIs(e, 1)

        e = 1.0**a
        self.assertAlmostEqual(e, 1.0)

        e = r**a
        self.assertIs(e, r)

        #
        # Check that taking the zeroth power returns one
        #
        e = 0**a
        self.assertEqual(e, 0)

        e = 0.0**a
        self.assertEqual(e, 0)

        e = q**a
        self.assertEqual(e, 0)

    def test_pow_expresions(self):
        a = self.a
        r = self.r

        e = a**r
        self.assertEqual( self.visitor.walk(e), 'a' )

        e = a**2
        self.assertEqual( self.visitor.walk(e), ['**','a','2'] )

        e = a**2.0
        self.assertEqual( self.visitor.walk(e), ['**','a','2.000'] )

        e = 2**a
        self.assertEqual( self.visitor.walk(e), ['**','2','a'] )

        e = 2.0**a
        self.assertEqual( self.visitor.walk(e), ['**','2.000','a'] )

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


class TestVariables(unittest.TestCase):

    def test_default_value(self):
        v = variable(3, name='v')
        self.assertTrue(math.isnan(v[0].value))
        self.assertTrue(math.isnan(v[1].value))
        self.assertTrue(math.isnan(v[2].value))

    def test_initialize(self):
        v = variable(3, name='v', initialize=3)
        self.assertEqual(v[0].value,3)
        self.assertEqual(v[1].value,3)
        self.assertEqual(v[2].value,3)

    def test_iterator(self):
        v = variable(3, name='v')
        for i in v:
            math.isnan(v[i].value)

    def test_getitem1(self):
        v = variable(3, name='v')
        v1 = v[0]
        v2 = v[0]
        self.assertIs(v1, v2)
    
    def test_getitem2(self):
        v = variable(3)
        v1 = v[0]
        v2 = v[0]
        self.assertIs(v1, v2)

    


if __name__ == "__main__":
    unittest.main()
