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

#   classification QLR2-AN-8-10

import poek as pk


model = pk.model()

# LOAD DATA
a = Param(list(range(1,8))
b = Param(list(range(1,7))
c = Param(list(range(2,8))

x = model.add_variable(list(range(1,8),bounds=(0.0,1.0),within=Integers,value=1)

# For Pyomo testing,
# generate the ConcreteModel version
# by loading the data
import os
if os.path.isfile(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat')):
    model = create_instance(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat'))

def obj_rule(model):
model.add_objective( (sum(a[j]*x[j]**2 for j in range(1,9)))+\
    (sum(b[j]*x[j]*x[j+1] for j in range(1,8)))+\
    (sum(c[j]*x[j] for j in range(2,9)))
obj = Objective(rule=obj_rule)

def con1(model,j):
model.add_constraint( x[2*j-1]+x[2*j]<=1.0
con1 = Constraint(list(range(1,4),rule=con1)

def con5(model,j):
model.add_constraint( sum(x[2*i-j] for i in range(1,5))<=2.0
con5 = Constraint(list(range(0,1),rule=con5)

def con7(model):
model.add_constraint( 0 <= 2.0*x[1] + x[3] - x[7]
con7 = Constraint(rule=con7)

def con8(model):
model.add_constraint( 0 <= 5.0*x[1] + 3.0*x[3] - 3.0*x[5] - x[7]
con8 = Constraint(rule=con8)

def con9(model):
model.add_constraint( 0 <= x[2] - x[4] - 3.0*x[6] - 5.0*x[8]
con9 = Constraint(rule=con9)

def con10(model):
model.add_constraint( 0 <= x[2] - 3.0*x[6] - 2.0*x[8]
con10 = Constraint(rule=con10)
