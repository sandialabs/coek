# TODO

import random
random.seed(1000)

from pyomo.environ import *
from poek.util import pyomo_to_poek


#
# Create a p-median optimization problem.  
# * Variables are initialized to zero to suppress warnings in the pyomo_to_poek function.
# * Parameters are mutable to illustrate their capture in pyomo_to_poek.
#
def create_model():

    model = AbstractModel()

    model.N = Param(within=PositiveIntegers)

    model.Locations = RangeSet(1,model.N)

    model.P = Param(within=RangeSet(1,model.N))

    model.M = Param(within=PositiveIntegers)

    model.Customers = RangeSet(1,model.M)

    model.d = Param(model.Locations, model.Customers, initialize=lambda n, m, model : random.uniform(1.0,2.0), within=Reals, mutable=True)

    model.x = Var(model.Locations, model.Customers, bounds=(0.0,1.0), initialize=0)

    model.y = Var(model.Locations, within=Binary, initialize=0)

    def rule(model):
        return sum( model.d[n,m]*model.x[n,m] for n in model.Locations for m in model.Customers )
    model.obj = Objective(rule=rule)

    def rule(model, m):
        return (sum( model.x[n,m] for n in model.Locations ), 1.0)
    model.single_x = Constraint(model.Customers, rule=rule)

    def rule(model, n,m):
        return (None, model.x[n,m] - model.y[n], 0.0)
    model.bound_y = Constraint(model.Locations, model.Customers, rule=rule)

    def rule(model):
        return (sum( model.y[n] for n in model.Locations ) - model.P, 0.0)
    model.num_facilities = Constraint(rule=rule)

    return model


#
# Create the Pyomo model
#
model = create_model()
instance = model.create_instance(filename='pmedian.dat')
#
# Translate to POEK
#
pm = pyomo_to_poek(instance)
#
# Display the model
#
pm.poek_model.print_equations()
#
# Mappings for variables and parameters
#
print("Variables: id(vardata) -> poek-variable")
for key, vardata in pm.pyo2pk_var.items():
    print(key, vardata, vardata.name)
print("Parameters: id(paramdata) -> poek-parameter")
for key, paramdata in pm.pyo2pk_param.items():
    print(key, paramdata, paramdata.name)
