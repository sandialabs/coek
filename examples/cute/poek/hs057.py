# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes and Gabriel Hackebeil.

import poek as pk


model = pk.model()

N = list(range(1,45))
M = list(range(1,3))

x = model.add_variable(index=M)
x[1].value = 0.42
x[2].value = 5.0

# LOAD DATA
b = Param(N)
a = Param(N)

model.add_objective( sum((b[i] - x[1] - (0.49 - x[1])*exp(-x[2]*(a[i]-8)))**2 for i in N) )

model.add_constraint( 0.09 <= 0.49*x[2] - x[1]*x[2] )
model.add_constraint( 0.4 <= x[1] )
model.add_constraint( -4 <= x[2] )
