import math
import pyutilib.th as unittest

from poek.expr import *


class Test_ConditionalValue(unittest.TestCase):

    def setUp(self):
        #
        # Create Model
        #
        Test_ConditionalValue.setUp(self)
        #
        # Create model instance
        #
        self.expectExpression = True
        self.expectConstExpression = False

    def create(self, val):
        tmp=variable()
        tmp.value=val
        return tmp


if __name__ == "__main__":
    unittest.main()
