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

#   Source:
#   M. Batholomew-Biggs and F.G. Hernandez,
#   "Some improvements to the subroutine OPALQP for dealing with large
#    problems",
#   Numerical Optimization Centre, Hatfield, 1992.

#   SIF input: Ph Toint, April 1992.

#   classification QBR2-AN-V-V

import poek as pk


model = pk.model()

N = 1000
x = model.add_variable(index=range(1, N + 1))

model.add_objective(
    (x[1] - 1) ** 2 + sum((x[i + 1] - x[i]) ** 2 for i in range(1, N)) + (1 - x[N]) ** 2
)

for i in range(1, N):
    model.add_constraint(pk.inequality(0.0, x[i], 0.9))
