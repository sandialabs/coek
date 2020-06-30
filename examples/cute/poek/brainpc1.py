# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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
#   N. Dautrebande,
#   private communication, 1994.

#   SIF input: Ph. Toint, Oct 1994.

#   classification SOR2-MY-6907-6900

import poek as pk


model = pk.model()

NS = 2.0
NP = 5.0
NO = 26.0
H = 1.0
NT = 3450.0

# LOAD DATA
TO = Param(list(range(1,NO))
U = Param(list(range(0,NT))
oc_init = Param(list(range(1,NO))

x = model.add_variable(list(range(1,NS),list(range(0,NT),bounds=(0,None),value=0.001)
k = model.add_variable(list(range(1,NP),bounds=(0,None), value=0.001)

x[1,0].value = 0.0
x[1,0].fixed = True
x[2,0].value = 0.0
x[2,0].fixed = True

model.add_objective( sum((-(x[1,value(TO[t])]+x[2,value(TO[t])])*k[1]+x[1,value(TO[t])]+\
    x[2,value(TO[t])]+U[value(TO[t])]*k[1]-oc_init[t])**2 for t in range(1,int(NO)+1)) )

def con1_rule(model,t):
model.add_constraint( (H*(k[3]+k[4])*x[1,t] - H*k[5]*x[2,t] -\
    H*U[t]*k[2] + x[1,t+1] - x[1,t]) == 0
cons1 = Constraint(list(range(0,int(NT)-1),rule=con1_rule)

def con2_rule(model,t):
model.add_constraint( (H*k[5]*x[2,t] - H*k[4]*x[1,t] +\
    x[2,t+1] - x[2,t]) == 0
cons2 = Constraint(list(range(0,int(NT)-1),rule=con2_rule)
