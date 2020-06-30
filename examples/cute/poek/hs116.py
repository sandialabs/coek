# TODO
# Formulated in pyomo by Logan Barnes. Taken from:

# hs116.mod LQR2-MN-13-41
# Original AMPL coding by Elena Bobrovnikova (summer 1996 at Bell Labs).

# 3-stage membrane separation

# Ref.: W. Hock and K. Schittkowski, Test Examples for Nonlinear Programming
# Codes.  Lecture Notes in Economics and Mathematical Systems, v. 187,
# Springer-Verlag, New York, 1981, p. 124.

# Number of variables: 13
# Number of constraints: 41
# Objective linear
# Nonlinear constraints

import poek as pk


model = pk.model()

N = 13
I = list(range(1,N+1))
a = 0.002
b = 1.262626
c = 1.231059
d = 0.03475
e = 0.975
f = 0.00975

x = model.add_variable(index=I, lb=0)
x[1].value = 0.5
x[2].value = 0.8
x[3].value = 0.9
x[4].value = 0.1
x[5].value = 0.14
x[6].value = 0.5
x[7].value = 489.0
x[8].value = 80.0
x[9].value = 650.0
x[10].value= 450.0
x[11].value= 150.0
x[12].value= 150.0
x[13].value= 150.0

model.add_objective( x[11]+x[12]+x[13] )

model.add_constraint( x[3]-x[2] >= 0)
model.add_constraint( x[2]-x[1] >= 0)
model.add_constraint( 1-a*x[7]+a*x[8] >= 0)
model.add_constraint( x[11]+x[12]+x[13] >= 50)
model.add_constraint( x[13] - b * x[10] + c * x[3] * x[10] >= 0)
model.add_constraint( x[5] - d * x[2] - e * x[2] * x[5] + f * x[2]**2 >= 0)
model.add_constraint( x[6] - d * x[3] - e * x[3] * x[6] + f * x[3]**2 >= 0)
model.add_constraint( x[4] - d * x[1] - e * x[1] * x[4] + f * x[1]**2 >= 0)
model.add_constraint( x[12] - b * x[9] + c * x[2] * x[9] >= 0)
model.add_constraint( x[11] - b * x[8] + c * x[1] * x[8] >= 0)
model.add_constraint( x[5] * x[7] - x[1] * x[8] - x[4] * x[7] + x[4] * x[8] >= 0)
model.add_constraint( 1 - a * (x[2] * x[9] + x[5] * x[8] - x[1] * x[8] - x[6] * x[9]) - x[5] - x[6] >= 0)
model.add_constraint( x[2] * x[9] - x[3] * x[10] - x[6] * x[9] - 500 * x[2] + 500 * x[6] + x[2] * x[10] >= 0)
model.add_constraint( x[2] - 0.9 - a * (x[2] * x[10] - x[3] * x[10]) >= 0)
model.add_constraint( x[11] + x[12] + x[13] <= 250)

model.add_constraint( pk.inequality(0.1, x[1], 1) )
model.add_constraint( pk.inequality(0.1, x[2], 1) )
model.add_constraint( pk.inequality(0.1, x[3], 1) )
model.add_constraint( pk.inequality(0.0001, x[4], 0.1) )
model.add_constraint( pk.inequality(0.1, x[5], 0.9) )
model.add_constraint( pk.inequality(0.1, x[6], 0.9) )
model.add_constraint( pk.inequality(0.1, x[7], 1000) )
model.add_constraint( pk.inequality(0.1, x[8], 1000) )
model.add_constraint( pk.inequality(500, x[9], 1000) )
model.add_constraint( pk.inequality(0.1, x[10], 500) )
model.add_constraint( pk.inequality(1, x[11], 150) )
model.add_constraint( pk.inequality(0.0001, x[12], 150) )
model.add_constraint( pk.inequality(0.0001, x[13], 150) )

