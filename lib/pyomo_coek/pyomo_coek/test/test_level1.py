# Maybe this should be a directory?


def test_func1(pyomo_module):
    pe = pyomo_module
    model = pe.ConcreteModel()
    model.x = pe.Var()
