# TODO
# Formulated in Pyomo by Carl D. Laird and Daniel P. Word
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
#   A. R. Conn, N. I. M. Gould and Ph. L. Toint,
#   "LANCELOT: a Fortran package for large-scale nonlinear optimization
#    (Release A)", Springer Series in Computational Mathematics 17,
#   Springer Verlag, 1992

#   SIF input: A. R. Conn, Nick Gould and Ph. L. Toint, June 1994.

#   classification OOR2-AY-V-V

import poek as pk

sin = pk.sin


model = pk.model()

n = 100
S = list(range(1, n + 1))

y = model.add_variable()
x = model.add_variable(index=S, value=0.5)
for i in S:
    x[i].lb = -1
    x[i].ub = i

model.add_objective(0.5 * ((x[1] - x[n]) * x[2] + y) ** 2)

for i in range(1, n):
    model.add_constraint(y + x[1] * x[i + 1] + (1 + 2.0 / i) * x[i] * x[n] <= 0)

for i in S:
    model.add_constraint(sin(x[i]) ** 2 <= 0.5)

model.add_constraint(1 == (x[1] + x[n]) ** 2)
