# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

from math import sqrt
import poek as pk
sin = pk.sin


model = pk.model()

N = list(range(1,6))

x = model.add_variable(index=N, value=2.0)

model.add_objective( (x[1]-1)**2 + (x[1] - x[2])**2\
                                 + (x[3]-1)**2 + (x[4]-1)**4 + (x[5]-1)**6)
                                 
model.add_constraint( x[1]**2*x[4] + sin(x[4]-x[5]) == 2*sqrt(2))
model.add_constraint( x[2] + x[3]**4*x[4]**2 == 8 + sqrt(2))
