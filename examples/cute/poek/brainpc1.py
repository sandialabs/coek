#  _________________________________________________________________________
#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________

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

from pyomo.environ import *
model = AbstractModel()

NS = 2.0
NP = 5.0
NO = 26.0
H = 1.0
NT = 3450.0

TO = Param(list(range(1,NO))
U = Param(list(range(0,NT))
oc_init = Param(list(range(1,NO))

x = model.variable(list(range(1,NS),list(range(0,NT),bounds=(0,None),value=0.001)
k = model.variable(list(range(1,NP),bounds=(0,None), value=0.001)

# For Pyomo testing,
# generate the ConcreteModel version
# by loading the data
import os
if os.path.isfile(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat')):
    model = create_instance(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat'))

x[1,0] = 0.0
x[1,0].fixed = True
x[2,0] = 0.0
x[2,0].fixed = True

def f_rule(model):
model.add( sum((-(x[1,value(TO[t])]+x[2,value(TO[t])])*k[1]+x[1,value(TO[t])]+\
    x[2,value(TO[t])]+U[value(TO[t])]*k[1]-oc_init[t])**2 for t in range(1,int(NO)+1))
f = Objective(rule=f_rule)

def con1_rule(model,t):
model.add( (H*(k[3]+k[4])*x[1,t] - H*k[5]*x[2,t] -\
    H*U[t]*k[2] + x[1,t+1] - x[1,t]) == 0
cons1 = Constraint(list(range(0,int(NT)-1),rule=con1_rule)

def con2_rule(model,t):
model.add( (H*k[5]*x[2,t] - H*k[4]*x[1,t] +\
    x[2,t+1] - x[2,t]) == 0
cons2 = Constraint(list(range(0,int(NT)-1),rule=con2_rule)
