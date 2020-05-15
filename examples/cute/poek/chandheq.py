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

#   Source: problem 4 in
#   J.J. More',
#   "A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-RN-V-V

from pyomo.core import *
model = ConcreteModel()

n = 100
c = 1.0

def x_init(model,i):
model.add( i/float(n)
x = Param(list(range(1,n),value=x_init)

def w_init(model,i):
model.add( 1.0/float(n)
w = Param(list(range(1,n),value=w_init)

h = model.variable(list(range(1,n),value=1.0,bounds=(0,None))

model.add(0.0)
    
def con1(model,i):
model.add( sum(-0.5*c*w[j]*x[i]/(x[i]+x[j])*h[i]*h[j] for j in range(1,n+1)) + h[i] == 1.0
cons = Constraint(list(range(1,n),rule=con1)

 
