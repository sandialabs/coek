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

#   Source: Problem 74 in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SXR2-AN-6-0

import poek as pk
from math import exp


model = pk.model()

N = 6
M = 13
S = list(range(1,N+1))
SS = list(range(1,M+1))

xinit = {}
xinit[1] = 1
xinit[2] = 2
xinit[3] = 1
xinit[4] = 1
xinit[5] = 4
xinit[6] = 3

x = model.add_variable(index=S)
for i in S:
    x[i].value = xinit[i]

sum1 = 0
for i in SS:
    sum1 += (-exp(-0.1*i)+5*exp(-i)-3*exp(-0.4*i)+ x[3]*pk.exp(-0.1*i*x[1])\
    - x[4]*pk.exp(-0.1*i*x[2]) + x[6]*pk.exp(-0.1*i*x[5]))**2
model.add_objective( sum1 )

model.add_constraint( x[6] == 3 )
