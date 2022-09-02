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

#   Source: Problem 16 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#30
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-4-0

import math
import poek as pk


model = pk.model()

M = 20
N = 4

S = [1,2,3,4]
St = list(range(1,M+1))

x = model.add_variable(index=S)
x[1].value = 25
x[2].value = 5
x[3].value = -5
x[4].value = -1

t = {}
for i in St:
    t[i] = i/5.0

expa = sum(( (x[1]+t[i]*x[2]-math.exp(t[i]))**2 \
    + (x[3]+x[4]*math.sin(t[i])-math.cos(t[i]))**2 )**2 for i in St)
model.add_objective( expa )
