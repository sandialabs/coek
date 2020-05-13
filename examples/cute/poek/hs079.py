# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

import poek as pk
from math import sqrt


model = pk.model()

N = list(range(1,6))

x = model.variable(index=N, value=2.0)

model.add( (x[1]-1)**2 + (x[1]-x[2])**2 + (x[2]-x[3])**2\
                                 + (x[3]-x[4])**4 + (x[4]-x[5])**4)

model.add( x[1] + x[2]**2 + x[3]**3 == 2 + 3*sqrt(2))
model.add( x[2] - x[3]**2 + x[4]    == -2 + 2*sqrt(2))
model.add( x[1] * x[5] == 2)
