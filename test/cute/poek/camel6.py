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

#   Source: Six hump camel in
#   L. C. W. Dixon and G. P. Szego (Eds.)
#   Towards Global Optimization
#   North Holland, 1975.

#   SIF input: A.R. Conn May 1995

#   classification OBR2-AN-2-0

import poek as pk


model = pk.model()

S = [1, 2]
x = model.add_variable(index=S, value=1.1)

model.add_objective(
    4 * x[1] ** 2 - 2.1 * x[1] ** 4 + x[1] ** 6 / 3 + x[1] * x[2] - 4 * x[2] ** 2 + 4 * x[2] ** 4
)

model.add_constraint(pk.inequality(-3, x[1], 3))

model.add_constraint(pk.inequality(-1.5, x[2], 1.5))
