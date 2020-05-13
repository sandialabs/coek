# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,4))

x = model.variable(index=N, value=2)

model.add( 1000 - x[1]**2 - 2*x[2]**2 - x[3]**2 - x[1]\
        *x[2] - x[1]*x[3] )

model.add( 8*x[1] + 14*x[2] + 7*x[3] == 56 )
model.add( x[1]**2 +  x[2]**2 + x[3]**2 == 25 )
