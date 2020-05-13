# Adapted from Pyomo model by William E. Hart
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

#   Source:  Problem 15 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-MN-4-0

import json
import poek as pk


model = pk.model()

N = 4
M = 11
n = list(range(1,N+1))
m = list(range(1,M+1))

with open('kowosb.json', 'r') as INPUT:
    data = json.load(INPUT)
y = {int(key):value for key,value in data["y"].items()}
u = {int(key):value for key,value in data["u"].items()}

x = model.variable(index=n)
x[1].value = 0.25
x[2].value = 0.39
x[3].value = 0.415
x[4].value = 0.39

model.add( sum((y[i]-x[1]*(u[i]**2+u[i]*x[2])/(u[i]**2+u[i]*x[3]+x[4]))**2 for i in m) )
