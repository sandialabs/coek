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

#   Source: problem P4 in
#   W.W. Hager,
#   "Multiplier Methods for Nonlinear Optimal Control",
#   SIAM J. on Numercal Analysis 27(4): 1061-1080, 1990.

#   SIF input: Ph. Toint, April 1991.

#   classification OLR2-AN-V-V

import poek as pk
from math import exp


model = pk.model()

n = 5000
h = 1.0/5000.0
one = 1
e = exp(1)
xx0 = (1+3*e)/(2-2*e)

Sx = list(range(0,n+1))
Su = list(range(1,n+1))
S = [0,1]

t = {i:i*h for i in Sx}
z = {i:exp(-2*t[i]) for i in Sx}
a = {i:-0.5*z[i] for i in S}
b = {i:a[i]*(t[i]+0.5) for i in S}
c = {i:a[i]*(t[i]**2 + t[i] + 0.5) for i in S}
scda = (a[1]-a[0])/2
scdb = (b[1]- b[0])*n
scdc = (c[1]- c[0])*n*n*0.5

x = model.add_variable(index=Sx, value=0.0)
u = model.add_variable(index=Su, ub=1, value=0.0)

sum_expr_1 = 0
sum_expr_2 = 0
for i in Su:
    sum_expr_1 += (scda*z[i-1]*x[i]**2 + scdb*z[i-1]*x[i]*(x[i-1]-x[i]) + scdc*z[i-1]*(x[i-1]-x[i])**2)
    sum_expr_2 += ((h)*(u[i])**2)*0.5
obj = sum_expr_1 + sum_expr_2
model.add_objective( obj )

for i in Su:
    model.add_constraint( 0 == (n-1)*x[i] - n*x[i-1] - exp(t[i])*u[i] )

x[0].value = xx0
x[0].fixed = True
