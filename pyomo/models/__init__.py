# pyomo_perf.models

__all__ = ['create_instance']


#
# Create model instance
#
from .fac import fac
from .fac_linear import fac_linear
from .knapsack import knapsack
from .knapsack_linear import knapsack_linear
from .lqcp import lqcp
from .lqcp_linear import lqcp_linear
from .nqueens import nqueens
from .nqueens_linear import nqueens_linear
from .pmedian import pmedian
from .pmedian_linear import pmedian_linear
from .srosenbr import srosenbr

modules = {
            'fac':fac,
            'fac-linear':fac_linear,
            'knapsack':knapsack,
            'knapsack-linear':knapsack_linear,
            'lqcp':lqcp,
            'lqcp-linear':lqcp_linear,
            'nqueens':nqueens,
            'nqueens-linear':nqueens_linear,
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

