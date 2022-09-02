# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk
log = pk.log


model = pk.model()

N = list(range(1,4))

x = model.add_variable(index=N, lb=0, ub=1)
x[1].value = 0.7
x[2].value = 0.2
x[3].value = 0.1

model.add_objective( -32.174*(255*log((x[1]+x[2]+x[3]+0.03)/(0.09*x[1] + x[2] + x[3] + 0.03))\
      +280*log((x[2]+x[3]+0.03)/(0.07*x[2] + x[3] + 0.03))\
      +290*log((x[3]+0.03)/(0.13*x[3] + 0.03))) )
     
model.add_constraint( x[1] + x[2] + x[3] == 1 )
