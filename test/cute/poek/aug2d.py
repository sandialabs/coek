# TODO
# Formulated in Pyomo by Juan Lopez
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

#   classification QLR2-AN-V-V

import poek as pk


model = pk.model()

nx = 10
ny = 10
snx = list(range(2, nx))
sny = list(range(2, ny))
sx = [(i, j) for i in range(1, nx + 1) for j in range(0, ny + 2)]
sy = [(i, j) for i in range(0, nx + 2) for j in range(1, ny + 1)]

x = model.add_variable(index=sx)
y = model.add_variable(index=sy)


model.add_objective(
    (
        sum((x[i, j] - 1) ** 2 for i in range(1, nx) for j in range(1, ny))
        + sum((y[i, j] - 1) ** 2 for i in range(1, nx) for j in range(1, ny))
        + sum((x[i, ny] - 1) ** 2 for i in range(1, nx))
        + sum((y[nx, j] - 1) ** 2 for j in range(1, ny))
    )
    / 2.0
)

for i in snx:
    for j in sny:
        model.add_constraint(((x[i, j] - x[i - 1, j]) + (y[i, j] - y[i, j - 1]) - 1) == 0)

for i in snx:
    model.add_constraint((x[i, 0] + (x[i, 1] - x[i - 1, 1]) + y[i, 1] - 1) == 0)

for i in snx:
    model.add_constraint(x[i, ny + 1] + (x[i, ny] - x[i - 1, ny]) - y[i, ny - 1] - 1 == 0)

for j in sny:
    model.add_constraint(y[0, j] + (y[1, j] - y[1, j - 1]) + x[1, j] - 1 == 0)

for j in sny:
    model.add_constraint(y[nx + 1, j] + (y[nx, j] - y[nx, j - 1]) - x[nx - 1, j] - 1 == 0)
