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

#   Source:  problem 8, eqs (8.10)--(8.11) in
#   J.J. More',
#   "A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-MN-V-V

from pyomo.core import *
model = AbstractModel()

n = 1000
n = Param(value=n)

pe = Param(value=5.0)
d = Param(value=0.135)
b = Param(value=0.5)
gamma = Param(value=25.0)

def h_rule(model):
model.add( 1/(float(n)-1)
h = Param(value=h_rule)
def ct1_rule(model):
model.add( -value(h)*value(pe)
ct1 = Param(value=ct1_rule)
def cti1_rule(model):
model.add( 1/value(h) + 1/((value(h)**2)*value(pe))
cti1 = Param(value=cti1_rule)
def cti_rule(model):
model.add( -1/value(h)-2/((value(h)**2)*value(pe))
cti = Param(value=cti_rule)

S = list(range(1,n)
t = model.variable(S, bounds=(0.0000001,None), value=1.0)

def f(model):
model.add( 0
f = Objective(rule=f,sense=minimize)

def cons1(model):
model.add( (value(ct1)*t[2]-t[1]+value(h)*value(pe)) == 0
cons1 = Constraint(rule=cons1)

SS = list(range(2,n-1)
def cons2(model, i):
model.add( (value(d)*(value(b)+1-t[i])*exp(value(gamma)-value(gamma)/t[i])+value(cti1)*t[i-1]\
    +value(cti)*t[i]+t[i+1]/(value(h)**2*value(pe))) == 0
cons2 = Constraint(SS,rule=cons2)

def cons3(model):
model.add( (t[value(n)]-t[value(n)-1]) == 0
cons3 = Constraint(rule=cons3)
