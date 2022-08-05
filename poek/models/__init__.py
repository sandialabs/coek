# poek.models

__all__ = ['create_instance']


from .knapsack import knapsack
from .knapsack_affine import knapsack_affine
from .nqueens import nqueens
from .nqueens_affine import nqueens_affine
from .pmedian import pmedian
from .pmedian_affine import pmedian_affine
from .srosenbr import srosenbr

modules = {'knapsack':knapsack,
           'knapsack_affine':knapsack_affine,
           'nqueens':nqueens,
           'nqueens_affine':nqueens_affine,
           'pmedian':pmedian,
           'pmedian_affine':pmedian_affine,
           'srosenbr':srosenbr}


def print_help():
    print("\nTEST MODELS")
    for name in sorted(modules.keys()):
        print("  "+name)
    print("")
    print("VALID FILENAME SUFFIXES")
    print("  lp     - Canonical LP file, written with FMT library")
    print("  fmtlp  - Canonical LP file, written with FMT library")
    print("  ostrlp - Canonical LP file, written with C++ ostream")
    print("  nl     - Canonical NL file, written with FMT library")
    print("  fmtnl  - Canonical NL file, written with FMT library")
    print("  ostrnl - Canonical NL file, written with C++ ostream")
    print("")


#
# Create model instance
#
def create_instance(model_name, data):
    if model_name in modules:
        return modules[model_name](*data)
    return None

