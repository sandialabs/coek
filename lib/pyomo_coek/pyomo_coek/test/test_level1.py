# IGNORE
# This is just a check for how to execute tests in a module-agnostic manner.
# The real tests will be in the "level#" directories

# Maybe this should be a directory?


def test_func1(pyomo_module):
    pe = pyomo_module
    model = pe.ConcreteModel()
    model.x = pe.Var()
