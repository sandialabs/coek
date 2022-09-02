# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes and Gabe Hackebeil.

import poek as pk


model = pk.model()

N = 8
I = list(range(1,N+1))
M = list(range(2,4))
L = list(range(4,9))
a = 0.0025
b = 0.01
c = 833.3325
d = 100.0
e = 83333.33
f = 1250.0
g = 1250000.0
h = 2500.0

x = model.add_variable(index=I)
x[1].value = 5000.0
x[2].value = 5000.0
x[3].value = 5000.0
x[4].value =  200.0
x[5].value =  350.0
x[6].value =  150.0
x[7].value =  225.0
x[8].value =  425.0

model.add_objective( x[1] + x[2] + x[3] )

model.add_constraint( 1 - a * (x[4] + x[6]) >= 0)
model.add_constraint( 1 - a * (x[5] + x[7] - x[4]) >= 0)
model.add_constraint( 1 - b * (x[8] - x[5]) >= 0)
model.add_constraint( x[1] * x[6] - c * x[4] - d * x[1] + e >= 0)
model.add_constraint( x[2] * x[7] - f * x[5] - x[2] * x[4] + f *x[4] >= 0)
model.add_constraint( x[3] * x[8] - g - x[3] * x[5] + h * x[5] >= 0)
model.add_constraint( pk.inequality(100, x[1], 10000) )
for i in M:
    model.add_constraint( pk.inequality(1000, x[i], 10000) )
for i in L:
    model.add_constraint( pk.inequality(10, x[i], 1000) )
