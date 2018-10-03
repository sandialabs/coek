import pyutilib.th as unittest

from poek.expr import *
from poek.intrinsic import *


class TestModel(unittest.TestCase):

    def test_model1(self):
        v1 = variable(name="v1")
        v2 = variable(name="v2")
        v3 = variable(name="v3")
        m = model()
        e = v1+v2
        m.add( (1-e) + 3*e + e*e + v3 )
        m.add( v1*v1+1 <= 3*v2 )
        m.add( v1**2 >= 3*v2 )
        m.add( 1 < v2 )
        m.add( v1 > 1 )
        m.add( v1 == 3*v2-3 )
        m.show(1)
        m.build()
        m.show(1)

    def test_error1(self):
        p = parameter(2,False)
        m = model()
        m.add(p)


if __name__ == "__main__":      #pragma:nocover
    unittest.main()


