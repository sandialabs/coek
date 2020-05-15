#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________

# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
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
#   M.J.D. Powell,
#   "A tolerant algorithm for linearly constrained optimization
#   calculations"'
#   Mathematical Programming 45(3), pp.561--562, 1989.

#   SDIF input: Ph. Toint and N. Gould, May 1990.

#   classification OLR2-AN-5-102

from pyomo.environ import *
model = AbstractModel()

R = 51.0

def T_rule(model,i):
model.add( 5*(i-1)/(R-1)
T = Param(list(range(1,R),value=T_rule,mutable=True)

def ET_rule(model,i):
model.add( exp(T[i])
ET = Param(list(range(1,R),value=ET_rule)

pinit = Param(list(range(0,2))

def P_init(model,i):
model.add( pinit[i]
P = model.variable(list(range(0,2),value=P_init)

Q = model.variable(list(range(1,2),value=0.0)

# For Pyomo testing,
# generate the ConcreteModel version
# by loading the data
import os
if os.path.isfile(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat')):
    model = create_instance(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat'))

def f_rule(model):
model.add( sum((\
    (P[0]+P[1]*T[i]+P[2]*T[i]**2)/\
    (ET[i]*(1+Q[1]*(T[i]-5)+Q[2]*(T[i]-5)**2))\
    -1 )**2 for i in range(1,int(R)+1))
f = Objective(rule=f_rule)

def cons1(model,i):
model.add( P[0]+P[1]*T[i]+P[2]*T[i]**2-(T[i]-5)*ET[i]*Q[1]-\
        (T[i]-5)**2*ET[i]*Q[2]-ET[i]>= 0
cons1 = Constraint(list(range(1,R),rule=cons1)

def cons2(model,i):
model.add( (T[i]-5)*Q[1] + (T[i]-5)**2*Q[2]+0.99999 >= 0
cons2 = Constraint(list(range(1,R),rule=cons2)
