import math
import pyutilib.th as unittest 

from expr import *
from intrinsic import *


class TestExpression_Intrinsic(unittest.TestCase):

    def setUp(self):
        self.v = variable()
        self.p = parameter(0, True)

    def test_ceil(self):
        v = self.v
        e = ceil(v)
        v.value = 1.5
        self.assertAlmostEqual(value(e), 2.0)
        v.value = -1.5
        self.assertAlmostEqual(value(e), -1.0)

    def test_floor(self):
        v = self.v
        e = floor(v)
        v.value = 1.5
        self.assertAlmostEqual(value(e), 1.0)
        v.value = -1.5
        self.assertAlmostEqual(value(e), -2.0)

    def test_exp(self):
        v = self.v
        e = exp(v)
        v.value = 1
        self.assertAlmostEqual(value(e), math.e)
        v.value = 0
        self.assertAlmostEqual(value(e), 1.0)

    def test_log(self):
        v = self.v
        e = log(v)
        v.value = 1
        self.assertAlmostEqual(value(e), 0)
        v.value = math.e
        self.assertAlmostEqual(value(e), 1)

    def test_log10(self):
        v = self.v
        e = log10(v)
        v.value = 1
        self.assertAlmostEqual(value(e), 0)
        v.value = 10
        self.assertAlmostEqual(value(e), 1)

    def test_pow(self):
        v = self.v
        p = self.p
        e = pow(v,p)
        v.value = 2
        p.value = 0
        self.assertAlmostEqual(value(e), 1.0)
        v.value = 2
        p.value = 1
        self.assertAlmostEqual(value(e), 2.0)

    def test_sqrt(self):
        v = self.v
        e = sqrt(v)
        v.value = 1
        self.assertAlmostEqual(value(e), 1.0)
        v.value = 4
        self.assertAlmostEqual(value(e), 2.0)

    def test_sin(self):
        v = self.v
        e = sin(v)
        v.value = 0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = math.pi/2.0
        self.assertAlmostEqual(value(e), 1.0)

    def test_cos(self):
        v = self.v
        e = cos(v)
        v.value = 0
        self.assertAlmostEqual(value(e), 1.0)
        v.value = math.pi/2.0
        self.assertAlmostEqual(value(e), 0.0)

    def test_tan(self):
        v = self.v
        e = tan(v)
        v.value = 0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = math.pi/4.0
        self.assertAlmostEqual(value(e), 1.0)

    def test_asin(self):
        v = self.v
        e = asin(v)
        v.value = 0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = 1.0
        self.assertAlmostEqual(value(e), math.pi/2.0)

    def test_acos(self):
        v = self.v
        e = acos(v)
        v.value = 1.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = 0.0
        self.assertAlmostEqual(value(e), math.pi/2.0)

    def test_atan(self):
        v = self.v
        e = atan(v)
        v.value = 0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = 1.0
        self.assertAlmostEqual(value(e), math.pi/4.0)

    def test_sinh(self):
        v = self.v
        e = sinh(v)
        v.value = 0.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = 1.0
        self.assertAlmostEqual(value(e), (math.e-1.0/math.e)/2.0)

    def test_cosh(self):
        v = self.v
        e = cosh(v)
        v.value = 0.0
        self.assertAlmostEqual(value(e), 1.0)
        v.value = 1.0
        self.assertAlmostEqual(value(e), (math.e+1.0/math.e)/2.0)

    def test_tanh(self):
        v = self.v
        e = tanh(v)
        v.value = 0.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = 1.0
        self.assertAlmostEqual(value(e), (math.e-1.0/math.e)/(math.e+1.0/math.e))

    def test_asinh(self):
        v = self.v
        e = asinh(v)
        v.value = 0.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = (math.e-1.0/math.e)/2.0
        self.assertAlmostEqual(value(e), 1.0)

    def test_acosh(self):
        v = self.v
        e = acosh(v)
        v.value = 1.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = (math.e+1.0/math.e)/2.0
        self.assertAlmostEqual(value(e), 1.0)

    def test_atanh(self):
        v = self.v
        e = atanh(v)
        v.value = 0.0
        self.assertAlmostEqual(value(e), 0.0)
        v.value = (math.e-1.0/math.e)/(math.e+1.0/math.e)
        self.assertAlmostEqual(value(e), 1.0)

if __name__ == "__main__":
    unittest.main()
