import math
import pyutilib.th as unittest 

from expr import *


class TestSumExpression(unittest.TestCase):

    def setUp(self):
        self.a = variable(name='a')
        self.b = variable(name='b')
        self.c = variable(name='c')
        self.d = variable(name='d')
        self.p = parameter(0, True)
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





    def Xtest_atanh(self):
        v = self.v
        e = atanh(v)
        v.value = 0.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = (math.e-1.0/math.e)/(math.e+1.0/math.e)
        self.assertAlmostEqual(value(e), 1.0)

if __name__ == "__main__":
    unittest.main()
