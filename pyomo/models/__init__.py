# pyomo_perf.models

__all__ = ['create_instance']


#
# Create model instance
#
from .fac import fac
from .knapsack import knapsack
from .lqcp import lqcp
from .nqueens import nqueens
from .pmedian import pmedian
from .pmedian_linear import pmedian_linear
from .srosenbr import srosenbr

modules = {
            'fac':fac,
            'knapsack':knapsack,
            'lqcp':lqcp,
            'nqueens':nqueens,
            'pmedian':pmedian,
            'pmedian-linear':pmedian_linear,
            'srosenbr':srosenbr
            }


def print_help():
    print("\nTEST MODELS")
    for name in sorted(modules.keys()):
        print("  "+name)
    print("")
    print("VALID FILENAME SUFFIXES")
    print("  lp     - LP file")
    print("  nl     - NL file")
    print("  gams   - GAMS file")
    print("  mps    - MPS file")
    print("")


#
# Create model instance
#
def create_instance(model_name, data):
    if model_name in modules:
        return modules[model_name](*data)
    return None

