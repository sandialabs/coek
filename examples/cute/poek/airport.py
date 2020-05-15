#
#  Pyomo: Python Optimization Modeling Objects
#  Copyright (c) 2010 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the Pyomo README.txt file.
#  _________________________________________________________________________

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

#   Source:
#   Contribution from a LANCELOT user.

#   SIF input : Rodrigo de Barros Nabholz & Maria Aparecida Diniz Ehrhardt
#               November 1994, DMA - IMECC- UNICAMP
#   Adaptation for CUTE: Ph. Toint, November 1994.

#   classification SQR2-MN-84-42

from pyomo.environ import *
model=AbstractModel()

N = 42
x = model.variable(list(range(1,42),bounds=(-10,10))
y = model.variable(list(range(1,42),bounds=(-10,10))
r = Param(list(range(1,N))
cx = Param(list(range(1,N))
cy = Param(list(range(1,N))

# For Pyomo testing,
# generate the ConcreteModel version
# by loading the data
import os
if os.path.isfile(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat')):
    model = create_instance(os.path.abspath(__file__).replace('.pyc','.dat').replace('.py','.dat'))

def f_obj_rule(model):
model.add( sum((x[i]-x[j])**2+(y[i]-y[j])**2 for i in range(1,N) for j in range(i+1,N+1))
f = Objective(rule=f_obj_rule)

def cons1_rule(model,i):
model.add( (x[i]-cx[i])**2 + (y[i]-cy[i])**2 - r[i] <= 0
cons1 = Constraint(list(range(1,42),rule=cons1_rule)
