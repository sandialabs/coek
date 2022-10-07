# TODO
# Formulated in pyomo by Logan Barnes. Taken from:
#
# hs109.mod OOR2-MY-9-26
# Original AMPL coding by Elena Bobrovnikova (summer 1996 at Bell Labs).

# Ref.: W. Hock and K. Schittkowski, Test Examples for Nonlinear Programming
# Codes.  Lecture Notes in Economics and Mathematical Systems, v. 187,
# Springer-Verlag, New York, 1981, p. 118.

# Number of variables:  9
# Number of constraints:  26
# Objective separable
# Objective nonconvex
# Nonlinear constraints

import poek as pk

sin = pk.sin
cos = pk.cos
import math

model = pk.model()

N = list(range(1, 10))
M = list(range(1, 3))
L = list(range(3, 10))

a = 50.176
b1 = 0.25
b = math.sin(b1)
c = math.cos(b1)

l_init = {}
l_init[1] = 0
l_init[2] = 0
l_init[3] = -0.55
l_init[4] = -0.55
l_init[5] = 196.0
l_init[6] = 196.0
l_init[7] = 196.0
l_init[8] = -400.0
l_init[9] = -400.0

u_init = {}
u_init[1] = 0
u_init[2] = 0
u_init[3] = 0.55
u_init[4] = 0.55
u_init[5] = 252.0
u_init[6] = 252.0
u_init[7] = 252.0
u_init[8] = 800.0
u_init[9] = 800.0

x = model.add_variable(index=N, value=0.0)
for i in N:
    if i in M:
        x[i].lb = 0
    elif i in L:
        x[i].lb = l_init[i]
        x[i].ub = u_init[i]

model.add_objective(3 * x[1] + 1e-6 * x[1] ** 3 + 2 * x[2] + 0.522074e-6 * x[2] ** 3)

model.add_constraint(x[4] - x[3] + 0.55 >= 0)
model.add_constraint(x[3] - x[4] + 0.55 >= 0)
model.add_constraint(2250000 - x[1] ** 2 - x[8] ** 2 >= 0)
model.add_constraint(2250000 - x[2] ** 2 - x[9] ** 2 >= 0)
model.add_constraint(
    x[5] * x[6] * sin(-x[3] - 0.25)
    + x[5] * x[7] * sin(-x[4] - 0.25)
    + 2 * b * x[5] ** 2
    - a * x[1]
    + 400 * a
    == 0
)
model.add_constraint(
    x[5] * x[6] * sin(x[3] - 0.25)
    + x[6] * x[7] * sin(x[3] - x[4] - 0.25)
    + 2 * b * x[6] ** 2
    - a * x[2]
    + 400 * a
    == 0
)
model.add_constraint(
    x[5] * x[7] * sin(x[4] - 0.25)
    + x[6] * x[7] * sin(x[4] - x[3] - 0.25)
    + 2 * b * x[7] ** 2
    + 881.779 * a
    == 0
)
model.add_constraint(
    a * x[8]
    + x[5] * x[6] * cos(-x[3] - 0.25)
    + x[5] * x[7] * cos(-x[4] - 0.25)
    - 200 * a
    - 2 * c * x[5] ** 2
    + 0.7533e-3 * a * x[5] ** 2
    == 0
)
model.add_constraint(
    a * x[9]
    + x[5] * x[6] * cos(x[3] - 0.25)
    + x[6] * x[7] * cos(x[3] - x[4] - 0.25)
    - 2 * c * x[6] ** 2
    + 0.7533e-3 * a * x[6] ** 2
    - 200 * a
    == 0
)
model.add_constraint(
    x[5] * x[7] * cos(x[4] - 0.25)
    + x[6] * x[7] * cos(x[4] - x[3] - 0.25)
    - 2 * c * x[7] ** 2
    + 22.938 * a
    + 0.7533e-3 * a * x[7] ** 2
    == 0
)
