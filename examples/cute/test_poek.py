#
# Test the NL writer on a subset of the CUTE test cases
#

import sys
import os
import os.path
from os.path import abspath, dirname

import pyutilib.th as unittest
import pyutilib.subprocess
from pyutilib.misc import timing
from . import CUTE_classifications as CUTE
#import CUTE_classifications as CUTE

currdir = os.path.join(dirname(abspath(__file__)), 'poek') + os.sep

"""
The following test function generates an nl file for the test case
and checks that it matches the current pyomo baseline nl file.
"""
@unittest.nottest
def poek_baseline_test(self, name):
    if name in CUTE.baseline_skipped_models:
        self.skipTest('Ignoring test '+name)
        return

    timing.tic("RUNNING: "+name)
    self.poek( currdir+name+'.py', currdir+name+'.test.nl' )
    timing.toc("DONE: "+name)

    # Check that the pyomo nl file matches its own baseline
    self.assertFileEqualsBaseline(
        currdir+name+'.test.nl', currdir+name+'.nl', tolerance=(1e-8, False))


#
# Setup test classes
#
class Tests(unittest.TestCase):

    def poek(self, modelpy, modelnl):
        os.chdir(currdir)
        model = pyutilib.misc.import_file(modelpy)
        model.model.write(modelnl)


class SmokeBaselineTests(Tests):

    def __init__(self, *args, **kwds):
        Tests.__init__(self, *args, **kwds)


@unittest.category('expensive')
class ExpensiveBaselineTests(Tests):

    def __init__(self, *args, **kwds):
        Tests.__init__(self, *args, **kwds)


for name in CUTE.smoke_models:
    SmokeBaselineTests.add_fn_test(fn=poek_baseline_test, name=name)
for name in CUTE.moderate_models:
    ExpensiveBaselineTests.add_fn_test(fn=poek_baseline_test, name=name)
for name in CUTE.expensive_models:
    ExpensiveBaselineTests.add_fn_test(fn=poek_baseline_test, name=name)

if __name__ == "__main__":
    unittest.main()
