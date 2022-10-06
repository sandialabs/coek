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
#   J.Ch. Gilbert,
#   "On the Realization of the Wolfe Conditions in Reduced Quasi-Newton
#   Methods for Equality Constrained Optimization",
#   RR-2127, INRIA (F), 1993.

#   SIF input: Ph. Toint, April 1994

#   classification QQR2-AN-V-1

import poek as pk


model = pk.model()

n = 1000

x = model.add_variable(index=range(1, n + 1))
for i in x:
    x[i].value = (-1.0) ** (i + 1) * 10.0

model.add_objective(sum(((n + 1 - i) * x[i] / n - 1.0) ** 2 for i in x) / 2.0)

model.add_constraint((sum(x[i] ** 2 for i in x) - 1.0) / 2.0 == 0.0)
