# TODO
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil. Taken from:

# hs114.mod QOR2-MY-10-31
# Original AMPL coding by Elena Bobrovnikova (summer 1996 at Bell Labs).

# Alkylation process

# Ref.: W. Hock and K. Schittkowski, Test Examples for Nonlinear Programming
# Codes.  Lecture Notes in Economics and Mathematical Systems, v. 187,
# Springer-Verlag, New York, 1981, p. 123.

# Number of variables:  10
# Number of constraints:  31
# Objective separable
# Objective nonconvex
# Nonlinear constraints

import poek as pk


model = pk.model()

a = 0.99
b = 0.90
n = 10
I = range(1, n + 1)
lb = {
    1: 0.00001,
    2: 0.00001,
    3: 0.00001,
    4: 0.00001,
    5: 0.00001,
    6: 85,
    7: 90,
    8: 3,
    9: 1.2,
    10: 145,
}
ub = {1: 2000, 2: 16000, 3: 120, 4: 5000, 5: 2000, 6: 93, 7: 95, 8: 12, 9: 4, 10: 162}
x0 = {
    1: 1745,
    2: 12000,
    3: 110,
    4: 3048,
    5: 1974,
    6: 89.2,
    7: 92.8,
    8: 8,
    9: 3.6,
    10: 145,
}

x = model.add_variable(index=I)
for i in I:
    x[i].lb = lb[i]
    x[i].ub = ub[i]
    x[i].value = x0[i]


model.add_objective(5.04 * x[1] + 0.035 * x[2] + 10.0 * x[3] + 3.36 * x[5] - 0.063 * x[4] * x[7])

# shared sub-expressions
G1 = 35.82 - 0.222 * x[10] - b * x[9]
G2 = -133 + 3 * x[7] - a * x[10]
G5 = 1.12 * x[1] + 0.13167 * x[1] * x[8] - 0.00667 * x[1] * x[8] ** 2 - a * x[4]
G6 = 57.425 + 1.098 * x[8] - 0.038 * x[8] ** 2 + 0.325 * x[6] - a * x[7]

model.add_constraint(G1 >= 0)
model.add_constraint(G2 >= 0)
model.add_constraint(-G1 + x[9] * (1 / b - b) >= 0)
model.add_constraint(-G2 + (1 / a - a) * x[10] >= 0)
model.add_constraint(G5 >= 0)
model.add_constraint(G6 >= 0)
model.add_constraint(-G5 + (1 / a - a) * x[4] >= 0)
model.add_constraint(-G6 + (1 / a - a) * x[7] >= 0)
model.add_constraint(1.22 * x[4] - x[1] - x[5] == 0)
model.add_constraint(98000.0 * x[3] / (x[4] * x[9] + 1000 * x[3]) - x[6] == 0)
model.add_constraint((x[2] + x[5]) / x[1] - x[8] == 0)
