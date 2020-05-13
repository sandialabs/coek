# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,4))

x = model.variable(index=N)
x[1].value = -5
x[2].value = 5
x[3].value = 0

model.add( (x[1] - x[2])**2 + (x[1] + x[2] - 10.0)**2/9.0\
    + (x[3] - 5)**2 )

model.add( x[1]**2 + x[2]**2 + x[3]**2 <= 48 )
model.add( pk.inequality(-4.5, x[1], 4.5) )
model.add( pk.inequality(-4.5, x[2], 4.5) )
model.add( pk.inequality(-5,   x[3],   5) )
