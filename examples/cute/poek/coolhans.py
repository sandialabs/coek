# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
# Taken from:

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
#   S. Ceria, private communication, 1995.

#   SIF input: Ph. Toint, Feb 1995.

#   classification NQR2-RN-9-9

import poek as pk


model = AbstractModel()

N = RangeSet(1,3)

# LOAD DATA
A = Param(N,N)
B = Param(N,N)
C = Param(N,N)

X = Var(N,N,initialize=0.0)

def _AXX(model,i,j):
    return sum(sum(A[i,k]*X[k,m] for k in range(1,4)) *X[m,j] for m in range(1,4))
AXX = Expression(N, N, rule=_AXX)

def _BX(model,i,j):
    return sum(B[i,k]*X[k,j] for k in range(1,4))
BX = Expression(N, N, rule=_BX)

# For Pyomo testing,
# generate the ConcreteModel version
# by loading the data
import os
if os.path.isfile(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat')):
    model = create_instance(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat'))

def f(model):
    return 0.0
f = Objective(rule=f)
    
def con1(model,i,j):
    return (AXX[i,j] + BX[i,j] + C[i,j]) == 0
matrix = Constraint(N,N,rule=con1)
    
