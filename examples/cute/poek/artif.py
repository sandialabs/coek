#  _________________________________________________________________________
#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________
#
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
#   K.M. Irani, M.P. Kamat, C.J. Ribbens, H.F.Walker and L.T. Watson,
#   "Experiments with conjugate gradient algoritms for homotopy curve
#    tracking" ,
#   SIAM Journal on Optimization, May 1991, pp. 222-251, 1991.

#   SIF input: Ph. Toint, May 1990.

#   classification NOR2-AN-V-V

from pyomo.core import *
model = ConcreteModel()

N = 5000

S = list(range(0,N+1)
SS = list(range(1,N)

x = model.variable(S, value=1.0)

def f(model):
model.add( 0
f = Objective(rule=f,sense=minimize)

def cons(model, i):
    expr = (-0.05*(x[i] + x[i+1] + x[i-1]) + atan( sin( (i%100)*x[i] ) ))
model.add( (0,expr)
cons = Constraint(SS, rule=cons)

x[0] = 0.0
x[0].fixed = True

x[N+1] = 0.0
x[N+1].fixed = True
