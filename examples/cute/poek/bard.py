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

#   Source: Problem 3 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#16.
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-3-0

import poek as pk


model = pk.model()

N = 3
M = 15

S = list(range(1,M+1))
# LOAD DATA
y = Param(S)

u = {}
v = {}
w = {}
for i in S:
    u[i] = i
    v[i] = 16-i
    w[i] = min(u[i], v[i])

SS = list(range(1,N+1))
x = model.add_variable(index=SS, value=1.0)

model.add_objective( sum ([( value(y[i])-(x[1]+value(u[i])/(value(v[i])*x[2]+value(w[i])*x[3])) )**2 for i in S])
f = Objective(rule=f,sense=minimize)
