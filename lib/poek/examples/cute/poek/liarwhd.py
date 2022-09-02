# TODO
# Formulated in pyomo by Logan Barnes. Taken from:

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
#   G. Li,
#   "The secant/finite difference algorithm for solving sparse
#   nonlinear systems of equations",
#   SIAM Journal on Optimization, (to appear), 1990.

#   SIF input: Ph. Toint, Aug 1990.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10000

range1 = list(range(1,N+1))

x = model.add_variable(index=range1, value=4.0)

model.add_objective(sum(4.0*(-x[1]+x[i]**2)**2 for i in range1) + sum((x[i]-1.0)**2 for i in range1))
