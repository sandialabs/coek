# TODO
# Formulated in pyomo by Logan Barnes.

import poek as pk
log = pk.log

def prod(x):
    expr = 1.0
    for j in N:
        expr *= x[j]
    return expr


model = pk.model()

N = list(range(1,11))

x = model.variable(index=N, lb=2.001, ub=9.999, value=9.0)

model.add( sum(log(x[j]-2.0)**2 + log(10.0 - x[j])**2 for j in N) - prod(x)**0.2 )
