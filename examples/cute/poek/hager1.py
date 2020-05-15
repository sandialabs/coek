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

#   Source: problem P1 in
#   W.W. Hager,
#   "Multiplier Methods for Nonlinear Optimal Control",
#   SIAM J. on Numerical Analysis 27(4): 1061-1080, 1990.

#   SIF input: Ph. Toint, March 1991.

#   classification SLR2-AN-V-V

import time
import poek as pk


model = pk.model()

N = 5000
Sx = list(range(0,N+1))
Sy = list(range(1,N+1))

x = model.variable(index=Sx, value=0.0)
u = model.variable(index=Sy, value=0.0)

sum_expr = sum((u[i]**2)/(2*N) for i in Sy)
expr = 0.5*x[N]**2 + sum_expr
model.add( expr )

for i in Sy:
    body = (N-0.5)*x[i] + (-N - 0.5)*x[i-1] - u[i]
    model.add( body == 0 )

model.add( x[0] == 1 )
