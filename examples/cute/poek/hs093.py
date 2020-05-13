# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes and Gabe Hackebeil.

import poek as pk
from poek.util import prod


model = pk.model()

N = list(range(1,7))

x = model.variable(index=N, lb=0)
x[1].value = 5.54
x[2].value = 4.4
x[3].value = 12.02
x[4].value = 11.82
x[5].value = 0.702
x[6].value = 0.852

model.add( 0.0204*x[1]*x[4]*(x[1] + x[2] + x[3]) +\
  0.0187*x[2]*x[3]*(x[1] + 1.57*x[2] + x[4]) +\
  0.0607*x[1]*x[4]*x[5]**2*(x[1] + x[2] + x[3]) +\
  0.0437*x[2]*x[3]*x[6]**2*(x[1] + 1.57*x[2] + x[4]) )
 
model.add( 0.001*prod(x) >= 2.07 )
model.add( 0.00062*x[1]*x[4]*x[5]**2\
                                *(x[1] + x[2] + x[3])\
                                + 0.00058*x[2]*x[3]*x[6]**2*(x[1] + 1.57*x[2] + x[4]) <= 1)
