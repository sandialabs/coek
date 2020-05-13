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
#   L.C.W. Dixon, personnal communication, Jan 1991.

#   SIF input: Ph. Toint, Feb 1991.

#   classification SOR2-AN-10-5

import poek as pk


model = pk.model()

S = list(range(1,11))
SS = list(range(1,8))

x0 = {
1:-2,
2:-0.5,
3:3,
4:0.33333,
5:-4,
6:-0.25,
7:5,
8:0.2,
9:-6,
10:-0.16667
}

x = model.variable(index=S)
for i in S:
    x[i].value = x0[i]

es1 = sum(100*(x[i+1]-x[i]**2)**2 for i in SS)
es2 = sum((x[i]-1)**2 for i in SS)
es3 = sum(90*(x[i+3]-x[i+2]**2)**2 for i in SS)
es4 = sum((x[i+2]-1)**2 for i in SS)
es5 = sum(10.1*(x[i+1]-1)**2 for i in SS)
es6 = sum(10.1*(x[i+3]-1)**2 for i in SS)
es7 = sum(19.8*(x[i+1]-1)*(x[i+3]-1) for i in SS)
model.add( es1 + es2 + es3 + es4 + es5 + es6 + es7 )


for i in [2,4,6,8,10]:
    expr = 1.0
    for j in range(1,i+1):
        expr *= x[j]
    model.add( expr == 1 )

"""
def cons1(model):
    ep1 = x[1] * x[2] - 1
    return ep1 == 0
cons1 = Constraint(rule=cons1)
def cons2(model):
    ep2 = x[1] * x[2] * x[3] * x[4]
    return ep2 - 1 == 0
cons2 = Constraint(rule=cons2)
def cons3(model):
    ep3 = x[1] * x[2] * x[3] * x[4] * x[5] * x[6]
    return ep3 - 1 == 0
cons3 = Constraint(rule=cons3)
def cons4(model):
    ep4 = x[1] * x[2] * x[3] * x[4] * x[5] * x[6] * x[7] * x[8]
    return ep4 - 1 == 0
cons4 = Constraint(rule=cons4)
def cons5(model):
    ep5 = x[1] * x[2] * x[3] * x[4] * x[5] * x[6] * x[7] * x[8] * x[9] * x[10]
    return ep5 - 1 == 0
cons5 = Constraint(rule=cons5)
"""
