#
# This API is not polished.  So this is just a draft set of tests
#

import pyutilib.th as unittest

from poek import *


class TestExprSequence(unittest.TestCase):

    def test_compact1(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = (y(i)) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["y(%d)" % i ] )
            i += 1

    def test_compact2(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = (y(i)+1) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["+", "y(%d)" % i, "1.000" ] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = (y(i)+i) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["+", "y(%d)" % i, "%d.000" % i ] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        B = RangeSet(1,6)
        y = variable(B, "y")
        i = index("i")
        #
        tmp = (y(i)+i*y(i+2)) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["+", "y(%d)" % i, ["*", "%d.000" % i, "y(%d)" % (i+2), ], ] )
            i += 1

    def test_compact4(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = Sum((y(i)) .Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", "y(1)", "y(2)", "y(3)", "y(4)"])

    def test_compact5(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, "x")
        i = index("i")
        #
        tmp = Sum((x(i,i)) .Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", "x(1,1)", "x(2,2)", "x(3,3)", "x(4,4)"])

    def test_compact6(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((x(i,i)) .Forall(i,j).In(A*A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(4,4)', 'x(4,4)', 'x(4,4)', 'x(4,4)'] )

    def test_compact7(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((x(i,i)) .Forall(i).In(A) .Forall(j).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(4,4)', 'x(4,4)', 'x(4,4)', 'x(4,4)'] )

    def test_compact8(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((y(i)*x(i,i)) .Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ['+', ['*', 'y(1)', 'x(1,1)'], ['*', 'y(2)', 'x(2,2)'], ['*', 'y(3)', 'x(3,3)'], ['*', 'y(4)', 'x(4,4)']] )

    def test_compact9(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((y(i)*Sum((x(i,j)) .Forall(j).In(A))) .Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(),   ['+', ['*', 'y(1)', ['+', 'x(1,1)', 'x(1,2)', 'x(1,3)', 'x(1,4)']], ['*', 'y(2)', ['+', 'x(2,1)', 'x(2,2)', 'x(2,3)', 'x(2,4)']], ['*', 'y(3)', ['+', 'x(3,1)', 'x(3,2)', 'x(3,3)', 'x(3,4)']], ['*', 'y(4)', ['+', 'x(4,1)', 'x(4,2)', 'x(4,3)', 'x(4,4)']]] )


class TestConSequence(unittest.TestCase):

    def test_compact1(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = (y(i)==0) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", "y(%d)" % i, "0" ] )
            i += 1

    def test_compact2(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = (y(i)+1==0) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", ["+", "y(%d)" % i, "1.000"], "0" ] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = (y(i)+i==0) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", ["+", "y(%d)" % i, "%d.000" % i] , "0"] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        B = RangeSet(1,6)
        y = variable(B, "y")
        i = index("i")
        #
        tmp = (y(i)+i*y(i+2)==0) .Forall(i).In(A)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", ["+", "y(%d)" % i, ["*", "%d.000" % i, "y(%d)" % (i+2)]], "0"] )
            i += 1

    def test_compact4(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        i = index("i")
        #
        tmp = Sum((y(i)) .Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", "y(1)", "y(2)", "y(3)", "y(4)" ], "0"])

    def test_compact5(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, "x")
        i = index("i")
        #
        tmp = Sum((x(i,i)) .Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", "x(1,1)", "x(2,2)", "x(3,3)", "x(4,4)"], "0"])

    def test_compact6(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((x(i,i)) .Forall(i,j).In(A*A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(4,4)', 'x(4,4)', 'x(4,4)', 'x(4,4)'], "0"] )

    def test_compact7(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((x(i,i)) .Forall(i).In(A) .Forall(j).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(1,1)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(2,2)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(3,3)', 'x(4,4)', 'x(4,4)', 'x(4,4)', 'x(4,4)'], "0"] )

    def test_compact8(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((y(i)*x(i,i)) .Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ['+', ['*', 'y(1)', 'x(1,1)'], ['*', 'y(2)', 'x(2,2)'], ['*', 'y(3)', 'x(3,3)'], ['*', 'y(4)', 'x(4,4)']], "0"] )

    def test_compact9(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, "y")
        x = variable(A*A, "x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum((y(i)*Sum((x(i,j)) .Forall(j).In(A))) .Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(),   ["==", ['+', ['*', 'y(1)', ['+', 'x(1,1)', 'x(1,2)', 'x(1,3)', 'x(1,4)']], ['*', 'y(2)', ['+', 'x(2,1)', 'x(2,2)', 'x(2,3)', 'x(2,4)']], ['*', 'y(3)', ['+', 'x(3,1)', 'x(3,2)', 'x(3,3)', 'x(3,4)']], ['*', 'y(4)', ['+', 'x(4,1)', 'x(4,2)', 'x(4,3)', 'x(4,4)']]], "0"] )


if __name__ == "__main__":      #pragma:nocover
    unittest.main()


