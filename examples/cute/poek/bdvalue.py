#  _________________________________________________________________________                                                                                \
#                                                                                                                                                           \
#  Pyomo: Python Optimization Modeling Objects                                                                                                           \
#  Copyright (c) 2010 Sandia Corporation.                                                                                                                   \
#  This software is distributed under the BSD License.                                                                                                      \
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,                                                                                   \
#  the U.S. Government retains certain rights in this software.                                                                                             \
#  For more information, see the Pyomo README.txt file.                                                                                                     \
#  _________________________________________________________________________                                                                                \

# Formulated in Pyomo by Juan Lopez
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

#   Source:  problem 28 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-MN-V-V

from pyomo.core import *
model = ConcreteModel()

ndp = 5002
h = 1.0/(ndp-1)
S = list(range(1,ndp)
SS = list(range(2,ndp-1)

def x_init(model,i):
model.add( ((i-1)*h)*((i-1)*h-1)
x = model.variable(S,value=x_init)

x[1] = 0
x[1].fixed = True
x[ndp].fixed = True

def f(model):
model.add( 0
f = Objective(rule=f)

def cons(model,i):
model.add( (-x[i-1]+2*x[i]-x[i+1]+0.5*h**2*(x[i]+i*h+1)**3 ) == 0
cons = Constraint(SS,rule=cons)
