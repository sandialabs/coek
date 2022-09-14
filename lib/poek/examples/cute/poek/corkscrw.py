# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, and Brandon C. Barrera
# Taken from:

# AMPL Model by Hande Y. Benson
#
# Copyright (C) 2001 Princeton University
# All Rights Reserved
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby
# granted, provided that the above copyright notice appear in all
# copies and that the copyright notice and this
# permission notice appear in all supporting documentation.

#   Source:
#   Ph. Toint, private communication.

#   SIF input: Ph. Toint, April 1991.

#   classification SOR2-AN-V-V

import poek as pk

sin = pk.sin
cos = pk.cos


model = pk.model()

t = 1000
xt = 10.0
mass = 0.37
tol = 0.1

h = xt / t
w = xt * (t + 1.0) / 2.0

fmax = xt / t

S = list(range(0, t + 1))
SS = list(range(1, t + 1))

x = model.add_variable(index=S)
for i in x:
    x[i].lb = 0
    x[i].ub = xt
y = model.add_variable(index=S)
z = model.add_variable(index=S)
vx = model.add_variable(index=S, value=1.0)
vy = model.add_variable(index=S)
vz = model.add_variable(index=S)

ux = model.add_variable(index=SS)
uy = model.add_variable(index=SS)
uz = model.add_variable(index=SS)
for i in SS:
    ux[i].lb = -fmax
    ux[i].ub = fmax
    uy[i].lb = -fmax
    uy[i].ub = fmax
    uz[i].lb = -fmax
    uz[i].ub = fmax

x[0].value = 0.0
x[0].fixed = True
y[0].value = 0.0
y[0].fixed = True
z[0].value = 1.0
z[0].fixed = True
vx[0].value = 0.0
vx[0].fixed = True
vy[0].value = 0.0
vy[0].fixed = True
vz[0].value = 0.0
vz[0].fixed = True
vx[t].value = 0.0
vx[t].fixed = True
vy[t].value = 0.0
vy[t].fixed = True
vz[t].value = 0.0
vz[t].fixed = True

model.add_objective(sum((i * h / w) * (x[i] - xt) ** 2 for i in SS))

for i in SS:
    model.add_constraint(mass * (vx[i] - vx[i - 1]) / h - ux[i] == 0)
for i in SS:
    model.add_constraint(mass * (vy[i] - vy[i - 1]) / h - uy[i] == 0)
for i in SS:
    model.add_constraint(mass * (vz[i] - vz[i - 1]) / h - uz[i] == 0)
for i in SS:
    model.add_constraint((x[i] - x[i - 1]) / h - vx[i] == 0)
for i in SS:
    model.add_constraint((y[i] - y[i - 1]) / h - vy[i] == 0)
for i in SS:
    model.add_constraint((z[i] - z[i - 1]) / h - vz[i] == 0)
for i in SS:
    model.add_constraint(
        (y[i] - sin(x[i])) ** 2 + (z[i] - cos(x[i])) ** 2 - tol**2 <= 0
    )
