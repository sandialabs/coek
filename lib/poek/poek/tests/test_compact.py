#
# This API is not polished.  So this is just a draft set of tests
#

import pyutilib.th as unittest

from poek import *
try:
    from poek import compact_model
    compact_model_available = True
except:
    compact_model_available = False


@unittest.skipIf(not compact_model_available, "Compact models not defined in this Python wrapper for COEK")
class TestExprSequence(unittest.TestCase):

    def test_compact1(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = ExpressionSequence(Forall(i).In(A), y[i])
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["y[%d]" % i ] )
            i += 1

    def test_compact2(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = ExpressionSequence(Forall(i).In(A), y[i]+1)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["+", "y[%d]" % i, to_string(1.0) ] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = ExpressionSequence(Forall(i).In(A), y[i]+i)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["+", "y[%d]" % i, to_string(i*1.0) ] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        B = RangeSet(1,6)
        y = variable(B, name="y")
        i = index("i")
        #
        tmp = ExpressionSequence(Forall(i).In(A), y[i]+i*y[i+2])
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["+", "y[%d]" % i, ["*", to_string(i*1.0), "y[%d]" % (i+2), ], ] )
            i += 1

    def test_compact4(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = Sum(y[i], Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", "y[1]", "y[2]", "y[3]", "y[4]"])

    def test_compact5(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, name="x")
        i = index("i")
        #
        tmp = Sum(x[i,i], Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", 'x[1,1]', 'x[2,2]', 'x[3,3]', 'x[4,4]'])

    def test_compact6(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(x[i,i], Forall(i,j).In(A*A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[4,4]', 'x[4,4]', 'x[4,4]', 'x[4,4]'] )

    def test_compact7(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(x[i,i], Forall(i).In(A) .Forall(j).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["+", 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[4,4]', 'x[4,4]', 'x[4,4]', 'x[4,4]'] )

    def test_compact8(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(y[i]*x[i,i], Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(), ['+', ['*', 'y[1]', 'x[1,1]'], ['*', 'y[2]', 'x[2,2]'], ['*', 'y[3]', 'x[3,3]'], ['*', 'y[4]', 'x[4,4]']] )

    def test_compact9(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(y[i]*Sum(x[i,j], Forall(j).In(A)), Forall(i).In(A))
        e = tmp.expand()
        self.assertEqual( e.to_list(),   ['+', ['*', 'y[1]', ['+', 'x[1,1]', 'x[1,2]', 'x[1,3]', 'x[1,4]']], ['*', 'y[2]', ['+', 'x[2,1]', 'x[2,2]', 'x[2,3]', 'x[2,4]']], ['*', 'y[3]', ['+', 'x[3,1]', 'x[3,2]', 'x[3,3]', 'x[3,4]']], ['*', 'y[4]', ['+', 'x[4,1]', 'x[4,2]', 'x[4,3]', 'x[4,4]']]] )


@unittest.skipIf(not compact_model_available, "Compact models not defined in this Python wrapper for COEK")
class TestConSequence(unittest.TestCase):

    def test_compact1(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = ConstraintSequence(Forall(i).In(A), y[i]==0)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", "y[%d]" % i, to_string(0.0) ] )
            i += 1

    def test_compact2(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = ConstraintSequence(Forall(i).In(A), y[i]+1==0)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", ["+", "y[%d]" % i, to_string(1.0)], to_string(0.0) ] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = ConstraintSequence(Forall(i).In(A), y[i]+i==0)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", ["+", "y[%d]" % i, to_string(i*1.0)] , to_string(0.0)] )
            i += 1

    def test_compact3(self):
        m = compact_model()
        A = RangeSet(1,4)
        B = RangeSet(1,6)
        y = variable(B, name="y")
        i = index("i")
        #
        tmp = ConstraintSequence(Forall(i).In(A), y[i]+i*y[i+2]==0)
        i = 1
        for e in tmp:
            self.assertEqual( e.to_list(), ["==", ["+", "y[%d]" % i, ["*", to_string(i*1.0), "y[%d]" % (i+2)]], to_string(0.0)] )
            i += 1

    def test_compact4(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        i = index("i")
        #
        tmp = Sum(y[i], Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", "y[1]", "y[2]", "y[3]", "y[4]" ], to_string(0.0)])

    def test_compact5(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, name="x")
        i = index("i")
        #
        tmp = Sum(x[i,i], Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", 'x[1,1]', 'x[2,2]', 'x[3,3]', 'x[4,4]'], to_string(0.0)])

    def test_compact6(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(x[i,i], Forall(i,j).In(A*A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[4,4]', 'x[4,4]', 'x[4,4]', 'x[4,4]'], to_string(0.0)] )

    def test_compact7(self):
        m = compact_model()
        A = RangeSet(1,4)
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(x[i,i], Forall(i).In(A) .Forall(j).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ["+", 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[1,1]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[2,2]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[3,3]', 'x[4,4]', 'x[4,4]', 'x[4,4]', 'x[4,4]'], to_string(0.0)] )

    def test_compact8(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(y[i]*x[i,i], Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(), ["==", ['+', ['*', 'y[1]', 'x[1,1]'], ['*', 'y[2]', 'x[2,2]'], ['*', 'y[3]', 'x[3,3]'], ['*', 'y[4]', 'x[4,4]']], to_string(0.0)] )

    def test_compact9(self):
        m = compact_model()
        A = RangeSet(1,4)
        y = variable(A, name="y")
        x = variable(A*A, name="x")
        i = index("i")
        j = index("j")
        #
        tmp = Sum(y[i]*Sum(x[i,j], Forall(j).In(A)), Forall(i).In(A)) == 0
        e = tmp.expand()
        self.assertEqual( e.to_list(),   ["==", ['+', ['*', 'y[1]', ['+', 'x[1,1]', 'x[1,2]', 'x[1,3]', 'x[1,4]']], ['*', 'y[2]', ['+', 'x[2,1]', 'x[2,2]', 'x[2,3]', 'x[2,4]']], ['*', 'y[3]', ['+', 'x[3,1]', 'x[3,2]', 'x[3,3]', 'x[3,4]']], ['*', 'y[4]', ['+', 'x[4,1]', 'x[4,2]', 'x[4,3]', 'x[4,4]']]], to_string(0.0)] )


if __name__ == "__main__":      #pragma:nocover
    unittest.main()


