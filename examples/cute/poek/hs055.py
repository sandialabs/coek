# TODO
# Taken from cute suite. Formulated in pyomo by Gabriel Hackebeil and Logan Barnes.

import poek as pk
exp = pk.exp


model = pk.model()

x = model.variable(index=list(range(1,7)), value=0.0)
x[1].ub = 1
x[4].ub = 1

x[1].value = 1.0
x[2].value = 2.0
x[6].value = 2.0

model.add( x[1] + 2.0*x[2] + 4.0*x[5] + exp(x[1]*x[4]))

model.add( x[1] + 2*x[2] + 5*x[5] == 6)
model.add( x[1] + x[2] + x[3] == 3)
model.add( x[4] + x[5] + x[6] == 2)
model.add( x[1] + x[4] == 1)
model.add( x[2] + x[5] == 2)
model.add( x[3] + x[6] == 2)
#model.add( x[1] <= 1)
#model.add( x[4] <= 1)
