# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk
from math import sqrt


model = pk.model()

N = list(range(1,4))

x = model.variable(index=N, value=2.0, lb=-10, ub=10)

model.add( (x[1] - 1)**2 + (x[1] - x[2])**2 + (x[2] - x[3])**4 )

model.add( x[1]*(1 + x[2]**2) + x[3]**4 == 4 + 3*sqrt(2) )
