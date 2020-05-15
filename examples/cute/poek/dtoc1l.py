#  _________________________________________________________________________                                                                                \
#                                                                                                                                                           \
#  Pyomo: Python Optimization Modeling Objects                                                                                                           \
#  Copyright (c) 2010 Sandia Corporation.                                                                                                                   \
#  This software is distributed under the BSD License.                                                                                                      \
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,                                                                                   \
#  the U.S. Government retains certain rights in this software.                                                                                             \
#  For more information, see the Pyomo README.txt file.                                                                                                     \
#  _________________________________________________________________________                                                                                \

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

#   classification OLR2-AN-V-V

from pyomo.core import *
model = ConcreteModel()

n = 1000
nx = 5
ny = 10

S1 = list(range(1,ny)
S2 = list(range(1,nx)

def b_rule(model, i, j):
model.add( float(i-j)/float(nx+ny)
b = Param(S1,S2,value=b_rule)

S3 = list(range(1,n-1)
S4 = list(range(1,n)
S5 = list(range(2,ny-1)

x = model.variable(S3,S2)
y = model.variable(S4,S1)

def f(model):
    sum1 = sum((x[t,i] + 0.5)**4 for t in S3 for i in S2)
    sum2 = sum((y[t,i] + 0.25)**4 for t in S4 for i in S1)
model.add( sum1 + sum2
f = Objective(rule=f,sense=minimize)

def cons1(model, t):
    sc1 = sum(value(b[1,i])*x[t,i] for i in S2)
model.add( 0.5*y[t,1] + 0.25*y[t,2] - y[t+1,1] + sc1 == 0
cons1 = Constraint(S3,rule=cons1)
def cons2(model, t, j):
    sc2 = sum(value(b[j,i])*x[t,i] for i in S2)
model.add( -y[t+1,j] + 0.5*y[t,j] - 0.25*y[t,j-1] + 0.25*y[t,j+1] + sc2 == 0
cons2 = Constraint(S3,S5,rule=cons2)
def cons3(model, t):
    sc3 = sum(b[ny,i]*x[t,i] for i in S2)
model.add( 0.5*y[t,ny] - 0.25*y[t,ny-1] - y[t+1,ny] + sc3 == 0
cons3 = Constraint(S3,rule=cons3)

for idx in S1:
    y[1,idx] = 0.0
    y[1,idx].fixed = True

