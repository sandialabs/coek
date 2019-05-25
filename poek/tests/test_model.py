import pyutilib.th as unittest

from poek import *


class TestModel(unittest.TestCase):

    def test_model1(self):
        m = model()
        v1 = variable(name="v1", initialize=1)
        v2 = variable(name="v2", initialize=2)
        v3 = variable(name="v3", initialize=3)

        e = v1+v2
        m.add( (1-e) + 3*e + e*e + v3 )
        m.add( v1*v1+1 <= 3*v2 )
        m.add( v1**2 >= 3*v2 )
        m.add( 1 < v2 )
        m.add( v1 > 1 )
        m.add( v1 == 3*v2-3 )
        m.show(True)

        m.build()
        #m.show(True)

        self.assertEqual(m.num_objectives(), 1)
        self.assertEqual(m.num_constraints(), 5)

        visitor = ValueVisitor()
        e = m.get_objective()
        self.assertEqual( visitor.walk(e), ['+', ['+', ['+', ['-', '1', ['+', 'v1', 'v2']], ['*', '3', ['+', 'v1', 'v2']]], ['*', ['+', 'v1', 'v2'], ['+', 'v1', 'v2']]], 'v3'] )
        e = m.get_constraint(0)
        self.assertEqual( e.value, -4 )
        e = m.get_constraint(1)
        self.assertEqual( e.value, 5 )
        e = m.get_constraint(2)
        self.assertEqual( e.value, -1 )
        e = m.get_constraint(3)
        self.assertEqual( e.value, 0 )
        e = m.get_constraint(4)
        self.assertEqual( e.value, -2 )

        self.assertEqual(m.compute_f(), 19)
        self.assertEqual(m.compute_c(), [-4, 5, -1, 0, -2])
        self.assertEqual(m.compute_df(), [8,8,1])
        self.assertEqual(m.compute_dc(0), [2,-3,0])
        self.assertEqual(m.compute_dc(1), [-2,3,0])
        self.assertEqual(m.compute_dc(2), [0,-1,0])
        self.assertEqual(m.compute_dc(3), [-1,0,0])
        self.assertEqual(m.compute_dc(4), [1,-3,0])

    def test_error1(self):
        m = model()
        p = parameter(2,False)
        m.add(p)


if __name__ == "__main__":      #pragma:nocover
    unittest.main()


