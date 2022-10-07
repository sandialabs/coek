# gurobipy_perf.models

__all__ = ["create_instance"]

import sys

try:
    import gurobipy
except:
    print("ERROR: Gurobipy is not installed")
    sys.exit(1)

from .knapsack import knapsack
from .nqueens import nqueens
from .pmedian import pmedian

modules = {"knapsack": knapsack, "nqueens": nqueens, "pmedian": pmedian}


def print_help():
    print("\nTEST MODELS")
    for name in sorted(modules.keys()):
        print("  " + name)
    print("")
    print("VALID FILENAME SUFFIXES")
    print("  lp     - Canonical LP file")
    print("")


#
# Create model instance
#
def create_instance(model, data):
    if model in modules:
        return modules[model](*data)
    return None
