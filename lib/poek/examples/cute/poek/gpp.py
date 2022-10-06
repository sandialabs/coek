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
#   Hans Mittelmann, private communication.

#   SIF input: N. Gould, Jan 1998

#   classification OOR2-AY-V-0

import poek as pk

exp = pk.exp

model = pk.model()

n = 250

x = model.add_variable(index=range(1, n + 1), value=1.0)

model.add_objective(sum(exp(x[j] - x[i]) for i in range(1, n) for j in range(i + 1, n + 1)))

for i in range(1, n):
    model.add_constraint(x[i] + x[i + 1] >= 0.0)

for i in range(1, n):
    model.add_constraint(exp(x[i]) + exp(x[i + 1]) <= 20.0)
