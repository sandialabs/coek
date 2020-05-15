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

#   classification QLR2-AN-V-V

from pyomo.core import *
model = ConcreteModel()

n = Param(value=1000)
b = Param(value=5)

Sn = list(range(1,n)
Sb = list(range(1,b)

x = model.variable(Sn, bounds=(-1,1), value=0.99)
y = model.variable(Sn, bounds=(-1,1), value=-0.99)
z = model.variable(Sb, bounds=(0,2), value=0.5)

def f(model):
    sum_expr_1 = 0
    sum_expr_2 = 0
    for i in Sn:
        sum_expr_1 += x[i]*y[i]
    for j in Sb:
        sum_expr_2 += 0.5*z[j]**2
    exp = sum_expr_1 + sum_expr_2
model.add( exp
f = Objective(rule=f,sense=minimize)

def cons1(model):
    sum_cexpr_1 = 0
    sum_cexpr_2 = 0
    for i in Sn:
        sum_cexpr_1 += x[i] + y[i]
    for j in Sb:
        sum_cexpr_2 += z[j]
    cexp = sum_cexpr_1 + sum_cexpr_2
model.add( (value(b)+ 1, cexp, None)
cons1 = Constraint(rule=cons1)

def cons2(model, i):
    csum = 0
    for j in Sb:
        csum += z[j]
model.add( x[i] + y[i] + csum == value(b)
cons2 = Constraint(Sn, rule=cons2)
