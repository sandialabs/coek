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

#   Source: problem P3 in
#   W.W. Hager,
#   "Multiplier Methods for Nonlinear Optimal Control",
#   SIAM J. on Numercal Analysis 27(4): 1061-1080, 1990.

#   SIF input: Ph. Toint, March 1991.

#   classification SLR2-AY-V-V

import poek as pk


model = pk.model()

n = 5000
h = 1.0/n

Sx = list(range(0,n+1))
Su = list(range(1,n+1))

x = model.variable(index=Sx, value=0.0)
u = model.variable(index=Su, value=0.0)

sum_expr_1 = 0
sum_expr_2 = 0
for i in Su:
    sum_expr_1 += h*(0.625*(x[i-1]**2 + x[i-1]*x[i] + x[i]**2) + ((x[i-1]+x[i])*u[i]))/8
    sum_expr_2 += (h*u[i]**2)/4
obj = sum_expr_1 + sum_expr_2
model.add( obj )

for i in Su:
    con = (n-0.25)*x[i] - (n+0.25)*x[i-1] - u[i]
    model.add( con == 0 )

x[0].value = 1.0
x[0].fixed = True
