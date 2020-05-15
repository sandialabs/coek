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

#   Source:  problem 30 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Toint#17 and Buckley#78.
#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-AN-V-V
from pyomo.core import *
model = ConcreteModel()

N = 10000
kappa1 = 2.0
kappa2 = 1.0

x = model.variable(list(range(1,N),value=-1.0)

def f_rule(model):
model.add( 0
f = Objective(rule=f_rule)

def con1(model):
model.add( (-2*x[2]+kappa2+(3-kappa1*x[1])*x[1]) == 0
cons1 = Constraint(rule=con1)

def con2(model,i):
model.add( (-x[i-1]-2*x[i+1]+kappa2+(3-kappa1*x[i])*x[i]) == 0
cons2 = Constraint(list(range(2,N-1),rule=con2)

def con3(model):
model.add( (-x[N-1]+kappa2+(3-kappa1*x[N])*x[N]) == 0
cons3 = Constraint(rule=con3)
