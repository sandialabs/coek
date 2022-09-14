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
#   K.M. Irani, M.P. Kamat, C.J. Ribbens, H.F.Walker and L.T. Watson,
#   "Experiments with conjugate gradient algoritms for homotopy curve
#    tracking" ,
#   SIAM Journal on Optimization, May 1991, pp. 222-251, 1991.

#   SIF input: Ph. Toint, May 1990.

#   classification NOR2-AN-V-V

import poek as pk

atan = pk.atan
sin = pk.sin


model = pk.model()

N = 5000

x = model.add_variable(N + 2, value=1.0)

model.add_objective(pk.expression(0))

for i in range(1, N + 1):
    model.add_constraint(
        0 == -0.05 * (x[i] + x[i + 1] + x[i - 1]) + atan(sin((i % 100) * x[i]))
    )

x[0].value = 0.0
x[0].fixed = True

x[N + 1].value = 0.0
x[N + 1].fixed = True
