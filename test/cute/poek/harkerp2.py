# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
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
#   P. T. Harker and J.-S. Pang,
#   "A damped Newton method for the linear complementarity problem",
#   in 'Allgower and Georg: Computational solution of nonlinear
#   systems of equations', AMS lectures in Applied Mathematics 26,
#   AMS, Providence, Rhode Island, USA, pp 265-284.

#   SIF input: Nick Gould, July 1993.
#   classification QBR2-AN-V-V

import poek as pk


model = pk.model()

N = 100

x = model.add_variable(index=range(1, N + 1), lb=0.0)
for i in range(1, N + 1):
    x[i].value = i

model.add_objective(
    sum(-1 * x[i] ** 2 * 0.5 for i in range(1, N + 1))
    + sum(-x[i] for i in range(1, N + 1))
    + (sum(x[i] for i in range(1, N + 1))) ** 2
    + sum(2 * (sum(x[i] for i in range(j, N + 1))) ** 2 for j in range(2, N + 1))
)
