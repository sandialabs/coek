# TODO
# Formulated in Pyomo by Gabe Hackebeil
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
#   K. Veselic,
#   "De forma catenarum in campo gravitatis pendentium",
#   Klasicna Gimnazija u Zagrebu, Zagreb, 1987.

#   SIF input: Ph. L. Toint, May 1993.

#   classification LQR2-AY-V-V

import poek as pk


model = pk.model()

N = 165

gamma = 9.81
tmass = 500.0
bl = 1.0
fract = 0.6

length = bl * (N + 1) * fract
mass = tmass / (N + 1)
mg = mass * gamma

S = list(range(0, N + 2))

x = model.add_variable(index=S)
for i in S:
    x[i].value = i * length / (N + 1)
y = model.add_variable(index=S, value=0)
z = model.add_variable(index=S, value=0.0)

model.add_objective(mg * y[0] / 2.0 + sum(mg * y[i] for i in range(1, N + 1)) + mg * y[N + 1] / 2.0)

for i in range(1, N + 2):
    model.add_constraint(
        (x[i] - x[i - 1]) ** 2 + (y[i] - y[i - 1]) ** 2 + (x[i] - z[i - 1]) ** 2 == bl**2
    )

x[0].value = 0.0
x[0].fixed = True
y[0].value = 0.0
y[0].fixed = True
z[0].value = 0.0
z[0].fixed = True
x[N + 1].value = length
x[N + 1].fixed = True
