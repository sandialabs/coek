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

#   Source:
#   P.N. Brown and Y. Saad,
#   "Hybrid Krylov Methods for Nonlinear Systems of Equations",
#   SIAM J. Sci. Stat. Comput. 11, pp. 450-481, 1990.
#   The boundary conditions have been set according to
#   I.E. Kaporin and O. Axelsson,
#   "On a class of nonlinear equation solvers based on the residual norm
#   reduction over a sequence of affine subspaces",
#   SIAM J, Sci. Comput. 16(1), 1995.

#   SIF input: Ph. Toint, Jan 1995.

#   classification NQR2-MY-V-V

from pyomo.core import *
model = ConcreteModel()
M = 100
M = Param(value=M)
H = Param(value=1.0/(M+2.0))
RE = Param(value=4500.0)

S1 = list(range(-1,M+2)
S2 = list(range(1,M)

y = model.variable(S1,S1,value=0.0)

def f(model):
model.add( 0
f = Objective(rule=f,sense=minimize)

def cons(model, i, j):
model.add( (20*y[i,j]-8*y[i-1,j]-8*y[i+1,j]\
    -8*y[i,j-1]-8*y[i,j+1]+2*y[i-1,j+1]+2*y[i+1,j-1]+2*y[i-1,j-1]+2*y[i+1,j+1] +\
    y[i-2,j] + y[i+2,j] + y[i,j-2] + y[i,j+2] + (RE/4.0)*(y[i,j+1]-y[i,j-1])\
    *(y[i-2,j]+y[i-1,j-1]+y[i-1,j+1]-4*y[i-1,j]-4*y[i+1,j]-y[i+1,j-1] \
    -y[i+1,j+1] - y[i+2,j]) - (RE/4.0)*(y[i+1,j]-y[i-1,j])*\
    (y[i,j-2]+y[i-1,j-1]+y[i+1,j-1]-4*y[i,j-1]-4*y[i,j+1]-y[i-1,j+1]-y[i+1,j+1] - y[i,j+2])) == 0
cons = Constraint(S2,S2,rule=cons)

for j in S1:
    y[-1,j] = 0.0
    y[-1,j].fixed = True

    y[0,j] = 0.0
    y[0,j].fixed = True

for i in S2:
    y[i,-1] = 0.0
    y[i,-1].fixed = True

    y[i,0] = 0.0
    y[i,0].fixed = True

    y[i,value(M)+1] = 0.0
    y[i,value(M)+1].fixed = True

    y[i,value(M)+2] = 0.0
    y[i,value(M)+2].fixed = True

for j in S1:
    y[value(M)+1,j] = -value(H)/2.0
    y[value(M)+1,j].fixed = True

    y[value(M)+2,j] = value(H)/2.0
    y[value(M)+2,j].fixed = True

