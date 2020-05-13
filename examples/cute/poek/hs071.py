# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,5))

x = model.variable(index=N, lb=1, ub=5)
x[1].value = 1
x[2].value = 5
x[3].value = 5
x[4].value = 1

model.add( x[1]*x[4]*(x[1] + x[2] + x[3]) + x[3] )

expr = 1.0
for i in range(1,5):
    expr *= x[i]
model.add( expr >= 25 )

model.add( sum(x[i]**2 for i in N) == 40)
