# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1,9))
x = model.variable(index=N, lb=0.1, ub=10.0)
x[1].value = 6.0
x[2].value = 3.0
x[3].value = 0.4
x[4].value = 0.2
x[5].value = 6.0
x[6].value = 6.0
x[7].value = 1.0
x[8].value = 0.5

model.add( (0.4*x[1]**0.67) * (x[7]**-0.67) + (0.4*x[2]**0.67)\
    * (x[8]**-0.67) + 10 - x[1] - x[2])

model.add( 1-0.0588*x[5]*x[7]-.1*x[1]>=0)
model.add( 1-0.0588*x[6]*x[8]-.1*x[1]-.1*x[2]>=0)
model.add( 1-(4.0*x[3]/x[5])-\
    (2.0/(x[3]**0.71*x[5]))-(0.0588*x[7]/(x[3]**1.3))>=0)
model.add( 1-4.0*x[4]/x[6]-\
    (2.0/(x[4]**0.71*x[6]))-(0.0588*x[8]/(x[4]**1.3))>=0)
model.add( 0.4*(x[1]**0.67)*(x[7]**(-0.67))\
    +.4*(x[2]**0.67)*(x[8]**(-0.67))+10-x[1]-x[2]>=0.1)
model.add( 0.4*(x[1]**0.67)*(x[7]**(-0.67))\
    +.4*(x[2]**0.67)*(x[8]**(-0.67))+10-x[1]-x[2]<=4.2)
