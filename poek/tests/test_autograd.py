import math
import pyutilib.th as unittest

from poek import *


class Test_Diff(unittest.TestCase):

    def setUp(self):
        self.model = model()

    def tearDown(self):
        self.model = None

    def test_param(self):
        p = parameter(2.0)
        v = variable(name='v')
        diff = p.diff(v)
        self.assertEqual(diff, ZeroParameter)

    def test_var(self):
        v = variable(name='v')
        w = variable(name='w')

        diff = w.diff(v)
        self.assertEqual(diff, ZeroParameter)

        diff = v.diff(v)
        self.assertEqual(diff, OneParameter)

    def test_expression(self):
        v = variable(name='v')
        w = variable(name='w')
        z = variable(name='z')
        visitor = ValueVisitor()

        e = w*v**2
        diff = e.diff(v)
        self.assertEqual( visitor.walk(diff), ['*','w',['*','2',['**','v',['-','2','1']]]] )
        diff = e.diff(w)
        self.assertEqual( visitor.walk(diff), ['**','v','2'] )
        diff = e.diff(z)
        self.assertEqual( visitor.walk(diff), '0' )
        diffs = e.diff([v,w,z])
        self.assertEqual( visitor.walk(diffs[0]), ['*','w',['*','2',['**','v',['-','2','1']]]] )
        self.assertEqual( visitor.walk(diffs[1]), ['**','v','2'] )
        self.assertEqual( visitor.walk(diffs[2]), '0' )

        e = w*v**2 + w**3
        diff = e.diff(v)
        self.assertEqual( visitor.walk(diff), ['*','w',['*','2',['**','v',['-','2','1']]]] )
        diff = e.diff(w)
        self.assertEqual( visitor.walk(diff), ['+',['**','v','2'],['*','3',['**','w',['-','3','1']]]] )

    def test_error1(self):
        v = variable(name='v')
        w = variable(name='w')

        e = w*v**2
        with self.assertRaisesRegex(TypeError, "Badly formated argument"):
            diff = e.diff({})


if __name__ == "__main__":
    unittest.main()
